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
		// your code
		var++;
		
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

char var_tmp = 0;

void TIMER1_IRQHandler (void)
{
	if(LPC_TIM1->IR & 1) // MR0
	{ 
		// your code
		static int counter = 0;
		var_tmp = (((var>>24)&0xFF)|((var>>16)&0xFF)) ^ (((var>>8)&0xFF)&((var)&0xFF));
		if((counter % 2) == 0) LED_Out(var_tmp);
		else LED_Out(~var_tmp);
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
		if(disattivato == 0){ // solo se non sono disattivato prendo valori
			vet[index] = var;
			index++;
			if(index == N){ // vettore pieno
				result = contatore_inrange(vet, N);
				disattivato = 1;
				disable_timer(1); // stoppo lampeggio delle pressioni altrimenti non si capisce result
				reset_timer(1);
				LED_Out(result);
				enable_timer(3); // dopo 6s tolgo result
				disable_timer(2);
				reset_timer(2);
			}
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
		LED_Out(0); // tolgo result dai LED
		index = 0;
		var = 0;
		svuotaVet(vet, N);
		disattivato = 0;
		enable_timer(2); // riparto con il sistema completo
		disable_timer(3);
		reset_timer(3);
		
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
