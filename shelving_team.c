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

    initializeProducts(); // Initialize products with mutexes
    initializeTeams();    // Initialize teams with manager and employee threads

    // Join threads and perform cleanup at the end of the simulation
    for (int i = 0; i < c.numberOfShelvingTeams; ++i) {
        pthread_join(teams[i].manager, NULL);
        for (int j = 0; j < c.numberOfEmployeesPerTeam; ++j) {
            pthread_join(teams[i].employees[j], NULL);
        }
    }


    clean_threads();

    return 0;
}

void clean_threads(){

    // Cleanup
    // Destroy mutexes and condition variables, free memory
    for (int i = 0; i < c.numberOfProducts; ++i) {
        pthread_mutex_destroy(&Products[i].lock);
    }

    for (int i = 0; i < c.numberOfShelvingTeams; ++i) {
        pthread_mutex_destroy(&teams[i].teamLock);
        pthread_cond_destroy(&teams[i].condition_var);
        free(teams[i].employees); // Free each team's employees array
    }

    free(teams); // Free teams array
    free(Products); // Free products array

    printf("Simulation is ending. Cleaning up resources...\n");
}
