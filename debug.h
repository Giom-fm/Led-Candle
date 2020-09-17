/* 
 * File:   debug.h
 * Author: Guillaume Fournier-Mayer
 *
 * Created on 25. Juli 2020, 14:54
 * 
 * A module to send some debug data over the USART interface.
 * This modules works in a blocking way!
 */

#ifndef USART_H
#define	USART_H

#define F_CPU 1000000UL
#define BAUD 9600

/**
 * Initialize the USART Interface to 8 databits and 1 stopbit
 * and sends a "new Line"
 */
void usart_init();

/**
 * Sends the passed string character by character by use of "usart_send_c".
 * @param s The string to send.
 */
void usart_send_s(const char *s);


/**
 * Debug makro that is used to send some data over the USART interface.
 * If DEBUG is false, the code should be optimized by the compiler.
 */
#define debug_print(...) \
            if(DEBUG){\
            char buffer[100];\
            sprintf(buffer, __VA_ARGS__);\
            usart_send_s(buffer);}


#endif	/* USART_H */

