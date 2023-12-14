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
    int scanningTime;
    int number_of_all_items ;// Time taken by the cashier to scan each item
    float total_sales;
} Cashier;

// Function to generate random scanning time based on config
int generate_scanning_time(int cashier_id) {



    srand((unsigned int)time(NULL) + 7*cashier_id);
    return rand() % c.scanning_time + 1; // Random number between 1 and scanning_time
}


int get_total_cashiers() {


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

    // Increment the totalCashiers value
    shared_data->totalCashiers= shared_data->totalCashiers + 1;

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


void change_score1_cashier1(float score1) {

    int shmid = shmget(cashier1_score_key, sizeof(cashier_score1_shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    cashier_score1_shared_memory *shared_data = (cashier_score1_shared_memory *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // mutex code

    sem_t* score1_mutex = get_semaphore(cashSem1_score_key);

    lock_sem(score1_mutex);


     shared_data->score1 = score1;

    unlock_sem(score1_mutex);
    close_semaphore(score1_mutex);

    //end mutex code

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }


}

void change_score2_cashier2(float score2){

    int shmid = shmget(cashier2_score_key , sizeof(cashier_score2_shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    cashier_score2_shared_memory *shared_data = (cashier_score2_shared_memory *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }


    sem_t* score2_mutex = get_semaphore(cashSem2_score_key);

    lock_sem(score2_mutex);

    shared_data->score2 = score2;

    unlock_sem(score2_mutex);
    close_semaphore(score2_mutex);

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }



}

void change_score3_cashier3(float score3) {

    int shmid = shmget(cashier3_score_key, sizeof(cashier_score3_shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    cashier_score3_shared_memory *shared_data = (cashier_score3_shared_memory *) shmat(shmid, NULL, 0);
    if (shared_data == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }


    sem_t *score3_mutex = get_semaphore(cashSem3_score_key);

    lock_sem(score3_mutex);

    shared_data->score3 = score3;

    unlock_sem(score3_mutex);
    close_semaphore(score3_mutex);

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

}


// num for people or  items to add
// queue_num_index for the queue number
// item_or_people_array_type 1 for people 2 for items

void write_score_att(int num ,int queue_num_index , int people_1_items_2){

    int shmid = shmget(score_atrributes_key, sizeof(score_atrributes), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    score_atrributes *shared_data = (score_atrributes *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // mutex code

    sem_t* score_att = get_semaphore(score_atrributes_key_sem);

    lock_sem(score_att);

    // 1 for people 2 for items


    if(people_1_items_2 == 1){
        shared_data->total_waiting_customers[queue_num_index -1] = num + shared_data->total_waiting_customers[queue_num_index -1];
    }else if(people_1_items_2 == 2){
        shared_data->total_num_of_items[queue_num_index-1] += num;
    }



    unlock_sem(score_att);
    close_semaphore(score_att);

    //end mutex code

    if (shmdt(shared_data) == -1) {

        perror("shmdt");
        exit(EXIT_FAILURE);

    }


}







#endif
