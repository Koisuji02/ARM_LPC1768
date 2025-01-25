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

#define N 4
extern volatile unsigned char led_value;

extern unsigned char vet[N];
extern unsigned int index;
extern char time;
extern unsigned char vet2[N];
extern unsigned int index2;
extern unsigned char analisi_accuratezza(unsigned char VETT[], unsigned char VETT2[], const unsigned int num, char RES[]);
extern char res[N];
extern unsigned char media;
extern unsigned int indexRes;

//extern unsigned char val; // 8 bit da prendere da TC alla pressione di KEY1
//extern unsigned int counter_tim0;
//extern int value_tmp;
//extern int disabilitoKey1;
//#define MAX_8BIT 0xFF
//extern unsigned char var;
//extern unsigned int totale;
extern int disattivato;
extern void svuotaVet(unsigned char* vet, unsigned int index);
extern void svuotaVetS(char* vet, unsigned int index);
#endif /* end __RIT_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
