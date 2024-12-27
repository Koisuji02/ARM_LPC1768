				AREA    |.text|, CODE, READONLY
				EXPORT BubbleSort
BubbleSort
				MOV   r12, sp
				PUSH{r4-r8,r10-r11,lr}	; save volatile registers			

				; r0 = buffer ordinato
                ; r1 = valore da inserire
                ; r2 = indice corrente (i)

                ADD r3, r0, r2, LSL #2     ; r3 = indirizzo buffer[i] (calcolo offset) [shifto di 2 perchè mi serve vera posizione quindi i*4 perchè sono su 32 bit]
loop
				SUB r3, r3, #4				; indirizzo di buffer[i-1]
				LDR r4, [r3]         		; Carica buffer[i-1]
                CMP r4, r1                 ; Confronta buffer[i-1] con valore inserito nella funct
                BLS done                   ; Se buffer[i-1] <= valore, termina
                STR r4, [r3]               ; Sposta buffer[i-1] a buffer[i]
                SUB r3, r3, #4             ; Decrementa l'indirizzo (sposta indietro)
                CMP r3, r0                 ; Confronta con inizio buffer
                BHI loop             ; Ripeti se non si è raggiunto l'inizio

done
                STR r1, [r3]               ; Inserisce il valore nella posizione corretta

				POP{r4-r8,r10-r11,lr}		; restore volatile registers
				BX lr
				; così mi salvo direttamente lr in pc e salto diretto alla istruzione corretta
				END