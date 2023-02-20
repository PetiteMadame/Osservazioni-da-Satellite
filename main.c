/* Codice principale progetto Osservazioni da Satellite */

/* Direttive */

#include "Euristiche.h"

/* Prototipi delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *filedati);

void Parse (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, Vcap* c_r, Vcap* c_c, int* m, int* n, char *filedati);

int Totale (Matrice* Premi, int m, int n);

int Zaino (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, int m, int n, Vcap* c_r, Vcap* c_c, stato** zaino_righe, stato** zaino_colonne);

Matrice Greedy (Matrice* PesiRiga, Matrice* PesiColonna, int m, int n, Vcap* c_r, Vcap* c_c, cella* lista);

Matrice DiscAsc (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, int m, int n, Vcap* c_r, Vcap* c_c, stato** zaino_righe);

Matrice AscDisc (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, int m, int n, Vcap* c_r, Vcap* c_c, stato** zaino_colonne);

Matrice Approx (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, int m, int n, Vcap* c_r, Vcap* c_c, stato** zaino_righe, stato** zaino_colonne, cella* lista);

int Stima_Euristica (Matrice* Premi, Matrice* MatSol, int m, int n);

/* Programma principale */
int main (int argc, char *argv[])
{
    /* Parte dichiarativa */
    char filedati[ROW_LENGTH];
    Matrice Premi, PesiRiga, PesiColonna; /* Matrici DATA */
    Vcap cap_row, cap_column; /* Vettori Capacita' */
    int n, m; /* Numero di colonne e di righe delle matrici */
    Matrice MatSolGreedy, MatSolDA, MatSolAD, MatSolApprox; /* Matrici di marcatura per conservare le soluzioni dei rispettivi algoritmi */
    int stima_greedy, stima_DA, stima_AD, stima_approx;
    int zaino;
    cella* lista_celle;
    stato** zaino_r;
    stato** zaino_c; 

    /* Parte esecutiva */

    /* Legge da linea di comando il file che contiene i dati */
    InterpretaLineaComando (argc, argv, filedati);

    /* Leggo e carico dati dal file con opportuno parsing */
    Parse (&Premi, &PesiRiga, &PesiColonna, &cap_row, &cap_column, &m, &n, filedati);

    /* Alloco memoria necessaria per gli zaini rispettivamente per righe e per colonne */
    zaino_r = (stato **) malloc(sizeof(stato*)*m);
    if (zaino_r == NULL)
    {
        fprintf(stderr,"Allocazione zaino righe fallita!\n");
        exit(EXIT_FAILURE);
    }
    zaino_c = (stato **) malloc(sizeof(stato*)*n);
    if (zaino_c == NULL)
    {
        fprintf(stderr,"Allocazione zaino colonne fallita!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Calcolo stima Zaino e costruisco i due zaini per righe e colonne risolvendo KP esattamente con il KSDinamico */
    zaino = Zaino (&Premi, &PesiRiga, &PesiColonna, m, n, &cap_row, &cap_column, zaino_r, zaino_c);

    /* Costuisco lista concatenata di celle, appendendo man mano una riga alla volta e poi una colonna alla volta */
    lista_celle = lista_concatenata (&Premi, &PesiRiga, &PesiColonna, m, n);

    /* Ordino la lista secondo i criteri richiesti */
    quick_sort (lista_celle, ultima_cella (lista_celle));
    
    /* Computo le soluzioni con i vari algoritmi euristici e le stime associate all'ottimo */
    MatSolGreedy = Greedy (&PesiRiga, &PesiColonna, m, n, &cap_row, &cap_column, lista_celle);
    MatSolDA = DiscAsc (&Premi, &PesiRiga, &PesiColonna, m, n, &cap_row, &cap_column, zaino_r);
    MatSolAD = AscDisc (&Premi, &PesiRiga, &PesiColonna, m, n, &cap_row, &cap_column, zaino_c);
    MatSolApprox = Approx (&Premi, &PesiRiga, &PesiColonna, m, n, &cap_row, &cap_column, zaino_r, zaino_c, lista_celle);

    stima_greedy = Stima_Euristica (&Premi, &MatSolGreedy, m,  n);
    stima_DA = Stima_Euristica (&Premi, &MatSolDA, m, n);
    stima_AD = Stima_Euristica (&Premi, &MatSolAD, m, n);
    stima_approx = Stima_Euristica (&Premi, &MatSolApprox, m, n);

    /* Libero le strutture dati che non mi servono piu': zaini, matrici dei pesi, vettori capacita' e lista concatenata */
    free_stato (*zaino_r);
    free (zaino_r);
    free_stato (*zaino_c);
    free (zaino_c);
    distruggi_matrice (&PesiRiga, m);
    distruggi_matrice (&PesiColonna, m);
    distruggi_vettore (&cap_row);
    distruggi_vettore (&cap_column);
    distruggi_lista (lista_celle);

    /* Stampo le soluzioni nel formato richiesto */
    printf("Greedy %d ", stima_greedy);
    stampa_soluzioni (&MatSolGreedy, m, n);
    printf("DiscAsc %d ", stima_DA);
    stampa_soluzioni (&MatSolDA, m, n);
    printf("AscDisc %d ", stima_AD);
    stampa_soluzioni (&MatSolAD, m, n);
    printf("Approx %d ", stima_approx);
    stampa_soluzioni (&MatSolApprox, m, n);
    printf("Totale %d\n", Totale (&Premi, m, n));
    printf("Zaino %d\n", zaino);

    /* Dealloco le strutture rimanenti */
    distruggi_matrice (&Premi, m);
    distruggi_matrice (&MatSolGreedy, m);
    distruggi_matrice (&MatSolDA, m);
    distruggi_matrice (&MatSolAD, m);
    distruggi_matrice (&MatSolApprox, m);

    return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

/* Legge dalla linea di comando il file che contiene i dati */
void InterpretaLineaComando (int argc, char *argv[], char *filedati)
{
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(filedati,argv[1]);
}

void Parse (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, Vcap* c_r, Vcap* c_c, int* m, int* n, char *filedati)
{
    FILE* fp;
    char chunk[ROW_LENGTH]; 
    int i, j, aux;
	

    /* Apro il file e controllo che sia andata a buon fine l'operazione di apertura */
	fp = fopen(filedati, "r");
    if (fp == NULL)
    {
        fprintf(stderr,"Errore nell'apertura del file %s!\n",filedati);
        exit(EXIT_FAILURE);
    }

    fscanf (fp, "param m := %d ;", m);
    fgets(chunk, ROW_LENGTH, fp); /* Vado alla riga successiva */
    fscanf (fp, "param n := %d ;", n);
    fgets(chunk, ROW_LENGTH, fp);

    /* Inizializzo matrici e i vettori */
    *Premi = costruisci_matrice (*m, *n);
    *PesiRiga = costruisci_matrice (*m, *n);
    *PesiColonna = costruisci_matrice (*m, *n);
    *c_r = costruisci_vettore (*m);
    *c_c = costruisci_vettore (*n);

    fgets(chunk, ROW_LENGTH, fp); /* Salto riga vuota */
    fgets(chunk, ROW_LENGTH, fp); /* Salto riga param pi := */
    /* Salvo dati nella matrice dei premi */
    while (fscanf(fp, " [%d ,%d ] %d ", &i, &j, &aux) == 3) /* Fino al punto e virgola... */
    {
        (*Premi)[i][j] = aux;
    }

    fgets(chunk, ROW_LENGTH, fp); /* salto riga ; */
    fgets(chunk, ROW_LENGTH, fp); 
    fgets(chunk, ROW_LENGTH, fp); /* salto riga param rho := */
    /* Salvo dati nella matrice dei pesi di riga */
    while (fscanf(fp, " [%d ,%d ] %d ", &i, &j, &aux) == 3)
    {
        (*PesiRiga)[i][j] = aux;
    }

    fgets(chunk, ROW_LENGTH, fp); 
    fgets(chunk, ROW_LENGTH, fp);
    fgets(chunk, ROW_LENGTH, fp); /* salto riga param sigma := */
    /* Salvo dati nella matrice dei pesi di colonna */
    while (fscanf(fp, " [%d ,%d ] %d ", &i, &j, &aux) == 3)
    {
        (*PesiColonna)[i][j] = aux;
    }

    
    fgets(chunk, ROW_LENGTH, fp);
    fgets(chunk, ROW_LENGTH, fp);
    fgets(chunk, ROW_LENGTH, fp); /* salto riga param r := */
    /* Salvo dati nel vettore capacita' di riga */
    while (fscanf(fp, " %d %d ", &i, &aux) == 2)
    {
        (*c_r)[i] = aux;
    }

    fgets(chunk, ROW_LENGTH, fp);
    fgets(chunk, ROW_LENGTH, fp);
    fgets(chunk, ROW_LENGTH, fp); /* salto riga param c := */
    /* Salvo dati nel vettore capacita' di colonna */
    while (fscanf(fp, "%d %d ", &j, &aux) == 2)
    {
        (*c_c)[j] = aux;
    }

	fclose(fp);
}

/* Scorro la matrice dei premi e sommo per la stima del Totale */
int Totale (Matrice* Premi, int m, int n)
{
    int i, j;
    int sum;

    sum = 0;

    for (i = 1; i<= m; i++)
    {    for (j = 1; j<= n; j++)
        {
            sum += (*Premi)[i][j];
        }
    }
    return sum;
}

/* Sommo gli ottimi degli KP associati alle righe e alle colonne, aggiornando man mano gli zaini relativi alle righe e alle colonne
   Risolvendo esattamente con l'utilizzo del KSDinamico */
int Zaino (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, int m, int n, Vcap* c_r, Vcap* c_c, stato** zaino_righe, stato** zaino_colonne)
{
    int i;
    int sum;
    stato* s;

    sum = 0;

    for (i = 1; i <= m; i++)
    {
        s = KSD_Righe (Premi, PesiRiga, i, n, m, c_r, NULL);
        zaino_righe[i-1] = s;
        sum += s->premio;
    }

    for (i = 1; i <= n; i++)
    {
        s = KSD_Colonne (Premi, PesiColonna, i, n, m, c_c, NULL);
        zaino_colonne[i-1] = s;
        sum += s->premio;
    }

    return sum;
}

/* Risolve il problema dello zaino con l'euristica Greedy */
Matrice Greedy (Matrice* PesiRiga, Matrice* PesiColonna, int m, int n, Vcap* c_r, Vcap* c_c, cella* lista)
{
    int i_r, i_c;
    Matrice MatSol;
    Direzione visita;
    Vcap cap_r, cap_c;

    cap_r = costruisci_vettore(m);
    cap_c = costruisci_vettore(n);
    copia_vettore(*c_r, cap_r, m);
    copia_vettore(*c_c, cap_c, n);
    MatSol = costruisci_matrice( m, n);

    /* Scorro tutta la lista sortata controllando di non violare i vincoli di doppia ripresa e capacita' */
    for (; lista->succ != NULL; lista = lista->succ)
    {
        i_r = lista->i;
        i_c = lista->j;
        visita = lista->dir;
        
        /* Se ripresa durante l'orbita Ascendente (Colonne) */
        if ( visita == A)
        {
            if (cap_c[i_c] > 0) /* Se ho memoria disponibile... */
            {
                /* Controllo sul vincolo di capacita' */
                if ( (*PesiColonna)[i_r][i_c] <= cap_c[i_c] )
                {
                    if (MatSol[i_r][i_c] == NONE) /* Controllo sul vincolo di doppia ripresa */
                    {
                        MatSol[i_r][i_c] = A; /* Marco la matrice soluzione con la direzione di ripresa */
                        cap_c[i_c] -= (*PesiColonna)[i_r][i_c]; /* Aggiorno la capacita' residua nella i_c-esima colonna */
                    }
                    
                }

            }
                
        }

        else
        {
            if (cap_r[i_r] > 0) /* Se ho memoria disponibile... */
            {
                /* Controllo sul vincolo di capacita' */
                if ( (*PesiRiga)[i_r][i_c] <= cap_r[i_r] )
                {
                    if (MatSol[i_r][i_c] == NONE) /* Controllo sul vincolo di doppia ripresa */
                    {
                        MatSol[i_r][i_c] = D; /* Marco la matrice soluzione con la direzione di ripresa */
                        cap_r[i_r] -= (*PesiRiga)[i_r][i_c]; /* Aggiorno la capacita' residua nella i_r-esima riga */
                    }
                    
                }

            }
        }
    }

    distruggi_vettore (&cap_r);
    distruggi_vettore (&cap_c);
    return MatSol;
}

/* Risolvo esattamente per ogni riga il KP e poi per colonna, considerando le scelte fatte nella prima fase */
Matrice DiscAsc (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, int m, int n, Vcap* c_r, Vcap* c_c, stato** zaino_righe)
{
    int i;
    Matrice MatSol;
    stato* s;

    MatSol = costruisci_matrice( m, n);

    /* Prima fase (Disc): risolvo il KP per righe sfuttando lo zaino delle righe */
    for (i = 1; i <= m; i++)
    {
        aggiorna_soluzione (zaino_righe[i-1], &MatSol, D);
    }

    /* Seconda fase (Asc): risolvo il KP per colonne con KSDinamico, tenendo conto delle scelte fatte nella fase Disc (conservate in MatSol) */
    for (i = 1; i <= n; i++)
    {
        s = KSD_Colonne (Premi, PesiColonna, i, n, m, c_c, MatSol);
        aggiorna_soluzione (s, &MatSol, A);
        free_stato(s); 
    }

    return MatSol;
}

/* Risolvo esattamente per ogni colonna il KP e poi per riga, considerando le scelte fatte nella prima fase */
Matrice AscDisc (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, int m, int n, Vcap* c_r, Vcap* c_c, stato** zaino_colonne)
{
    int i;
    Matrice MatSol;
    stato* s;

    MatSol = costruisci_matrice( m, n);

    /* Prima fase (Asc): risolvo il KP per colonne sfuttando lo zaino delle colonne */
    for (i = 1; i <= n; i++)
    {
            aggiorna_soluzione (zaino_colonne[i-1], &MatSol, A);
    }
    
    /* Seconda fase (Disc): risolvo il KP per righe con KSDinamico, tenendo conto delle scelte fatte nella fase Asc (conservate in MatSol) */
    for (i = 1; i <= m; i++)
    {
        s = KSD_Righe (Premi, PesiRiga, i, n, m, c_r, MatSol);
        aggiorna_soluzione (s, &MatSol, D);
        free_stato(s); 
    }

    return MatSol;
}

/* Risolvo all'ottimo tutti i problemi di zaino associati alle righe e alle colonne in modo indipendente (gia' fatto e soluzioni salvate nei rispettivi zaini)
   Per le celle considerate nell'ottimo sia da zaino_righe che da zaino_ colonne, considera quella con peso minore e cancella quella con peso maggiore dall'ottimo
   In caso di parita' si seleziona la scelta ripresa in direzione Ascendente (colonne)
   Lo spazio in memoria liberato sopra porta a considerare le celle non selezionate e valutare se possano o meno essere inserite nell'ottimo: ancora una volta 
   la scelta e' fatta in base al ratio maggiore tra premio e peso, a patto che vengano rispettati i vincoli 
   (non la stessa cella due volte e vincolo di memoria (capacita'))
   L'algoritmo termina quando non vi sono piu' soluzioni ammissibili da aggiungere all'ottimo */
Matrice Approx (Matrice* Premi, Matrice* PesiRiga, Matrice* PesiColonna, int m, int n, Vcap* c_r, Vcap* c_c, stato** zaino_righe, stato** zaino_colonne, cella* lista)
{
    int i_r, i_c, d;
    Matrice MatSol1, MatSol2;
    MatSol1 = costruisci_matrice( m, n);
    MatSol2 = costruisci_matrice( m, n);

    /* Salvo nelle due matrici soluzioni l'ottimo per riga e poi per colonna, estraendo i risultati dagli zaini */
    for (i_r = 1; i_r <= m; i_r++)
    {
        aggiorna_soluzione(zaino_righe[i_r-1], &MatSol1, D);
    }

    for (i_c = 1; i_c<= n; i_c++)
    {
        aggiorna_soluzione(zaino_colonne[i_c-1], &MatSol2, A);
    }

    /* Scorro le due matrici soluzione */
    for (i_r = 1; i_r <= m; i_r++)
    {
        for (i_c = 1; i_c <= n; i_c++)
        {
            /* Se la cella [i_r, i_c] e' nell'ottimo per colonne... */
            if (MatSol2[i_r][i_c] != NONE)
            {
                /* E anche nell'ottimo per righe... */
                if (MatSol1[i_r][i_c] != NONE)
                {
                    /* Quando il peso per riga e' maggiore a quello per colonna... */
                    if ((*PesiRiga)[i_r][i_c] > (*PesiColonna)[i_r][i_c])
                    MatSol1[i_r][i_c] =  A; /* Rimarco la matrice soluzioni per riga con la direzione delle colonne (Ascendente)*/

                    /* Altrimenti lascio la scelta fatta durante l'ottimo per righe */
                }
                /* Nel caso in cui la cella [i_r, i_c] non e' gia' nell'ottimo per righe, la inserisco nella matrice di marcatura */
                else
                {
                    MatSol1[i_r][i_c] =  A;

                }
            }

        }
    }

    distruggi_matrice (&MatSol2, m);

    /* Ricerco eventuali celle da inserire che possano essere aggiunte all'ottimo senza violare i vincoli */
    for (; lista->succ != NULL; lista = lista->succ) /* Scorro la lista ordinata di celle */
    {
        
        i_r = lista->i;
        i_c = lista->j;
        if (MatSol1[i_r][i_c] == NONE) /* Se la cella non e' stata ancora presa */
        {
            d = lista->dir;
            if (d == A)  
            {
                if (residuo(&MatSol1, PesiColonna, c_c, (*PesiColonna)[i_r][i_c], d, i_c, m)) /* Controllo che rispetti i vincoli... */
                MatSol1[i_r][i_c] = A;

            }
            else
            {
                if (residuo(&MatSol1, PesiRiga, c_r, (*PesiRiga)[i_r][i_c], d, i_r, n)) /* Controllo che rispetti i vincoli... */
                MatSol1[i_r][i_c] = D;
                
            }
        }
    }
    
    return MatSol1;
}

/* Calcola il valore dell'ottimo nell'euristica computata */
int Stima_Euristica (Matrice* Premi, Matrice* MatSol, int m, int n)
{
    int stima;
    int i, j;

    stima = 0;

    /* Scorro tutta la matrice delle soluzioni e aggiungo i premi delle celle nell'ottimo alla stima */
    for (i = 1; i <= m; i++)
    {
        for (j = 1; j <= n; j++)
        {
            if ((*MatSol)[i][j] != NONE)
                stima += (*Premi)[i][j];

        }
    }

    return stima;
}