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
int counter0 = 0;

void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1) // MR0
	{ 
		// your code
		if(counter0 == 8){ // 1s = 8*0.125s
			// controllo fine vettore
			do{
				if(index <= N-1)index++;
				else break;
			}while(vet[index] != 1 && vet[index] != 2); // ignoro elementi inutili
			if(index == N){
				// fine vettore
				enable_timer(1); // altro lampeggio
			} else{
				if(vet[index] == 1){
					// KEY1
					LED_Out(0);
					LED_On(1); // LED1 e LED2 della traccia non esistono sullo schematico, quindi intendo 1 e 2
					LED_Off(2);
				} else if(vet[index] == 2){
					// KEY2
					LED_Out(0);
					LED_On(2);
					LED_Off(1);
				}
			}
			disable_timer(0);
			reset_timer(0);
		}
		else{
			if((counter0%2) == 0) LED_Out(0xFF);
			else LED_Out(0);
			counter0++;
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
		static int counter = 0;
		if(counter == 16){ // 2s = 16*0.125s
			enable_timer(2);
			disable_timer(1);
			reset_timer(1);
		}
		if((counter % 2) == 0){
			// entrambi accesi
			LED_On(1);
			LED_On(2);
		} else LED_Out(0);
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
		if(counter == 48){ // fine -> 3s = 0.0625s*48
			result = compare(vet, vet2, N);
			LED_Out(result);
			disable_timer(2);
			reset_timer(2);
		}
		if((counter%2) == 0) LED_Out(0xFF);
		else LED_Out(0);
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
