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
    pthread_mutex_t teamLock; // Mutex for team coordination
    pthread_cond_t condition_var; // Condition variable for signaling employees
    RestockInfo restockInfo; // Shared information for restocking

} Team;


// Global dynamic arrays of products and teams
Team *teams;

// Function prototypes
void *managerFunction(void *arg);

void *employeeFunction(void *arg);

// Function to initialize products
void initializeProducts() {

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


    //   Products = malloc(c.numberOfProducts * sizeof(Product)); // Dynamic allocation for products


    for (int i = 0; i < c.numberOfProducts; ++i) {
        pthread_mutex_init(&shared_items[i].lock, NULL);
    }


    unlock_sem(pick_up_items_mutex);
    sem_close(pick_up_items_mutex);

    // Detach from shared memory
    if (shmdt(shared_items) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

void initializeTeams() {

    teams = malloc(c.numberOfShelvingTeams * sizeof(Team)); // Dynamic allocation for teams

    for (int i = 1; i <= c.numberOfShelvingTeams; ++i) {

        teams[i].employees = malloc(c.numberOfEmployeesPerTeam * sizeof(pthread_t)); // Dynamic allocation for employees

        pthread_mutex_init(&teams[i].teamLock, NULL); // Initialize mutex
        pthread_cond_init(&teams[i].condition_var, NULL); // Initialize condition variable

        // Initialize manager for each team
        int *managerTeamId = malloc(sizeof(int)); // Allocate memory for manager's team ID
        *managerTeamId = i; // Set manager's team ID

        if (pthread_create(&teams[i].manager, NULL, managerFunction, managerTeamId)) {
            perror("Failed to create manager thread");
            exit(EXIT_FAILURE);
        }

        usleep(100000);

        for (int j = 1; j <= c.numberOfEmployeesPerTeam; ++j) {

            int *employeeTeamId = malloc(sizeof(int)); // Allocate memory for employee's team ID
            *employeeTeamId = i; // Set employee's team ID

            // Initialize employees for each team
            if (pthread_create(&teams[i].employees[j], NULL, employeeFunction, employeeTeamId)) {
                perror("Failed to create employee thread");
                exit(EXIT_FAILURE);
            }
        }

        usleep(100000);

        printf("Team %d has been initialized \n ", i);
        printf("******************************\n");
        printf("\n");


    }
}

int selectProductToRestock() {
    return rand() % c.numberOfProducts; // Randomly selecting a product for simplicity
}

// Function for manager's behavior
void *managerFunction(void *arg) {
    int teamIndex = *(int *) arg;
    printf("******************************\n");

    printf("Manager of team %d is running\n", teamIndex);

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



        pthread_mutex_lock(&shared_items[productIndex].lock);
         // Lock the product's mutex before accessing or modifying it



        if (shared_items[productIndex].shelfAmount < c.productRestockThreshold && shared_items[productIndex].storageAmount > 0  ) {



            printf("Manager of team %d restocking product %s\n", teamIndex, shared_items[productIndex].name);
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
            printf("Manager is going to fetch %d items of product %s\n", amountToTakeFromStorage, shared_items[productIndex].name);

            // Adjust the quantities on shelves and in storage

            pick_product_semaphore = get_semaphore(shared_items[productIndex].semName);
            lock_sem(pick_product_semaphore);

            shared_items[productIndex].storageAmount -= amountToTakeFromStorage;

            unlock_sem(pick_product_semaphore);
            close_semaphore(pick_product_semaphore);


            printf(" storage amount is %d \n", shared_items[productIndex].storageAmount);



            sleep(3); // fetch time and go to the location




            // Signal employees that product is ready to restock
            pthread_mutex_lock(&teams[teamIndex].teamLock); // Acquire team's mutex
            teams[teamIndex].restockInfo.productIndex = productIndex; // Set shared restock info
            teams[teamIndex].restockInfo.amountAvailable = amountToTakeFromStorage; // Set shared restock info
            teams[teamIndex].restockInfo.taskTaken = 0; // Reset task taken flag
            pthread_mutex_unlock(&teams[teamIndex].teamLock); // Release team's mutex
            pthread_cond_broadcast(&teams[teamIndex].condition_var); // Signal employees reaching the location.
            printf("Manager of team %d has fetched products. Employees can restock now.\n", teamIndex);

            sleep(3); // for making sure that employees are done with restocking


        }


        pthread_mutex_unlock(&shared_items[productIndex].lock);

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
    int teamIndex = *(int *)arg;
    printf("Employee of team %d is running\n", teamIndex);

    while (1) {

        // Wait for manager's signal
        pthread_mutex_lock(&teams[teamIndex].teamLock);

        // Wait until there is a task available that hasn't been taken
        while (teams[teamIndex].restockInfo.amountAvailable > 0 && teams[teamIndex].restockInfo.taskTaken) {
            pthread_cond_wait(&teams[teamIndex].condition_var, &teams[teamIndex].teamLock);
        }

        // If there is available work, and it hasn't been taken, proceed.

        if (teams[teamIndex].restockInfo.amountAvailable > 0) {
            teams[teamIndex].restockInfo.taskTaken = 1;  // Mark the task as taken.
            int productIndex = teams[teamIndex].restockInfo.productIndex;
            int what_Available = teams[teamIndex].restockInfo.amountAvailable;
            pthread_mutex_unlock(&teams[teamIndex].teamLock);  // Release the team's mutex

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

            printf("Employee of team %d by amount %d is restocking product %s\n", teamIndex, what_Available, shared_items[productIndex].name);
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
        } else {
            // No work available or already taken; just release lock and continue
            pthread_mutex_unlock(&teams[teamIndex].teamLock);
        }
    }

    free(arg);  // Free the dynamically allocated team ID
    pthread_exit(NULL);
}



#endif
