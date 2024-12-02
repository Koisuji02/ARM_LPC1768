				AREA asm_functions, CODE, READONLY					
				EXPORT  my_division
my_division		; r0 = a, r1 = b
				PUSH {r4-r7,lr}		; salvo i registri
				IMPORT __aeabi_fdiv			; ci vuole la IMPORT per usarla
				; devo prendere i valori dagli indirizzi
				LDR r0, [r0]			; r0 = *(&area_f)
				LDR r1, [r1]			; r1 = *(&r_quadrato)
				BL __aeabi_fdiv				; chiama la div della lib Arm (emulare SW la fp unit)
				; questa div prende automaticamente r0 e r1 e mette il risultato in r0 quindi non devo scrivere altro
				POP {r4-r7,lr}		; come nella check ripristno
				BX lr				; ritorno alla chiamata
                END