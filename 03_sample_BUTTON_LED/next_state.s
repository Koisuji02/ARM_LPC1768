        AREA asm_functions, CODE, READONLY
        EXPORT next_state
			
next_state
        PUSH {r4-r8, r10-r11, lr}  		; Salva i registri utilizzati
        
        ; Ingresso:
        ; R0 -> current_state
        ; R1 -> taps
        ; R2 -> output_bit
        
        ; calcolo dell'output_bit (LSB del current_state)
        AND r4, r0, #1				  	; r4 = ultimo bit (LSB) del current_state
        STR r4, [r2]                  	; salvo nell'output_bit il bit preso prima

        ; r5 = input (ovvero il risultato precedente, guarda immagine per capire) è inizializzato a 0
        MOV r5, #0
		MOV r6, r0						; copio current_state in r6
		MOV r7, r1						; e taps in r7 (così lavoro su questi e non sugli originali)
		MOV r8, #0						; r8 = contatore per il loop (fino a 8 bit)
		MOV r11, #1						; r11 = 1
		
loop_taps
		CMP r8, #8						; finito gli 8 bit, fine ciclo
		BEQ exit_loop
		
		; controllo
		TST r7, r11						; AND tra taps e 00000001 (controllo se LSB bit di taps è 1 oppure 0) e setto il flag z (nel caso sia 0, la TST fa così)
		; se z == 1, vuol dire che risultato dell'AND è 0 (quindi sono in un bit di taps messo a 0, non quello cercato)
		; se z == 0, sono in uno dei bit di taps segnati a 1 e faccio:		
		ANDNE r10, r6, #1				; salvo in r10 LSB di current_state
		EORNE r5, r5, r10				; r5 = r5 ^ (LSB_current_state) [XOR, guarda immagine]
		
		; shifto e incremento per prossima iterazione
		LSR r6, r6, #1					; shifto current_state a dx di 1
		LSR r7, r7, #1					; shifto taps a dx di 1
		ADD r8, r8, #1					; counter_loop++
		B loop_taps

exit_loop
		LSR r0, r0, #1					; shifto il current_state di 1 a dx
		LSL r5, r5, #7					; shifto l'input (ovvero il risultato precedente) di 7 a sx
		ORR r0, r0, r5					; next_state = current_state^(input shiftato di 7 a sx) [XOR, guarda immagine]
		
		POP{r4-r8, r10-r11, lr}
		BX lr
        END
