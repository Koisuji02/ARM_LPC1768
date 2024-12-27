#include "button.h"
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "GLCD/AsciiLib.h"
#include "timer/timer.h"
#include "RIT/RIT.h"

volatile int flag1 = 0;
extern int gameOver;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{
	if(gameOver!=1 && victory !=1){
		if(flag1 == 0) { // attivo RIT alla 1^ pressione
			flag1 = 1;
			enable_RIT();									// passo la gestione del button EINT0 nel RIT
		}
		NVIC_DisableIRQ(EINT0_IRQn);
		LPC_PINCON->PINSEL4 &= ~(1<<20);
		LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
	}
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


