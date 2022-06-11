#define F_CPU  16000000UL     // CPU runs at 16 MHz

#include <avr/io.h>
#include <util/delay.h>			// AVR Delay Header (For delay_ms function)
#include <avr/interrupt.h>

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "Custom_GPIO.H"
#include "LIB_TWI.h"


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

#define TRUE				1
#define FALSE				0

#define I2C_READ			1
#define I2C_WRITE			0

#define TEST_BIT(var, bit)	((var) & (1<<bit))
#define SET_BIT(var, bit)	((var) |= (1<<bit))

#define HI_BYTE(w)			(((w) >> 8) & 0xFF)		// Extract high-order byte 
													// from unsigned word
#define LO_BYTE(w)			((w) & 0xFF)			// Extract low-order byte 
													// from unsigned word


#define DEBOUNCING_DELAY   10

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// INPUTS ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
#define		NAME_OF_DEFINE	MACRO						// ARDUINO PIN
 */

#define		DELAY_ADJ		ADCH0						// A0
#define		MOTION_SENS_1	(PINC & (1 << PINC1))		// A1
#define		MOTION_SENS_2	(PINC & (1 << PINC2))		// A2

#define		BUTTON_1		(PIND & (1 << PIND7))		// 7
#define		BUTTON_B		(PIND & (1 << PIND6))		// 6
#define		BUTTON_C		(PIND & (1 << PIND5))		// 5
#define		BUTTON_D		(PIND & (1 << PIND4))		// 4
#define		BUTTON_ENTER	(PIND & (1 << PIND3))		// 3
#define		BUTTON_CANCEL	(PIND & (1 << PIND2))		// 2

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// OUTPUTS ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
#define		NAME_OF_DEFINE			MACRO				// ARDUINO PIN
 */

#define		STROBE			OUTPUT_PIN(PORTB, 5)		// 13
#define		LED_ZONE_1		OUTPUT_PIN(PORTB, 4)		// 12
#define		LED_ZONE_2		OUTPUT_PIN(PORTB, 3)		// 11

#define		SIREN_OUT		OUTPUT_PIN(PORTB, 2)		// 10
#define		PWM_OUT_AUX		OUTPUT_PIN(PORTB, 1)		// 9
#define		LED_ARMED		OUTPUT_PIN(PORTB, 0)		// 8
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CONSTANTS ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// VARIABLES ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned char buffer[16];

unsigned long timerCounter = 0;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// PROTOTYPES //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// General Setup for Atmega328P and external peripherals
//void Setup();






////////////////////////////////////////////////////////////////////////////////
////////////////////////////// INTERRUPTS FUNCTION /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// TMR0 ISR ///////////////////////////////////

ISR ( TIMER0_COMP_vect )
{
	
	timerCounter++;
	

}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// MAIN FUNCTION ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


int main(void)
{
//	Setup();
//	Siren_PWM_Off();
//	
//	LCD_Clear(addr);
//	LCD_Position(addr, 0xC0);
//	LCD_Write(addr, clear_msg, 16);
	
	
	
	
	/////////////////////////////// MAIN PROCESS ///////////////////////////////
	
	while(1){
        
    }
	
    
}
