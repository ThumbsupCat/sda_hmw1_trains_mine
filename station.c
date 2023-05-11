#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"


/* Creeaza o gara cu un numar fix de peroane.
 * 
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation* open_train_station(int platforms_no) {
    TrainStation *newStation = (TrainStation*) malloc(sizeof(TrainStation));
    newStation->platforms_no = platforms_no;
    newStation->platforms = (Train**) malloc(sizeof(Train) * platforms_no);
    for (int i = 0 ; i < platforms_no; i++) {
        newStation->platforms[i] = malloc(sizeof(Train));
        newStation->platforms[i]->locomotive_power = -1;
        newStation->platforms[i]->train_cars = NULL;
    }
    return newStation;
    return NULL;
}


/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station) {
    for (int i = 0; i < station->platforms_no ; i++) {
        TrainCar* goThrough = station->platforms[i]->train_cars;
        TrainCar* deleteIt;
        while (goThrough != NULL) {
            deleteIt = goThrough;
            goThrough = goThrough->next;
            free(deleteIt);
            deleteIt = NULL;
        }
    }
    for (int i = station->platforms_no - 1; i > -1; i--)
        free(station->platforms[i]);
    free(station->platforms);
    free(station);
}


/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f) {
    if (station == NULL) {
        return;
    } else {
        for (int i = 0; i < station->platforms_no; i++) {
            fprintf(f, "%d: ", i);
            if (station->platforms[i]->locomotive_power != -1) {
                TrainCar  *goThrough = station->platforms[i]->train_cars;
                fprintf(f, "(%d)", station->platforms[i]->locomotive_power);
                while (goThrough != NULL) {
                    fprintf(f, "-|%d|", goThrough->weight);
                    goThrough = goThrough->next;
                }
            }
                fprintf(f,"\n");
        }
    }
}


/* Adauga o locomotiva pe un peron.
 * 
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power) {
    if (station == NULL) 
       return;
    else if (station->platforms_no > platform && platform > -1 && 
                station->platforms[platform]->locomotive_power == -1) {
        station->platforms[platform]->locomotive_power = locomotive_power;      
    }
}


/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform) {
    if (platform < station->platforms_no && platform > -1) {
        station->platforms[platform]->locomotive_power = -1;
        TrainCar* goThrough = station->platforms[platform]->train_cars;
        while (station->platforms[platform]->train_cars != NULL) {
            TrainCar* freePlatform = station->platforms[platform]->train_cars;
            station->platforms[platform]->train_cars = station->platforms[platform]->train_cars->next;
            free(freePlatform);
            freePlatform = NULL;
        }
    }
}


/* Adauga un vagon la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight) {
    if (platform < station->platforms_no && platform > -1) {
        TrainCar *newTrainCar = (TrainCar*) malloc (sizeof(TrainCar));
        newTrainCar->weight = weight;
        newTrainCar->next = NULL;
        if (station->platforms[platform]->train_cars == NULL) {
            station->platforms[platform]->train_cars = newTrainCar;
        } else {
            TrainCar* goThrough = station->platforms[platform]->train_cars;
            while (goThrough->next != NULL) {
                goThrough = goThrough->next;
            }
            goThrough->next = newTrainCar;
        }
    }
    else
        return;
}


/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight) {
    TrainCar *deleteIt, *prev = NULL;
    if (platform < station->platforms_no && platform > -1) {
        TrainCar *goThrough = station->platforms[platform]->train_cars;
        while (goThrough != NULL) {
        if (goThrough->weight == weight) {
            deleteIt = goThrough;
            if (prev == NULL) {
                goThrough = goThrough->next;
                station->platforms[platform]->train_cars = goThrough;
                free(deleteIt);
                deleteIt = NULL;
            } else {
                prev->next = deleteIt->next;
                goThrough = goThrough->next;
                free(deleteIt);
                deleteIt = NULL;
            }
        } else {
            prev = goThrough;
            goThrough = goThrough->next;
        }
    }
}
}
/* Muta o secventa de vagoane dintr-un tren in altul.
 * 
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a, 
                                int cars_no, int platform_b, int pos_b) {
    
    if (cars_no < 1 || station == NULL || platform_a >= station->platforms_no ||
                                            platform_b >= station->platforms_no ||
                                                            pos_a <= 0 || pos_b <= 0) {
        return;
    } else {
        if (station->platforms[platform_a]->locomotive_power != -1 &&
                station->platforms[platform_b]->locomotive_power != -1) {

            int i = 1;
            TrainCar* start_car1 = station->platforms[platform_a]->train_cars; // primul vagon din sir
            TrainCar* before_start1; // elementul de dinaintea lui start_car1
            while (start_car1 != NULL && i < pos_a) {  //caut pos_a
                i++;
                before_start1 = start_car1;
                start_car1 = start_car1->next;
            }
            if (start_car1 != NULL) {
                int count_cars = 1;
                TrainCar* goThrough = start_car1;  // capatul secventei de vagoane de mutat
                while (goThrough != NULL && count_cars < cars_no) {
                    count_cars++;
                    goThrough = goThrough->next;
                }
                if (goThrough == NULL) {
                    return;
                }
                TrainCar* start_car2 = station->platforms[platform_b]->train_cars;
                TrainCar* before_start2;  // elementul de dinaintea lui start_car2
                int j = 1;
                while (start_car2 != NULL && j < pos_b) { //caut pos_b 
                        j++;
                        before_start2 = start_car2;
                        start_car2 = start_car2->next;
                }
                if (j < pos_b) {
                    return;
                }
                if (pos_a == 1) {
                    station->platforms[platform_a]->train_cars = goThrough->next; // schimba head cu capat->next
                } else {
                    before_start1->next = goThrough->next; // leaga nodul de dinaintea sirului cu cele de dupa sir
                }
                if (pos_b == 1) {
                    station->platforms[platform_b]->train_cars = start_car1; // noul head devine inceputul sirului
                    goThrough->next = start_car2;  // capatul sirului e legat cu head-ul initial
                } else {
                    before_start2->next = start_car1;  // sir pus inaintea lui start_car2
                    goThrough->next = start_car2;  // face legatura cu capatul sirului de noduri si start_car2
                }
            } else {
                return;
            }
        }
    }
}   

/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station) {
    int sum, show_index = 0, maximal = - 1;
    for (int i = 0; i < station->platforms_no; i++) {
        sum = 0;
        if (station->platforms[i]->locomotive_power != -1) {
            TrainCar* goThrough = station->platforms[i]->train_cars;
            while (goThrough != NULL) {
                sum += goThrough->weight;
                goThrough = goThrough->next;
            }
            if (maximal <= (station->platforms[i]->locomotive_power - sum) ) {
                maximal = station->platforms[i]->locomotive_power - sum;
                show_index = i;
            }
        }
    }
    if (maximal != -1) {
        return show_index;
    }
    return -1;
}


/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station) {
    if (station == NULL) {
        return -1;
    }
    for (int i = 0; i < station->platforms_no; i++) {
        if (station->platforms[i]->locomotive_power != -1) {
            int locomotive_speed = station->platforms[i]->locomotive_power;
            TrainCar* goThrough = station->platforms[i]->train_cars;
            while (goThrough != NULL) {
                locomotive_speed -= goThrough->weight;
                goThrough = goThrough->next;
            }
            if (locomotive_speed < 0) { 
                return i;  //solutie unica
            }
        }
    }
    return -1;
}


/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station) {
    int maximal = 10000, show_index = -1;
    for (int i = 0; i < station->platforms_no; i++) {
        if (station->platforms[i]->locomotive_power != -1) {
            int sum = 0;
            TrainCar* goThrough = station->platforms[i]->train_cars;
            while (goThrough != NULL) {
                sum += goThrough->weight;
                goThrough = goThrough->next;
            }
            if (maximal >= station->platforms[i]->locomotive_power - sum &&
                                station->platforms[i]->locomotive_power - sum > 0) {        
                show_index = i;
                maximal = station->platforms[i]->locomotive_power - sum;
            }
        }
    }
    if (show_index != -1) {
        return show_index;
    } else {
        return -1;
    }
}


/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car) {
    int heavy = 0, show_index = -1;
    *(start_car) = NULL;
    for (int i = 0; i < station->platforms_no; i++) {
        if (station->platforms[i]->locomotive_power != -1) {
            int ok = 0;
            TrainCar* goThrough = station->platforms[i]->train_cars;
            while (ok != 1 && goThrough != NULL) {
                int count_cars = 0, sum = 0;
                TrainCar* start_car_count = goThrough;
                while (count_cars < cars_no && start_car_count != NULL) {
                    count_cars++;
                    sum += start_car_count->weight;
                    start_car_count = start_car_count->next;
                }
                if (count_cars == cars_no) {
                    if (heavy <= sum) {
                        show_index = i;
                        (*start_car) = goThrough;
                        heavy = sum;
                    }
                } else {
                    ok = 1;
                }
                goThrough = goThrough->next;
            }
        }
    }
    if ((*start_car) == NULL && heavy == 0) {  //pointer initial null, heavy neschimbat
        return -1;
    } else {
        return show_index;
    }
}


/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */


void order_train(TrainStation *station, int platform) {
    int i, j, ok , n = 0;
    TrainCar* goThrough = station->platforms[platform]->train_cars;
    while (goThrough != NULL) {
        n++;
        goThrough = goThrough->next;
    }
    //Bubble Sort
    for (i = 0; i < n; i++) {
        TrainCar **first = &(station->platforms[platform]->train_cars);
        ok = 0;
        for (j = 0; j < n - i - 1; j++) {
            TrainCar* cars = *first;
            TrainCar* cars_next = cars->next;
            if (cars->weight < cars_next->weight) {
                    TrainCar *aux = cars_next->next;  //swap la noduri
                    cars_next->next = cars;
                    cars->next = aux;
                    *first = cars_next;
                    ok = 1;
            }
            first = &(*first)->next;
        }
        if (ok == 0) // sortat complet
            break;
    }
    
}


/* Scoate un vagon din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station) {
    if (station == NULL) return;
    int i = find_overload_train(station);
    if (i == -1) {
        return;
    }
    int sum = 0;
    if (station->platforms[i]->train_cars == NULL) return;
    TrainCar* goThrough = station->platforms[i]->train_cars;
    while (goThrough) {
        sum += goThrough->weight;
        goThrough = goThrough->next;
    }
    goThrough = station->platforms[i]->train_cars;
    TrainCar* saveIt;
    int optim = 10000, ok = 1;
    int power = station->platforms[i]->locomotive_power;
    while (goThrough) {
        int abs = sum - (goThrough->weight);
        if (power - abs  >= 0 && power - abs < optim && optim >= 0) {
            optim = power - abs;
            saveIt = goThrough;
            ok = 0;
        }
        goThrough = goThrough->next;
    }
    if (ok == 0) {
        goThrough = station->platforms[i]->train_cars;
        if (goThrough->weight == saveIt->weight) {
            station->platforms[i]->train_cars = station->platforms[i]->train_cars->next;
        } else {
            while (goThrough->next != saveIt) {
                goThrough = goThrough->next;
            }
            goThrough->next = goThrough->next->next;
        }
        free(saveIt);
        saveIt = NULL;
    }
    return ;
}
