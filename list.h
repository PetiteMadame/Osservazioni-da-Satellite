#ifndef __list_h
#define __list_h

#include "MatriceVettori.h"

/* Elemento della lista di celle */
typedef struct cella {
    int i;
    int j;
    double ratio;
    Direzione dir;
    struct cella* succ; 
} cella;

cella* costruisci_cella (int i_r, int i_c, double r, Direzione d);

cella* costruisci_lista_celle (Matrice* Premi, Matrice* Pesi, int n, int i, Direzione d);

cella* lista_concatenata (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, int m, int n);

void distruggi_lista (cella* lista);

void swap (cella* cella1, cella* cella2);

cella* partition (cella* first, cella* last);

cella* ultima_cella (cella* radice_cella);

void quick_sort (cella* first, cella* last);

void check_allocazione_cella (cella* c);

#endif