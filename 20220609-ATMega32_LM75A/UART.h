/*
 * USART0.h
 *
 * Created: 4/1/2016 2:54:16 PM
 *  Author: Mark
 */ 


#ifndef USART0_H_
#define USART0_H_

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

/*********** Config Def's ***********/
 
#define ESC				27

void UART_Init(unsigned int);
 /* ******************************************************
	Call This Funtion to initialize the Uart. For the BAUD
	parameter select the U2X=0 column of UBRR setting for 
	crystal frequency of your setup. 
******************************************************/
 
 unsigned char  UART_putc(char);
/* ****************************************************
	Places a character in the Tx buffer and sends.
	Returns 1 if successfull
	Returns 0 if Buffer is full.
******************************************************/

 void UART_puts(char *);
/* ******************************************************
	Call this function to  transfer a string stored in RAM 
	to the output buffer.  Will loop until the entire string 
	is transfered to the buffer.
*******************************************************/

void UART_puts_P(PGM_P s);
 /* *****************************************************
	Call this function to  transfer a string from program 
	memory (FLASH) to the output buffer. Will loop until
	the entire string is transfered to the buffer.
*********************************************************/
 
 unsigned char UART_RxCount (void);
/* ****************************************************
	Call this function to check for the presence of a 
	character in the  Rx Buffer.  The function returns 
	the character count.
******************************************************/

char  UART_getc(void);
/*+*****************************************************
	Returns one Character form Rx Buffer
*****************************************************+*/

unsigned char  UART_write_buff_full(void);
/* ************************************************
	Call this function to check for the Tx Buffer is full.  
	The function returns 1 if full 0 if not.
*************************************************/

extern FILE  UART_Stream;

#endif /* USART0_H_ */