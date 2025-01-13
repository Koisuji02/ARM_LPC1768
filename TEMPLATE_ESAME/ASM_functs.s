								AREA |.text|, CODE, READONLY

								EXPORT call_svc
								; call_svc = naked function (non salvo i registri)
call_svc        				PROC

								SVC #0x15          ; Chiamata SVC con numero fisso (16, syscall(10) ovvero modalità supervisor)
								; ho quindi una SVC che ha 0x15 = 2_10101 negli 8 bit bassi (LSB)
								BX lr
								ENDP
				
								EXPORT  check_square
check_square					PROC
								PUSH {r4,r5,r6,r7,lr}	; salvo i registri
								MUL r4, r0, r0			; r4 = x^2
								MUL r5, r1, r1			; r5 = y^2
								ADD r6, r4, r5			; r6 = x^2 + y^2
								MUL r7, r2, r2			; r7 = r^2
								CMP r6, r7				; confronto
								MOVLE r0, #1			; se x^2+y^2<=r^2 torna 1
								MOVGT r0, #0			; altrimenti 0
								POP {r4,r5,r6,r7,pc}	; ripristino i registri ai valori prima della checksquare
								ENDP
				
								EXPORT  my_division
my_division						PROC
								; r0 = a, r1 = b
								PUSH {r4-r7,lr}		; salvo i registri
								IMPORT __aeabi_fdiv			; ci vuole la IMPORT per usarla
								; devo prendere i valori dagli indirizzi
								LDR r0, [r0]			; r0 = *(&area_f)
								LDR r1, [r1]			; r1 = *(&r_quadrato)
								BL __aeabi_fdiv				; chiama la div della lib Arm (emulare SW la fp unit)
								; questa div prende automaticamente r0 e r1 e mette il risultato in r0 quindi non devo scrivere altro
								POP {r4-r7,pc}		; come nella check ripristno
								ENDP

								EXPORT next_state
			
next_state						PROC
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
								
								POP{r4-r8, r10-r11, pc}
								ENDP
					
								EXPORT extractValue
			
extractValue					PROC
								PUSH {r4-r8, r10-r11, lr}  		; Salva i registri utilizzati
								; r0 = value_tmp
								LSR r0, r0, #16
								AND r0, r0, #0xFF		; prendo gli 8 bit shiftati (23-16) del value e lo metto in char
								
								POP{r4-r8, r10-r11, pc}
								ENDP
								
								EXPORT get_and_sort		; Sort decrescente

get_and_sort 					PROC
								PUSH {r4-r8, r10-r11, lr}        ; Salva i registri utilizzati
								; r0 = vet
								; r1 = val
								; r2 = index
								MOV r4, r0                      ; r4 = vet (base address of the array)
								MOV r5, r2                      ; r5 = index
								SUB r5, r5, #1                  ; r5 = index - 1 (maximum index)
								MOV r8, #0                      ; r8 = i (outer loop)
								MOV r9, #0                      ; r9 = j (inner loop)
								MOV r11, #0                     ; massimo locale
								MOV r12, r4                     ; r12 = pos massimo locale (base address of the array)

loop_outer_sort
								CMP r8, r5                      ; Verifica se abbiamo completato tutti i cicli
								BGT end_sort
								MOV r10, r4                     ; r10 = puntatore all'inizio di vet per il ciclo interno
								MOV r11, #0                     ; r11 = massimo locale
								MOV r12, r4                     ; r12 = pos massimo locale (base address of the array)

loop_inner_sort
								CMP r9, r5                      ; Confronta j con index-1
								BGT next_outer_iteration
								ADD r6, r10, r9                 ; r6 = indirizzo di vet[j]
								LDRB r7, [r6]                   ; r7 = vet[j]
								CMP r7, r11                     ; Confronta massimo locale con l'elemento corrente
								BGE skip_swap                   ; Se massimo >= r7, non fare nulla
								MOV r11, r7                     ; Aggiorna massimo locale
								MOV r12, r6                     ; Aggiorna la posizione del massimo

skip_swap
								ADD r9, r9, #1                  ; Incrementa j
								B loop_inner_sort

next_outer_iteration			; Scambia l'elemento massimo con l'elemento corrente
								ADD r6, r4, r8                  ; r6 = indirizzo di vet[i]
								LDRB r7, [r6]                   ; r7 = vet[i]
								LDRB r10, [r12]                 ; r10 = massimo trovato
								STRB r10, [r6]                  ; Metti massimo in vet[i]
								STRB r7, [r12]                  ; Metti vet[i] nel posto del massimo
								ADD r8, r8, #1                  ; Incrementa i
								MOV r9, r8                      ; Imposta j per il prossimo ciclo interno (inizia da i+1)
								B loop_outer_sort

end_sort						MOV r0, r1                      ; r0 = metto val e torno
								POP {r4-r8, r10-r11, pc}         ; Ritorna dai salvataggi dei registri
								ENDP

								EXPORT svuotaVet
			
svuotaVet						PROC
								PUSH {r4-r8, r10-r11, lr}  		; Salva i registri utilizzati
				
								; r0 = vet
								; r1 = index
								MOV r4, r0	; r4 = vet
								MOV r5, #0	; r5 = i
								MOV r6, #0	; r6 = 0 = valore azzerante
loop_svuota						CMP r5, r1
								BGE	end_svuota
								STRB r6, [r4]
								ADD r4, r4, #1		; vet[i]++
								ADD r5, r5, #1		; i++ (per confronto)
								B loop_svuota
				
end_svuota						POP{r4-r8, r10-r11, pc}
								ENDP

								EXPORT totale_pressioni_con_filtro
			
totale_pressioni_con_filtro		PROC
								PUSH {r4-r8, r10-r11, lr}  		; Salva i registri utilizzati
								; r0 = vet
								; r1 = index
								; r2 = min
								; r3 = max
								
								MOV r4, r0		; r4 = vet
								MOV r5, #0		; r5 = i
								MOV r7, #0		; r7 = totale finale
loop_confronto					CMP r5, r1
								BGE	end_confronto
								LDRB r6, [r4] 		; vet[i]
								ADD r4, r4, #1		; i++
								ADD r5, r5, #1		; i da confrontare++
confronto_min					CMP r6, r2
								BLT loop_confronto
confronto_max					CMP r6, r3
								BGT loop_confronto
								ADD r7, r7, r6		; totale += vet[i]
								B loop_confronto
					
end_confronto					MOV r0, r7			; torno il totale
								POP{r4-r8, r10-r11, pc}
								ENDP
									
								; versione di bubble sort crescente su 32 BIT (se 8 bit, LDRB, STRB, ADD #1 etc...)
								EXPORT sort_crescente
			
sort_crescente					PROC
								PUSH {r4-r8, r10-r11, lr}  		; Salva i registri utilizzati
								
								; r0 = vet
								; r1 = index
								MOV r4, r0                  ; r4 = vet (32 bit, altrimenti vedi su)
								MOV r5, r1                  ; r5 = index (# elementi)

								CMP r5, #1                  ; se la lunghezza del vettore è 1 o meno, esci
								BLE sort_crescente_exit

sort_crescente_outer_loop		MOV r6, #0                  ; Flag per controllare se ci sono stati scambi
								MOV r11, r5					; lunghezza ciclo interno

sort_crescente_inner_loop		LDR r7, [r4]                ; r7 = vet[i]
								ADD r10, r4, #4				; r10 = i+1
								LDR r8, [r10]            	; r8 = vet[i+1]

								CMP r7, r8                  ; Confronta i due elementi
								BLE sort_crescente_skip_swap

								; Scambia gli elementi
								STR r8, [r4]
								STR r7, [r10]
								MOV r6, #1                  ; Imposta il flag se c'è stato uno scambio

sort_crescente_skip_swap		ADD r4, r4, #4              ; i++
								SUBS r11, r11, #1             ; Decrementa il contatore del ciclo interno
								BGT sort_crescente_inner_loop	; se ho ancora elementi salta

								CMP r6, #1                  ; Controlla se ci sono stati scambi
								BNE sort_crescente_outer_loop

sort_crescente_exit				POP{r4-r8, r10-r11, pc}
								ENDP
									
								; versione di bubble sort decrescente su 32 BIT (se 8 bit, LDRB, STRB, ADD #1 etc...)
								EXPORT sort_decrescente
			
sort_decrescente				PROC
								PUSH {r4-r8, r10-r11, lr}  		; Salva i registri utilizzati
								
								; r0 = vet
								; r1 = index
								MOV r4, r0                  ; r4 = vet (32 bit, altrimenti vedi su)
								MOV r5, r1                  ; r5 = index (# elementi)

								CMP r5, #1                  ; se la lunghezza del vettore è 1 o meno, esci
								BLE sort_decrescente_exit

sort_decrescente_outer_loop		MOV r6, #0                  ; Flag per controllare se ci sono stati scambi
								MOV r11, r5					; lunghezza ciclo interno

sort_decrescente_inner_loop		LDR r7, [r4]                ; r7 = vet[i]
								ADD r10, r4, #4				; r10 = i+1
								LDR r8, [r10]            	; r8 = vet[i+1]

								CMP r7, r8                  ; Confronta i due elementi
								BGE sort_decrescente_skip_swap

								; Scambia gli elementi
								STR r8, [r4]
								STR r7, [r10]
								MOV r6, #1                  ; Imposta il flag se c'è stato uno scambio

sort_decrescente_skip_swap		ADD r4, r4, #4              ; i++
								SUBS r11, r11, #1             ; Decrementa il contatore del ciclo interno
								BGT sort_decrescente_inner_loop	; se ho ancora elementi salta

								CMP r6, #1                  ; Controlla se ci sono stati scambi
								BNE sort_decrescente_outer_loop

sort_decrescente_exit			POP{r4-r8, r10-r11, pc}
								ENDP
								
								END