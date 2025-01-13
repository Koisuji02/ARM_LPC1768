/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through debounced buttons and Joystick
 *        	- key1 switches on the led at the left of the current led on, 
 *					- it implements a circular led effect,
 * 					- joystick UP function returns to initial configuration (led11 on) .
 * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.h"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include <stdint.h>
#include <stdlib.h>

//#define N 100
volatile unsigned char led_value;
//unsigned char vet[N];
unsigned int index = 0;
//int value_tmp = 0;
//unsigned char val; // 8 bit da prendere da TC alla pressione di KEY1
//unsigned int counter_tim0 = 0;

//#define MAX_8BIT 0xFF
//unsigned char var;
//unsigned int totale = 0;

unsigned int S = 0; // counter soddisfatti
unsigned int N = 0; // counter neutri
unsigned int I = 0; // counter insoddisfatti

char PS = 0;
char PN = 0;
char PI = 0;

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
// volatile = non permette ottimizzazione alle variabili (es. per registri, variabili in interrupt condivise...)
// static = se definita in un blocco, quel blocco la vede come una variabile globale, ma fuori da esso non esiste

extern void call_svc(void);

// utile se ci chiedono di manipolare matrici
/* extern int check_square(int x, int y, int r);
extern float my_division(float* a, float* b);

extern uint8_t ROWS[]; // dichiaro tutto come matrici per poter prendere il valore all'interno
extern uint8_t COLUMNS[];
extern int Matrix_Coordinates[]; // posso passare da DCD (32 bit) a int (32 bit) proprio perchè su 32 bit

float	compute_pigreco(int raggio){
	// dichiarazioni e inizializzazioni
	int i, x, y, area = 0, row = ROWS[0], col = COLUMNS[0];
	int maxsize = row*col;
	float r_quadrato = (float)(raggio*raggio), area_f = 0; // casting a float per la funzione my_division
	float pigreco = 0;
	
	// calcolo dell'area sulla matrice di punti
	for(i=0; i<maxsize; i+=2){
		x = Matrix_Coordinates[i];
		y = Matrix_Coordinates[i+1];
		area += check_square(x,y,raggio); // se il punto è incluso +1, altrimenti +0; metto il casting a float per la my_division
	}
	
	// applico la formula e trovo pigreco
	area_f = (float) area;
	pigreco = my_division(&area_f, &r_quadrato);
	return pigreco;
} */

int main (void) {
  
	//call_svc();														// per chiamata a SVC se richiesta
	SystemInit();  												/* System Initialization 							*/
	
  LED_init();                           /* LED Initialization                 */
	//LED_Out(0);			// all'inizio tutti spenti
	//LED_Out(0xFF); // inizializzo i led tutti accesi
	
  BUTTON_init();												/* BUTTON Initialization              */
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x1312D0);									/* RIT Initialization 50 msec       	*/
	//enable_RIT();													/* RIT enabled												*/
	// il rit faccio enable nei pulsanti se serve, altrimenti direttamente nel main se serve
	
	S = 0; // rinizializzo le variabili a 0 al reset
	N = 0;
	I = 0;
	
	// sintassi della init_timer:
	// init_timer(#timer_da_attivare, prescaler [inutile], MR#, SRI = stop, reset, interrupt, count = T*25*10^6)
	// SRI è stop (vedi dal grafico chi stoppa, qui nessuno), reset (chi ferma e fa ripartire l'onda, in questo caso MR1 a fine onda) e interrupt (chi chiama l'interrupt_handler, in questo caso sia MR0 e sia MR1, ma con gestioni diverse)
	
	init_timer(0,0,0,3,0.1*0x17D7840); // 1s
	
	init_timer(1,0,0,3,0.1*0x2FAF080); // 2s
	
	init_timer(2,0,0,3,0.1*0x4C4B40); // timer per lampeggio (2s -> 0.2 acceso e spento)
	
	init_timer(3,0,0,3,0.1*0x2FAF080); // 2s
	
	//enable_timer(1);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
