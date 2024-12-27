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
extern volatile int flag_start; // faccio partire timer1 quando uso joystick

void functVictory(){
	victory = 1;								// segno vittoria
	disable_timer(1);						// stoppo timer1
	disable_timer(2);
	disable_timer(0);
	LCD_Clear(Black);
	GUI_Text(7*10+10, 13*10+40, (uint8_t *) " Victory! ", Green, Black);
	disable_RIT();
	while(1); // ciclo infinito (stessa cosa del GameOver)
	return;
}

void RIT_IRQHandler(void) {
    static int flagStop = 0;  // Evito di ridisegnare Pacman più volte
    static int position = 0;
    static int victoryFlag = 0;
    static int debounceState = 0; // Stato debounce: 0 = rilascio, 1 = pressione in corso

    if (gameOver != 1 && victory != 1) {
        // Verifica stato del bottone (pressione)
        if ((LPC_GPIO2->FIOPIN & (1 << 10)) == 0) { // Bottone INT0 premuto
            if (debounceState == 0) { // Bottone rilevato per la prima volta
                debounceState = 1;   // Passa allo stato di pressione
                if (pause == 0) {
                    pause = 1;
                    disable_timer(1);
                    disable_timer(2); // Stop anche generazione dei pillarM
                    GUI_Text(9 * 10 + 11, 12 * 10 + 32, (uint8_t *)"PAUSE", Yellow, Blue);
                } else {
                    pause = 0;
                    enable_timer(1);
                    enable_timer(2);
                    GUI_Text(9 * 10 + 11, 12 * 10 + 32, (uint8_t *)"     ", Blue, Blue);
                    cancellaCella(10, 11);
                    cancellaCella(11, 11);
                }
            }
        } else { // Bottone rilasciato
            debounceState = 0; // Ritorna allo stato di rilascio
            LPC_PINCON->PINSEL4 |= (1 << 20); // Ripristina il pin come interrupt
            NVIC_EnableIRQ(EINT0_IRQn);
        }

        if (pause == 0) {
            if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) { // JOYSTICK UP
							position = 1;
							if (flag_start == 0) {
								flag_start = 1;
								enable_timer(1);
								enable_timer(2);
								attivaFantasma();
							}
					} else if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) { // JOYSTICK RIGHT
							position = 2;
							if (flag_start == 0) {
								flag_start = 1;
								enable_timer(1);
								enable_timer(2);
								attivaFantasma();
							}
					} else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) { // JOYSTICK LEFT
							position = 3;
							if (flag_start == 0) {
								flag_start = 1;
								enable_timer(1);
								enable_timer(2);
								attivaFantasma();
							}
					} else if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) { // JOYSTICK DOWN
							position = 4;
							if (flag_start == 0) {
								flag_start = 1;
								enable_timer(1);
								enable_timer(2);
								attivaFantasma();
							}
					}

					switch (position) {
							case 1: // UP
									if (schermo[i_start][j_start - 1] != -1) { // se non è muro
											cancellaCella(i_start, j_start); // cancello pacman
											if(schermo[i_start][j_start - 1] != 0){ // se c'è un pillar (quindi non è già vuota)
												score += schermo[i_start][j_start - 1]; // score += cella destinazione
												if((score%1000)-(local_score%1000) == 50) attivaFrightenedMode();
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
											if((score%1000)-(local_score%1000) == 50) attivaFrightenedMode();
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
												if((score%1000)-(local_score%1000) == 50) attivaFrightenedMode();
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
											if((score%1000)-(local_score%1000) == 50) attivaFrightenedMode();
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
												if((score%1000)-(local_score%1000) == 50) attivaFrightenedMode();
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
												if((score%1000)-(local_score%1000) == 50) attivaFrightenedMode();
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
					
					// gestione mangio fantasma in Frightened Mode
					if(j_start == Fy_start && i_start == Fx_start &&  frightenedFlag == 1){
						score += 100;
						local_score += 100;
						mangiatoFantasma = 1;
						inizializzato = 0;			// per non farlo muovere in chase mode fino a che non ho finito i 3s nel recinto
						disattivaFrightenedMode();
						
						LPC_TIM3->MCR &= ~(1 << 0);  // Disabilita interrupt per MR0
						LPC_TIM3->MCR |= (1 << 3);   // Abilita interrupt per MR1
						//reset_timer(3); // Resetta il timer
						counter_tim0_MR1 = 3; // 3s per il ricollocamento del fantasma
						enable_timer(3);
					}
					
					// gestione fantasma mi mangia in Frightened Mode
					else if(j_start == Fy_start && i_start == Fx_start &&  frightenedFlag == 0){
						life -= 1;
						PutChar(117, 283, life + 48 , Red, Black); // stampo nuova vita
						flagStoppato = 1; // segno che fantasma ha appena mangiato pacman e fino che non si sposta pacman non faccio nulla
						//Fx_start_stopped = Fx_start;
						//Fy_start_stopped = Fy_start;
						//Fx_start = 0; // fingo che il fantasma non sia qua, ma sia in una casella dove non può stare
						//Fy_start = 0;
						disegnaFantasma(Fx_start, Fy_start, Red);
					}
					
					if(life == 0){
						callerGameOver = 1;
						functGameOver();
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
						functVictory();
					}
				}
    }
    LPC_RIT->RICTRL |= 0x1; // Clear interrupt flag
    return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
