#include <stdlib.h>
// #include <rt_fp.h> // per la __aeabi_fdiv, ma in realtà non serve ho già IMPORT
#include <stdint.h> // per il tipo uint8_t (da byte ad unsigned_int su 8 bit)

extern int check_square(int x, int y, int r);
extern float my_division(float* a, float* b);

extern uint8_t ROWS[]; // dichiaro tutto come matrici per poter prendere il valore all'interno
extern uint8_t COLUMNS[];
extern int Matrix_Coordinates[]; // posso passare da DCD (32 bit) a int (32 bit) proprio perchè su 32 bit

float	compute_pigreco(int raggio){
	// dichiarazioni e inizializzazioni
	int i, x, y, area = 0, row = ROWS[0], col = COLUMNS[0];
	int maxsize = row*col;
	float r_quadrato = (float)(raggio*raggio), area_f = 0; // casting a float per la funzione my_division
	float pigreco = 0;
	
	// calcolo dell'area sulla matrice di punti
	for(i=0; i<maxsize; i+=2){
		x = Matrix_Coordinates[i];
		y = Matrix_Coordinates[i+1];
		area += check_square(x,y,raggio); // se il punto è incluso +1, altrimenti +0; metto il casting a float per la my_division
	}
	
	// applico la formula e trovo pigreco
	area_f = (float) area;
	pigreco = my_division(&area_f, &r_quadrato);
	return pigreco;
}

int main(void){
	// faccio le chiamate a compute_pigreco con i valori richiesti del raggio
	compute_pigreco(2);
	compute_pigreco(3);
	compute_pigreco(5);
	while(1); // oppure return 0; ? da capire
}
