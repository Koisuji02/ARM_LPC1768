				AREA asm_functions, CODE, READONLY				
				EXPORT  check_square
check_square	
				PUSH {r4,r5,r6,r7,lr}	; salvo i registri
				MUL r4, r0, r0			; r4 = x^2
				MUL r5, r1, r1			; r5 = y^2
				ADD r6, r4, r5			; r6 = x^2 + y^2
				MUL r7, r2, r2			; r7 = r^2
				CMP r6, r7				; confronto
				MOVLE r0, #1			; se x^2+y^2<=r^2 torna 1
				MOVGT r0, #0			; altrimenti 0
				POP {r4,r5,r6,r7,lr}	; ripristino i registri ai valori prima della checksquare
				BX lr					; ritorno alla chiamata
                END