#include "button.h"
#include "LPC17xx.h"
#include "../led/led.h"

extern unsigned char led_value;					/* defined in funct_led								*/

void EINT0_IRQHandler (void)	  	/* INT0														 */
{
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	static uint8_t position=0;
  if( position == 7){
		LED_On(0);
		LED_Off(7);
		position = 0;
	}
	else{
		LED_Off(position);
		LED_On(++position);
	}
			
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


