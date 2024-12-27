/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: 
 *		to control led11 and led 10 through EINT buttons (similarly to project 03_)
 *		to control leds9 to led4 by the timer handler (1 second - circular cycling)
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

/* Led external variables from funct_led */
extern unsigned char led_value;					/* defined in lib_led								*/
#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

#define BUFFER_SIZE 7000
uint32_t circular_buffer[BUFFER_SIZE];
volatile uint32_t buffer_index = 0;
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/

int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
	
	// abilito i TIMER2 e TIMER3
	LPC_SC -> PCONP |= (1 << 22);  // TURN ON TIMER 2
	LPC_SC -> PCONP |= (1 << 23);  // TURN ON TIMER 3	
	


//	init_timer(2, 0, 2, 1, 0x017D7840);							/* TIMER0 Initialization              */
																				/* K = T*Fr = [s]*[Hz] = [s]*[1/s]	  */
																				/* T = K / Fr = 0x017D7840 / 25MHz    */
																				/* T = K / Fr = 25000000 / 25MHz      */
																				/* T = 1s	(one second)   							*/							
	//enable_timer(2);
	LED_Out(0xFF);			// tutti 1 per inizializzare
	// f = 75 Hz -> T = 1/f = 1/75 -> count = T*25*10^6
	// sintassi della init_timer:
	// init_timer(#timer_da_attivare, prescaler [inutile], MR#, SRI = stop, reset, interrupt, count = T*25*10^6)
	// SRI è stop (vedi dal grafico chi stoppa, qui nessuno), reset (chi ferma e fa ripartire l'onda, in questo caso MR1 a fine onda) e interrupt (chi chiama l'interrupt_handler, in questo caso sia MR0 e sia MR1, ma con gestioni diverse)
	init_timer(2, 0, 0, 1, 0x32DCD); //MR0 -> SRI = 001 (solo handler)
	// metto come count = 1/120*25*10^6 = 0x65B9A, ovvero la metà del count di MR1
	init_timer(2, 0, 1, 3, 0x65B9A); //MR1 -> SRI = 011 (handler + reset del segnale)
	// metto come count = 1/60*25*10^6 = 0x65B9A
	
	enable_timer(2);
	
	LED_Out(0xFF);			// tutti 1 per inizializzare
	// f = 75 Hz -> T = 1/f = 1/75 -> count = T*25*10^6
	init_timer(3, 0, 0, 1, 0x28B0A); //MR0 -> SRI = 001 (solo handler)
	init_timer(3, 0, 1, 3, 0x51615); //MR1 -> SRI = 011 (handler + reset del segnale)
	
	enable_timer(3);
	
	LPC_SC->PCONP |= (1 << 1);				// TURN ON TIMER0
	LPC_SC->PCONP |= (1 << 2);				// TURN ON TIMER1
	
	enable_timer(0);
	
	init_timer(1, 0, 0, 1, 0x823);
	enable_timer(1);
	// metto come count = 1/(120*100)*25*10^6 = 2083 = 0x823

	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= 0xFFFFFFFFD;						
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}


//	LPC_SC->PCONP |= (1 << 22); // Enable Timer 2
//  LPC_SC->PCONP |= (1 << 23); // Enable Timer 3