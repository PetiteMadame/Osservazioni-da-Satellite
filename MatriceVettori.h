#ifndef __MatriceVettori_h
#define __MatriceVettori_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_LENGTH 256

typedef int boolean;
#define TRUE  1
#define FALSE 0

typedef enum _Direzione {A = 1, D = -1, NONE = 0} Direzione; /* Enum per direzione di ripresa della cella */

typedef int** Matrice; /* Matrice dinamica di interi */
typedef int* Vcap; /* Vettore dinamico di interi */


Matrice costruisci_matrice (int m, int n);

void distruggi_matrice (Matrice* M, int m);

void stampa_soluzioni (Matrice* MSoluzioni, int m, int n);

Vcap costruisci_vettore (int n);

void copia_vettore (Vcap V_orig, Vcap V_dest, int n);

void distruggi_vettore (Vcap* V);

void check_allocazione_matrice (Matrice M);

void check_allocazione_vettore (Vcap v);

#endif