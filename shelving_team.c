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

volatile sig_atomic_t terminate_flag = 0;

void simple_sig_handler(int sig) {

    printf("Signal %d received. Terminating...\n", sig);
    terminate_flag = 1;
}

int main() {

    // Make sure Config c is initialized properly before this point
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = simple_sig_handler;
    sa.sa_flags = 0;

    if(sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Error setting signal handler");
        exit(EXIT_FAILURE);
    }


    c= read_config("config.txt");
    read_items("supermarket_items.txt");


    initializeProducts(); // Initialize products with mutexes
    initializeTeams();    // Initialize teams with manager and employee threads


    while (!terminate_flag) {
        sleep(1); // Simple wait instead of busy wait, adjust time as needed
    }

    for (int i = 1; i <= c.numberOfShelvingTeams; ++i) {
        pthread_mutex_destroy(&teams[i].teamLock);
        pthread_cond_destroy(&teams[i].condition_var);
        free(teams[i].employees); // Free each team's employees array
    }


    free(teams); // Free teams array

    printf("Simulation is ending. Cleaning up resources...\n");

    return 0;


}

