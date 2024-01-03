#ifndef CASHIER_HEADER
#define CASHIER_HEADER
#define MAX_LINE_LENGTH 100
#include "header.h"
#include "customer_header.h"
#include "semphores.h"



// Structure for Team
typedef struct Team {
    pthread_t manager;
    pthread_t *employees; // Dynamic array of employee threads
    pthread_mutex_t teamLock; // Mutex for team coordination
    pthread_cond_t condition_var; // Condition variable for signaling employees
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
        pthread_mutex_init(&teams[i].teamLock, NULL); // Initialize mutex
        pthread_cond_init(&teams[i].condition_var, NULL); // Initialize condition variable

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

// Function for manager's behavior
void *managerFunction(void *arg) {
    int teamIndex = *(int *)arg;
    printf("Manager of team %d is running\n", teamIndex);

    // Simulate the manager's tasks
    // In real code, this would involve monitoring product levels and restocking

    // Notify employees using condition variable
    pthread_mutex_lock(&teams[teamIndex].teamLock);
    pthread_cond_broadcast(&teams[teamIndex].condition_var);
    pthread_mutex_unlock(&teams[teamIndex].teamLock);

    free(arg); // Free the dynamically allocated team ID
    pthread_exit(NULL);
}

// Function for employee's behavior
void *employeeFunction(void *arg) {
    int teamIndex = *(int *)arg;
    printf("Employee of team %d is running\n", teamIndex);

    // Wait for manager's signal
    pthread_mutex_lock(&teams[teamIndex].teamLock);
    pthread_cond_wait(&teams[teamIndex].condition_var, &teams[teamIndex].teamLock);
    pthread_mutex_unlock(&teams[teamIndex].teamLock);

    // Simulate restocking one product at a time

    pthread_exit(NULL);
}



#endif
