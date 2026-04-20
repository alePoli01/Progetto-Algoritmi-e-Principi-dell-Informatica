#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 6000
#define STATIONS 55000
#define HASH_TABLE_SIZE 65536

typedef struct Car {
    int autonomy;
    struct Car* next;
} Car;

typedef struct Station {
    int number;
    Car* carList;
    struct Station* next;
    struct Station* prev;
} Station;

typedef struct HashNode {
    int stationNumber;
    Station* station;
    struct HashNode* next;
} HashNode;

Car* createCar(int autonomy) {
    Car* newCar = malloc(sizeof(Car));
    newCar->autonomy = autonomy;
    newCar->next = NULL;
    return newCar;
}

int insertCar(Station* root, int autonomy) {
    Car* newCar = createCar(autonomy);
    if (newCar == NULL) {
        return 0;
    }

    Car* current = root->carList;
    Car* prev = NULL;

    while (current != NULL && current->autonomy > autonomy) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        newCar->next = root->carList;
        root->carList = newCar;
    } else {
        prev->next = newCar;
        newCar->next = current;
    }

    return 1;
}

HashNode* hashTable[HASH_TABLE_SIZE] = { NULL };

void removeFromHashTable(int stationNumber) {
    int index = stationNumber % HASH_TABLE_SIZE;
    HashNode* current = hashTable[index];
    HashNode* prev = NULL;

    while (current != NULL) {
        if (current->stationNumber == stationNumber) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void insertIntoHashTable(int stationNumber, Station* station) {
    int index = stationNumber % HASH_TABLE_SIZE;
    HashNode* newNode = malloc(sizeof(HashNode));
    if (newNode == NULL) {
        exit(1);
    }
    newNode->stationNumber = stationNumber;
    newNode->station = station;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

Station* searchStationInHashTable(int stationNumber) {
    int index = stationNumber % HASH_TABLE_SIZE;
    HashNode* current = hashTable[index];
    while (current != NULL) {
        if (current->stationNumber == stationNumber) {
            return current->station;
        }
        current = current->next;
    }
    return NULL;
}

void updateHashTable(int stationNumber, Station* station) {
    removeFromHashTable(stationNumber);
    insertIntoHashTable(stationNumber, station);
}

Station* createStation(int number) {
    Station* newStation = malloc(sizeof(Station));
    newStation->number = number;
    newStation->carList = NULL;
    newStation->next = NULL;
    return newStation;
}

Station* insertStation(Station* root, int number) {
    Station* existingStation = searchStationInHashTable (number);
    if (existingStation != NULL) {
        return root;
    }

    Station* newStation = createStation (number);

    if (newStation == NULL) {
        return root;
    }

    newStation->next = root;
    if (root != NULL) {
        root->prev = newStation;
    }

    insertIntoHashTable(number, newStation);

    return newStation;
}

int demolishStation(Station** root, int stationNumber) {
    Station* targetStation = searchStationInHashTable (stationNumber);
    if (targetStation != NULL) {
        removeFromHashTable(stationNumber);

        if (*root == targetStation) {
            *root = targetStation->next;
            if (targetStation->next != NULL) {
                targetStation->next->prev = NULL;
            }
        } else {
            targetStation->prev->next = targetStation->next;
            if (targetStation->next != NULL) {
                targetStation->next->prev = targetStation->prev;
            }
        }

        free(targetStation);
        return 1;
    }
    return 0;
}

void addStation(Station** root, int stationNumber, int carsNumber, int* carAutonomyArray) {
    Station* targetStation = searchStationInHashTable (stationNumber);
    if (targetStation != NULL) {
        free(carAutonomyArray);
        printf("non aggiunta\n");
        return;
    }

    *root = insertStation(*root, stationNumber);
    targetStation = searchStationInHashTable (stationNumber);

    for (int i = 0; i < carsNumber; i++) {
        if (carAutonomyArray[i] != -1) {
            insertCar(targetStation, carAutonomyArray[i]);
        }
    }

    printf("aggiunta\n");
    free(carAutonomyArray);
    return;
}

int scrapCar(Station* root, int autonomy) {
    Car* current = root->carList;
    Car* prev = NULL;

    while (current != NULL) {
        if (current->autonomy == autonomy) {
            if (prev == NULL) {
                root->carList = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return 1;
        }

        prev = current;
        current = current->next;
    }

    return 0;
}

Station* removeStation(Station* root, int number) {
    if (root == NULL) {
        return NULL;
    }

    if (root->number == number) {
        Station* newRoot = root->next;
        if (newRoot != NULL) {
            newRoot->prev = NULL;
        }
        free(root);
        return newRoot;
    }

    Station* current = root;
    while (current->next != NULL && current->next->number != number) {
        current = current->next;
    }

    if (current->next != NULL) {
        Station* toRemove = current->next;
        current->next = toRemove->next;
        if (toRemove->next != NULL) {
            toRemove->next->prev = current;
        }
        free(toRemove);
    }

    return root;
}

void scrapCarCommand(Station* root, int stationNumber, int carAutonomy) {
    Station* targetStation = searchStationInHashTable (stationNumber);
    if (targetStation != NULL) {
        int success = scrapCar(targetStation, carAutonomy);
        if (success) {
            printf("rottamata\n");
        } else {
            printf("non rottamata\n");
        }
    } else {
        printf ("non rottamata\n");
    }
    return;
}

void addCar(Station* root, int stationNumber, int carAutonomy) {
    Station* targetStation = searchStationInHashTable (stationNumber);
    if (targetStation != NULL) {
        int success = insertCar(targetStation, carAutonomy);
        if (success) {
            printf("aggiunta\n");
        } else {
            printf("non aggiunta\n");
        }
        return;
    } else {
        printf ("non aggiunta\n");
    }
    return;
}

void findPathAB(Station* startNode, int arrivalStation) {
    int i, j, z, flag;
    int counter, next;
    int tempPathLength = 0;
    int tempPath[STATIONS];
    Station* station;
    
    station = startNode;
    tempPath[tempPathLength] = station->number;

    while ((station->number + station->carList->autonomy) < arrivalStation) {
        flag = 0;
        if ((station->number + station->carList->autonomy) < station->next->number) {
            next = station->next->number; //Station to get
            while (!flag) {
                tempPathLength--;
                if (tempPathLength < 0) {
                    printf ("nessun percorso\n");
                    return;
                }
                station =  searchStationInHashTable (tempPath[tempPathLength]);
                if ((station->number + station->carList->autonomy) >= next) {
                    flag = 1;
                }
            }
            tempPathLength++;
            tempPath[tempPathLength] = next;
            station = searchStationInHashTable (next);
        } else {
            tempPathLength++;
            station = station->next;
            tempPath[tempPathLength] = station->number;
        }
        if (station->carList == NULL) {
            next = station->next->number; //Station to get
            while (!flag) {
                tempPathLength--;
                if (tempPathLength < 0) {
                    printf ("nessun percorso\n");
                    return;
                }
                station = searchStationInHashTable (tempPath[tempPathLength]);
                if ((station->number + station->carList->autonomy) >= next) {
                    flag = 1;
                }
            }
            tempPathLength++;
            tempPath[tempPathLength] = next;
            station = searchStationInHashTable (next);
        }
    }
    
    tempPathLength++;
    tempPath[tempPathLength] = arrivalStation;
    
    if (tempPathLength > 2) {        
        // Set as last station the second to last
        arrivalStation = station->number;

        do {
            counter = 1;
            flag = 0;
            station = startNode;
            z = 0;
            // If we have enough autonomy to reach the arrival station, check for a shorter path
            do {
                if (station->number + station->carList->autonomy >= arrivalStation) {
                    flag = 1;
                    for (i = 0; tempPath[i] != station->number; i++);
                    for (j = 0; tempPath[j] != arrivalStation; j++);
                    while (tempPath[i+1+z] < tempPath[j+z] && ((i+1+z) < tempPathLength)) {
                        tempPath[i+1+z] = tempPath[j+z];
                        z++;
                    }
                    while (j-i > 1) {
                        tempPathLength--;
                        j--;
                    }
                } else {
                    if (!flag) {
                        station = searchStationInHashTable (tempPath[counter]);
                        counter++;
                    }
                }
            } while (!flag && tempPath[counter] != arrivalStation);

            arrivalStation = tempPath[counter-1];
        } while (tempPathLength > 2 && arrivalStation != tempPath[0]);
    }

    for (int i = 0; i < tempPathLength; i++) {
        printf("%d ", tempPath[i]);
    }
    printf ("%d", tempPath[tempPathLength]);
    printf("\n");
}

void findPathBA(Station* startNode, int arrivalStation) {
    int i, j, z;
    int counter, flag, next, fixes;
    int tempPathLength = 0;
    int tempPath[STATIONS];
    Station* station;
    Station* prec;
    
    station = startNode;
    tempPath[tempPathLength] = station->number;


    while ((station->number - station->carList->autonomy) > arrivalStation) {
        flag = 0;
        if ((station->number - station->carList->autonomy) > station->prev->number) {
            next = station->prev->number;
            while (!flag) {
                tempPathLength--;
                if (tempPathLength < 0) {
                    printf ("nessun percorso\n");
                    return;
                }
                station = searchStationInHashTable (tempPath[tempPathLength]);
                if ((station->number - station->carList->autonomy) <= next) {
                    flag = 1;
                }
            }
            tempPathLength++;
            tempPath[tempPathLength] = next;
            station = searchStationInHashTable (next);
        } else {
            tempPathLength++;
            station = station->prev;
            tempPath[tempPathLength] = station->number;
        }
        if (station->carList == NULL) {
            next = station->prev->number;
            while (!flag) {
                tempPathLength--;
                if (tempPathLength < 0) {
                    printf ("nessun percorso\n");;
                    return;
                }   
                station = searchStationInHashTable (tempPath[tempPathLength]);
                if ((station->number - station->carList->autonomy) <= next) {
                    flag = 1;
                }
            }
            tempPathLength++;
            tempPath[tempPathLength] = next;
            station = searchStationInHashTable (next);
        }
    }
    
    tempPathLength++;
    tempPath[tempPathLength] = arrivalStation;


    if (tempPathLength > 2) {        

        arrivalStation = station->number;

        do {
            counter = 1;
            flag = 0;
            station = startNode;
            z = 0;
            

            do {
                if (station->number - station->carList->autonomy <= arrivalStation) {           
                    flag = 1;
                    for (i = 0; tempPath[i] != station->number; i++);
                    for (j = 0; tempPath[j] != arrivalStation; j++);
                    while (tempPath[i+1+z] > tempPath[j+z] && ((i+1+z) < tempPathLength)) {
                        tempPath[i+1+z] = tempPath[j+z];
                        z++;
                    }
                    while (j-i > 1) {
                        tempPathLength--;
                        j--;
                    }
                } else {
                    if (!flag) {
                        station = searchStationInHashTable (tempPath[counter]);
                        counter++;
                    }
                }
            } while (!flag && tempPath[counter] != arrivalStation);
            
            arrivalStation = tempPath[counter-1];
        } while (tempPathLength > 2 && arrivalStation != tempPath[0]);
    }
    
    for (i = tempPathLength; i >= 2; i--) {
        fixes = 0;
        flag = 0;
        prec = searchStationInHashTable (tempPath[i-1]);
        next = prec->number;
        prec = prec->prev;
        station = searchStationInHashTable (tempPath[i-2]);
        
        while (prec->number != tempPath [i] && !flag) {
            if ((station->number - station->carList->autonomy) <= prec->number) {
                if ((prec->number - prec->carList->autonomy) <= tempPath[i]) {
                    next = prec->number;
                    fixes = 1;
                    flag = 1;    
                }
                prec = prec->prev;     
            } else {
                flag = 1;
            }
        }
        tempPath[i-1] = next;
        if (fixes) {
            i = tempPathLength+1;
        }
    }

    for (i = 0; i < tempPathLength; i++) {
        printf("%d ", tempPath[i]);
    }
    printf ("%d", tempPath[tempPathLength]);
    printf ("\n");
}

void findRoad(Station* root, int startStation, int arrivalStation) {

    Station* startNode = searchStationInHashTable (startStation);
    Station* arrivalNode = searchStationInHashTable (arrivalStation);

    if (startNode == NULL || arrivalNode == NULL || startNode->carList == NULL || startNode->carList->autonomy == 0) {
        printf("nessun percorso\n");
        return;
    }

    if (startNode == arrivalNode) {
        printf("%d\n", startStation);
        return;
    }


    if (startStation < arrivalStation) {
        findPathAB(startNode, arrivalStation);
    } else {
        findPathBA(startNode, arrivalStation);
    }
}

Station* merge(Station* left, Station* right) {
    Station* result = NULL;

    if (left == NULL) {
        return right;
    } else if (right == NULL) {
        return left;
    }

    if (left->number <= right->number) {
        result = left;
        result->next = merge(left->next, right);
        result->next->prev = result;
    } else {
        result = right;
        result->next = merge(left, right->next);
        result->next->prev = result;
    }

    return result;
}

Station* mergesort(Station* root) {
    if (root == NULL || root->next == NULL) {
        return root;
    }

    Station* middle = root;
    Station* fast = root->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            middle = middle->next;
            fast = fast->next;
        }
    }

    Station* left = root;
    Station* right = middle->next;
    middle->next = NULL;

    left = mergesort(left);
    right = mergesort(right);

    return merge(left, right);
}

void reorderStations(Station** root) {
    if (*root == NULL || (*root)->next == NULL) {
        return;
    }

    *root = mergesort(*root);
}

int main() {
    Station* root = NULL;
    char input[MAX_INPUT_LENGTH];

    while (1) {
        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {  
            return 0;
        }

        if (strcmp(input, "\n") == 0) {
            printf ("\n");
            return 0;
        }

        char* token = strtok(input, " ");
        char* command = token;

        if (strcmp(command, "aggiungi-stazione") == 0) {

            int stationNumber = atoi(strtok(NULL, " "));
            int carsNumber = atoi(strtok(NULL, " "));

            int* carAutonomyArray = malloc(carsNumber * sizeof(int));
            if (carAutonomyArray == NULL) {
                printf("Errore di allocazione di memoria!\n");
                return 1;
            }

            for (int i = 0; i < carsNumber; i++) {
                token = strtok(NULL, " ");
                if (token == NULL) {
                    printf("non aggiunta\n");
                    free(carAutonomyArray);
                    return 1;
                }
                carAutonomyArray[i] = atoi(token);
            }
            addStation(&root, stationNumber, carsNumber, carAutonomyArray);

        } else if (strcmp(command, "demolisci-stazione") == 0) {

            int stationNumber = atoi(strtok(NULL, " "));
            int result = demolishStation(&root, stationNumber);
            if (result == 1) {
                printf("demolita\n");
            } else {
                printf("non demolita\n");
            }

        } else if (strcmp(command, "rottama-auto") == 0) {

            int stationNumber = atoi(strtok(NULL, " "));
            int carAutonomy = atoi(strtok(NULL, " "));
            scrapCarCommand(root, stationNumber, carAutonomy);

        } else if (strcmp(command, "aggiungi-auto") == 0) {

            int stationNumber = atoi(strtok(NULL, " "));
            int carAutonomy = atoi(strtok(NULL, " "));
            addCar(root, stationNumber, carAutonomy);

        } else if (strcmp(command, "pianifica-percorso") == 0) {

            int startStation = atoi(strtok(NULL, " "));
            int arrivalStation = atoi(strtok(NULL, " "));
            reorderStations (&root);
            findRoad(root, startStation, arrivalStation);
        }
    }
}