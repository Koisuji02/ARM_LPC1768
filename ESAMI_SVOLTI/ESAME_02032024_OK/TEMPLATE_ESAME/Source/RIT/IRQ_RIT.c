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

//unsigned char extractValue(int value);
//unsigned char get_and_sort(unsigned char* vet, unsigned char val, int index);
//int disabilitoKey1 = 0;

void svuotaVet(char* vet, unsigned int index);
void svuotaVetU(unsigned char* vet, unsigned int index);
//unsigned int totale_pressioni_con_filtro(unsigned char VETT[], unsigned int  numero_misure, unsigned char MAX, unsigned char MIN);
unsigned int compress (unsigned char VETT[], const unsigned int num, char RES[]);

int disattivato =  0;
int turno = 0;

void RIT_IRQHandler(void) {
	
		static int position = 0; // da usare se joystick (1 = UP; 2 = RIGHT; 3 = LEFT; 4 = DOWN)
	
		static int debounce0 = 0;
		static int debounce1 = 0;
		static int debounce2 = 0;
	
		// GESTIONE BOTTONE INT0
		if((LPC_PINCON->PINSEL4 & (1 << 20)) == 0){ // bottone premuto
				debounce0++;
				// INT0 = 10; KEY1 = 11; KEY2 = 12
				if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){   //bottone premuto ma in modalità interrupt con debounce attivo
					reset_RIT();
					switch(debounce0){
						case 1: // dopo il time_interval con cui init_RIT (1 colpo, no debounce)
							
							// LETTURA VALORE TC
							/*disable_timer(1);
							value_tmp = LPC_TIM1->TC;
							enable_timer(1);*/
							
							// AZIONE INT0
							
							break;
						default:
							// se non devo fare niente apparte debouncing, non scrivo niente
							break;
					}
				}	else{
					debounce0 = 0;
					// de-commento disable e reset solo senza joystick (altrimenti deve essere sempre attivo il RIT per rilevare le nuove direzioni o le direzioni già avviate [es. pacman])
					//disable_RIT();
					//reset_RIT();
					
					// AZIONE DA FARE AL RILASCIO DI INT0
					
					// INT0 = 20; KEY1 = 22; KEY2 = 24
					NVIC_EnableIRQ(EINT0_IRQn); // cambia pulsante_IRQn se altri pulsanti
					LPC_PINCON->PINSEL4 |= (1<<20);
				}
			}
	
	
		// GESTIONE BOTTONE KEY1
		if((LPC_PINCON->PINSEL4 & (1 << 22)) == 0 && disattivato == 0){ // bottone premuto
			//if(disattivato == 0){
				debounce1++;
				// INT0 = 10; KEY1 = 11; KEY2 = 12
				if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){   //bottone premuto ma in modalità interrupt con debounce attivo
					reset_RIT();
					switch(debounce1){
						case 1: // dopo il time_interval con cui init_RIT (1 colpo, no debounce)
							
							// LETTURA VALORE TC
							/*disable_timer(1);
							value_tmp = LPC_TIM1->TC;
							enable_timer(1);*/
							
							// AZIONE KEY1
							var++;
							enable_timer(0);
							if((turno == 2 || turno == 0) && index <= N){
								turno = 1;
								vet[index] = var;
								index++;
							}
							
							break;
						default:
							// se non devo fare niente apparte debouncing, non scrivo niente
							break;
					}
				}	else{
					debounce1 = 0;
					// de-commento disable e reset solo senza joystick (altrimenti deve essere sempre attivo il RIT per rilevare le nuove direzioni o le direzioni già avviate [es. pacman])
					//disable_RIT();
					//reset_RIT();
					
					// AZIONE DA FARE AL RILASCIO DI KEY1
					
					// INT0 = 20; KEY1 = 22; KEY2 = 24
					NVIC_EnableIRQ(EINT1_IRQn); // cambia pulsante_IRQn se altri pulsanti
					LPC_PINCON->PINSEL4 |= (1<<22);
				}
			//}
		}
		
		// GESTIONE BOTTONE KEY2
		if((LPC_PINCON->PINSEL4 & (1 << 24)) == 0 && disattivato == 0){ // bottone premuto
			//if(debounce1 == 0 && disattivato == 0){ // ho aggiunto debounce1 == 0 apposta per "a valle"
				debounce2++;
				// INT0 = 10; KEY1 = 11; KEY2 = 12
				if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){   //bottone premuto ma in modalità interrupt con debounce attivo
					reset_RIT();
					switch(debounce2){
						case 1: // dopo il time_interval con cui init_RIT (1 colpo, no debounce)
							
							// LETTURA VALORE TC
							/*disable_timer(1);
							value_tmp = LPC_TIM1->TC;
							enable_timer(1);*/
							
							// AZIONE KEY2
							var++;
							enable_timer(0);
							if((turno == 1 || turno == 0) && index <= N){
								turno = 2;
								vet[index] = var;
								index++;
							}
						
							break;
						default:
							// se non devo fare niente apparte debouncing, non scrivo niente
							break;
					}
				}	else{
					debounce2 = 0;
					// de-commento disable e reset solo senza joystick (altrimenti deve essere sempre attivo il RIT per rilevare le nuove direzioni o le direzioni già avviate [es. pacman])
					//disable_RIT();
					//reset_RIT();
					
					// AZIONE DA FARE AL RILASCIO DI KEY2
					
					// INT0 = 20; KEY1 = 22; KEY2 = 24
					NVIC_EnableIRQ(EINT2_IRQn); // cambia pulsante_IRQn se altri pulsanti
					LPC_PINCON->PINSEL4 |= (1<<24);
				}
			//}
		}
		

		// GESTIONE JOYSTICK (25 sarebbe il tasto premuto del joystick L3, R3)
	
		if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) { // JOYSTICK UP
				// AZIONE UP
		} else if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) { // JOYSTICK RIGHT
				// AZIONE RIGHT
		} else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) { // JOYSTICK LEFT
				// AZIONE LEFT
		} else if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0 && disattivato == 0) { // JOYSTICK DOWN
				// AZIONE DOWN
				result = compress(vet, index, res);
				enable_timer(1);		// per visualizzazione di res sui led
				disattivato = 1;
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

	LPC_RIT->RICTRL |= 0x1; // Clear interrupt flag (lo tolgo se devo fare più di 1 mossa con 1 singolo input del RIT [es pacman che va avanti per la sua strada dopo che imposto position])
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
