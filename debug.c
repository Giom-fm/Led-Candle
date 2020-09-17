#include "debug.h"
#include <avr/io.h>
#include <util/setbaud.h>

void usart_init() {
    // Enable UART Transmitter mode
    UCSR0B = _BV(TXEN0);
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif

    // Set Frame format to 8 data with 1 stop bits
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    // Sends a "new Line"
    usart_send_s("\r\n");

}

/*
 * Blocks until buffer is empty and sends the passed character.
 * @param c The character to send.
 */
void usart_send_c(unsigned char c) {

    // Wait until UART Data Register is empty
    while (!(UCSR0A & (1 << UDRE0))) {
    }
    // Set Character into Data Register
    UDR0 = c;

}

void usart_send_s(const char *s) {
    while (*s) {
        usart_send_c(*s);
        s++;
    }
}
