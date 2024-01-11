#include "header.h"
#include "shelving_team.h"
#include "semphores.h"
#include "shared_memories.h"
#include "functions.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

Config c;  // Assume this is defined and initialized properly elsewhere

void clean_threads();

volatile sig_atomic_t terminate_flag = 0;

void simple_sig_handler(int sig) {
    terminate_flag = 1;
}

int main(int argc, char *argv[]) {

     signal(SIGINT, simple_sig_handler);


    if (argc != 2) {
        fprintf(stderr, "Usage: %s <index>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int index = atoi(argv[1]); // Convert the argument to an integer

    if (index < 0) {
        fprintf(stderr, "Invalid index: %d\n", index);
        return EXIT_FAILURE;
    }

    c= read_config("config.txt");
    read_items("supermarket_items.txt");

    team.teamId = index;

    initializeTeams();    // Initialize teams with manager and employee threads


    while (!terminate_flag) {
        sleep(1); // Simple wait instead of busy wait, adjust time as needed
    }

    printf("Team %d has been terminated\n", team.teamId);
    pthread_mutex_destroy(&team.teamLock);
    pthread_mutex_destroy(&employeeLock);
    pthread_barrier_destroy(&barrier_for_employee);


    return 0;


}

