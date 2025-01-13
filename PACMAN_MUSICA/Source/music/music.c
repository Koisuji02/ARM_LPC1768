#include "music.h"

#include "timer/timer.h"
#include "LPC17xx.h"

extern volatile NOTE tmp;

void playNote(NOTE* note)
{
		tmp = *note;
    // Se la frequenza è diversa da REST (ovvero silenzio), emetti il suono
    if (note->freq != REST)
    {
        // Reset del timer 0 per garantire che non ci siano interferenze da esecuzioni precedenti
        reset_timer(0);

        // Inizializzazione del timer con la frequenza della nota
        init_timer(0, 0, 0, 3, note->freq * AMPLIFIER * VOLUME); // Il periodo viene settato in base alla frequenza della nota

        // Abilita il timer per far partire il suono
        enable_timer(0);
    }
		//reset_timer(2);
		init_timer(2, 0, 1, 7, 0x3D090);
		enable_timer(2);
}


BOOL isNotePlaying(void)
{
	return ((LPC_TIM0->TCR != 0) || (LPC_TIM2->TCR != 0));
}
