#include <QSerialPortInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timeOutTimer(this)
{
    ui->setupUi(this);
    setEnabled(false);
    timeOutTimer.setInterval(4 * 1000);
    connect(&timeOutTimer, &QTimer::timeout, this, &MainWindow::on_timeOutWaitingForDevice);
    waitForDevice();
    timeOutTimer.start();
    connect(ui->periodSpinBox, &QSpinBox::valueChanged, this, [this] (int period) {
        this->ui->widthSpinBox->setMaximum(period - 1);
    });
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendUartMessage(const char *data, size_t size) {
    auto written = activePort->write(data, size);
    if  (written != size) {
        qWarning() << "Не удалось отправить сообщение целиком";
    } else {
        bytesToWrite += written;
    }
}

void MainWindow::on_ledOnBtn_clicked()
{
    const auto msg = uartMessageLedOn;
    sendUartMessage(reinterpret_cast<const char*>(&msg), sizeof(msg));
}

void MainWindow::on_ledOffBtn_clicked()
{
    const auto msg = uartMessageLedOff;
    sendUartMessage(reinterpret_cast<const char*>(&msg), sizeof(msg));
}

void MainWindow::setupSerial(QSerialPort &port, const QSerialPortInfo &portInfo) {
    port.setPort(portInfo);
    port.setBaudRate(QSerialPort::Baud115200);
    port.setStopBits(QSerialPort::StopBits::OneStop);
    port.setParity(QSerialPort::NoParity);
    port.setDataBits(QSerialPort::Data8);
    port.setFlowControl(QSerialPort::NoFlowControl);
}

void MainWindow::on_setupPwmBtn_clicked() {
    UartPwmSettingsMessage msg = {
            .hdr =  {
            .magic = UART_MESSAGE_MAGIC,
            .kind = UART_MESSAGE_KIND_PWM_SETTINGS,
        },

    };
    msg.period = ui->periodSpinBox->value(),
    msg.width = ui->widthSpinBox->value(),
    sendUartMessage(reinterpret_cast<const char*>(&msg), sizeof(msg));
}

void MainWindow::waitForDevice() {
    availablePortsInfo = QSerialPortInfo::availablePorts();
    for (const auto &portInfo: availablePortsInfo) {
        auto port = new QSerialPort(this);
        setupSerial(*port, portInfo);
        if (port->open(QIODevice::ReadWrite)) {
            ports.append(port);
            connect(port, &QSerialPort::readyRead, this, [this, port]() {
                const auto data = port->readAll();
                const UartMessageHeader expect = {
                    .magic = UART_MESSAGE_MAGIC,
                    .kind = UART_MESSAGE_KIND_BROADCAST,
                };
                if (data.size() == sizeof(expect) &&
                        std::memcmp(reinterpret_cast<const char*>(&expect), data.data() , sizeof(expect)) == 0) {
                    for (const auto portToClose: this->ports) {
                        if (portToClose != port) delete portToClose;
                    }
                    this->activePort = port;
                    this->ports.clear();
                    this->activePort->disconnect();
                    this->timeOutTimer.disconnect();
                    this->timeOutTimer.stop();
                    connect(&timeOutTimer, &QTimer::timeout, this, [this]() {
                        this->setEnabled(false);
                        delete activePort;
                        timeOutTimer.disconnect();
                        connect(&timeOutTimer, &QTimer::timeout, this, &MainWindow::on_timeOutWaitingForDevice);
                        waitForDevice();
                    });
                    connect(port, &QSerialPort::readyRead, this, [this, port]() {
                        const auto data = port->readAll();
                        const UartMessageHeader expect = {
                            .magic = UART_MESSAGE_MAGIC,
                            .kind = UART_MESSAGE_KIND_BROADCAST,
                        };
                        if (data.size() == sizeof(expect) &&
                                std::memcmp(reinterpret_cast<const char*>(&expect), data.data() , sizeof(expect)) == 0) {
                            this->timeOutTimer.start();
                        }
                    });
                    this->setEnabled(true);
                    this->timeOutTimer.start();
                }
            });
        } else {
            delete port;
        }
    }
}

void MainWindow::on_timeOutWaitingForDevice() {
    for (const auto portToClose: ports) {
        delete portToClose;
    }
    ports.clear();
    waitForDevice();
}



