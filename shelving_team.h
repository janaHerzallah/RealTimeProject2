#ifndef CASHIER_HEADER
#define CASHIER_HEADER
#define MAX_LINE_LENGTH 100

#include "header.h"
#include "customer_header.h"
#include "semphores.h"
#include "shared_memories.h"
#include "constants.h"


typedef struct RestockInfo {
    int productIndex;
    int amountAvailable;
    int restockComplete;

} RestockInfo;

// Structure for Team
typedef struct Team {
    pthread_t manager;
    pthread_t *employees; // Dynamic array of employee threads
    pthread_cond_t condition_var; // Condition variable for signaling employees
    RestockInfo restockInfo; // Shared information for restocking
    pthread_mutex_t teamLock; // Mutex for team coordination
    pthread_mutex_t employeeLock; // Mutex for employee coordination
    int *employeeIDs; // Array of employee IDs
    int totalRestocked;


    int teamId; // Team ID

} Team;

int totalRestocked = 0;
int safe_unlock = 0;


// Global dynamic arrays of products and teams


// Function prototypes
void *managerFunction(void *arg);
void *employeeFunction(void *arg);


struct Team  team ;

void initializeTeams() {




        team.employees = malloc(c.numberOfEmployeesPerTeam * sizeof(pthread_t)); // Dynamic allocation for employees
        team.employeeIDs = malloc(c.numberOfEmployeesPerTeam * sizeof(int)); // Allocate memory for employee IDs

    pthread_cond_init(&team.condition_var, NULL); // Initialize condition variable

        if (pthread_create(&team.manager, NULL, managerFunction, NULL)) {
            perror("Failed to create manager thread");
            exit(EXIT_FAILURE);
        }
        pthread_mutex_init(&team.teamLock, NULL); // Initialize mutex
        pthread_mutex_init(&team.employeeLock, NULL); // Initialize mutex


    usleep(100000);

        for (int i = 1; i <= c.numberOfEmployeesPerTeam; ++i) {

            team.employeeIDs[i] = i ; // Assign a unique ID to each employee

            // Initialize employees for each team
            if (pthread_create(&team.employees[i], NULL, employeeFunction, &team.employeeIDs[i])) {
                perror("Failed to create employee thread");
                exit(EXIT_FAILURE);
            }


        }


         sleep(c.numberOfEmployeesPerTeam - 0.7 *c.numberOfEmployeesPerTeam);

        printf("Team %d has been initialized \n ", team.teamId);
        printf("******************************\n");
        printf("\n");



}

int selectProductToRestock() {
    srand(time(NULL));   // for making random choices for each customer
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
        safe_unlock = 0; // for making sure that employees are done with restocking

        printf(" i am after lock manger %d i will check product %s i see num on shelf %d  's \n ", team.teamId, shared_items[productIndex].name, shared_items[productIndex].shelfAmount);

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

            printf(" storage amount is %d \n", shared_items[productIndex].storageAmount);
            sleep(1); // fetch time and go to the location

            team.restockInfo.productIndex = productIndex; // Set shared restock info
            team.restockInfo.amountAvailable = amountToTakeFromStorage; // Set shared restock info

            printf("Manager of team %d has fetched %s Employees can restock now.\n", team.teamId, shared_items[productIndex].name);
            usleep(1000);

            pthread_cond_broadcast(&team.condition_var); // Signal employees

            sleep(3); // for making sure that employees are done with restocking


        }else{
            unlock_sem(pick_product_semaphore);
            close_semaphore(pick_product_semaphore);
        }


    goback:

        // Detach from shared memory
        if (shmdt(shared_items) == -1) {
            printf(" error \n");
            perror("shmdt");
            exit(EXIT_FAILURE);
        }

        sleep(BETWEEN_MANGER_CHECKS);  // Wait a bit before checking again
    }


}
void *employeeFunction(void *arg) {
    int employeeID = *((int *)arg);
    printf("Employee with ID %d of team %d is running\n", employeeID, team.teamId);

    while (1) {
        // Wait for manager's signal
        while (team.restockInfo.amountAvailable <= 0) {
            pthread_cond_wait(&team.condition_var, &team.teamLock); // Wait for signal, automatically releases and re-acquires teamLock
        }

        pthread_mutex_unlock(&team.teamLock); // Unlock teamLock (automatically re-acquired after signal


        // If there is available work, proceed with restocking
        int productIndex = team.restockInfo.productIndex;
        totalRestocked=0;
        // Access shared memory segment
        int shmid = shmget(ITM_SMKEY, num_of_products * sizeof(Product), 0666);
        if (shmid == -1) {
            perror("shmget");
            pthread_mutex_unlock(&team.teamLock);
            exit(EXIT_FAILURE);
        }

        Product *shared_items = (Product *)shmat(shmid, NULL, 0);
        if (shared_items == (Product *)-1) {
            perror("shmat");
            pthread_mutex_unlock(&team.teamLock);
            exit(EXIT_FAILURE);
        }

        usleep(BETWEEN_EMPLOYEES_RESTOCK); // to make sure that employees are not restocking at the same time
        // Restocking process
        while (totalRestocked < team.restockInfo.amountAvailable) {

            pthread_mutex_lock(&team.employeeLock); // Lock employeeLock for shared resource access

            if (!(shared_items[productIndex].shelfAmount >= c.amountPerProductOnShelves) && totalRestocked < team.restockInfo.amountAvailable) {

                shared_items[productIndex].shelfAmount++;
                totalRestocked++;
            }

            printf("Employee %d of team %d is restocking product %s, what we restocked for now on shelf %d\n", employeeID, team.teamId, shared_items[productIndex].name, totalRestocked);

            int sleepTime = rand() % 1000000; // Random sleep time in microseconds
            usleep(sleepTime);

            pthread_mutex_unlock(&team.employeeLock); // Unlock

            sleep(TIME_TO_EACH_EMPLOYEE_TO_RESTOCK); // Simulate restocking time (sleep

        }

        // Detach from shared memory
        if (shmdt(shared_items) == -1) {
            perror("shmdt");
            pthread_mutex_unlock(&team.teamLock);
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&team.employeeLock); // Lock employeeLock for shared resource access
        safe_unlock++;
        pthread_mutex_unlock(&team.employeeLock); // Unlock

        if(safe_unlock ==1){
            usleep(10000); // wait all other employees to finish
            printf(" from team %d safe unlock done \n", team.teamId);
            unlock_sem(pick_product_semaphore);
            close_semaphore(pick_product_semaphore);
        }

        team.restockInfo.amountAvailable = 0; // Reset shared restock info

    }

    free(arg); // Free dynamically allocated team ID
    pthread_exit(NULL);
}



#endif
