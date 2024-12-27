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
#include "GLCD/GLCD.h" 
#include "GLCD/AsciiLib.h"
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
extern int victory;


void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1) // MR0
	{ 
		// your code
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

int gameOver = 0;
void functGameOver(){
	
	if(callerGameOver == 1){ // RIT
		disable_timer(2);
		disable_timer(0);
		disable_timer(3);
		disable_timer(1);
		gameOver = 1;
		LCD_Clear(Black);
		GUI_Text(7*10+10, 13*10+40, (uint8_t *) " Game Over! ", Red, Black);
		disable_RIT();
	}
	else if(callerGameOver == 2){ // timer1
		disable_RIT();
		disable_timer(2);
		disable_timer(0);
		disable_timer(3);
		gameOver = 1;
		LCD_Clear(Black);
		GUI_Text(7*10+10, 13*10+40, (uint8_t *) " Game Over! ", Red, Black);
		disable_timer(1);
	}
	
	LPC_TIM1->IR = 1;			//clear interrupt flag
	while(1);				// così non si disegna più niente sulla scheda (neanche per errore)
	return;
}

void TIMER1_IRQHandler (void)
{
	static int counter_schermo = 60;
	static int counter = 240;		// 60s come tempo di gioco disponibile (2:1)
	// counter_schermo > 40s (ovvero counter > 160) voglio 1/4 velocità
	// dopo 20s (counter_schermo tra 20 e 40s, ovvero tra 80 e 160 counter) voglio %2 (1/2 velocità)
	// ultimi 20 s(counter_schermo < 20, ovvero counter < 80) voglio velocità intera (ogni count 1 movimento)
	static int turno = 0; // per gestire 1/4 di count
	
	if(LPC_TIM1->IR & 1) // MR0
	{ 
		int i;
		counter--;
		turno++;
		if(counter == 0){		// quando arrivo a 0
			if(victory == 0){
				// gestire fine gioco (stoppare tutto come nel victory)
				callerGameOver = 2;
				functGameOver();
			}
			disable_timer(1);
		}
		
		if(counter >= 160){
			if(turno == 4){
				if(frightenedFlag == 0 && inizializzato == 1 && flagStoppato == 0)
					muoviChaseMode(); // solo se non sono in frightened mode
			}
		}
		else if(counter >= 80 && counter < 160){
			if(counter%2==0){
				if(frightenedFlag == 0 && inizializzato == 1 && flagStoppato == 0)
					muoviChaseMode();
			}
		}
		else{ // ultimi 20 secondi
			if(frightenedFlag == 0 && inizializzato == 1 && flagStoppato == 0)
				muoviChaseMode();
		}
		
		if(turno == 4){ // voglio aggiornare il tempo e togliere il blocco con velocità 1/4 rispetto al timer (così ho 1s)
			if(flagStoppato == 1) { // se bloccato (fantasma rosso ha appena mangiato pacman), do 1 ciclo di sblocco
				disegnaFantasma(Fx_start, Fy_start, Red);
				flagStoppato = 0; // sblocco il ciclo dopo
			}
			turno = 0; // rinizializzo dopo i 4 count
			counter_schermo--;
			PutChar(40, 20, (counter_schermo/10 + 48) , Red, Black);
			PutChar(50, 20, (counter_schermo%10 + 48) , Red, Black);
			PutChar(60, 20, (int)'s' , Red, Black);
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
volatile int pillarsM_generated = 0;
volatile int time_elapsed = 0; // per calcolare i 50 secondi (non voglio generare pillars negli ultimi 10 secondi di gioco)

// gestisce la generazione casuale dei pillars medi
void TIMER2_IRQHandler(void)
{	
	if(gameOver!=1 && victory != 1){
		if (LPC_TIM2->IR & 1) { // MR0
			
			time_elapsed++; // Incrementa il tempo trascorso
			
			if (pillarsM_generated < 6) {
				if ((rand() % 10) < 3) { // Probabilità casuale di generare un pilastro
					int i_rand = rand() % N;
					int j_rand = rand() % N;

					// Trova una posizione valida
					while (schermo[i_rand][j_rand] != 10) {
						i_rand = rand() % N;
						j_rand = rand() % N;
					}
					disegnaPillarM(i_rand, j_rand);
					pillarsM_generated++;
				}
			}
			
			if (time_elapsed >= 50 || pillarsM_generated >= 6) {
				disable_timer(2); // Disabilita il timer se abbiamo finito
			}
			LPC_TIM2->IR = 1; // Clear interrupt flag
		}
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
void TIMER3_IRQHandler (void)
{
  //static int counter = 10;		// 10s in frightened mode
	//static int counter_MR1 = 3;	// 3s in frightened mode
	if(LPC_TIM3->IR & 1) // MR0
	{ 
		counter_tim0--;
		
		muoviFrightenedMode(); // 1 movimento ogni secondo dei 10 secondi
		
		if(counter_tim0 == 0){		// quando arrivo a 0
			if(mangiatoFantasma == 0 && frightenedFlag == 1){ // non ho mangiato il fantasma e la frightened mode ancora attiva
				disattivaFrightenedMode();
			}
		}
		LPC_TIM3->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM3->IR & 2)
	{ // MR1
		counter_tim0_MR1--;
		if(counter_tim0_MR1 == 0){		// quando arrivo a 0
			//counter_tim0_MR1 = 3; // prossima iterazione
			inizializzaFantasmi();
			attivaFantasma();
		}
		LPC_TIM3->IR = 2;			// clear interrupt flag 
	}
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
