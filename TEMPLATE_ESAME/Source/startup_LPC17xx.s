;/*****************************************************************************
; * @file:    startup_LPC17xx.s
; * @purpose: CMSIS Cortex-M3 Core Device Startup File 
; *           for the NXP LPC17xx Device Series 
; * @version: V1.01
; * @date:    21. December 2009
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * Copyright (C) 2009 ARM Limited. All rights reserved.
; * ARM Limited (ARM) is supplying this software for use with Cortex-M3 
; * processor based microcontrollers.  This file can be freely distributed 
; * within development tools that are supporting such ARM based processors. 
; *
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; *****************************************************************************/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WDT_IRQHandler            ; 16: Watchdog Timer
                DCD     TIMER0_IRQHandler         ; 17: Timer0
                DCD     TIMER1_IRQHandler         ; 18: Timer1
                DCD     TIMER2_IRQHandler         ; 19: Timer2
                DCD     TIMER3_IRQHandler         ; 20: Timer3
                DCD     UART0_IRQHandler          ; 21: UART0
                DCD     UART1_IRQHandler          ; 22: UART1
                DCD     UART2_IRQHandler          ; 23: UART2
                DCD     UART3_IRQHandler          ; 24: UART3
                DCD     PWM1_IRQHandler           ; 25: PWM1
                DCD     I2C0_IRQHandler           ; 26: I2C0
                DCD     I2C1_IRQHandler           ; 27: I2C1
                DCD     I2C2_IRQHandler           ; 28: I2C2
                DCD     SPI_IRQHandler            ; 29: SPI
                DCD     SSP0_IRQHandler           ; 30: SSP0
                DCD     SSP1_IRQHandler           ; 31: SSP1
                DCD     PLL0_IRQHandler           ; 32: PLL0 Lock (Main PLL)
                DCD     RTC_IRQHandler            ; 33: Real Time Clock
                DCD     EINT0_IRQHandler          ; 34: External Interrupt 0
                DCD     EINT1_IRQHandler          ; 35: External Interrupt 1
                DCD     EINT2_IRQHandler          ; 36: External Interrupt 2
                DCD     EINT3_IRQHandler          ; 37: External Interrupt 3
                DCD     ADC_IRQHandler            ; 38: A/D Converter
                DCD     BOD_IRQHandler            ; 39: Brown-Out Detect
                DCD     USB_IRQHandler            ; 40: USB
                DCD     CAN_IRQHandler            ; 41: CAN
                DCD     DMA_IRQHandler            ; 42: General Purpose DMA
                DCD     I2S_IRQHandler            ; 43: I2S
                DCD     ENET_IRQHandler           ; 44: Ethernet
                DCD     RIT_IRQHandler            ; 45: Repetitive Interrupt Timer
                DCD     MCPWM_IRQHandler          ; 46: Motor Control PWM
                DCD     QEI_IRQHandler            ; 47: Quadrature Encoder Interface
                DCD     PLL1_IRQHandler           ; 48: PLL1 Lock (USB PLL)
				DCD		USBActivity_IRQHandler    ; USB Activity interrupt to wakeup
				DCD		CANActivity_IRQHandler    ; CAN Activity interrupt to wakeup


                IF      :LNOT::DEF:NO_CRP
                AREA    |.ARM.__at_0x02FC|, CODE, READONLY
CRP_Key         DCD     0xFFFFFFFF
                ENDIF


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
				
				; imposto il PSP in thread mode (unprivileged) [per SVC]
				; MOV		r0, #3
				; MSR		CONTROL, r0
				; LDR		sp, =Stack_Mem
				
                IMPORT  __main
                LDR     r0, =__main
				;LDR 	r1, =Heap_Mem		; da usare se uso matrice sotto
                BX      r0
				
				; imposto MSP in thread mode (unprivileged) quando torno dall'SVC
				; MOV		r0, #0
				; MSR		CONTROL, r0
				
; InfLoop         B      	InfLoop	; infinite loop per la cosa vista a lezione sui programmi c

                ENDP

				; da usare se ci chiedono lavori su matrice (da non modificare, solo lettura)
				/*AREA |.data|, DATA, READONLY
				; se volessimo averla come area anche scrivibile AREA |.data|, DATA, READWRITE
				; per avere i dati nel main.c uso EXPORT
				EXPORT Matrix_Coordinates
				EXPORT ROWS
				EXPORT COLUMNS

Matrix_Coordinates 	DCD -5,5,-4,5,-3,5,-2,5,-1,5,0,5,1,5,2,5,3,5,4,5,5,5
					DCD -5,4,-4,4,-3,4,-2,4,-1,4,0,4,1,4,2,4,3,4,4,4,5,4
					DCD -5,3,-4,3,-3,3,-2,3,-1,3,0,3,1,3,2,3,3,3,4,3,5,3
					DCD -5,2,-4,2,-3,2,-2,2,-1,2,0,2,1,2,2,2,3,2,4,2,5,2
					DCD -5,1,-4,1,-3,1,-2,1,-1,1,0,1,1,1,2,1,3,1,4,1,5,1
					DCD -5,0,-4,0,-3,0,-2,0,-1,0,0,0,1,0,2,0,3,0,4,0,5,0
					DCD -5,-1,-4,-1,-3,-1,-2,-1, -1,-1 ,0,-1,1,-1,2,-1,3,-1,4,-1,5,-1
					DCD -5,-2,-4,-2,-3,-2,-2,-2,-1,-2,0,-2,1,-2,2,-2,3,-2,4,-2,5,-2
					DCD -5,-3,-4,-3,-3,-3,-2,-3,-1,-3,0,-3,1,-3,2,-3,3,-3,4,-3,5,-3
					DCD -5,-4,-4,-4,-3,-4,-2,-4,-1,-4,0,-4,1,-4,2,-4,3,-4,4,-4,5,-4
					DCD -5,-5,-4,-5,-3,-5,-2,-5,-1,-5,0,-5,1,-5,2,-5,3,-5,4,-5,5,-5
ROWS 				DCB 11
COLUMNS 			DCB 22

				AREA |.text|, CODE, READONLY*/
					
; Dummy Exception Handlers (infinite loops which can be modified)                

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
					
				;MRS	r1, psp					; r1 = psp (perchè control = 3 messo sopra)
				
				; leggi l'istruzione svc
				;LDR r0, [r1, #24]			; carico indirizzo PC
				;SUB r0, r0, #2				; il PC contiene l'istruzione dopo la SVC, quindi devo andare indietro di 2 byte (16 bit = istruzione)
				;LDRB r0, [r0]				; leggo il byte dalla memoria all'indirizzo r0 = PC-2
				;AND r0, r0, #0xFF 			; estrae i bit bassi della SVC (msg da codificare)
				
				; solo ora salvo il contesto perchè altrimenti mi sballa psp e non riesco a prendere il valore della SVC dai bit bassi del PC
				;PUSH {r4-r11, lr}			; salvo contesto
				
				;CMP r0, #0x15			; controllo se SVC è quella cercata
				;BNE uscita
				
				; codifica (3,1) a ripetizione se SVC #16
				;MOV r2, #0          ; r2 = messaggio codificato
				;MOV r3, #0          ; r3 = indice del bit
				;MOV r4, #0         ; r4 = posizione iniziale dei bit nel buffer (24-3)

;codifica_loop
				;TST r0, #1          	; controlla LSB (bit meno significativo)
				;ADDEQ r4, r4, #3     		; vado avanti al prossimo gruppo di 3 bit
				;LSREQ r0, r0, #1 			; sposto il msg a dx di 1 bit
				;ADDEQ r3, r3, #1      	; (indice del bit)++
				;MOVNE r5, #7          	; carica 111 (7) in r5
				;LSLNE r5, r5, r4       	; sposta a sinistra il valore in r5 di r4 bit
				;ORRNE r2, r2, r5       	; or logico tra r2 e r5 (il risultato dello shift)
				;CMPEQ r3, #8          	; controlla se sono stati elaborati tutti gli 8 bit, senno torna al loop
				;BNE codifica_loop
				
;uscita			POP {r4-r11, pc}
				; mette messaggio codificato nello stack
				;STR r2, [r1]
				B	.				; da togliere se scommenti tutto
                ENDP
				
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  TIMER0_IRQHandler         [WEAK]
                EXPORT  TIMER1_IRQHandler         [WEAK]
                EXPORT  TIMER2_IRQHandler         [WEAK]
                EXPORT  TIMER3_IRQHandler         [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  UART2_IRQHandler          [WEAK]
                EXPORT  UART3_IRQHandler          [WEAK]
                EXPORT  PWM1_IRQHandler           [WEAK]
                EXPORT  I2C0_IRQHandler           [WEAK]
                EXPORT  I2C1_IRQHandler           [WEAK]
                EXPORT  I2C2_IRQHandler           [WEAK]
                EXPORT  SPI_IRQHandler            [WEAK]
                EXPORT  SSP0_IRQHandler           [WEAK]
                EXPORT  SSP1_IRQHandler           [WEAK]
                EXPORT  PLL0_IRQHandler           [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  EINT0_IRQHandler          [WEAK]
                EXPORT  EINT1_IRQHandler          [WEAK]
                EXPORT  EINT2_IRQHandler          [WEAK]
                EXPORT  EINT3_IRQHandler          [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  BOD_IRQHandler            [WEAK]
                EXPORT  USB_IRQHandler            [WEAK]
                EXPORT  CAN_IRQHandler            [WEAK]
                EXPORT  DMA_IRQHandler            [WEAK]
                EXPORT  I2S_IRQHandler            [WEAK]
                EXPORT  ENET_IRQHandler           [WEAK]
                EXPORT  RIT_IRQHandler            [WEAK]
                EXPORT  MCPWM_IRQHandler          [WEAK]
                EXPORT  QEI_IRQHandler            [WEAK]
                EXPORT  PLL1_IRQHandler           [WEAK]
				EXPORT  USBActivity_IRQHandler    [WEAK]
				EXPORT  CANActivity_IRQHandler    [WEAK]

WDT_IRQHandler           
TIMER0_IRQHandler         
TIMER1_IRQHandler         
TIMER2_IRQHandler         
TIMER3_IRQHandler         
UART0_IRQHandler          
UART1_IRQHandler          
UART2_IRQHandler          
UART3_IRQHandler          
PWM1_IRQHandler           
I2C0_IRQHandler           
I2C1_IRQHandler           
I2C2_IRQHandler           
SPI_IRQHandler            
SSP0_IRQHandler           
SSP1_IRQHandler           
PLL0_IRQHandler           
RTC_IRQHandler            
EINT0_IRQHandler          
EINT1_IRQHandler          
EINT2_IRQHandler          
EINT3_IRQHandler          
ADC_IRQHandler            
BOD_IRQHandler            
USB_IRQHandler            
CAN_IRQHandler            
DMA_IRQHandler          
I2S_IRQHandler            
ENET_IRQHandler       
RIT_IRQHandler          
MCPWM_IRQHandler             
QEI_IRQHandler            
PLL1_IRQHandler           
USBActivity_IRQHandler
CANActivity_IRQHandler

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
