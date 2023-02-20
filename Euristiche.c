#include "Euristiche.h"

/* Libreria di funzioni ausiliarie per l'implementazione degli algoritmi euristici DiscAsc, AscDisc e Approx */

/* Implementazione dinamica dell'algoritmo iterativo KP mediante tecnica bottom up */

/* Funzione massimo tra due interi */
int Max (int a, int b) 
{
    if (a > b) return a;
    return b;
}

/* Algoritmo di base dinamico per il KnapSack problem 
   Premi = Matrice Premi
   Pesi = Matrice dei Pesi (rispettivamente Riga e Colonna nelle chiamate successive)
   n = #colonne
   m = #righe
   riga = colonna notevole da vautare
   colonna = riga notevole da valutare
   delta_righe = step di riga
   delta_colonne = step di colonna 
   capacita = capacita' corrente della rispettiva riga/colonna da valutare 
   MatSol = matrice delle soluzioni dell'algoritmo 
   Idea: suddivido nei sottoprblemi e aggiorno l'ottimo a ogni passo, inserendo in una matrice (n_elementi+)x(capacita'+1) la soluzione migliore
   valutando il peso e il premio dell'elemento della corrispondente riga rispetto al vincolo di capacita' presente sulla colonna corrispondente */
stato* KnapSackDinamico (Matrice* Premi, Matrice* Pesi, int m, int n, int riga, int colonna, int delta_righe, int delta_colonne, int capacita, Matrice MatSol)
{
	
	int i, j;
    int n_elementi, elemento;
    int capacita_attuale;
    int peso_nuovo_elemento;
    int premio_nuovo_elemento;
    int ottimo_precedente;
    Matrice knapsack;
    stato* ottimo; /* Qui verranno inseriti gli elementi (i,j) visitando la matrice delle soluzioni knapsack */
    /* Determina se sto percorrendo righe o colonne */
    if (delta_righe == 0) /* Sto percorrendo una riga */
    {   
        n_elementi = n;
    }
    else /* Sto percorrendo una colonna */
    {
        n_elementi = m;
    }
    /* Matrice dei sottoproblemi di knapsack */
    knapsack = costruisci_matrice (n_elementi, capacita); /* Inizializzata a zero con la calloc, per cui il caso base e' gia' stato inserito */

	for (    /* Per ogni elemento della riga o della colonna che sto considerando */
            i = riga,          j = colonna,        elemento=1;    /* Riga e colonna iniziali dipendono dalla visita (Disc o Asc); a prescindere considero il primo elemento dello zaino */
            i <= m  && j <= n;                                    /* Termina iterazione quando ho considerato tutti gli elementi della riga o della colonna*/
            i += delta_righe,  j += delta_colonne, elemento++)    /* Aumenta solo la riga o solo la colonna a seconda della visita (Disc o Asc) */
    {
        /* Considero la riga pari all'elemento attuale */
        for (capacita_attuale = 1; capacita_attuale <= capacita; capacita_attuale++) /* Risolvo tutti i sottoproblemi per capacita' crescente*/
        {
            ottimo_precedente = knapsack[elemento-1][capacita_attuale];
            knapsack[elemento][capacita_attuale] = ottimo_precedente; /* La soluzione ottima e' buona almeno quanto quella per il sottoproblema precedente */
            
            /* Posso aggiungere il nuovo elemento allo zaino? */
            peso_nuovo_elemento = (*Pesi)[i][j];
            if (peso_nuovo_elemento <= capacita_attuale)
            {
                /* Se si', posso aggiungere il nuovo elemento o l'ho gia' preso nella visita precedente? */
                if ( MatSol == NULL || MatSol[i][j] == NONE)
                {
                    /* Dovrei aggiungerlo oppure no? Migliora l'ottimo? */
		            premio_nuovo_elemento = (*Premi)[i][j];
		            knapsack[elemento][capacita_attuale] = Max (ottimo_precedente, premio_nuovo_elemento + knapsack[elemento-1][capacita_attuale-peso_nuovo_elemento]);
                }
            }
        }
    }
    /* Ricostruisci soluzione percorrendo la matrice knapsack */
    ottimo = (stato*) calloc(1, sizeof(stato));
    check_allocazione_stato (ottimo);
    elemento = n_elementi;
    capacita_attuale = capacita;
    ottimo -> premio = knapsack[elemento][capacita_attuale];
    ottimo -> spazio = 0; 
    for(
            premio_nuovo_elemento = ottimo->premio;
            premio_nuovo_elemento != 0 && elemento > 0;
            elemento--) /* Se il premio e' 0 sono arrivato al caso base */
    {
        premio_nuovo_elemento = knapsack[elemento][capacita_attuale];

        if (premio_nuovo_elemento != knapsack[elemento-1][capacita_attuale]) /* Ho preso questo elemento */
        {
            /* Determina se sto percorrendo righe o colonne */
            if (delta_righe == 0) /* Sto percorrendo una riga */
            {   
                i = riga;
                j = elemento;
            }
            else /* Sto percorrendo una colonna */
            {
                i = elemento;
                j = colonna;
            }
            peso_nuovo_elemento = (*Pesi)[i][j];
            /* Aggiungi soluzione allo stato */
            ottimo->spazio+= peso_nuovo_elemento;
            push (&(ottimo->zaino), i, j);
            /* Aggiorna indice */
            capacita_attuale -= peso_nuovo_elemento;
        }
    }
    distruggi_matrice (&knapsack, n_elementi);
    return ottimo;
}

/* Algoritmo KSDinamico per Righe: richiama l'algoritmo di base KSDinamico per trovare l'ottimo nella riga, valutando il vincolo sulla capacita' */
stato* KSD_Righe (Matrice* Premi, Matrice* PesiRiga, int riga, int n, int m, Vcap* c_r, Matrice MatSol)
{
	return KnapSackDinamico (Premi, PesiRiga, m, n, riga, 1, 0, 1, (*c_r)[riga], MatSol);
}

/* Algoritmo KSDinamico per Colonne: richiama l'algoritmo di base KSDinamico per trovare l'ottimo nella colonna, valutando il vincolo sulla capacita' */
stato* KSD_Colonne (Matrice* Premi, Matrice* PesiColonna, int colonna, int n, int m, Vcap* c_c, Matrice MatSol)
{
	return KnapSackDinamico (Premi, PesiColonna, m, n, 1, colonna, 1, 0, (*c_c)[colonna], MatSol);
}

/* Allo stato attuale ks, aggiorna la soluzione MatSol marcandola con la direzione di visita ottima */
void aggiorna_soluzione (stato* ks, Matrice* MatSol, Direzione visita)
{
    stack elementi;
    int i, j;


    for(elementi = ks->zaino; elementi != NULL; elementi = elementi->succ)
    {
        i = elementi->i;
        j = elementi->j;
        (*MatSol)[i][j] = visita;
    }

}

/* Controlla che la cella non ripresa rispetti il vincolo di capacita' per essere aggiunta nell'ottimo aggiornato */
boolean residuo (Matrice* MatSol, Matrice* Pesi, Vcap* capacita, int peso, Direzione visita , int index, int n)
{
    int i;
    int sum;

    sum = 0;

    /* Visita Ascendente, scorro la colonna index-esima e aggiungo il peso della cella corrente alla memoria occupata */
    if (visita == A)
    {
        for (i = 1; i <= n; i++)
        {
            if ((*MatSol)[i][index] == A)
            sum += (*Pesi)[i][index];
        }

    }

    /* Visita Discendente, scorro la riga index-esima e aggiungo il peso della cella corrente alla memoria occupata */
    else
    {
        for (i = 1; i <= n; i++)
        {
            if ((*MatSol)[index][i] == D)
            sum += (*Pesi)[index][i];
        }

    }
    if (((*capacita)[index]-sum-peso)>=0) return TRUE; /* Controllo che venga rispettato il vincolo di capacita' massimo */
    
    else return FALSE;

}