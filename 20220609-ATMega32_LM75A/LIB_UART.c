
#include "LIB_UART.h"


void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);   /* Turn on the transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); /* Use 8-bit character sizes */

	UBRRL = BAUD_PRESCALE; 					/* Load lower 8-bits of the baud rate value */
	UBRRH = (BAUD_PRESCALE >> 8);			/*Load upper 8-bits*/
}



char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0); /*Do nothing until data have been received*/
	return(UDR);				/* return the byte*/
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE))); /*Wait for empty transmit buffer*/
	UDR = ch ;
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)	/*send string up to null */
	{
		UART_TxChar(str[j]);	
		j++;
	}
}

char getch(void)
{
    return UART_RxChar();
}

void putch(char txData)
{
    UART_TxChar(txData);
}
