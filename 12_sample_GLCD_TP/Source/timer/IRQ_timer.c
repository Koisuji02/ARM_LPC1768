/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"
#include "GLCD/GLCD.h" 
#include "GLCD/AsciiLib.h"
#include "RIT/RIT.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern int victory;


void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1) // MR0
	{ 
		// your code
		LPC_TIM0->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM0->IR & 2){ // MR1
		// your code	
		LPC_TIM0->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 4){ // MR2
		// your code	
		LPC_TIM0->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 8){ // MR3
		// your code	
		LPC_TIM0->IR = 8;			// clear interrupt flag 
	}
  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


void TIMER1_IRQHandler (void)
{
	static int counter = 60;		// 60s come tempo di gioco disponibile
	int i, j;
	if(LPC_TIM1->IR & 1) // MR0
	{ 
		counter--;
		PutChar(40, 20, (counter/10 + 48) , Red, Black);
		PutChar(50, 20, (counter%10 + 48) , Red, Black);
		PutChar(60, 20, (int)'s' , Red, Black);
		if(counter == 0){		// quando arrivo a 0
			disable_timer(1);
			if(victory == 0){
				// gestire fine gioco (stoppare tutto come nel victory)
				disable_RIT();		// stop definitivo al gioco
				LCD_Clear(Black);
				GUI_Text(7*10+10, 13*10+40, (uint8_t *) " Game Over! ", Red, Black);
			}
		}
		LPC_TIM1->IR = 1;			//clear interrupt flag
	}

	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

// gestisce il debounce di INT0
void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1) // MR0 -> lavoro con MR0 per il debounce
	{
		disable_timer(2);			// disattivo il timer 2 (usato per il debounce)
		LPC_TIM2->IR = 1;			//clear interrupt flag
	}
	
	/*else if(LPC_TIM2->IR & 2){ // MR1
		// your code	
		LPC_TIM2->IR = 2;			// clear interrupt flag 
	}*/
	return;
}


/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void)
{
  if(LPC_TIM3->IR & 1) // MR0
	{ 
		// your code
		LPC_TIM3->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM3->IR & 2){ // MR1
		// your code	
		LPC_TIM3->IR = 2;			// clear interrupt flag 
	}
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
