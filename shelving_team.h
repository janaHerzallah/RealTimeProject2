#ifndef CASHIER_HEADER
#define CASHIER_HEADER
#define MAX_LINE_LENGTH 100

#include "header.h"
#include "customer_header.h"
#include "semphores.h"
#include "shared_memories.h"


typedef struct RestockInfo {
    int productIndex;
    int amountAvailable;
    int taskTaken;
} RestockInfo;

// Structure for Team
typedef struct Team {
    pthread_t manager;
    pthread_t *employees; // Dynamic array of employee threads
    pthread_cond_t condition_var; // Condition variable for signaling employees
    RestockInfo restockInfo; // Shared information for restocking
    pthread_mutex_t teamLock; // Mutex for team coordination

    int teamId; // Team ID

} Team;


// Global dynamic arrays of products and teams


// Function prototypes
void *managerFunction(void *arg);
void *employeeFunction(void *arg);


struct Team  team ;

void initializeTeams() {




        team.employees = malloc(c.numberOfEmployeesPerTeam * sizeof(pthread_t)); // Dynamic allocation for employees
        pthread_cond_init(&team.condition_var, NULL); // Initialize condition variable

        if (pthread_create(&team.manager, NULL, managerFunction, NULL)) {
            perror("Failed to create manager thread");
            exit(EXIT_FAILURE);
        }
        pthread_mutex_init(&team.teamLock, NULL); // Initialize mutex


    usleep(100000);

        for (int i = 1; i <= c.numberOfEmployeesPerTeam; ++i) {


            // Initialize employees for each team
            if (pthread_create(&team.employees[i], NULL, employeeFunction,NULL)) {
                perror("Failed to create employee thread");
                exit(EXIT_FAILURE);
            }
        }

        usleep(100000);

        printf("Team %d has been initialized \n ", team.teamId);
        printf("******************************\n");
        printf("\n");



}

int selectProductToRestock() {
    return rand() % c.numberOfProducts; // Randomly selecting a product for simplicity
}

// Function for manager's behavior
void *managerFunction(void *arg) {

    printf("******************************\n");
    printf("Manager of team %d is running\n",team.teamId);

    while (1) {  // Replace with a real condition

        int productIndex = selectProductToRestock();

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


        pick_product_semaphore = get_semaphore(shared_items[productIndex].semName);

        lock_sem(pick_product_semaphore);

        if (shared_items[productIndex].shelfAmount < c.productRestockThreshold && shared_items[productIndex].storageAmount > 0  ) {



            printf("Manager of team %d restocking product %s\n", team.teamId, shared_items[productIndex].name);
            printf(" shelf amount is %d \n", shared_items[productIndex].shelfAmount);

            // Calculate how much more is needed to fill shelves up to the desired full amount
            int neededToFillShelves = c.amountPerProductOnShelves - shared_items[productIndex].shelfAmount;

            printf(" needed to fill shelves is %d \n", neededToFillShelves);

            // Ensure that the needed amount is not negative
            if (neededToFillShelves < 0) {
                neededToFillShelves = 0;
            }

            // Determine how much to take from storage based on what's needed and what's available
            int amountToTakeFromStorage;
            if (neededToFillShelves > shared_items[productIndex].storageAmount) {
                // Take whatever is available in storage if needed amount exceeds available amount
                amountToTakeFromStorage = shared_items[productIndex].storageAmount;
            } else {
                // Otherwise, take exactly what's needed to fill the shelves
                amountToTakeFromStorage = neededToFillShelves;
            }

            // Print how much is being fetched for clarity
            printf("Manager with id %d is going to fetch %d items of product %s which has storage %d \n", team.teamId, amountToTakeFromStorage, shared_items[productIndex].name, shared_items[productIndex].storageAmount);

            // Adjust the quantities on shelves and in storage

            if (shared_items[productIndex].storageAmount == 0) {
                goto goback;
            }

            shared_items[productIndex].storageAmount -= amountToTakeFromStorage;


            close_semaphore(pick_product_semaphore);

            printf(" storage amount is %d \n", shared_items[productIndex].storageAmount);
            sleep(3); // fetch time and go to the location

            pthread_mutex_lock(&team.teamLock); // Acquire team's mutex
            team.restockInfo.productIndex = productIndex; // Set shared restock info
            team.restockInfo.amountAvailable = amountToTakeFromStorage; // Set shared restock info
            team.restockInfo.taskTaken = 0; // Reset task taken flag
            pthread_mutex_unlock(&team.teamLock); // Release team's mutex

            printf("Manager of team %d has fetched %s Employees can restock now.\n", team.teamId, shared_items[productIndex].name);
            usleep(1000);
            pthread_mutex_unlock(&team.teamLock);
            pthread_cond_broadcast(&team.condition_var); // Signal employees reaching the location.



            sleep(3); // for making sure that employees are done with restocking


        }

        unlock_sem(pick_product_semaphore);

        goback:


        // Detach from shared memory
        if (shmdt(shared_items) == -1) {
            printf(" error \n");
            perror("shmdt");
            exit(EXIT_FAILURE);
        }



        sleep(6);  // Wait a bit before checking again
    }




}
void *employeeFunction(void *arg) {

    printf("Employee of team %d is running\n", team.teamId);

    while (1) {

        pthread_mutex_lock(&team.teamLock);

        // Wait for manager's signal

        // Wait until there is a task available that hasn't been taken
        while (team.restockInfo.amountAvailable > 0 && team.restockInfo.taskTaken) {
            pthread_cond_wait(&team.condition_var, &team.teamLock);
        }



        // If there is available work, and it hasn't been taken, proceed.

        if (team.restockInfo.amountAvailable > 0) {
            team.restockInfo.taskTaken = 1;  // Mark the task as taken.
            int productIndex = team.restockInfo.productIndex;
            int what_Available = team.restockInfo.amountAvailable;

            pthread_mutex_unlock(&team.teamLock);

            // Simulate restocking one product at a time
            // Here, we'll use a shared memory segment similar to the manager function
            int shmid = shmget(ITM_SMKEY, num_of_products * sizeof(Product), 0666);
            if (shmid == -1) {
                perror("shmget");
                exit(EXIT_FAILURE);
            }

            Product *shared_items = (Product *)shmat(shmid, NULL, 0);
            if (shared_items == (Product *)-1) {
                perror("shmat");
                exit(EXIT_FAILURE);
            }

            pick_product_semaphore = get_semaphore(shared_items[productIndex].semName);
            lock_sem(pick_product_semaphore);

            printf("Employee of team %d by amount %d is restocking product %s\n", team.teamId , what_Available, shared_items[productIndex].name);
           // sleep(3); // restock time

            shared_items[productIndex].shelfAmount += what_Available;


            unlock_sem(pick_product_semaphore);
            close_semaphore(pick_product_semaphore);


            // Unlock the product's mutex after modification is done

            // Detach from shared memory
            if (shmdt(shared_items) == -1) {
                perror("shmdt");
                exit(EXIT_FAILURE);
            }


            // Some delay before the next wait for manager's signal
            sleep(1);
        }else {
            // No work available or already taken; just release lock and continue
            pthread_mutex_unlock(&team.teamLock);
        }
    }

    free(arg);  // Free the dynamically allocated team ID
    pthread_exit(NULL);
}



#endif
