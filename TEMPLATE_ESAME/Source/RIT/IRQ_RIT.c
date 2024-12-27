/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int flag_start = 0; // faccio partire timer1 quando uso joystick

void RIT_IRQHandler(void) {
		static int position = 0; // position da usare se joystick (1 = UP; 2 = RIGHT; 3 = LEFT; 4 = DOWN)
		static int debounce = 0;
	
		// GESTIONE BOTTONI
	
		debounce++;
		// INT0 = 10; KEY1 = 11; KEY2 = 12
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){   //bottone premuto EINT0 ma in modalità interrupt con debounce attivo
			// reset_RIT();
			switch(debounce){
				case 1: // dopo il time_interval con cui init_RIT (1 colpo, no debounce)
					// FAI AZIONE
					break;
				default:
					// se non devo fare niente apparte debouncing, non scrivo niente
					break;
			}
		}	else{
			debounce = 0;
			// disable_RIT();
			// reset_RIT();
			// INT0 = 20; KEY1 = 22; KEY2 = 24
			NVIC_EnableIRQ(EINT0_IRQn); // cambia pulsante_IRQn se altri pulsanti
			LPC_PINCON->PINSEL4 |= (1<<20);
	}
		

		// GESTIONE JOYSTICK (25 sarebbe il tasto premuto del joystick L3, R3)
	
		if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) { // JOYSTICK UP
				position = 1;
				if (flag_start == 0) {
					flag_start = 1;
					enable_timer(1);
				}
		} else if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) { // JOYSTICK RIGHT
				position = 2;
				if (flag_start == 0) {
					flag_start = 1;
					enable_timer(1);
				}
		} else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) { // JOYSTICK LEFT
				position = 3;
				if (flag_start == 0) {
					flag_start = 1;
					enable_timer(1);
				}
		} else if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) { // JOYSTICK DOWN
				position = 4;
				if (flag_start == 0) {
					flag_start = 1;
					enable_timer(1);
				}
		}

		switch (position) {
				case 1:
						// UP
						break;

				case 2:
						// RIGHT
						break;

				case 3:
						// LEFT
						break;

				case 4:
						// DOWN
						break;

				default:
						break;
		}

	//LPC_RIT->RICTRL |= 0x1; // Clear interrupt flag (non mi serve qua, altrimenti fa solo 1 mossa)
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
