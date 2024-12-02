#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"

extern unsigned char next_state(unsigned char current_state, unsigned char taps, int *output_bit);
volatile unsigned char current_state = 0xAA;					// in binario: 10101010
volatile unsigned char taps = 0x11;										// in binario: 00011101
int output_bit = 0;
volatile unsigned char start_state = 0xAA;						// aggiunto per il controllo su EINT2

void EINT0_IRQHandler (void)	  
{
	LED_On(0);
  LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  
{
	int i;
	for (i = 0; i < 8; i++) LED_Off(i);		// spengo tutti i led
	current_state = next_state(current_state, taps, &output_bit);
	
	LED_Out(current_state);					// visualizzo lo state sui LED
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  
{
	unsigned int count = 0;
	do{						//uso do while per la prima iterazione altrimenti viene count = 0
		current_state = next_state(current_state, taps, &output_bit);
		count++;
	} while(current_state != start_state);
	LED_Out(count);							// mi viene chiesto di printare con i led il count di tutti i cicli fino a che current_state non è = a start_state
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


