#define F_CPU  16000000UL     // CPU runs at 16 MHz

#include <avr/io.h>
#include <util/delay.h>			// AVR Delay Header (For delay_ms function)
#include <avr/interrupt.h>

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "Custom_GPIO.H"
#include "LIB_TWI.h"
#include "LIB_UART.h"
#include "LIB_LM75A.h"



//FUSES = {
//	.low = 0xDE, // LOW {SUT_CKSEL=EXTXOSC_8MHZ_XX_258CK_14CK_65MS, CKOUT=CLEAR, CKDIV8=CLEAR}
//	.high = 0xD9, // HIGH {BOOTRST=CLEAR, BOOTSZ=2048W_3800, EESAVE=CLEAR, WDTON=CLEAR, SPIEN=SET, DWEN=CLEAR, RSTDISBL=CLEAR}
//	.extended = 0xFF, // EXTENDED {BODLEVEL=DISABLED}
//};
//
//LOCKBITS = 0xFF; // {LB=NO_LOCK, BLB0=NO_LOCK, BLB1=NO_LOCK}



////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// DEFINES ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define ON					1
#define OFF					0

#define HIGH				1
#define LOW					0

#define PORT_AS_INPUT		0
#define PORT_AS_OUTPUT		0xFF
#define PORT_INPUT_PULLUPS	0xFF

#define DISP_DELAY			10

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// INPUTS ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Dont modify these lines if you want to keep the pin numeration of the
// port previously selected for Buttons

#define		BUTTON_SCA			(PIND & (1 << PD4))
#define		BUTTON_MAX			(PIND & (1 << PD5))
#define		BUTTON_CUR			(PIND & (1 << PD6))
#define		BUTTON_MIN			(PIND & (1 << PD7))

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// OUTPUTS ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Modify next 3 lines if you want to change Buttons port
#define		DISPLAY_PORT_DIR	DDRA	//
#define		DISPLAY_PORT		PORTA	//

#define		DISPLAY_CTRL_DIR	DDRB	//
#define		DISPLAY_CTRL_PORT	PORTB	//

#define		CONTROL_DISP1		OUTPUT_PIN(DISPLAY_CTRL_PORT, 0)
#define		CONTROL_DISP2		OUTPUT_PIN(DISPLAY_CTRL_PORT, 1)
#define		CONTROL_DISP3		OUTPUT_PIN(DISPLAY_CTRL_PORT, 2)
#define		CONTROL_DISP4		OUTPUT_PIN(DISPLAY_CTRL_PORT, 3)

#define		DP					0x80

#define		LEDS_PORT			PORTC	//

#define		LED_MAX				OUTPUT_PIN(LEDS_PORT, 5)
#define		LED_CUR				OUTPUT_PIN(LEDS_PORT, 6)
#define		LED_MIN				OUTPUT_PIN(LEDS_PORT, 7)
#define		LED_SYSTEM			OUTPUT_PIN(DISPLAY_CTRL_PORT, 7)

// 7 Segment Array table for cathode display common terminal
const unsigned int N7Seg[10] ={  
	0b0111111,   // 0
	0b0000110,   // 1
	0b1011011,   // 2
	0b1001111,   // 3
	0b1100110,   // 4
	0b1101101,   // 5
	0b1111101,   // 6
	0b0000111,   // 7
	0b1111111,   // 8
	0b1101111    // 9
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// VARIABLES ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

char buffer[64];


uint8_t digit1, digit2, digit3, digit4, disp_cnt = 0, disp_cnt_bk;

uint32_t timerTick = 0;

uint32_t display_ticks = 0;

// Machine States 
typedef enum{MODE_MIN, MODE_CUR, MODE_MAX} tempMode_te;
typedef enum{TEMP_C, TEMP_K, TEMP_F} tempUnit_te;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// PROTOTYPES //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// General Setup for Atmega328P and external peripherals
//void Setup();






////////////////////////////////////////////////////////////////////////////////
////////////////////////////// INTERRUPTS FUNCTION /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void tmr1_Init()
{
	cli();
			
	// Clear registers
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
		
	// 1000 Hz (16000000/((249+1)*64))
	// 1mS overflow
	OCR1A = 249;
	OCR1A = 249;
	OCR1A = 249;
	// CTC
	TCCR1B |= (1 << WGM12);
	// Prescaler 64
	TCCR1B |= (1 << CS11) | (1 << CS10);
	// Output Compare Match A Interrupt Enable
	TIMSK |= (1 << OCIE1A);
		
	sei();
	
}

/////////////////////////////////// TMR0 ISR ///////////////////////////////////

ISR ( TIMER1_COMPA_vect )
{
	// Ticks counter, it increments every ms
	timerTick++;
	LED_SYSTEM = ~LED_SYSTEM;

}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// MAIN FUNCTION ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


int main(void)
{
	
	tempMode_te tempMode = MODE_CUR;
	tempUnit_te tempUnit = TEMP_C;
	
	
	uint32_t temperature;
	uint32_t temperatureMin, temperatureMax, tempToDisplay;
	
	double floatingTemp = 0, integerTemp = 0, convertedTemp = 0;
	
	
	

	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC |= (1 << PC5) | (1 << PC6) | (1 << PC7);
	DDRD = 0x0E;
	
	PORTD |= (1 << PD4) | (1 << PD5) | (1 << PD6)  | (1 << PD7);

	// Configures Button Port and internal pullup resistors
	//BUTTON_PORT_DIR	= PORT_AS_INPUT;
	//BUTTON_P_PORT = PORT_INPUT_PULLUPS;		// Port Pullups enabled

	// Configures Leds port
	DISPLAY_PORT_DIR = PORT_AS_OUTPUT;

	
	UART_init(9600);
	I2C_Init();
	
	
	tmr1_Init();
	
	while(timerTick < 100);					// Waits 100 ms
	
	I2C_Start(LM75A_WR_ADR);
	I2C_Write(0x00);						/* Write start memory address */
	I2C_Repeated_Start(LM75A_RD_ADR);
	//LCD_Char(I2C_Read_Ack());
	temperature = I2C_Read_Ack();
	I2C_Read_Nack();						/* Read flush data with nack */
	I2C_Stop();								/* Stop I2C */
	

	sprintf(buffer, "\nvalue = %lu", temperature);
	UART_SendString(buffer);
	
	temperatureMin = temperature;
	temperatureMax = temperature;
	
	
	display_ticks = timerTick;
	
	do{
		
		if(temperature < temperatureMin)
			temperatureMin = temperature;
			
		if(temperature > temperatureMax)
			temperatureMax = temperature;
		
		
		I2C_Start(LM75A_WR_ADR);
		I2C_Write(0x00);						/* Write start memory address */
		I2C_Repeated_Start(LM75A_RD_ADR);
		//LCD_Char(I2C_Read_Ack());
		temperature = I2C_Read_Ack();
		I2C_Read_Nack();						/* Read flush data with nack */
		I2C_Stop();								/* Stop I2C */
	

		sprintf(buffer, "\nvalue = %lu", temperature);
		UART_SendString(buffer);
		
		
		
		if(!BUTTON_MIN)	tempMode = MODE_MIN;
		if(!BUTTON_CUR) tempMode = MODE_CUR;
		if(!BUTTON_MAX) tempMode = MODE_MAX;
		
		switch(tempMode){
			
			case MODE_MIN:
				tempToDisplay = temperatureMin;
				LED_MIN = ON;
				LED_CUR = OFF;
				LED_MAX = OFF;
			break;
			
			case MODE_CUR:
				tempToDisplay = temperature;
				LED_MIN = OFF;
				LED_CUR = ON;
				LED_MAX = OFF;
			break;
			
			case MODE_MAX:
				tempToDisplay = temperatureMax;
				LED_MIN = OFF;
				LED_CUR = OFF;
				LED_MAX = ON;
			break;
			
			default:
				tempToDisplay = temperature;
				LED_MIN = OFF;
				LED_CUR = ON;
				LED_MAX = OFF;
			break;
			
		}
		
		
		if(!BUTTON_SCA){
			if(tempUnit == TEMP_K)
				tempUnit = TEMP_C;
			else
				tempUnit++;
		}
		
				
		switch(tempUnit){
			case TEMP_C:
				convertedTemp = (double) tempToDisplay;
			break;
			
			case TEMP_F:
				convertedTemp = (double) ((tempToDisplay * 9 / 5) + 32);
			break;
			
			case TEMP_K:
				convertedTemp = (double) (tempToDisplay + 273.15);
			break;
			
		}
				
		floatingTemp = modf(convertedTemp, &integerTemp);
		
		floatingTemp = floatingTemp * 100;
		
		tempToDisplay = (uint32_t) integerTemp;
		 
		
		// Data split from common integer to multiple integers
		digit1 = (tempToDisplay/100)%10;       // Extract thousands of mv
		digit2 = (tempToDisplay/10)%10;        // Extract hundreds of millivolts if you
		digit3 = (tempToDisplay/1)%10;       // Extract tens of mv
		digit4 = (uint8_t) floatingTemp;
		digit4 = (digit4/1)%10;
		
		if(timerTick > (display_ticks + DISP_DELAY))
		{
			display_ticks = timerTick;
			if(disp_cnt == 4)
				disp_cnt = 1;
			else
				disp_cnt++;
		
		
			switch(disp_cnt){
			
				case 1:
					CONTROL_DISP4 = OFF;
					DISPLAY_PORT = N7Seg[digit1];
					CONTROL_DISP1 = ON;
				break;
			
				case 2:
					CONTROL_DISP1 = OFF;
					DISPLAY_PORT = N7Seg[digit2];
					CONTROL_DISP2 = ON;
				break;
			
				case 3:
					CONTROL_DISP2 = OFF;
					DISPLAY_PORT = N7Seg[digit3] | DP;
					CONTROL_DISP3 = ON;
				break;
			
				case 4:
					CONTROL_DISP3 = OFF;
					DISPLAY_PORT = N7Seg[digit4];
					CONTROL_DISP4 = ON;
				break;
			
			
			}
			
		}
		
		
	}while(1);
	
	
	
	
	
	
	
	
	
	
	
	
	/////////////////////////////// MAIN PROCESS ///////////////////////////////
	
	while(1){
		sprintf(buffer, "\nhola %02d:%02d:%04d", 11, 6, 2022);
        UART_SendString(buffer);
    }
	
    
}

