#include "stack.h"

/* Libreria per la gestione di Stack Dinamica (LIFO) implementata dinamicamente tramite linked list 
   Struttura dati per il KSDinamico (DiscAsc e AscDisc) */

/* Controlla se lo stack e' vuoto */
boolean isEmpty (stack S)
{
    if (S == NULL) return TRUE;

    return FALSE;
}


/* Aggiunge in testa il nodo con [i_r,i_c] */
void push (stack* S, int i_r, int i_c)
{
    nodo* nuovo_nodo;

    nuovo_nodo = (nodo *) malloc(sizeof(nodo));
    check_allocazione_nodo (nuovo_nodo);
    nuovo_nodo->i = i_r;
    nuovo_nodo->j = i_c;
    nuovo_nodo->succ = *S;
    *S = nuovo_nodo;
}

/* Elimina nodo in testa */
void pop (stack* S)
{
    nodo* temp;

    temp = (nodo *) malloc(sizeof(nodo));
    check_allocazione_nodo (temp);

    if (isEmpty (*S) == TRUE) /* Se lo stack e' vuoto */
        printf("Lo stack e' vuoto.\n");
    
    else
    {
        temp = *S;
        temp->i = (*S)->i;
        temp->j = (*S)->j;
        *S = (*S)->succ; /* Scalo */
        free (temp);
    }

}

/* Distruggo lo stato e lo stack S associato */
void free_stato (stato* Stack_stato)
{
    posizione pos_succ, pos_att;

    pos_att = Stack_stato->zaino;
    while (pos_att != NULL)
    {
        pos_succ = pos_att->succ;
        pop (&pos_att); 
        pos_att = pos_succ;
    }
    free(Stack_stato);
}

/* Controlla se l'allocazione del nodo e' andata a buon fine */
void check_allocazione_nodo (nodo* n)
{
    if (n == NULL)
    {
        fprintf(stderr,"Allocazione nodo fallita!\n");
        exit(EXIT_FAILURE);
    }
}

/* Controlla se l'allocazione del nodo e' andata a buon fine */
void check_allocazione_stato (stato* s)
{
    if (s == NULL)
    {
        fprintf(stderr,"Allocazione stato fallita!\n");
        exit(EXIT_FAILURE);
    }
}