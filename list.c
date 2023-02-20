#include "list.h"

/* Implementazione lista undirezionale con radice tramite puntatori: liste da ordinare per scelta el max in ogni riga e colonna
   Struttura dati per Greedy e Approx */

/* Costruttore di cella con campo indice riga, colonna, ratio per ordinamento e modalita' di ripresa */
cella* costruisci_cella (int i_r, int i_c, double r, Direzione d)
{
	cella* nuova_cella = (cella *) malloc (sizeof(cella));
    check_allocazione_cella (nuova_cella);
	nuova_cella->i = i_r;
	nuova_cella->j = i_c;
    nuova_cella->ratio = r;
    nuova_cella->dir = d;
	nuova_cella->succ = NULL;
	return nuova_cella;
}


/* Costruttore lista di celle della riga i_esima/ colonna j_esima
   Premi = matrice di Premi
   Pesi = matrice dei Pesi (risp Riga o Colonna)
   n = numero di righe o colonne (se voglio lista righe passo numero colonne e viceversa)
   i = indice della riga/colonna che voglio estrarre 
   d = indica se voglio prenderla per righe o colonne (D = Righe (Discendente)/ A = Colonne (Ascendente))
   */
cella* costruisci_lista_celle (Matrice* Premi, Matrice* Pesi, int n, int i, Direzione d)
{
	int j;
	cella* nuova_cella;
    cella* radice_cella;
    cella* cella_corrente;

    radice_cella = NULL; 
    cella_corrente = radice_cella;

	for (j  = 1; j <= n; j++)
	{
        
        if (d == D)
		    nuova_cella = costruisci_cella (i, j, (double) (*Premi)[i][j]/(*Pesi)[i][j], d);
        else if (d == A)
		    nuova_cella = costruisci_cella (j, i, (double) (*Premi)[j][i]/(*Pesi)[j][i], d);
        else
            printf("La modalita' di estrazione delle celle non e' consentita!\n");
        if (radice_cella == NULL)
            radice_cella = nuova_cella;
        else
            cella_corrente->succ = nuova_cella;

		cella_corrente = nuova_cella;
	}
    cella_corrente->succ = NULL;
	
	return radice_cella;
}

/* Costruisce lista concatenata di celle prima per righe e poi per colonne: a ogni passo appende in coda alla lista corrente la successiva */
cella* lista_concatenata (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, int m, int n)
{
    int i;
    cella* lista;
    cella* testa;
    cella* temp;
    lista = NULL;
    testa = NULL;

    for (i = 1; i <= m; i++)
    {
        temp = costruisci_lista_celle (Premi, PesiRiga, n, i, D);
        if (testa == NULL)
            testa = temp;
        else
            lista->succ = temp;

        lista = ultima_cella (temp);

    }

    for (i = 1; i <= n; i++)
    {
        temp = costruisci_lista_celle (Premi, PesiColonna, m, i, A);
        lista->succ = temp;
        lista = ultima_cella (temp);
    }

    return testa;
}
void distruggi_lista (cella* lista) 
{
    cella* cella_corrente;

    while (cella_corrente != NULL)
    {
        cella_corrente = lista->succ;
        free (lista);
        lista = cella_corrente;
    }
}

/* Scambia le due celle cella1 e cella2 */
void swap (cella* cella1, cella* cella2)
{
    cella* temp = (cella *) malloc(sizeof(cella));
    check_allocazione_cella (temp);

    /* temp "=" cella2 */
    temp->i = cella2->i;
    temp->j = cella2->j;
    temp->ratio = cella2->ratio;
    temp->dir = cella2->dir;

    /* Scambio le celle campo per campo */
    cella2->i = cella1->i;
    cella2->j = cella1->j;
    cella2->ratio = cella1->ratio;
    cella2->dir = cella1->dir;

    cella1->i = temp->i;
    cella1->j = temp->j;
    cella1->ratio = temp->ratio;
    cella1->dir = temp->dir;

    free(temp);
}

/* Scorre la sottolista tra first e last e sposta le celle confrontando
   -il ratio (premio/peso)
   -a parita' di ratio prende la cella con indice di riga maggiore
   -a parita' di riga prende la cella con indice di colonna maggiore 
   -a parita' di cella prende quella in direzione Discendente (Righe) */
cella* partition (cella* first, cella* last)
{
    /* Partendo dal primo nodo */
    cella* pivot;
    cella* front;

    pivot = first;
    front = first;

    while ((front != NULL) && (front != last)) /* Finche' ci sono nodi da confontare... */
    {
        if (front->ratio > last->ratio) /* Primo ordinamento sul ratio */
        {
            pivot = first;
            swap (front, first);
            first = first->succ; /* Passo al nodo successivo */
        }
        else if (front->ratio == last->ratio) /* A parita' di ratio faccio un secondo ordinamento indice di riga */
        {
            if (front->i > last->i)
            {
                pivot = first;
                swap (front, first);
                first = first->succ;
            }
            else if (front->i == last->i) /* A parita' di indice di riga, ordino per indice di colonna */
            {
                if (front->j > last->j)
                {
                    pivot = first;
                    swap (front, first);
                    first = first->succ;
                }
                else if (front->j == last->j) /* Se la stessa cella, la prendo per righe */
                {
                    if (front->dir == D)
                    {
                        pivot = first;
                        swap (front, first);
                        first = first->succ;
                    }
                    
                }
            }
        }
        front = front->succ; /* Nodo successivo */
    }
    swap (last,first);
    return pivot;
}

/* Trova l'ultimo nodo nella lista di celle
   radice_cella: elemento che punta alla lista di celle */
cella* ultima_cella (cella* radice_cella)
{
    cella* temp;

    temp = radice_cella;
    while ((temp != NULL) && (temp->succ != NULL)) /* Scorro tutta la lista */
    {
        temp = temp->succ;
    }
    return temp;
}

/* Ordina la lista di celle con l'algoritmo QuickSort  (Caso medio O((mxn)xlog(mxn) caso peggiore O(m^2xn^2))
   first: primo nodo della lista da ordinare
   last: ultimo nodo della lista da ordinare */
void quick_sort (cella* first, cella* last)
{

    cella* pivot;

    if (first == last) return; /* Lista gia' ordinata (un solo elemento): caso base */
    
    pivot = partition (first, last);

    if ((pivot != NULL) && (pivot->succ != NULL)) /* Quicksort su sottolista dx */
    {
        quick_sort (pivot->succ, last);
    }
    if ((pivot != NULL) && (first != pivot)) /* Quicksort su sottolista sx */
    {
        quick_sort (first, pivot);
    }
}

/* Controlla se l'allocazione della cella e' andata a buon fine */
void check_allocazione_cella (cella* c)
{
    if (c == NULL)
    {
        fprintf(stderr,"Allocazione cella fallita!\n");
        exit(EXIT_FAILURE);
    }
}