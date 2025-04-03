#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>

#include "uart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ledOffBtn_clicked();
    void on_ledOnBtn_clicked();
    void on_setupPwmBtn_clicked();

    void on_periodSpinBox_editingFinished();

    void on_widthSpinBox_editingFinished();

private:
    Ui::MainWindow *ui;
    QList<QSerialPort*> ports;
    QList<QSerialPortInfo> availablePortsInfo;
    QSerialPort *activePort;
    QTimer timeOutTimer;
    int bytesToWrite = 0;
    void sendUartMessage(const char *data, size_t size);
    void setupSerial(QSerialPort &port, const QSerialPortInfo &portInfo);
    void updateSerialPorts();
    void waitForDevice();
    void on_timeOutWaitingForDevice();
};
#endif // MAINWINDOW_H
