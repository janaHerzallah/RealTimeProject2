#ifndef CUSTOMERS_HEADER
#define CUSTOMERS_HEADER
#define MAX_QUEUE_SIZE 20
#define MAX_ITEMS_IN_CART 5
#define MAX_SHOPPING_TIME 10


struct Customer {
    int id;
    int num_items;
    int shopping_time;
    int cart[MAX_ITEMS_IN_CART];
} ;

/** functions starts ****************************************************************************************/


#include "functions.h" // num_of_products of all , items array
#include "header.h" // Config c
#include "shared_memories.h"
#include "semphores.h"
#include "message_queues.h"

Config c;
static int totalCustomers = 0;
int protection_for_termination = 0;

void increment_total_customers(int num);

int get_total_customers();

/** Random number generator --------------------------------------------------------------------------------------*/
int generate_random_number() {
    // Generate a random number between 1 and MAX_ITEMS_IN_CART
    return (rand() % MAX_ITEMS_IN_CART) + 1;
}

// Function to generate a random shopping time
int generate_shopping_time() {

    return (rand() % c.shopping_time_max) + 1;

}


/** Random number generator end ----------------------------------------------------------------------------------*/


void pick_up_items(struct Customer *customer, Product *shared_items) {



    srand((unsigned int)time(NULL) + customer->id);


    printf("Customer %d is picking up items:\n", customer->id);

    // Generate and print a random number between 1 and MAX_ITEMS_IN_CART
    int random_number = rand() % c.maxItemsPerCustomer + 1;
    printf("Random Number: %d ... customer %d \n", random_number, customer->id);



        // Check if all Storage have quantity = 0 to end the program.
        int all_storage_zero = 1;
        for (int j = 0; j < c.numberOfProducts ; ++j) {
            if (shared_items[j].storageAmount > 0) {
                all_storage_zero = 0;
                break;
            }
        }
        if (all_storage_zero) {


            // Acquire semaphore for picking up items
            printf("\n the storage is empty \n");
            kill(getppid(), SIGINT);
            exit(0);
        }



    for (int i = 0; i < random_number; ++i) {
        // Check if all items have quantity = 0
        int allItemsZero = 1;
        for (int j = 0; j < num_of_products; ++j) {
            if (shared_items[j].shelfAmount > 0) {
                allItemsZero = 0;
                break;
            }
        }


        if (allItemsZero) {
            // All items have quantity = 0, break the loop
            sem_post(pick_up_items_mutex);
            printf("No more items to pick. Customer %d finished picking up items.\n", customer->id);
            return;
        }

        int random_index;
        do {
            random_index = rand() % num_of_products;
        } while (shared_items[random_index].shelfAmount <= 0);



        shared_items[random_index].shelfAmount--;
        customer->cart[i] = random_index;
        customer->num_items++;

        //sleep((5-(customer->id % 5))%5);
        printf("Customer %d picked up %s\n", customer->id, shared_items[customer->cart[i]].name);
        // Release the mutex


    }

    printf("Customer %d finished picking up items.\n", customer->id);
}


void fill_cart(struct Customer *customer) {

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


    srand(getpid());  // Seed the random number generator with the process ID
    int sleepTime = rand() % 10 + 1;  // Generate a random sleep time between 1 and 10 seconds
    sleep(sleepTime);


    // Acquire semaphore for picking up items
    pick_up_items_mutex = get_semaphore(Pick_key);
    lock_sem(pick_up_items_mutex);


    // Critical section starts here
    customer->num_items = 0;

    printf("\n----------------------------------------\n");
    pick_up_items(customer, shared_items);

    printf("\n----------------------------------------\n");
    // Simulate shopping time
    int random_shopping_time = generate_shopping_time();
    sleep(random_shopping_time);

    // Critical section ends here

    // Release semaphore
    unlock_sem(pick_up_items_mutex);
    sem_close(pick_up_items_mutex);

    // Detach from shared memory
    if (shmdt(shared_items) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return;

}

void get_shared_products(Product *destinationArray) {

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


    // Copy items from shared memory to destination array



        memcpy(destinationArray, shared_items, num_of_products * sizeof(Product));



    if (shmdt(shared_items) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Release semaphore


}

int get_total_customers() {
    int shmid = shmget(CUS_NUM_KEY, sizeof(SharedData), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // mutex code

    customers_total_mutex = get_semaphore(total_customers_key);

    lock_sem(customers_total_mutex);


    int total_cus = shared_data->totalCustomers;

    unlock_sem(customers_total_mutex);
    close_semaphore(customers_total_mutex);

    //end mutex code

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return total_cus;
}


void increment_total_customers(int num ) {
    // Get the shared memory ID
    int shmid = shmget(CUS_NUM_KEY, sizeof(SharedData), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory
    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Acquire the semaphore before modifying shared memory
    sem_t *customers_total_mutex = get_semaphore(total_customers_key);

    lock_sem(customers_total_mutex);

    // Increment the totalCustomers value
    shared_data->totalCustomers= shared_data->totalCustomers + num;

    // Release the semaphore
    unlock_sem(customers_total_mutex);
    close_semaphore(customers_total_mutex);

    // Detach from shared memory
    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}



#endif

