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

//extern volatile unsigned int led_value;
//uint32_t val_TC;		// timer counter

void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1) // MR0
	{ 
		// your code+
		time++;
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
	if(LPC_TIM1->IR & 1) // MR0
	{ 
		// your code
		static int counter = 0;
		if(counter == 5){// fine timer
			disable_timer(1);
			reset_timer(1);
			indexRes++;
			if(indexRes == N){// fine vettore
				svuotaVet(vet2, N);
				svuotaVetS(res, N);
				index2 = 0;
				indexRes = 0;
				index = 0;
				disattivato = 0;
				disable_timer(2);
				reset_timer(2);
			}
			if(res[indexRes] > media){
				// timer1
				//reset_timer(1);
				enable_timer(1);
			} else{
				// timer2
				//reset_timer(2);
				enable_timer(2);
			}
		}
		if((counter % 2) == 0){
				LED_Out(res[indexRes]);
			} else{
				LED_Out(0);
			}
		counter++;
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
//extern int debounce_active;

void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1) // MR0 -> lavoro con MR0 per il debounce
	{
		// your code
		static int counter = 0;
		if(counter == 4){// fine timer
			disable_timer(2);
			reset_timer(2);
			indexRes++;
			if(indexRes == N){// fine vettore
				svuotaVet(vet2, N);
				svuotaVetS(res, N);
				index2 = 0;
				indexRes = 0;
				index = 0;
				disattivato = 0;
				disable_timer(1);
				reset_timer(1);
			}
			if(res[indexRes] > media){
				// timer1
				//reset_timer(1);
				enable_timer(1);
				//disable_timer(2);
			} else{
				// timer2
				//reset_timer(2);
				enable_timer(2);
			}
		}
		if((counter % 2) == 0){
			LED_Out(res[indexRes]);
		} else{
			LED_Out(0);
		}
		counter++;
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
