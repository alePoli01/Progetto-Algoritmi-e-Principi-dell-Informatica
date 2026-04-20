# Highway Route Planner 🛣️🚗

Un sistema in C per la gestione dinamica di stazioni di servizio autostradali e per la pianificazione di percorsi ottimali. 

## 📝 Descrizione
Questo progetto simula un'infrastruttura autostradale in cui le **stazioni di servizio** sono posizionate a distanze specifiche. Ogni stazione possiede un parco auto, e ogni auto è caratterizzata dalla sua **autonomia** massima. 
Il cuore del sistema è l'algoritmo di **pathfinding**, che calcola il percorso per andare da una stazione `A` a una stazione `B` effettuando il **minor numero possibile di fermate**, valutando l'autonomia dei veicoli disponibili in ogni tappa.

## 🚀 Funzionalità e Comandi Supportati
Il programma legge da `stdin` e supporta i seguenti comandi testuali:

* `aggiungi-stazione <distanza> <numero_auto> <autonomia_1> ... <autonomia_n>`
  Aggiunge una nuova stazione alla distanza specificata, popolandola con il numero di veicoli indicato. 
  *(Output: `aggiunta` o `non aggiunta`)*

* `demolisci-stazione <distanza>`
  Rimuove la stazione alla distanza specificata e tutte le auto al suo interno.
  *(Output: `demolita` o `non demolita`)*

* `aggiungi-auto <distanza_stazione> <autonomia_auto>`
  Aggiunge un'auto con una determinata autonomia in una stazione esistente.
  *(Output: `aggiunta` o `non aggiunta`)*

* `rottama-auto <distanza_stazione> <autonomia_auto>`
  Rimuove un'auto specifica da una stazione esistente.
  *(Output: `rottamata` o `non rottamata`)*

* `pianifica-percorso <partenza> <arrivo>`
  Calcola e stampa il percorso con il minor numero di tappe tra la stazione di partenza e quella di arrivo (funziona in entrambe le direzioni di marcia). Se a parità di tappe esistono più percorsi, predilige quello con le stazioni più vicine all'inizio dell'autostrada.
  *(Output: le distanze delle stazioni toccate separate da spazio, o `nessun percorso`)*

## 🧠 Strutture Dati e Ottimizzazioni
Per garantire alte prestazioni anche con un elevato numero di comandi e stazioni (fino a 55.000):
* **Hash Table ($O(1)$ lookup):** Utilizzata per la ricerca rapida delle stazioni tramite la loro distanza (chiave). Dimensione fissa a 65.536 slot con gestione delle collisioni a lista concatenata.
* **Liste Doppiamente Concatenate:** Mantengono l'ordine relativo tra le stazioni per i calcoli di percorso.
* **MergeSort:** Algoritmo di ordinamento implementato (`O(n log n)`) per riordinare dinamicamente la lista delle stazioni prima di eseguire un calcolo di rotta, assicurando che la sequenza fisica sia rispettata.
* **Greedy Pathfinding:** Algoritmo custom (`findPathAB` e `findPathBA`) ottimizzato per "saltare" le stazioni in base all'autonomia massima disponibile.

## 🛠️ Compilazione e Avvio
Il codice non richiede librerie esterne. Per compilare ed eseguire usando GCC:

```bash
# Compilazione
gcc -O2 -Wall main.c -o highway_planner

# Esecuzione
./highway_planner
