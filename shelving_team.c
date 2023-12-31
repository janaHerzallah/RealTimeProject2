#include "header.h"
#include "shelving_team.h"
#include "semphores.h"
#include "message_queues.h"
#include "shared_memories.h"




Cashier cashier_info ;

void fill_data();
void print_cashier_data( );


extern Config c;  // Declare or include the Config structure
int number_of_people_served = 0;

float score( );


int main(int argc, char** argv ) {


    c = read_config("config.txt");

    fill_data();  // Populate customer_info with data


    printf("**********************\n ");
    print_cashier_data();
    printf("**********************\n ");



    exit(0);


}


void fill_data(){

    increment_total_cashiers(1,0);

    srand(time(NULL));   // for making random choices for each customer
    cashier_info.id = get_total_cashiers(0);
    cashier_info.numPeopleInQueue = 0;
    cashier_info.happiness = 5;
    cashier_info.scanningTime = 0;


}

void print_cashier_data( ) {



    printf("Cashier ID: %d\n", cashier_info.id);
    printf("Cashier Queue Size: %d\n", cashier_info.numPeopleInQueue);
    printf("Cashier Happiness: %d\n", cashier_info.happiness);
    printf("Cashier Scanning Time: %d\n", cashier_info.scanningTime);

}


