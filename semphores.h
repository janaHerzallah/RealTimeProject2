#ifndef SEMAPHORES
#define SEMAPHORES

#include "header.h"


const char *TIMER_SEMAPHORE = "1254";
const char *Pick_key = "Picking_mutex";
const char *total_customers_key = "total_customers";
const char *total_cashiers_key = "total_cashiers";

const char *cashier_waiting_key = "cashier_waiting";

const char *queu1_key = "queue1";
const char *queu2_key = "queue2";
const char *queu3_key = "queue3";

const char *cashSem1_score_key = "cashier1_score";
const char *cashSem2_score_key = "cashier2_score";
const char *cashSem3_score_key = "cashier3_score";

const char *score_atrributes_key_sem = "score_atrributes";

const char *totalsale_key_sem = "totalsale";

const char *happiest_cashier_key_sem = "happiest_cashier";

sem_t* pick_up_items_mutex; // Semaphore for picking up items in customers header
sem_t* customers_total_mutex; // Semaphore for picking up items in customers header


/*
 * Sem: 1
 * P1: Sem --> 0 through operation, when finish release it to 1 Sem --> 1
 */
sem_t* get_semaphore(char* key){

    sem_t* sem = sem_open(key, O_CREAT, 0777, 0);
    if(sem == SEM_FAILED){
        perror("Semaphore Open Error\n");
        exit(-1);
    }

    return sem;
}

void lock_sem(sem_t* sem){

    // When return 0 for successful access
    if (sem_wait(sem) < 0){
        perror("Semaphore Wait Error\n");
        exit(-1);
    }

}

void unlock_sem(sem_t* sem){

    // When return -1 then wrong issue happened
    if (sem_post(sem) < 0){
        perror("Semaphore Release Error\n");
        exit(-1);
    }



}


void close_semaphore(sem_t* sem){
    // Close the Semaphore
    sem_close(sem);
}




#endif
