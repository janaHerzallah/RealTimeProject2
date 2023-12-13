#include "header.h"
#include "constants.h"
#include "cashier_header.h"
#include "semphores.h"
#include "message_queues.h"
#include "shared_memories.h"



Cashier cashier_info ;
void fill_data();
void print_cashier_data2( );
void receive_and_process_messages(int cashier_id);
extern Config c;  // Declare or include the Config structure



int main(int argc, char** argv ) {

    c = read_config("config.txt");


    fill_data();  // Populate customer_info with data

    receive_and_process_messages(cashier_info.id);


    exit(0);


}


void fill_data(){


    increment_total_cashiers();
    srand(time(NULL));   // for making random choices for each customer

    int r = rand() % 2;      // Returns a pseudo-random integer between 0 and 1.

    // Put the current time as id for the new person
    cashier_info.id = get_total_cashiers();
    cashier_info.numPeopleInQueue = 0;
    cashier_info.happiness = 20;
    cashier_info.scanningTime = generate_scanning_time( cashier_info.id);

    printf("\n ");
    print_cashier_data2();
    printf("\n ");


}

void print_cashier_data2( ) {



    printf("Cashier ID: %d\n", cashier_info.id);
    printf("Cashier Queue Size: %d\n", cashier_info.numPeopleInQueue);
    printf("Cashier Happiness: %d\n", cashier_info.happiness);
    printf("Cashier Scanning Time: %d\n", cashier_info.scanningTime);

}



void receive_and_process_messages(int cashier_id) {
    int queue = get_queue(C_KEY);
    customerQueue customer_data;

    // Receive and process messages
    while (1) {
        while (1){
            // Check if the queue is not empty
            if (!check_queue_empty(queue)){

                // Receive customer data from the queue
                if(msgrcv(queue, &customer_data, sizeof(customer_data), 0, 0) == -1){
                    perror("Queue receive error\n");
                    exit(-1);
                }

                printf("Cashier %d received customer %d ", cashier_id, customer_data.customer_id);
                printf(" with %d items\n", customer_data.item_count
                );
            }
        }

    }

    }
