#include "header.h"
#include "shelving_team.h"
#include "semphores.h"
#include "message_queues.h"
#include "shared_memories.h"
#include "functions.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

Config c;  // Assume this is defined and initialized properly elsewhere

void clean_threads();

int main() {
    // Make sure Config c is initialized properly before this point

    c= read_config("config.txt");
    read_items("supermarket_items.txt");


    initializeProducts(); // Initialize products with mutexes
    initializeTeams();    // Initialize teams with manager and employee threads

    // wait threads and perform cleanup at the end of the simulation
    for (int i = 1; i <= c.numberOfShelvingTeams; ++i) {
        pthread_join(teams[i].manager, NULL);
        for (int j = 1; j <= c.numberOfEmployeesPerTeam; ++j) {
            pthread_join(teams[i].employees[j], NULL);
        }
    }

    printf(" i am done \n");



    clean_threads();

    return 0;
}

void clean_threads(){

    for (int i = 1; i <= c.numberOfShelvingTeams; ++i) {
        pthread_mutex_destroy(&teams[i].teamLock);
        pthread_cond_destroy(&teams[i].condition_var);
        free(teams[i].employees); // Free each team's employees array
    }


    int shmid = shmget(ITM_SMKEY, num_of_products * sizeof(Product), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    Product *shared_items = (Product *) shmat(shmid, NULL, 0);
    if (shared_items == (Product *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);

    }

    // Acquire semaphore for picking up items
    pick_up_items_mutex = get_semaphore(Pick_key);
    lock_sem(pick_up_items_mutex);


    for (int i = 0; i < c.numberOfProducts; ++i) {

        pthread_mutex_destroy(&shared_items[i].lock);

    }


    unlock_sem(pick_up_items_mutex);
    sem_close(pick_up_items_mutex);

    // Detach from shared memory
    if (shmdt(shared_items) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }


    free(teams); // Free teams array

    printf("Simulation is ending. Cleaning up resources...\n");
}
