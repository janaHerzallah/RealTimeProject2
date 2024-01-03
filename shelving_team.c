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

// Structure for Team
typedef struct Team {
    pthread_t manager;
    pthread_t *employees; // Dynamic array of employee threads
} Team;

// Global dynamic arrays of products and teams
Product *Products;
Team *teams;

// Function prototypes
void *managerFunction(void *arg);
void *employeeFunction(void *arg);

// Function to initialize products
void initializeProducts() {
    Products = malloc(c.numberOfProducts * sizeof(Product)); // Dynamic allocation for products
    for (int i = 0; i < c.numberOfProducts; ++i) {
        pthread_mutex_init(&Products[i].lock, NULL);
    }
}

// Function to initialize teams
void initializeTeams() {
    teams = malloc(c.numberOfShelvingTeams * sizeof(Team)); // Dynamic allocation for teams
    for (int i = 0; i < c.numberOfShelvingTeams; ++i) {
        teams[i].employees = malloc(c.numberOfEmployeesPerTeam * sizeof(pthread_t)); // Dynamic allocation for employees

        int *teamId = malloc(sizeof(int)); // Allocate memory for team ID
        *teamId = i; // Set team ID

        // Initialize manager for each team
        if (pthread_create(&teams[i].manager, NULL, managerFunction, teamId)) {
            perror("Failed to create manager thread");
            exit(EXIT_FAILURE);
        }

        // Initialize employees for each team
        for (int j = 0; j < c.numberOfEmployeesPerTeam; ++j) {
            if (pthread_create(&teams[i].employees[j], NULL, employeeFunction, teamId)) {
                perror("Failed to create employee thread");
                exit(EXIT_FAILURE);
            }
        }
    }
}

// Placeholder function for manager's behavior
void *managerFunction(void *arg) {
    int teamIndex = *(int *)arg;
    printf("Manager of team %d is running\n", teamIndex);

    // Manager's tasks would go here

    free(arg); // Free the dynamically allocated team ID
    pthread_exit(NULL);
}

// Placeholder function for employee's behavior
void *employeeFunction(void *arg) {
    // Employees don't need to free the team ID; it's handled in the managerFunction.
    int teamIndex = *(int *)arg;
    printf("Employee of team %d is running\n", teamIndex);

    // Employee's tasks would go here

    pthread_exit(NULL);
}

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

    // Cleanup
    // Destroy mutexes and free memory
    for (int i = 0; i < c.numberOfProducts; ++i) {
        pthread_mutex_destroy(&Products[i].lock);
    }

    // Free memory for teams and employees
    for (int i = 0; i < c.numberOfShelvingTeams; ++i) {
        free(teams[i].employees); // Free each team's employees array
    }
    free(teams); // Free teams array
    free(Products); // Free products array

    printf("Simulation is ending. Cleaning up resources...\n");
    return 0;
}
