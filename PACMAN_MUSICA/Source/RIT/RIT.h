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
#include "CAN/CAN.h"
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
extern int gameOver;
extern int pillarsRemaining;
extern int pause;
extern void disegnaPacmanUp(int i, int j);
extern void disegnaPacmanDown(int i, int j);
extern void disegnaPacmanLeft(int i, int j);
extern void disegnaPacmanRight(int i, int j);
extern void cancellaCella(int i, int j);
extern void inizializzaFantasmi();
extern void attivaFantasma();
extern void attivaFrightenedMode();
extern int frightenedFlag;
extern int mangiatoFantasma;
extern void disattivaFrightenedMode();
extern int Fx_start;
extern int Fy_start;
extern int counter_tim0;		// 10s in frightened mode
extern int counter_tim0_MR1;	// 3s in frightened mode
extern void functGameOver();
extern void muoviFrightenedMode();
extern void muoviChaseMode();
extern void disegnaFantasma(int i, int j, int colore);
extern void cancellaFantasma(int i, int j);
extern int flagStoppato;
extern int inizializzato;
extern int callerGameOver;
extern void stopMusic();
extern void startMusic();
extern volatile int time_elapsed;

#ifdef SIMULATOR
#define TIMER0 0xC8*5
#define TIMER1 0x1E848*5 	// 10ms/2 -> 5ms = 125000 = 0x1E848
#define TIMER2 0x3D090*5 	// 10ms
#define TIMER3 0x3D090*5	// 10 ms (scalata da 20 perchè troppo lenta)
#define RIT 0x67C28*5 // 17ms = 0x67C28			// 20ms = 0x7A120
#else
#define TIMER0 0x17D7840
#define TIMER1 0x17D7840
#define TIMER2 0x17D7840
#define TIMER3 0x17D7840
#define RIT 0x2FAF080
#endif

#endif /* end __RIT_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
