#ifndef CASHIER_HEADER
#define CASHIER_HEADER

#define MAX_QUEUE_CAPACITY 10
#define NUM_CASHIERS 3
#define MAX_ITEMS_IN_CART 10
#define MAX_LINE_LENGTH 100
#include "constants.h"
#include "header.h"
#include "customer_header"
#include "semphores.h"
#include "shared_memories.h"

int get_total_cashiers();

typedef struct {
    int id;                     // Cashier ID
    int numPeopleInQueue;       // Number of people in the cashier's queue
    struct Customer queue[MAX_QUEUE_SIZE]; // Queue of customers
    int happiness;              // Happiness level of the cashier
    int scanningTime;           // Time taken by the cashier to scan each item
} Cashier;

// Function to generate random scanning time based on config
int generate_scanning_time(int cashier_id) {
    srand((unsigned int)time(NULL) + cashier_id);
    return rand() % c.scanning_time + 1; // Random number between 1 and scanning_time
}


int get_total_cashiers() {


    int shmid = shmget(CUS_NUM_KEY, sizeof(SharedData), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // mutex code
    // Acquire the semaphore before modifying shared memory
    sem_t *cashier_mutex = get_semaphore(total_cashiers_key);

    lock_sem(cashier_mutex);

    // Increment the totalCustomers value
    int total_cash = shared_data->totalCashiers;

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

void increment_total_cashiers() {
    // Get the shared memory ID
    int shmid = shmget(CUS_NUM_KEY, sizeof(SharedData), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory
    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Acquire the semaphore before modifying shared memory
    sem_t *cashier_mutex = get_semaphore(total_cashiers_key);

    lock_sem(cashier_mutex);

    // Increment the totalCustomers value
    shared_data->totalCashiers++;

    // Release the semaphore
    unlock_sem(cashier_mutex);
    close_semaphore(cashier_mutex);

    // Detach from shared memory
    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}




#endif
