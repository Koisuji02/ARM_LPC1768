#include "button.h"
#include "LPC17xx.h"
#include "timer/timer.h"
#include "led/led.h"
#include "RIT/RIT.h"

/*extern unsigned char next_state(unsigned char current_state, unsigned char taps, int *output_bit);
volatile unsigned char current_state = 0xAA;					// in binario: 10101010
volatile unsigned char taps = 0x11;										// in binario: 00011101volatile unsigned char start_state = 0xAA;						// aggiunto per il controllo su EINT2
volatile unsigned char start_state = 0xAA;						// aggiunto per il controllo su EINT2
*/

void EINT0_IRQHandler (void)	  	/* INT0														 */
{
	//if(pause == 1) 
		enable_RIT();									// passo la gestione del button EINT0 nel RIT
	NVIC_DisableIRQ(EINT0_IRQn);
	LPC_PINCON->PINSEL4 &= ~(1<<20);
	
	//LED_On(0);
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	//LED_Out(current_state);					// visualizzo lo state sui LED
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
	//NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	//LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


