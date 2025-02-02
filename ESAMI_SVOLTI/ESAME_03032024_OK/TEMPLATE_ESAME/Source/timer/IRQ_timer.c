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
int counter_tim0 = 0;
int counter_tim1 = 0;
int counter_tim2 = 0;

void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1) // MR0
	{ 
		// your code
		//static int counter = 4; // 1s
		if(counter_tim0 == 0){ // finito
			// avanzo vet
			if(index < N-1){
					index++;
					if(vet[index] == 1){
						// LED1
						LED_Out(0);
						LED_On(1);
					} else if (vet[index] == 2){
						// LED2
						LED_Out(0);
						LED_On(2);
					} else{
						while(vet[index] != 1 && vet[index] != 2) index++;
					}
			}
			disable_timer(0);
		}
		else{
			if((counter_tim0 % 2) == 0)LED_Out(0xFF); // lampeggio
			else LED_Out(0);
			counter_tim0--;
		}
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
		//static int counter = 8; // 2s
		if(counter_tim1 == 0){ // finito
			counter_tim2 = 24;
			enable_timer(2); // attesa di 3 s
			disable_timer(1);
			//reset_timer(1);
		}
		else{
			if((counter_tim1 % 2) == 0){
				LED_On(1); // lampeggio
				LED_On(2);
			}
			else LED_Out(0);
			counter_tim1--;
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
//extern int debounce_active;
unsigned int compare (unsigned char VETT[], unsigned char VETT2[], const unsigned int num);

void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1) // MR0 -> lavoro con MR0 per il debounce
	{
		// your code
		//static int counter = 24; // 3s
		if(counter_tim2 == 0){ // finito
			result = compare(vet, vet2, N);
			LED_Out(result);
			disable_timer(2);
			//reset_timer(2);
		}
		else{
			if((counter_tim2 % 2) == 0)LED_Out(0xFF); // lampeggio
			else LED_Out(0);
			counter_tim2--;
		}
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
