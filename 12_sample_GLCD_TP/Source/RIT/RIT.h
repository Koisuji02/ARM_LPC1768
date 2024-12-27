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
#ifndef __RIT_H
#define __RIT_H
#define N 22

/* init_RIT.c */
extern uint32_t init_RIT( uint32_t RITInterval );
extern void enable_RIT( void );
extern void disable_RIT( void );
extern void reset_RIT( void );
/* IRQ_RIT.c */
extern void RIT_IRQHandler (void);
extern int schermo[N][N];				// richiamo schermo dentro RIT
extern int j_start;
extern int i_start;
extern int score;
extern int local_score;
extern int life;
extern int victory;
extern int pillarsRemaining;
extern int pause;
extern void disegnaPacmanUp(int i, int j);
extern void disegnaPacmanDown(int i, int j);
extern void disegnaPacmanLeft(int i, int j);
extern void disegnaPacmanRight(int i, int j);
extern void cancellaCella(int i, int j);

#endif /* end __RIT_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
