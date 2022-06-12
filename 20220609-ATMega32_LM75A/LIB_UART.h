#ifndef LIB_UART_H
#define	LIB_UART_H

#ifndef F_CPU
#define F_CPU  16000000UL     // CPU runs at 16 MHz
#endif


#include <avr/io.h>								/* Include AVR std. library file */
#include <util/delay.h>							/* Include inbuilt defined Delay header file */

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

char UART_RxChar();
void UART_TxChar(char ch);
void UART_SendString(char *str);

void UART_init(long USART_BAUDRATE);

char getch(void);
void putch(char txData);


#endif