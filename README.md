# Osservazioni-da-Satellite
Algoritmi Euristici per il problema dello zaino "generalizzato"

# Il problema: #
Esiste una famiglia di satelliti che sono dedicati specificamente alla
ripresa della superficie terrestre in alta definizione. 
Per motivi tecnici, questi satelliti
percorrono un’orbita (quasi) polare. Sono dotati di una telecamera, che è puntata
fissa in direzione “nadir”, cioè verso il centro della Terra per ridurre al minimo le
perturbazioni nella ripresa. La striscia di superficie che questi satelliti riprendono
è di ampiezza costante. Siccome la Terra ruota su se stessa in verso antiorario,
tale striscia non è orientata precisamente in direzione nord-sud, ma è obliqua. In
particolare, l’inclinazione che risulta durante le semiorbite discendenti (da nord a
sud) è opposta a quella che risulta durante le semiorbite ascendenti (da sud a nord).
Di conseguenza, le strisce riprese nelle due direzioni si incrociano trasversalmente,
mentre quelle riprese nella stessa direzione tendono ad essere approssimativamente
parallele e hanno sovrapposizioni trascurabili.
Poichè non tutta la superficie ha lo stesso interesse e la memoria disponibile a
bordo per conservare le immagini `e limitata, esiste il problema di scegliere quali
porzioni di ciascuna striscia riprendere, accendendo e spegnendo alternativamente
la telecamera.
Per semplificare il problema, si possono rappresentare le semiorbite ascendenti e
discendenti, rispettivamente, come le colonne e le righe di una matrice. Ogni cella
della matrice corrisponde all’intersezione fra la striscia di superficie potenzialmente
ripresa durante la semiorbita che corrisponde alla colonna e quella che corrisponde
alla riga. Questo presuppone che una data porzione di superficie sia coperta solo
durante una semiorbita di ciascuno dei due tipi.
L’ipotesi corrisponde a limitare l’orizzonte temporale in modo che si possa sorvolare
la stessa zona due volte in direzioni opposte (ascendente e discendente),
ma mai due volte nella stessa direzione.
Ad ogni cella della matrice è associato un premio proporzionale al valore di quella
parte della regione di interesse che è contenuta in essa (eventualmente, valore nullo).
Non è necessario riprendere interamente una cella che contenga una parte della
regione di interesse. Al contrario, si può ridurre la ripresa di immagini inutili in
modo da risparmiare memoria.
Ad ogni cella è associata a due diverse quantità di memoria occupata, indicate come
peso di riga e peso di colonna.
Il satellite ha una memoria finita per conservare le immagini riprese e può scaricare 
la memoria stessa solo trasmettendo le immagini a terra durante il passaggio
sopra il polo nord o il polo sud. Siccome il passaggio ha una durata limitata e la
memoria va scaricata interamente, l’insieme di celle che si possono riprendere durante
ogni semiorbita è limitato: il suo peso totale non deve superare la capacità
associata alla semiorbita stessa.
Riassumendo, il problema riguarda tre matrici con m righe e n colonne (la
matrice {πij} dei premi, la matrice {ρij} dei pesi di riga e la matrice {σij} dei pesi
di colonna), un vettore r = [r1 . . . rm] di capacità di riga e un vettore c = [c1 . . . cn] di
capacità di colonna. Si deve determinare il sottoinsieme di celle (i, j) che corrisponde
alla massima somma di premi πij , e indicare per ogni cella del sottoinsieme se la
si riprende durante la semiorbita discendente (cioè come parte della riga i) oppure
durante la semiorbita ascendente (cioè come parte della colonna j). E' vietato `
riprendere la cella in entrambi i versi, incassando due volte il premio associato.
D’altra parte, farlo incassando il premio una volta sola è vietato perchè sarebbe
possibile, ma inutile rispetto al premio e dannoso rispetto al consumo di memoria.
Per ogni riga i = 1, . . . , m le celle riprese come parte della riga devono avere somma
dei pesi di riga ρij non superiore alla capacità ri. Per ogni colonna j = 1, . . . , n le
celle riprese come parte della colonna devono avere somma dei pesi di colonna σij
non superiore alla capacità cj .
# Il progetto:
Il programma da realizzare deve caricare da un file di testo i dati di
un’istanza del problema. I dati sono tutti numeri interi e obbediscono al seguente
formato: le parole chiave param m := e param n := introducono il numero di righe e
di colonne delle tre matrici, seguiti da un punto e virgola (;). Quindi le parole chiave
param pi := introducono la matrice dei premi delle mn celle (i, j) nel formato
[i,j] pi ij, dove i è l’indice di riga, j l’indice di colonna e pi ij il valore del premio πij
associato alla cella. La sequenza dei premi termina con un punto e virgola (;).
Con lo stesso formato vengono riportati i pesi di riga e di colonna, ma le parole
chiave introduttive sono, rispettivamente, param rho := e param sigma :=.
Le capacità di riga sono introdotte dalle parole chiave param r :=, seguite dai
pesi delle m righe nel formato i r_i, dove i è l’indice di riga e r_i la capacità
associata. La sequenza delle capacità termina con un punto e virgola (;).
Infine, le capacità di colonna seguono lo stesso formato, ma le parole chiave introduttive sono param c :=.

Il problema è fortemente NP-completo, per cui è molto improbabile che si possa
determinarne la soluzione ottima in tempo polinomiale. Realizzeremo quindi una
serie di algoritmi euristici. Questi forniranno soluzioni ammissibili (che rispettano
i vincoli), ma in generale non ottime.
Tutte le euristiche sono ispirate agli algoritmi per il problema dello zaino.
Il problema, infatti, ha forti analogie con quello dello zaino: si tratta di
scegliere oggetti indivisibili (le celle) guadagnando un premio, ma caricandosi di un
peso non superiore a una data capacità. Si hanno però molti zaini, uno per ogni
riga e uno per ogni colonna. Inoltre, anzichè essere semplicemente presi o no, gli
oggetti possono essere presi in uno solo fra due modi diversi (riga o colonna) oppure
non presi. Queste differenze complicano il problema e costringono ad adattare gli
algoritmi noti, anzichè applicarli semplicemente.
Si devono realizzare le seguenti euristiche:
# 1. Greedy:
per ogni cella {i, j} si valutano i rapporti fra premio e peso in
ciascuna delle due direzioni (cio`e πij/ρij e πij/σij ); iterativamente, si scelgono
la cella e la direzione più promettenti, cioè con rapporto massimo fra quelle che
non violano nè il vincolo di capacità nè quello di evitare la doppia ripresa, e si
fissa la decisione; quando tutte le decisioni possibili sono parte della soluzione
o sono inammissibili, l’algoritmo termina.
Nota: in caso di parità fra due rapporti, si consideri maggiore quello con
indice di riga i maggiore; se la parità persiste, si consideri maggiore quello
con indice di colonna j maggiore; se la parità persiste ancora (cioè i due
rapporti si riferiscono alla stessa cella), si consideri maggiore quello associato
alla direzione discendente (riga).
# 2. DiscAsc:
la prima fase (Disc) risolve esattamente per ogni riga i il problema
dello zaino ottenuto considerando i premi delle celle, i pesi di riga e la capacità ri;
la seconda fase (Asc) risolve esattamente per ogni colonna j il problema
dello zaino ottenuto considerando i premi, i pesi di colonna e la capacità cj,
tenendo però conto delle scelte fatte nella prima, cioè considerando solo le
celle ancora disponibili, selezionate nella prima fase.
# 3. AscDisc: 
questo algoritmo procede come il precedente, ma la prima fase
risolve i problemi di zaino associati alle colonne, e la seconda quelli associati
alle righe, sempre tenendo conto delle scelte fatte nella prima fase.
# 4. Approx: 
questo algoritmo risolve all’ottimo tutti i problemi di zaino delle righe
e delle colonne della matrice, indipendentemente fra loro; poi elenca le
celle riprese in entrambe le direzioni, cancella la scelta associata al peso maggiore 
(in caso di parità, quella associata alla direzione ascendente, cioè alla
colonna); questo libera dello spazio in memoria, che va occupato elencando
per tutte le celle ancora disponibili i rapporti πij/ρij e πij/σij e scegliendo
iterativamente la cella e la direzione con rapporto massimo, purchè si rispettino
tutti i vincoli del problema; l’algoritmo termina quando tutte le decisioni
possibili sono state incluse nella soluzione o sono inammissibili.
Nota: poichè l’algoritmo risolve tutti i problemi di zaino all’ottimo, la sua
prima fase ottiene un valore certamente non inferiore all’ottimo del problema;
probabilmente, il valore è più alto, dato che alcune celle verranno selezionate
due volte; la seconda fase dell’algoritmo genera invece una soluzione ammissibile,
dunque di valore non superiore all’ottimo. Ma questo valore è almeno
pari a metà del precedente, dato che dimezza i premi delle sole celle conflittuali 
(nel peggiore dei casi, tutte). Siccome il valore della soluzione finale è
almeno pari a metà di un valore non inferiore all’ottimo, sarà pari almeno a
metà dell’ottimo (e in generale molto meglio). Tecnicamente, si dice che è
2-approssimata, proprietà di cui le altre euristiche non godono.
Occorre quindi determinare le seguenti due semplici stime per eccesso del valore
dell’ottimo:
# 1. Totale: 
la somma dei premi di tutte le celle della matrice è certamente non
inferiore all’ottimo.
# 2. Zaino:
la somma dei valori ottimi dei problemi di zaino associati a tutte le
righe e a tutte le colonne è certamente non inferiore all’ottimo.
Terminati l’esecuzione delle quattro euristiche e il calcolo delle due stime per
eccesso, si devono stampare a video sei righe contenenti i risultati. In ciascuna
delle prime quattro righe, si riporta il nome di un’euristica, seguito dal valore della
soluzione ottenuta e dall’elenco delle celle e delle direzioni scelte dall’euristica stessa,
nel formato (i,j) dir, dove i e j sono gli indici di riga e colonna della cella e 
dir è A per le celle riprese in direzione ascendente (colonna) e D per quelle riprese 
in direzione discendente (riga).
Le ultime due righe riportano invece il nome della stima (Totale e Zaino) seguito
dal valore.
