#include "uart.h"

const UartMessageHeader uartMessageLedOn = { .magic = UART_MESSAGE_MAGIC, .kind = UART_MESSAGE_KIND_LED_ON };
const UartMessageHeader uartMessageLedOff = { .magic = UART_MESSAGE_MAGIC, .kind = UART_MESSAGE_KIND_LED_OFF };
const UartMessageHeader uartMessageLedBrightDown = { .magic = UART_MESSAGE_MAGIC, .kind = UART_MESSAGE_KIND_LED_BRIGHT_DOWN };
const UartMessageHeader uartMessageLedBrightUp = { .magic = UART_MESSAGE_MAGIC, .kind = UART_MESSAGE_KIND_LED_BRIGHT_UP };
