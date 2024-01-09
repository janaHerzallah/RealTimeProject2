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

    printf("Signal %d received. Terminating...\n", sig);
    terminate_flag = 1;
}

int main(int argc, char *argv[]) {

    // Make sure Config c is initialized properly before this point
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = simple_sig_handler;
    sa.sa_flags = 0;

    if(sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Error setting signal handler");
        exit(EXIT_FAILURE);
    }

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

    for (int i = 1; i <= c.numberOfShelvingTeams; ++i) {
        pthread_cond_destroy(&team.condition_var);
        free(team.employees);
    }


    printf("Simulation is ending. Cleaning up resources...\n");

    return 0;


}

