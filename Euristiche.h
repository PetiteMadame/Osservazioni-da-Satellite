#ifndef __Euristiche_h
#define __Euristiche_h

#include "stack.h"
#include "list.h"

int Max (int a, int b);

stato* KnapSackDinamico (Matrice* Premi, Matrice* Pesi, int m, int n, int riga, int colonna, int delta_righe, int delta_colonne, int capacita, Matrice MatSol);

stato* KSD_Righe (Matrice* Premi, Matrice* PesiRiga, int riga, int n, int m, Vcap* c_r, Matrice MatSol);

stato* KSD_Colonne (Matrice* Premi, Matrice* PesiColonna, int colonna, int n, int m, Vcap* c_c, Matrice MatSol);

void aggiorna_soluzione (stato* ks, Matrice* MatSol, Direzione visita);

boolean residuo (Matrice* MatSol, Matrice* Pesi, Vcap* capacita, int peso, Direzione visita, int index, int n);

#endif