#include <LPC17xx.h>                  /* LPC17xx definitions */
#include "CAN.h"                      /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"

extern uint8_t icr ;  // icr and result must be global in order to work with both real and simulated LandTiger.
extern uint32_t result;
extern CAN_msg CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg CAN_RxMsg;    /* CAN message for receiving */                                

static int puntiRicevuti1 = 0;
static int puntiInviati1 = 0;

static int puntiRicevuti2 = 0;
static int puntiInviati2 = 0;

uint16_t val_RxCoordX = 0;            /* Locals used for display */
uint16_t val_RxCoordY = 0;

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler (void)  {
  /* check CAN controller 1 */
	icr = 0;
  icr = (LPC_CAN1->ICR | icr) & 0xFF;               /* clear interrupts */
	
  if (icr & (1 << 0)) {                           /* CAN Controller #1 message is received */
		CAN_rdMsg (1, &CAN_RxMsg);                    /* Read the message */
    LPC_CAN1->CMR = (1 << 2);                     /* Release receive buffer */
		
		uint8_t time_remaining = CAN_RxMsg.data[0];
		uint8_t lives_remaining = CAN_RxMsg.data[1];
		uint16_t score = (CAN_RxMsg.data[2] << 8) | CAN_RxMsg.data[3];
		
		puntiRicevuti1++;
  }
	if (icr & (1 << 1)) {                         /* CAN Controller #1 message is transmitted */
		puntiInviati1++;
	}
		
	/* check CAN controller 2 */
	icr = 0;
	icr = (LPC_CAN2->ICR | icr) & 0xFF;             /* clear interrupts */

	if (icr & (1 << 0)) {                          /* CAN Controller #2 message is received */
		CAN_rdMsg (2, &CAN_RxMsg);                    /* Read the message */
    LPC_CAN2->CMR = (1 << 2);                     /* Release receive buffer */
		
		uint8_t time_remaining = CAN_RxMsg.data[0];
		uint8_t lives_remaining = CAN_RxMsg.data[1];
		uint16_t score = (CAN_RxMsg.data[2] << 8) | CAN_RxMsg.data[3];
		
		puntiRicevuti2++;
	}
	if (icr & (1 << 1)) {                         /* CAN Controller #2 message is transmitted */
		puntiInviati2++;
	}
}
