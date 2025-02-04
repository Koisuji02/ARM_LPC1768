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

volatile unsigned char led_value;

#define N 100
uint8_t vet[N];
int index = 0;

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
extern int Matrix_Coordinates[]; // posso passare da DCD (32 bit) a int (32 bit) proprio perch� su 32 bit

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
		area += check_square(x,y,raggio); // se il punto � incluso +1, altrimenti +0; metto il casting a float per la my_division
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
	LED_Out(0);					// metto inizialmente tutti spenti
  BUTTON_init();												/* BUTTON Initialization              */
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x1312D0);									/* RIT Initialization 50 msec       	*/
	enable_RIT();													/* RIT enabled												*/
	// il rit faccio enable nel int0 se serve
	
	// sintassi della init_timer:
	// init_timer(#timer_da_attivare, prescaler [inutile], MR#, SRI = stop, reset, interrupt, count = T*25*10^6)
	// SRI � stop (vedi dal grafico chi stoppa, qui nessuno), reset (chi ferma e fa ripartire l'onda, in questo caso MR1 a fine onda) e interrupt (chi chiama l'interrupt_handler, in questo caso sia MR0 e sia MR1, ma con gestioni diverse)
	init_timer(0,0,0,3,0x17D7840); // timer0 = up = 1s
	init_timer(1,0,0,3,0x17D7840); // timer1 = down = 1s
	init_timer(2,0,0,3,0x47868C0); // timer2 = key1 = 3s
	init_timer(3,0,0,3,0x2FAF080); // timer3 = ASM_punto2 = 2s
	
	NVIC_DisableIRQ(EINT0_IRQn); // disabilito INT0
	LPC_PINCON->PINSEL4 &= ~(1<<20);
	
	//LED_Out(0xFF); // inizializzo i led tutti accesi
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
