#ifndef CASHIER_HEADER
#define CASHIER_HEADER
#define MAX_LINE_LENGTH 100
#include "header.h"
#include "customer_header.h"
#include "semphores.h"



int get_total_cashiers( int num_of_cashier_queue) {


    int shmid = shmget(total_num_cashiers_key, sizeof(total_cashiers), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    total_cashiers *shared_data = (total_cashiers *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // mutex code
    // Acquire the semaphore before modifying shared memory
    sem_t *cashier_mutex = get_semaphore(total_cashiers_key);

    lock_sem(cashier_mutex);

    // Increment the totalCustomers value

    int total_cash ;

    if(num_of_cashier_queue == 0){
        total_cash = shared_data->totalCashiers;
    }
    else if(num_of_cashier_queue == 1){
        total_cash = shared_data->scanning_time1;
    }else if(num_of_cashier_queue == 2){
        total_cash = shared_data->scanning_time2 ;
    }else if(num_of_cashier_queue == 3) {
        total_cash = shared_data->scanning_time3 ;
    }


    // Release the semaphore
    unlock_sem(cashier_mutex);
    close_semaphore(cashier_mutex);
    //end mutex code

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return total_cash;
}


void increment_total_cashiers(int num, int num_of_cashier_queue ) {
    // Get the shared memory ID
    int shmid = shmget(total_num_cashiers_key, sizeof(total_cashiers), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory
    total_cashiers *shared_data = (total_cashiers *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Acquire the semaphore before modifying shared memory
    sem_t *cashier_mutex = get_semaphore(total_cashiers_key);


    lock_sem(cashier_mutex);


    if(num_of_cashier_queue == 0){
        shared_data->totalCashiers = shared_data->totalCashiers + num;
    }
     else if(num_of_cashier_queue == 1){
        shared_data->scanning_time1 = num;
    }else if(num_of_cashier_queue == 2){
        shared_data->scanning_time2 = num;
     }else if(num_of_cashier_queue == 3) {
        shared_data->scanning_time3 = num;
    }





    // Release the semaphore
    unlock_sem(cashier_mutex);

    // Close the semaphore
    close_semaphore(cashier_mutex);

    // Detach from shared memory
    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}













#endif
