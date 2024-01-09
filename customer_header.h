#ifndef CUSTOMERS_HEADER
#define CUSTOMERS_HEADER
#define MAX_QUEUE_SIZE 20
#define MAX_ITEMS_IN_CART 5
#define MAX_SHOPPING_TIME 10
#define MAX_NAME_LENGTH 20
#define MAX_NAMES 1000

struct Customer {
    int id;
    int num_items;
    int shopping_time;
    int cart[MAX_ITEMS_IN_CART];
    char name[MAX_NAME_LENGTH]; // Added field for the name

} ;

/** functions starts ****************************************************************************************/


#include "functions.h" // num_of_products of all , items array
#include "header.h" // Config c
#include "shared_memories.h"
#include "semphores.h"

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


void pick_up_items(struct Customer *customer) {





    srand((unsigned int)time(NULL) + customer->id);


   // printf("Customer %d is picking up items:\n", customer->id);

    // Generate and print a random number between 1 and MAX_ITEMS_IN_CART
    int random_number = rand() % c.maxItemsPerCustomer + 1;
 //   printf("Random Number: %d ... customer %d \n", random_number, customer->id);

    int last_picked_index = -1;
    for (int i = 0; i < random_number; ++i) {

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
            printf("No more items to pick. Customer %d finished picking up items.\n", customer->id);
            return;
        }

        int random_index;
        do {
            random_index = rand() % num_of_products;
        } while (shared_items[random_index].shelfAmount <= 0 || random_index == last_picked_index);

        last_picked_index = random_index;

        // Acquire semaphore for picking up items
        pick_product_semaphore = get_semaphore(shared_items[random_index].semName);
        lock_sem(pick_product_semaphore);

        shared_items[random_index].shelfAmount--;
        customer->cart[i] = random_index;
        customer->num_items++;

        unlock_sem(pick_product_semaphore);
        close_semaphore(pick_product_semaphore);


       printf("Customer %s with id %d picked up %s the remaining in shelf is %d what in storage %d \n", customer->name, customer->id, shared_items[random_index].name, shared_items[random_index].shelfAmount , shared_items[random_index].storageAmount);

        // Detach from shared memory
        if (shmdt(shared_items) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }

        sleep(14); // Sleep for 5 seconds


    }


    printf("Customer %s with id %d finished picking up items.\n", customer->name, customer->id);
}


void fill_cart(struct Customer *customer) {


    // Simulate shopping time
    int random_shopping_time = generate_shopping_time();
    printf("Customer %s with id %d is shopping for %d seconds.\n", customer->name, customer->id, random_shopping_time);
    sleep(random_shopping_time);



    srand(getpid());  // Seed the random number generator with the process ID
    int sleepTime = rand() % 10 + 1;  // Generate a random sleep time between 1 and 10 seconds
    sleep(sleepTime);




   // printf("\n----------------------------------------\n");
    pick_up_items(customer);

    //printf("\n----------------------------------------\n");






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


    for (int i = 0; i < num_of_products; i++) {
        pick_product_semaphore = get_semaphore(shared_items[i].semName);
        lock_sem(pick_product_semaphore);

      destinationArray[i] = shared_items[i] ;
        unlock_sem(pick_product_semaphore);
        close_semaphore(pick_product_semaphore);


    }


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

int load_names(const char* filename, char names[MAX_NAMES][MAX_NAME_LENGTH]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open the file");
        return 0;
    }

    int count = 0;
    while (fgets(names[count], MAX_NAME_LENGTH, file) && count < MAX_NAMES) {
        names[count][strcspn(names[count], "\n")] = 0; // Remove newline character
        count++;
    }

    fclose(file);
    return count; // Return the total number of names read
}


void assign_random_name(struct Customer* customer, char names[MAX_NAMES][MAX_NAME_LENGTH], int num_names) {

    srand(time(NULL) + customer->id); // Seed the random number generator with a unique seed


    int random_index = rand() % num_names; // Get a random index
    strncpy(customer->name, names[random_index], MAX_NAME_LENGTH); // Assign a random name
}

#endif

