#include "MatriceVettori.h"

/* Libreria per la gestione delle funzioni di base per Matrici e Vettori dinamici */


/* Costruttore matrice di m righe e n colonne */
Matrice costruisci_matrice (int m, int n)
{
    Matrice M;
    int i;

    M = (Matrice) calloc(m+1,sizeof(int *));

    check_allocazione_matrice (M);

    for (i = 0; i <= m; i++)
    {
        M[i] = (int *) calloc(n+1,sizeof(int));
        check_allocazione_vettore (M[i]);
    }

    return M;
}

/* Distruttore Matrice mxn */
void distruggi_matrice (Matrice* M, int m)
{
    int i;

    for (i = 0; i <= m; i++)
    {
        free((*M)[i]);
    }
    free(*M);

}

/* Stampo le soluzioni contenute nella matrice di marcatura MSoluzioni nel formato richiesto */
void stampa_soluzioni (Matrice* MSoluzioni, int m, int n)
{
    int i, j;

    for (i = 1; i <= m; i++)
    {
        for (j = 1; j <= n; j++)
        {
            if (((*MSoluzioni)[i][j]) == A) /* Se la cella e' presa durante l'orbita Ascendente (Colonne) marco la cella con A */
                printf("(%d,%d) A ", i, j);
             else if (((*MSoluzioni)[i][j]) == D) /* Se la cella e' presa durante l'orbita Discendente (Righe) marco la cella con D */
                printf("(%d,%d) D ", i, j);
            
            /* Altimenti la cella non e' nella soluzione e non stampo nulla */
        }
    }

    printf("\n"); 

}

/* Costruttore di vettore n dimensionale */
Vcap costruisci_vettore (int n)
{
    Vcap v;

    v = (int *) calloc(n+1,sizeof(int));

    check_allocazione_vettore (v);

    return v;
}

/* Copia il vettore V_orig nel vettore V_dest: i due vettori sono dinamici di dimensione n */
void copia_vettore (Vcap V_orig, Vcap V_dest, int n)
{
    int i;

    for (i = 0; i <= n; i++)
    {
        V_dest[i] = V_orig[i];
    }
}


/* Distuttore vettore */
void distruggi_vettore (Vcap* V)
{
    free(*V);
}

/* Controlla se l'allocazione della matrice e' andata a buon fine */
void check_allocazione_matrice (Matrice M)
{
    if (M == NULL)
    {
        fprintf(stderr,"Allocazione matrice dinamica fallita!\n");
        exit(EXIT_FAILURE);
    }
}

/* Controlla se l'allocazione del vettore e' andata a buon fine */
void check_allocazione_vettore (Vcap v)
{
    if (v == NULL)
    {
        fprintf(stderr,"Allocazione vettore dinamico fallita!\n");
        exit(EXIT_FAILURE);
    }
}