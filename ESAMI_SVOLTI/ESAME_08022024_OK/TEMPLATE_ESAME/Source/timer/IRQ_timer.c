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


char shift_tmp1 = 0;
char shift_tmp2 = 0;
char finale = 0;

void TIMER1_IRQHandler (void)
{
	if(LPC_TIM1->IR & 1) // MR0
	{ 
		// your code
		static int turn = 0;
		if((turn % 2) == 0){
			shift_tmp1 = (var>>24) & 0xFF;
			shift_tmp2 = (var>>16) & 0xFF;
			finale = shift_tmp1 | shift_tmp2; // OR tra 31-24 e 23-16
			shift_tmp1 = (var>>8) & 0xFF;
			shift_tmp2 = var & 0xFF;
			shift_tmp1 = shift_tmp1 & shift_tmp2; // AND tra 15-8 e 7-0
			finale = finale ^ shift_tmp1; // XOR tra i 2 blocchi
			LED_Out(finale);
		} else{
			LED_Out(!finale);
		}
		turn++;
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

unsigned char contatore_inrange(unsigned int VETT[], const unsigned int num);

void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1) // MR0 -> lavoro con MR0 per il debounce
	{
		// your code
		vett[index] = var;
		index++;
		if(index == N){ // vettore saturo
			result = contatore_inrange(vett, N);
			LED_Out(result);
			enable_timer(3);
			disattivato = 1;
			disable_timer(2);
			reset_timer(2);
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
		LED_Out(0);
		var = 0;
		svuotaVet(vett, index);
		index = 0;
		disattivato = 0; // riavvio con disattivato = 0
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
