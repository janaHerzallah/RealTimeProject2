#include "header.h"
#include "constants.h"
#include "customer_header"

void fill_data();
void print_customer_data(const struct Customer *customer);
void fill_cart( struct Customer *customer);
struct Customer customer_info;
extern Config c;  // Declare or include the Config structure

int main(int argc, char** argv ) {

    c = read_config("config.txt");


     printf("I'm the customer  with the id:%d \n", getpid());

     fill_data();  // Populate customer_info with data

     print_customer_data(&customer_info);  // Print the customer's data




}




void fill_data(){


    srand(time(NULL));   // for making random choices for each customer

    int r = rand() % 2;      // Returns a pseudo-random integer between 0 and 1.

    // Put the current time as id for the new person
    customer_info.id = getpid();
    customer_info.num_items = 0;

    printf("iam inside fill_data() 2 \n");

    customer_info.shopping_time= generate_shopping_time();

    printf("iam inside fill_data() 3 \n");


    // The Patience for the person
    // how much time he/she can wait in the OIM
    r = rand() % 4;
    if(r == 0){
        customer_info.patience_degree = LOW;
    }else if(r == 1){
        customer_info.patience_degree = MEDIUM;
    }else if(r == 2){
        customer_info.patience_degree = HIGH;
    }else {
        customer_info.patience_degree = VERY_HIGH;
    }


    printf("before print_customer_data() \n");
    print_customer_data(&customer_info);

    fill_cart(&customer_info);


}


void print_customer_data(const struct Customer *customer) {
    printf("Customer ID: %d\n", customer->id);
    printf("Number of Items: %d\n", customer->num_items);
    printf("Shopping Time: %d\n", customer->shopping_time);
    printf("Patience Degree: ");

    switch (customer->patience_degree) {
        case LOW:
            printf("Low\n");
            break;
        case MEDIUM:
            printf("Medium\n");
            break;
        case HIGH:
            printf("High\n");
            break;
        case VERY_HIGH:
            printf("Very High\n");
            break;
        default:
            printf("Unknown\n");
    }
}

void fill_cart( struct Customer *customer) {

    printf("i am inside fill_cart() \n");

    int shmid = shmget(ITM_SMKEY, num_items * sizeof(Item), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    Item *shared_items = (Item *)shmat(shmid, NULL, 0);
    if (shared_items == (Item *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize shared memory with initial data
    memcpy(shared_items, items, num_items * sizeof(Item));

    // Create semaphore
    mutex = sem_open("/mutex", O_CREAT | O_EXCL, 0666, 1);
    if (mutex == SEM_FAILED) {
        if (errno == EEXIST) {
            // Handle case where semaphore already exists
            printf("Semaphore already exists.\n");
        } else {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }
    }


    // critical section area -----------------------------------------------------------------------------------------
    int totalCustomers = 0;



    pick_up_items(&customer, shared_items);

    // Simulate shopping time
    int randomshopping_time = generate_shopping_time();

    sleep(randomshopping_time);

    totalCustomers++;


    // Detach from shared memory
    if (shmdt(shared_items) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Remove shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    // Close and unlink the semaphore
    sem_close(mutex);
    sem_unlink("/mutex");

    // critical section area end -------------------------------------------------------------------------------------

}

