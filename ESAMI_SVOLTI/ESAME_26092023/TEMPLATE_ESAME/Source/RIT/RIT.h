/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           RIT.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_RIT, funct_RIT, IRQ_RIT .c files
** Correlated files:    lib_RIT.c, funct_RIT.c, IRQ_RIT.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <stdint.h>
#include "led/led.h"
#ifndef __RIT_H
#define __RIT_H

/* init_RIT.c */
extern uint32_t init_RIT( uint32_t RITInterval );
extern void enable_RIT( void );
extern void disable_RIT( void );
extern void reset_RIT( void );
/* IRQ_RIT.c */
extern void RIT_IRQHandler (void);

#define N 8
extern volatile unsigned char led_value;

extern unsigned short int vet[N];
extern unsigned int index;
extern unsigned short int var;
extern int flagModify;
extern void svuotaVet(unsigned short int* vet, unsigned int index);
extern unsigned int concat_sum(unsigned short int VETT[], unsigned int dim, char* alarm);
extern char alarm;
extern unsigned int result;

//extern unsigned char val; // 8 bit da prendere da TC alla pressione di KEY1
//extern unsigned int counter_tim0;
//extern int value_tmp;
//extern int disabilitoKey1;
//#define MAX_8BIT 0xFF
//extern unsigned char var;
//extern unsigned int totale;
//extern int disattivato;

#endif /* end __RIT_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
