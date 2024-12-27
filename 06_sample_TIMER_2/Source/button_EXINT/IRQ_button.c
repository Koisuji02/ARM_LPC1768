#include "button.h"
#include "LPC17xx.h"


#include "../led/led.h"
#include "../timer/timer.h"

#define BUFFER_SIZE 7000

extern uint32_t circular_buffer[];
extern void BubbleSort(uint32_t* buffer, uint32_t val, int i);
uint32_t vet_sorted[BUFFER_SIZE];
float media;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	disable_timer(1);
	int i;
	
	// ordinamento bubble sort (algoritmo O(n^2))
	for(i = 0; i < BUFFER_SIZE; i++)
		BubbleSort(vet_sorted, circular_buffer[i], i);
	enable_timer(1);
	
	// calcolo media
	media = 0.0;
	for(i = 0; i < BUFFER_SIZE; i++){
		media += (float)vet_sorted[i];
		circular_buffer[i] = 0xCAFECAFE; // pulisco già con CAFECAFE così mi evito un ciclo
	}
	media /= BUFFER_SIZE;
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */  
	enable_timer(0);  
}


