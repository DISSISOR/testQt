#ifndef UART_H
#define UART_H

#include <stdint.h>

#define UART_MESSAGE_KIND_INV 0
#define UART_MESSAGE_KIND_LED_ON 1
#define UART_MESSAGE_KIND_LED_OFF 2
#define UART_MESSAGE_KIND_LED_BRIGHT_UP 3
#define UART_MESSAGE_KIND_LED_BRIGHT_DOWN 4
#define UART_MESSAGE_KIND_RESPONSE_OK 5
#define UART_MESSAGE_KIND_RESPONSE_INV_REQ 6
#define UART_MESSAGE_KIND_BROADCAST 7
#define UART_MESSAGE_KIND_PWM_SETTINGS 8

#define UART_MESSAGE_MAGIC 0x985

typedef struct __attribute__((packed))  UartMessageHeader {
    uint32_t magic;
    uint16_t kind;
} UartMessageHeader;

typedef struct  __attribute__((packed)) {
    UartMessageHeader hdr; // kind = UART_MESSAGE_KIND_PWM_SETTINGS
    uint16_t period;
    uint16_t width;
} UartPwmSettingsMessage;

extern const UartMessageHeader uartMessageLedOn;
extern const UartMessageHeader uartMessageLedOff;
extern const UartMessageHeader uartMessageLedBrightUp;
extern const UartMessageHeader uartMessageLedBrightDown;

#endif // UART_H

