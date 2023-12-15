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

    fill_data();  // Populate customer_info with data



    printf("**********************\n ");
    print_cashier_data();
    printf("**********************\n ");
    // make the cashier goes to its own queue.

  // change_score1_cashier1(3);
   // change_score2_cashier2(4);

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

    increment_total_cashiers(1,0);

    srand(time(NULL));   // for making random choices for each customer
    cashier_info.id = get_total_cashiers(0);
    cashier_info.numPeopleInQueue = 0;
    cashier_info.happiness = 20;
    cashier_info.scanningTime = generate_scanning_time( cashier_info.id);

    switch (cashier_info.id) {
        case 1:
            increment_total_cashiers(cashier_info.scanningTime,1);
            break;
        case 2:
            increment_total_cashiers(cashier_info.scanningTime,2);
            break;
        case 3:
            increment_total_cashiers(cashier_info.scanningTime,3);
            break;


    }





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

    int array1[3] = {0,0,0};
    int array2[3] = {0,0,0};

    printf(" Cashier 1 is here \n \n");
    // Receive and process messages
    while (1) {
        while (1){
            // Check if the queue is not empty
            if (!check_queue_empty(queue)){

                printf("\n \n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n \n");

                get_score_att(array1,array2);
                printf(" people in queue %d \n", array1[0]);
                printf(" number of items %d \n", array2[0]);

                int last_coustmer_id = customer_data.customer_id;
                // Receive customer data from the queue
                if(msgrcv(queue, &customer_data, sizeof(customer_data), 0, 0) == -1){
                    perror("Queue receive error\n");
                    exit(-1);
                }

                int current_coustmer_id = customer_data.customer_id;
                if(last_coustmer_id == current_coustmer_id){
                    continue;
                }

                printf("Cashier 1 is selling custermor %d the scnning time is %d \n",customer_data.customer_id,cashier_info.scanningTime);

                sleep(cashier_info.scanningTime);

                printf("Cashier 1 is done selling custermor %d \n",customer_data.customer_id);

               // printf(" the result Customer %d , Total price : %0.2f\n", customer_data.customer_id, customer_data.total_price);
              ///  printf(" with %d items\n", customer_data.item_coun );


                cashier_info.total_sales= cashier_info.total_sales + customer_data.total_price;
                printf("Total sales: %f\n", cashier_info.total_sales);
                change_score1_cashier1(cashier_info.total_sales,2);
                number_of_people_served++;
        //        printf("Number of people served : %d\n", number_of_people_served);

                write_score_att(-(customer_data.item_count),1,2);
                write_score_att(-1,1,1);
                change_score1_cashier1(score( array1[0], array2[0], cashier_info.scanningTime, cashier_info.happiness),1);
                printf(" Score 1 : %f\n", score( array1[0], array2[0], cashier_info.scanningTime, cashier_info.happiness));

                printf("\n \n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n \n");

            }
        }

    }

}

void receive_and_process_messages2(int cashier_id) {
    int queue = get_queue(C_KEY2);
    customerQueue customer_data;

    printf(" Cashier 2 is here \n \n");

    int array1[3] = {0,0,0};
    int array2[3] = {0,0,0};

    // Receive and process messages
    while (1) {
        while (1){
            // Check if the queue is not empty
            if (!check_queue_empty(queue)){

                printf("\n \n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n \n");

                get_score_att(array1,array2);

                int last_coustmer_id = customer_data.customer_id;

                // Receive customer data from the queue
                if(msgrcv(queue, &customer_data, sizeof(customer_data), 0, 0) == -1){
                    perror("Queue receive error\n");
                    exit(-1);
                }

                int current_coustmer_id = customer_data.customer_id;
                if(last_coustmer_id == current_coustmer_id){
                    continue;
                }

                printf("Cashier 2 is selling custermor %d the scnning time is %d \n",customer_data.customer_id,cashier_info.scanningTime);

                sleep(cashier_info.scanningTime);

                printf("Cashier 2 is done selling custermor %d \n",customer_data.customer_id);

                printf(" the result Customer %d , Total price : %0.2f\n", customer_data.customer_id, customer_data.total_price);
                // printf(" with %d items\n", customer_data.item_count);

                cashier_info.total_sales= cashier_info.total_sales + customer_data.total_price;
                change_score2_cashier2(cashier_info.total_sales,2);


                number_of_people_served++;
             //   printf("Number of people served : %d\n", number_of_people_served);

                write_score_att(-1,2,1);
                write_score_att(-(customer_data.item_count),2,2);
                change_score2_cashier2(score( array1[1], array2[1], cashier_info.scanningTime, cashier_info.happiness),1);
                printf(" Score 2 : %f\n", score( array1[1], array2[1], cashier_info.scanningTime, cashier_info.happiness));

                printf("\n \n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n \n");

            }
        }

    }

}

void receive_and_process_messages3(int cashier_id) {
    int queue = get_queue(C_KEY3);
    customerQueue customer_data;

    int array1[3] = {0,0,0};
    int array2[3] = {0,0,0};

    printf(" Cashier 3 is here \n \n");
    // Receive and process messages
    while (1) {
        while (1){
            // Check if the queue is not empty
            if (!check_queue_empty(queue)){


                printf("\n \n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n \n");

                int last_coustmer_id = customer_data.customer_id;

                get_score_att(array1,array2);


                cashier_info.numPeopleInQueue++;


                // Receive customer data from the queue
                if(msgrcv(queue, &customer_data, sizeof(customer_data), 0, 0) == -1){
                    perror("Queue receive error\n");
                    exit(-1);
                }

                int current_coustmer_id = customer_data.customer_id;

                if(last_coustmer_id == current_coustmer_id){
                    continue;
                }




                printf("Cashier 3 is selling custermor %d the scnning time is %d \n",customer_data.customer_id,cashier_info.scanningTime);

                sleep(cashier_info.scanningTime);

                printf("Cashier 3 is done selling custermor %d \n",customer_data.customer_id);

                printf(" the result Customer %d , Total price : %0.2f\n", customer_data.customer_id, customer_data.total_price);

                /*float current_score = score();
                printf("Current score: %f\n", current_score);
                change_score2_cashier2(current_score);
*/

                number_of_people_served++;
               // printf("Number of people served : %d\n", number_of_people_served);


                cashier_info.numPeopleInQueue--;

                write_score_att(-(customer_data.item_count),3,2);


                cashier_info.total_sales= cashier_info.total_sales + customer_data.total_price;
                printf("Total sales: %f\n", cashier_info.total_sales);

                change_score3_cashier3(cashier_info.total_sales,2);

                write_score_att(-1,3,1);
                change_score3_cashier3(score( array1[2], array2[2], cashier_info.scanningTime, cashier_info.happiness),1);
                printf(" Score 3 : %f\n", score( array1[2], array2[2], cashier_info.scanningTime, cashier_info.happiness));

                printf("\n \n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n \n");


            }
        }

    }

}

float score( int numPeopleInQueue, int number_of_all_items, int scanningTime, int happiness  ){

    float queueSizeWeight = -0.5; // Negative because a larger queue is worse
    float totalItemsWeight = -0.3; // Negative because more items generally mean slower processing
    float scanningSpeedWeight = 0; // Positive, assuming higher value means faster scanning
    float behaviorScoreWeight = 0; // Positive, higher happiness score is better

    // Calculate the weighted score
    float score = queueSizeWeight * numPeopleInQueue + totalItemsWeight * number_of_all_items + scanningSpeedWeight * scanningTime + behaviorScoreWeight * happiness;

    return score;

}