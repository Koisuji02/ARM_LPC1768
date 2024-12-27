/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "GLCD/AsciiLib.h"
#include "button_EXINT/button.h"
#include <stdint.h>



#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

#define N 22
#define N_PILLARS 240

int schermo[N][N];
int i_start = 10;
int j_start = 13;
int score = 0;				// punteggio da visualizzare (inizialmente 0)
int local_score = 0;	// parallelo a score, ma si azzera ogni 1000 punti in modo da poter segnare l'incremento delle vite
int life = 1;				// inizialmente 1 vita (aumento ogni 1000 punti)
int victory = 0;			// se victory = 1 vinto, altrimenti game over a fine timer
int pillarsRemaining = N_PILLARS;
int pause;


// -1 = muro
// 0 = regolari
// N = pillars (N)

void disegnaMuro(int i, int j){ // per cancellare pacman e pillars
	if(schermo[i][j] == -1 || i >= N || j >= N) return;
	int x = i*10+10, y = j*10+40;
	LCD_DrawLine(x, y, x+9, y, Blue);
	LCD_DrawLine(x, y+1, x+9, y+1, Blue);
	LCD_DrawLine(x, y+2, x+9, y+2, Blue);
	LCD_DrawLine(x, y+3, x+9, y+3, Blue);
	LCD_DrawLine(x, y+4, x+9, y+4, Blue);
	LCD_DrawLine(x, y+5, x+9, y+5, Blue);
	LCD_DrawLine(x, y+6, x+9, y+6, Blue);
	LCD_DrawLine(x, y+7, x+9, y+7, Blue);
	LCD_DrawLine(x, y+8, x+9, y+8, Blue);
	LCD_DrawLine(x, y+9, x+9, y+9, Blue);
	schermo[i][j] = -1;
	return;
}

void cancellaMuro(int i, int j){ // per ancellare muro (come cancella Cella)
	if(i >= N || j >= N) return;
	int x = i*10+10, y = j*10+40;
	LCD_DrawLine(x, y, x+9, y, Black);
	LCD_DrawLine(x, y+1, x+9, y+1, Black);
	LCD_DrawLine(x, y+2, x+9, y+2, Black);
	LCD_DrawLine(x, y+3, x+9, y+3, Black);
	LCD_DrawLine(x, y+4, x+9, y+4, Black);
	LCD_DrawLine(x, y+5, x+9, y+5, Black);
	LCD_DrawLine(x, y+6, x+9, y+6, Black);
	LCD_DrawLine(x, y+7, x+9, y+7, Black);
	LCD_DrawLine(x, y+8, x+9, y+8, Black);
	LCD_DrawLine(x, y+9, x+9, y+9, Black);
	schermo[i][j] = 0;
	return;
}

void disegnaForma(int i, int j, char tipo){ // i, j = posizioni angolo alto sinistra
	switch(tipo){
		case 'A':												// muro semplice (quadrato)
			disegnaMuro(i, j);
			break;
		case 'B':												// forma a T
			disegnaMuro(i, j);
			disegnaMuro(i+1, j);
			disegnaMuro(i+2, j);
			disegnaMuro(i+1, j+1);
			break;
		case 'C':												// pollicione
			disegnaMuro(i, j);
			disegnaMuro(i, j+1);
			disegnaMuro(i, j+2);
			disegnaMuro(i-1, j+1);
			disegnaMuro(i-1, j+2);
			break;
		case 'D':												// pistola larga
			disegnaMuro(i, j);
			disegnaMuro(i, j+1);
			disegnaMuro(i, j+2);
			disegnaMuro(i, j+3);
			disegnaMuro(i, j+4);
			disegnaMuro(i+1, j+2);
			disegnaMuro(i+1, j+3);
			break;
		case 'E':												// assalto
			disegnaMuro(i, j);
			disegnaMuro(i, j+1);
			disegnaMuro(i, j+2);
			disegnaMuro(i, j+3);
			disegnaMuro(i, j+4);
			disegnaMuro(i, j+5);
			disegnaMuro(i-1, j+2);
			break;
		case 'F':												// castello
			disegnaMuro(i, j);
			disegnaMuro(i, j+1);
			disegnaMuro(i+2, j+1);
			disegnaMuro(i+4, j+1);
			disegnaMuro(i+4, j);
			break;
		case 'G':												// barra (con buchi per avere 248 spazi vuoti)
			disegnaMuro(i, j);
			disegnaMuro(i, j+1);
			disegnaMuro(i, j+2);
			disegnaMuro(i, j+3);
			disegnaMuro(i, j+5);
			break;
		case 'H':												// angolo (per formare quadrato centrale)
			disegnaMuro(i, j);
			disegnaMuro(i, j+1);
			disegnaMuro(i+1, j);
			break;
		
		default:
			break;
	}
	return;
}

// uso disegnaFormaQuadrante per specchiare le forme nei quadranti
void disegnaFormaQuadrante(int i, int j, char tipo, int quadrante) {
	int x = i, y = j;
	switch (quadrante) {
		case 1: // Alto sx (nessuna modifica)
			break;
		case 2: // Alto dx (simmetria rispetto all'asse y)
			y = N - 1 - j;
			break;
		case 3: // Basso sx (simmetria rispetto all'asse x)
			x = N - 1 - i;
			break;
		case 4: // Basso dx (simmetria rispetto a x e y)
			x = N - 1 - i;
			y = N - 1 - j;
			break;
		default:
			return;
	}
	disegnaForma(x, y, tipo);
	return;
}

// disegno il singolo quadrante
void disegnaQuadrante() {
	int i, j;
	// Disegna il quadrante base (alto sx)
	disegnaForma(2, 2, 'B');
	disegnaForma(1, 4, 'A');
	disegnaForma(6, 2, 'A');
	disegnaForma(3, 5, 'C');
	disegnaForma(5, 4, 'D');
	disegnaForma(8, 2, 'E');
	disegnaForma(10, 2, 'G');
	disegnaForma(3, 9, 'F');
	disegnaForma(1, 9, 'A');
	disegnaForma(9, 9, 'H');

	// Replica nei quadranti
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (schermo[i][j] == -1) { // Se c'è un muro nel quadrante base
				disegnaFormaQuadrante(i, j, 'A', 2); // Alto dx
				disegnaFormaQuadrante(i, j, 'A', 3); // Basso sx
				disegnaFormaQuadrante(i, j, 'A', 4); // Basso dx
			}
		}
	}
	// blocchi a mano per avere da 248 a 246 blocchi liberi per i pillars e i 4 fantasmi + pacman (+ quello affianco vuoto)
	disegnaMuro(6,10);
	disegnaMuro(6,11);
	return;
}

void disegnaPacmanLeft(int i, int j){ // coordinate fittizie della mia matrice 20x20
	if(i >= N || j >= N) return;
	int x = i*10+10, y = j*10+40;					// pixel effettivi con offset (laterale e verticale per avere bene lo schermo)
	LCD_DrawLine(x+3, y, x+6, y, Yellow);
	LCD_DrawLine(x+2, y+1, x+7, y+1, Yellow);
	LCD_DrawLine(x+1, y+2, x+8, y+2, Yellow);
	LCD_DrawLine(x+2, y+3, x+9, y+3, Yellow);
	LCD_DrawLine(x+4, y+4, x+9, y+4, Yellow);
	LCD_DrawLine(x+6, y+5, x+9, y+5, Yellow);
	LCD_DrawLine(x+4, y+6, x+9, y+6, Yellow);
	LCD_DrawLine(x+2, y+7, x+9, y+7, Yellow);
	LCD_DrawLine(x+1, y+8, x+8, y+8, Yellow);
	LCD_DrawLine(x+2, y+9, x+7, y+9, Yellow);
	return;
}

void disegnaPacmanRight(int i, int j){ // coordinate fittizie della matrice 20x20
	if(i >= N || j >= N) return;
	int x = i*10+10, y = j*10+40; // pixel effettivi con offset (laterale e verticale per avere bene lo schermo)
	LCD_DrawLine(x+4, y, x+7, y, Yellow);
	LCD_DrawLine(x+3, y+1, x+8, y+1, Yellow);
	LCD_DrawLine(x+2, y+2, x+9, y+2, Yellow);
	LCD_DrawLine(x+1, y+3, x+8, y+3, Yellow);
	LCD_DrawLine(x+1, y+4, x+6, y+4, Yellow);
	LCD_DrawLine(x+1, y+5, x+4, y+5, Yellow);
	LCD_DrawLine(x+1, y+6, x+6, y+6, Yellow);
	LCD_DrawLine(x+1, y+7, x+8, y+7, Yellow);
	LCD_DrawLine(x+2, y+8, x+9, y+8, Yellow);
	LCD_DrawLine(x+3, y+9, x+8, y+9, Yellow);
	return;
}


void disegnaPacmanUp(int i, int j){ // Bocca rivolta verso l'alto
	if(i >= N || j >= N) return;
	int x = i*10+10, y = j*10+40;
	LCD_DrawLine(x, y+3, x, y+6, Yellow);
	LCD_DrawLine(x+1, y+2, x+1, y+7, Yellow);
	LCD_DrawLine(x+2, y+1, x+2, y+8, Yellow);
	LCD_DrawLine(x+3, y+2, x+3, y+9, Yellow);
	LCD_DrawLine(x+4, y+4, x+4, y+9, Yellow);
	LCD_DrawLine(x+5, y+6, x+5, y+9, Yellow);
	LCD_DrawLine(x+6, y+4, x+6, y+9, Yellow);
	LCD_DrawLine(x+7, y+2, x+7, y+9, Yellow);
	LCD_DrawLine(x+8, y+1, x+8, y+8, Yellow);
	LCD_DrawLine(x+9, y+2, x+9, y+7, Yellow);
	return;
}

void disegnaPacmanDown(int i, int j){ // Bocca rivolta verso il basso
	if(i >= N || j >= N) return;
	int x = i*10+10, y = j*10+40;
	LCD_DrawLine(x, y+4, x, y+7, Yellow);
	LCD_DrawLine(x+1, y+3, x+1, y+8, Yellow);
	LCD_DrawLine(x+2, y+2, x+2, y+9, Yellow);
	LCD_DrawLine(x+3, y+1, x+3, y+8, Yellow);
	LCD_DrawLine(x+4, y+1, x+4, y+6, Yellow);
	LCD_DrawLine(x+5, y+1, x+5, y+4, Yellow);
	LCD_DrawLine(x+6, y+1, x+6, y+6, Yellow);
	LCD_DrawLine(x+7, y+1, x+7, y+8, Yellow);
	LCD_DrawLine(x+8, y+2, x+8, y+9, Yellow);
	LCD_DrawLine(x+9, y+3, x+9, y+8, Yellow);
	return;
}

void cancellaCella(int i, int j){ // per cancellare pacman e pillars
	if(i >= N || j >= N) return;
	int x = i*10+10, y = j*10+40;
	LCD_DrawLine(x, y, x+9, y, Black);
	LCD_DrawLine(x, y+1, x+9, y+1, Black);
	LCD_DrawLine(x, y+2, x+9, y+2, Black);
	LCD_DrawLine(x, y+3, x+9, y+3, Black);
	LCD_DrawLine(x, y+4, x+9, y+4, Black);
	LCD_DrawLine(x, y+5, x+9, y+5, Black);
	LCD_DrawLine(x, y+6, x+9, y+6, Black);
	LCD_DrawLine(x, y+7, x+9, y+7, Black);
	LCD_DrawLine(x, y+8, x+9, y+8, Black);
	LCD_DrawLine(x, y+9, x+9, y+9, Black);
	schermo[i][j] = 0;
	return;
}

int countPillar = 0;

void disegnaPillarS(int i, int j){
	schermo[i][j] = 10;
	countPillar += 1;
	int x, y;
	x = i*10 + 10;
	y = j*10 + 40;
	
	LCD_DrawLine(x+4, y+3, x+4, y+6, White);
	LCD_DrawLine(x+5, y+3, x+5, y+6, White);
	LCD_DrawLine(x+3, y+4, x+6, y+4, White);
	LCD_DrawLine(x+3, y+5, x+6, y+5, White);

}

void disegnaPillarM(int i, int j){
	schermo[i][j] = 50;
	int x, y;
	x = i*10 + 10;
	y = j*10 + 40;
	LCD_DrawLine(x+2, y+4, x+7, y+4, Orange);
	LCD_DrawLine(x+2, y+5, x+7, y+5, Orange);
	
	LCD_DrawLine(x+4, y+2, x+4, y+7, Orange);	
	LCD_DrawLine(x+5, y+2, x+5, y+7, Orange);
	
	LCD_DrawLine(x+3, y+3, x+6, y+3, Orange);	
	LCD_DrawLine(x+3, y+6, x+6, y+6, Orange);	
}

void inizializzaPillars(){
	int i, j;
	volatile int i_rand = 0, j_rand = 0;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(schermo[i][j] == 0 && (i != i_start || j != j_start) && (i != (i_start+1) || j != j_start) && (i != N/2 - 1 || j != N/2 -1) && (i != N/2 - 1 || j != N/2) && (i != N/2 || j != N/2 -1) && (i != N/2|| j != N/2)){
				disegnaPillarS(i, j);
			}
		}
	}
	for(i = 0; i < 6; i++){
		i_rand = rand() % N; 
		j_rand = rand() % N;
		for(; schermo[i_rand][j_rand] != 10; i_rand = rand() % N, j_rand = rand() % N); 
		disegnaPillarM(i_rand, j_rand);
	}
}

void inizializzaSchermo(){
	int i, j;
	LCD_Clear(Black);
	// stampo Ready
	GUI_Text(8*10+10, 13*10+40, (uint8_t *) " Ready! ", Yellow, Black);
	LCD_Clear(Black);
	// stampo Go!
	GUI_Text(9*10+10, 13*10+40, (uint8_t *) " Go! ", Yellow, Black);
	LCD_Clear(Black);
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			schermo[i][j] = 0;
		}
	}
	// stampo reticolo esterno
	for(i = 0; i < N; i++){
		disegnaMuro(0,i);
		disegnaMuro(i,0);
		disegnaMuro(i, N-1);
		disegnaMuro(N-1,i);
	}
	// creo i buchi per teletrasporto
	cancellaMuro(0,N/2-1);
	cancellaMuro(0,N/2);
	cancellaMuro(N-1, N/2-1);
	cancellaMuro(N-1, N/2);
	
	// stampo forme labirinto
	disegnaQuadrante();
	
	// disegno pacman
	disegnaPacmanLeft(i_start,j_start);
	
	// countdown scritta
	GUI_Text(20, 7, (uint8_t *) "COUNTDOWN", Red, Black);
	
	// score scritta
	GUI_Text(180,7, (uint8_t *) "SCORE", Red, Black);
	PutChar(180, 20, (score/1000 + 48) , Red, Black); // 3248 -> 3
	PutChar(190, 20, ((score/100)%10 + 48) , Red, Black); // 3248 -> 32 -> 2
	PutChar(200, 20, ((score/10)%10 + 48) , Red, Black); // 3248 -> 324 -> 4
	PutChar(210, 20, (score%10 + 48) , Red, Black); // 3248 -> 8
	
	// life scritta
	GUI_Text(105, 270, (uint8_t *) "LIFE", Red, Black);
	PutChar(117, 283, life + 48 , Red, Black);
	
	return;
}

//fantasmi (suppongo solo 2 per ora)
int F1x;
int F1y;
int F2x;
int F2y;

void disegnaFantasma(int i, int j, int colore){
	int x = i*10+10;
	int y = j*10+40;
	if(schermo[i][j] == -1) // se muro errore
		return;
	//corpo
	LCD_DrawLine(x+2, y, x+7, y, colore);	
	LCD_DrawLine(x+1, y+1, x+8, y+1, colore);	
	LCD_DrawLine(x, y+2, x, y+7, colore);	
	LCD_DrawLine(x+9, y+2, x+9, y+7, colore);	
	LCD_DrawLine(x+8, y+4, x+9, y+4, colore);	
	LCD_DrawLine(x+1, y+2, x+1, y+2, colore);	
	LCD_DrawLine(x+8, y+2, x+8, y+2, colore);	
	LCD_DrawLine(x+3, y+2, x+6, y+2, colore);	
	LCD_DrawLine(x+4, y+3, x+5, y+3, colore);
	LCD_DrawLine(x+3, y+4, x+6, y+4, colore);	
	LCD_DrawLine(x, y+4, x+1, y+4, colore);
	LCD_DrawLine(x, y+5, x+9, y+5, colore);
	LCD_DrawLine(x, y+6, x+9, y+6, colore);
	LCD_DrawLine(x, y+7, x+9, y+7, colore);
	LCD_DrawLine(x+1, y+8, x+1, y+9, colore);
	LCD_DrawLine(x+3, y+8, x+3, y+9, colore);
	LCD_DrawLine(x+6, y+8, x+6, y+9, colore);
	LCD_DrawLine(x+8, y+8, x+8, y+9, colore);
	
	//occhi
	LCD_DrawLine(x+1, y+3, x+3, y+3, White);
	LCD_DrawLine(x+2, y+2, x+2, y+4, White);
	LCD_DrawLine(x+6, y+3, x+8, y+3, White);
	LCD_DrawLine(x+7, y+2, x+7, y+4, White);
	LCD_DrawLine(x+2, y+3, x+2, y+3, Black);
	LCD_DrawLine(x+7, y+3, x+7, y+3, Black);
}

void cancellaFantasma(int i, int j){
	if(i >= N || j >= N) return;
	int x = i*10+10, y = j*10+40;
	LCD_DrawLine(x, y, x+9, y, Black);
	LCD_DrawLine(x, y+1, x+9, y+1, Black);
	LCD_DrawLine(x, y+2, x+9, y+2, Black);
	LCD_DrawLine(x, y+3, x+9, y+3, Black);
	LCD_DrawLine(x, y+4, x+9, y+4, Black);
	LCD_DrawLine(x, y+5, x+9, y+5, Black);
	LCD_DrawLine(x, y+6, x+9, y+6, Black);
	LCD_DrawLine(x, y+7, x+9, y+7, Black);
	LCD_DrawLine(x, y+8, x+9, y+8, Black);
	LCD_DrawLine(x, y+9, x+9, y+9, Black);
	// se passo sopra i pillars, i fantasmi non li mangiano
	if(schermo[i][j] == 10)
		disegnaPillarS(i, j);
	else if(schermo[i][j] == 50)
		disegnaPillarM(i, j);
	return;
}

void inizializzaFantasmi(){
	disegnaFantasma(10, 11, Cyan);
	disegnaFantasma(11, 10, Green);
	F1x = 10;
	F1y = 11;
	F2x = 11;
	F2y = 10;
	// recinto (da togliere prima il muro)
	// prima cancella muro, ma non usare cancella cella perchè va gestito recinto solo in uscita e non cella normale
	LCD_DrawLine(111, 139, 129, 139, Red);
}

void attivaFantasmi(){ // da modificare perchè non deve diventare libero, ma deve essere gestita la sola uscita
	LCD_DrawLine(111, 139, 130, 139, Black);
	schermo[10][9] = 0;
	schermo[11][9] = 0;
}

void muoviFantasma(int num){ // num = scelgo quale dei 2 fantasmi muovere (*)
	int colore, Fx, Fy, pillarS = 0, pillarM = 0;

	if(num == 1){ //(*)
		colore = Cyan;
		Fx = F1x;
		Fy = F1y;
	}
	else{
		colore = Green;
		Fx = F2x;
		Fy = F2y;
	}

	cancellaFantasma(Fx, Fy);
	
	//TODO CAPIRE QUI SE PUOI SEMPLIFICA O CAMBIA
	if(i_start > Fx && Fx+1 < N && schermo[Fx+1][Fy] != -1 && (Fx+1 != F1x || Fy != F1y) && (Fx+1 != F2x || Fy != F2y)){
		Fx = Fx + 1;
	}
	
	else if(i_start < Fx && Fx-1 >= 0 && schermo[Fx-1][Fy] != -1 && (Fx-1 != F1x || Fy != F1y) && (Fx-1 != F2x || Fy != F2y)){
		Fx = Fx - 1;
	}
	
	else if(j_start < Fy && Fy-1 >= 0 && schermo[Fx][Fy-1] != -1 && (Fx != F1x || Fy-1 != F1y) && (Fx != F2x || Fy-1 != F2y)){
		Fy = Fy - 1;
	}
	
	else if(j_start > Fy && Fy+1 < N && schermo[Fx][Fy+1] != -1 && (Fx != F1x || Fy+1 != F1y) && (Fx != F2x || Fy+1 != F2y)){
		Fy = Fy + 1;
	}
	
	else if(Fx+1 < N && schermo[Fx+1][Fy] != -1 && (Fx+1 != F1x || Fy != F1y) && (Fx+1 != F2x || Fy != F2y)){
		Fx = Fx + 1;
	}
	else if(Fx-1 >= 0 && schermo[Fx-1][Fy] != -1 && (Fx-1 != F1x || Fy != F1y) && (Fx-1 != F2x || Fy != F2y)){
		Fx = Fx - 1;
	}
	
	else if(Fy-1 >= 0 && schermo[Fx][Fy-1] != -1 && (Fx != F1x || Fy-1 != F1y) && (Fx != F2x || Fy-1 != F2y)){
		Fy = Fy - 1;
	}
	
	else if(Fy+1 < N && schermo[Fx][Fy+1] != -1 && (Fx != F1x || Fy+1 != F1y) && (Fx != F2x || Fy+1 != F2y)){
		Fy = Fy + 1;
	}
	disegnaFantasma(Fx, Fy, colore);
	
	//aggiorna F1 e F2
	if(num == 1){
		F1x = Fx;
		F1y = Fy;
	}
	else{
		F2x = Fx;
		F2y = Fy;
	}
	if(num == 1)
		muoviFantasma(2);
	return;
}

#ifdef SIMULATOR
#define TIMER0 0xC8
#define TIMER1 0x7A120
#define TIMER2 0x3D090
#define RIT 0x7A120
#else
#define TIMER0 0x17D7840
#define TIMER1 0x17D7840
#define TIMER2 0x17D7840
#define RIT 0x17D7840
#endif

int main(void){
	
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
	inizializzaSchermo();
	inizializzaPillars();
	inizializzaFantasmi();
	
	// gestione inizio in pause
	pause = 1;
	GUI_Text(9*10+11, 12*10+32, (uint8_t *) "PAUSE", Yellow, Blue);
	
	BUTTON_init();
	//NVIC_DisableIRQ(EINT0_IRQn);
	//NVIC_EnableIRQ(EINT0_IRQn); // Abilita interrupt INT0 per il pause
	
	init_timer(0, 0, 0, 3, TIMER0); // 1 ms = 61A8 -> 1s = 17D7840
	enable_timer(0);
	
	// timer per conteggio time rimasto (1s)
	init_timer(1, 0, 0, 3, TIMER1); // (?) ms = 7A120 -> 1s = 17D7840
	
	// timer per generazione pillars casuali
	//init_timer(2, 0, 0, 3, TIMER2);
	
	init_RIT(RIT); // 50ms = 1312D0 -> 1s = 17D7840
	//enable_RIT();
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
		
  {
		__ASM("wfi");
  }
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
