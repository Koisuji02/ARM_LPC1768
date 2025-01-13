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

//volatile int flag_start = 0; // faccio partire timer1 quando uso joystick
volatile int stop = 0; // flag per stoppare frase (rinizio solo con pressione KEY2)

void RIT_IRQHandler(void) {
		static int position = 0; // position da usare se joystick (1 = UP; 2 = RIGHT; 3 = LEFT; 4 = DOWN)
		static int debounce = 0; // KEY1
		static int debounce1 = 0; // KEY2
		
		if(stop == 0 && index <= 99){
			// GESTIONE BOTTONI -> devo gestire KEY1 nell'acquisizione
			if((LPC_PINCON->PINSEL4 & (1 << 22)) == 0){ // bottone premuto
				debounce++;
				// INT0 = 10; KEY1 = 11; KEY2 = 12
				if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){   //bottone premuto ma in modalità interrupt con debounce attivo
					reset_RIT();
					switch(debounce){
						case 1: // dopo il time_interval con cui init_RIT (1 colpo, no debounce)
							vet[index] = 4; // 4 = termine frase
							index++;
							stop = 1;
							LED_Out(0xFF);
							enable_timer(2);
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
					NVIC_EnableIRQ(EINT1_IRQn); // cambia pulsante_IRQn se altri pulsanti
					LPC_PINCON->PINSEL4 |= (1<<22);
				}
			}
			
			// GESTIONE JOYSTICK (25 sarebbe il tasto premuto del joystick L3, R3)
		
			if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) { // JOYSTICK UP
				vet[index] = 0; // 0 = .
				index++;
				LED_On(7); // LED11 = 7
				enable_timer(0);
				
			} else if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) { // JOYSTICK RIGHT
				vet[index] = 2; // 2 = cambio lettera
				index++;
			} else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) { // JOYSTICK LEFT
				vet[index] = 3; // 3 = spazio
				index++;
			} else if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) { // JOYSTICK DOWN
				vet[index] = 1; // 1 = -
				index++;
				LED_On(4);
				LED_On(5);
				LED_On(6);
				LED_On(7);
				enable_timer(1);
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
		} else{
			if((LPC_PINCON->PINSEL4 & (1 << 24)) == 0){ // bottone premuto
				debounce1++;
				// INT0 = 10; KEY1 = 11; KEY2 = 12
				if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){   //bottone premuto ma in modalità interrupt con debounce attivo
					reset_RIT();
					switch(debounce1){
						case 1: // dopo il time_interval con cui init_RIT (1 colpo, no debounce)
							index = 0;
							stop = 0;
							break;
						default:
							// se non devo fare niente apparte debouncing, non scrivo niente
							break;
					}
				}	else{
					debounce1 = 0;
					// disable_RIT();
					// reset_RIT();
					// INT0 = 20; KEY1 = 22; KEY2 = 24
					NVIC_EnableIRQ(EINT2_IRQn); // cambia pulsante_IRQn se altri pulsanti
					LPC_PINCON->PINSEL4 |= (1<<24);
				}
			}
		}
	LPC_RIT->RICTRL |= 0x1; // Clear interrupt flag (non mi serve qua, altrimenti fa solo 1 mossa)
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
