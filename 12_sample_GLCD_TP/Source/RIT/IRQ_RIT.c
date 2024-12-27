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
#include "GLCD/GLCD.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

#define N 22
volatile int flag_start = 0; // faccio partire timer1 quando uso joystick

void RIT_IRQHandler(void) {
    static int flagStop = 0; // evito di ridisegnare pacman più volte dopo la 1^ volta che trovo un muro
		static int position = 0;
		static int victoryFlag = 0;
		static int debounce = 0;
		
		debounce++;
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){   //bottone premuto EINT0 ma in modalità interrupt con debounce attivo	
			switch(debounce){
				case 1:
					if(pause == 0){
						pause = 1;
						disable_timer(1);
						GUI_Text(9*10+11, 12*10+32, (uint8_t *) "PAUSE", Yellow, Blue);
					} else{
						pause = 0;
						enable_timer(1);
						GUI_Text(9*10+11, 12*10+32, (uint8_t *) "     ", Blue, Blue);
						cancellaCella(10, 11);
						cancellaCella(11, 11);
					}
					break;
				default:
					// se non devo fare niente apparte debouncing, non scrivo niente
					break;
			}
		}	else{
			debounce = 0;
			NVIC_EnableIRQ(EINT0_IRQn);
			//LPC_PINCON->PINSEL4 |= (1<<20);
			/*if(pause==1){
				disable_RIT();
				reset_RIT();
			}*/
	}
		
	if(pause == 0){
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
				case 1: // UP
						if (schermo[i_start][j_start - 1] != -1) { // se non è muro
								cancellaCella(i_start, j_start); // cancello pacman
								if(schermo[i_start][j_start - 1] != 0){ // se c'è un pillar (quindi non è già vuota)
									score += schermo[i_start][j_start - 1]; // score += cella destinazione
									local_score += schermo[i_start][j_start - 1]; // local_score += cella destinazione (uso per le vite)
									cancellaCella(i_start, j_start - 1);	// cella destinazione svuotata
									pillarsRemaining -=1;
								}
								disegnaPacmanUp(i_start, j_start - 1); // disegno pacman in cella destinazione
								j_start -= 1;
						} else if (flagStop == 0) {
								cancellaCella(i_start, j_start);
								disegnaPacmanUp(i_start, j_start);
								flagStop = 1;
						}
						break;

				case 2: // RIGHT
						if((i_start) == N-1){ // teletrasporto
							cancellaCella(i_start, j_start);
							if(schermo[0][j_start] != 0){ // se c'è un pillar (quindi non è già vuota)
								score += schermo[0][j_start]; // score += cella destinazione
								local_score += schermo[0][j_start]; // local_score += cella destinazione (uso per le vite)
								cancellaCella(0, j_start);	// cella destinazione svuotata
								pillarsRemaining -=1;
							}
							disegnaPacmanLeft(0, j_start);
							i_start = 0;
						}
						if (schermo[i_start + 1][j_start] != -1) {
								cancellaCella(i_start, j_start);
								if(schermo[i_start + 1][j_start] != 0){ // se c'è un pillar (quindi non è già vuota)
									score += schermo[i_start + 1][j_start]; // score += cella destinazione
									local_score += schermo[i_start + 1][j_start]; // local_score += cella destinazione (uso per le vite)
									cancellaCella(i_start + 1, j_start);	// cella destinazione svuotata
									pillarsRemaining -=1;
								}
								disegnaPacmanRight(i_start + 1, j_start);
								i_start += 1;
						} else if (flagStop == 0) {
								cancellaCella(i_start, j_start);
								disegnaPacmanRight(i_start, j_start);
								flagStop = 1;
						}
						break;

				case 3: // LEFT
						if((i_start) == 0){ // teletrasporto
							cancellaCella(i_start, j_start);
							if(schermo[N-1][j_start] != 0){ // se c'è un pillar (quindi non è già vuota)
								score += schermo[N-1][j_start]; // score += cella destinazione
								local_score += schermo[N-1][j_start]; // local_score += cella destinazione (uso per le vite)
								cancellaCella(N-1, j_start);	// cella destinazione svuotata
								pillarsRemaining -=1;
							}
							disegnaPacmanLeft(N-1, j_start);
							i_start = N-1;
						}
						if (schermo[i_start - 1][j_start] != -1) {
								cancellaCella(i_start, j_start);
								if(schermo[i_start - 1][j_start] != 0){ // se c'è un pillar (quindi non è già vuota)
									score += schermo[i_start - 1][j_start]; // score += cella destinazione
									local_score += schermo[i_start - 1][j_start]; // local_score += cella destinazione (uso per le vite)
									cancellaCella(i_start - 1, j_start);	// cella destinazione svuotata
									pillarsRemaining -=1;
								}
								disegnaPacmanLeft(i_start - 1, j_start);
								i_start -= 1;
						} else if (flagStop == 0) {
								cancellaCella(i_start, j_start);
								disegnaPacmanLeft(i_start, j_start);
								flagStop = 1;
						}
						break;

				case 4: // DOWN
						if (schermo[i_start][j_start + 1] != -1) {
								cancellaCella(i_start, j_start);
								if(schermo[i_start][j_start + 1] != 0){ // se c'è un pillar (quindi non è già vuota)
									score += schermo[i_start][j_start + 1]; // score += cella destinazione
									local_score += schermo[i_start][j_start + 1]; // local_score += cella destinazione (uso per le vite)
									cancellaCella(i_start, j_start + 1);	// cella destinazione svuotata
									pillarsRemaining -=1;
								}
								disegnaPacmanDown(i_start, j_start + 1);
								j_start += 1;
						} else if (flagStop == 0) {
								cancellaCella(i_start, j_start);
								disegnaPacmanDown(i_start, j_start);
								flagStop = 1;
						}
						break;

				default:
						break;
		}

		// Aggiorno lo score
		PutChar(180, 20, (score / 1000 + 48), Red, Black);
		PutChar(190, 20, ((score / 100) % 10 + 48), Red, Black);
		PutChar(200, 20, ((score / 10) % 10 + 48), Red, Black);
		PutChar(210, 20, (score % 10 + 48), Red, Black);
		
		// gestione vite
		if(local_score >= 1000){
			life += 1;
			local_score -= 1000;
			PutChar(117, 283, life + 48 , Red, Black); // stampo nuova vita
		}
		
		// gestione victory
		if(pillarsRemaining == 0){
			victory = 1;								// segno vittoria
			disable_timer(1);						// stoppo timer1
			disable_RIT();
			LCD_Clear(Black);
			GUI_Text(7*10+10, 13*10+40, (uint8_t *) " Victory! ", Green, Black);
		}
	}
	//LPC_RIT->RICTRL |= 0x1; // Clear interrupt flag (non mi serve qua, altrimenti fa solo 1 mossa)
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
