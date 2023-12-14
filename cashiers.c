#include "header.h"
#include "constants.h"
#include "cashier_header.h"
#include "semphores.h"
#include "message_queues.h"
#include "shared_memories.h"



Cashier cashier_info ;

void fill_data();
void print_cashier_data( );

void receive_and_process_messages(int cashier_id);
void receive_and_process_messages2(int cashier_id);
void receive_and_process_messages3(int cashier_id);

extern Config c;  // Declare or include the Config structure
int number_of_people_served = 0;

float score( );


int main(int argc, char** argv ) {

    c = read_config("config.txt");


    increment_total_cashiers();

    fill_data();  // Populate customer_info with data



    printf("**********************\n ");
    print_cashier_data();
    printf("**********************\n ");
    // make the cashier goes to its own queue.

    change_score1_cashier1(3);

    switch (cashier_info.id) {
        case 1:
            receive_and_process_messages(cashier_info.id);
            break;
        case 2:
            receive_and_process_messages2(cashier_info.id);
            break;
        case 3:
            receive_and_process_messages3(cashier_info.id);
            break;
        default:
            break;

    }




    exit(0);


}


void fill_data(){



    srand(time(NULL));   // for making random choices for each customer



    cashier_info.id = get_total_cashiers();
    cashier_info.numPeopleInQueue = 0;
    cashier_info.happiness = 20;
    cashier_info.scanningTime = generate_scanning_time( cashier_info.id);





}

void print_cashier_data( ) {



    printf("Cashier ID: %d\n", cashier_info.id);
    printf("Cashier Queue Size: %d\n", cashier_info.numPeopleInQueue);
    printf("Cashier Happiness: %d\n", cashier_info.happiness);
    printf("Cashier Scanning Time: %d\n", cashier_info.scanningTime);

}



void receive_and_process_messages(int cashier_id) {
    int queue = get_queue(C_KEY);
    customerQueue customer_data;

    printf(" Cashier 1 is here \n \n");
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

                printf("Cashier %d received customer %d , Total price : %0.2f\n", cashier_id, customer_data.customer_id, customer_data.total_price);
                printf(" with %d items\n", customer_data.item_count
                );
                number_of_people_served++;
                printf("Number of people served : %d\n", number_of_people_served);
            }
        }

    }

}

void receive_and_process_messages2(int cashier_id) {
    int queue = get_queue(C_KEY2);
    customerQueue customer_data;

    printf(" Cashier 2 is here \n \n");
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

                printf("Cashier %d received customer %d , Total price : %0.2f\n", cashier_id, customer_data.customer_id, customer_data.total_price);
                printf(" with %d items\n", customer_data.item_count
                );
                number_of_people_served++;
                printf("Number of people served : %d\n", number_of_people_served);
            }
        }

    }

}

void receive_and_process_messages3(int cashier_id) {
    int queue = get_queue(C_KEY3);
    customerQueue customer_data;

    printf(" Cashier 3 is here \n \n");
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

                printf("Cashier %d received customer %d , Total price : %0.2f\n", cashier_id, customer_data.customer_id, customer_data.total_price);
                printf(" with %d items\n", customer_data.item_count
                );
                number_of_people_served++;
                printf("Number of people served : %d\n", number_of_people_served);
            }
        }

    }

}

float score( ){

    float queueSizeWeight = 0.5;
    float totalItemsWeight = 0.25;
    float scanningSpeedWeight = 0.25;
    float behaviorScoreWeight = 0.25;

    // Calculate the weighted score
    int score = (cashier_info.numPeopleInQueue * queueSizeWeight) +
                (cashier_info.number_of_all_items * totalItemsWeight) +
                (cashier_info.scanningTime * scanningSpeedWeight) +
                (cashier_info.happiness * behaviorScoreWeight);

    return score;

}