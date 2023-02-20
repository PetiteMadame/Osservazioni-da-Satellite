#ifndef __stack_h
#define __stack_h

#include "MatriceVettori.h"

typedef struct _nodo nodo;
typedef nodo* stack; /* Top dello stack */
typedef nodo* posizione;

/* Struttura stato per algoritmo KSDinamico, ad ogni stato e' associato uno stack con le celle aggiunte alla soluzione corrente */
typedef struct stato{
    int premio;
    int spazio;
    stack zaino;
} stato;

struct _nodo{
    int i; /* Indice riga cella */
    int j; /* Indice colonna cella */
    posizione succ; /* puntatore a nodo successivo nella stack */
};

boolean isEmpty (stack S);

void push (stack* S, int i, int j);

void pop (stack* S);

void free_stato (stato* Stack_stato);

void check_allocazione_nodo (nodo* n);

void check_allocazione_stato (stato* s);

#endif