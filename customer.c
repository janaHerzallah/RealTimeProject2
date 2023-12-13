#include "header.h"
#include "constants.h"
#include "customer_header"

void fill_data();
void print_customer_data(const struct Customer *customer);

struct Customer customer_info;

int main(int argc, char** argv ) {

     printf("I'm the customer  with the id:%d \n", getpid());

     fill_data();  // Populate customer_info with data

     print_customer_data(&customer_info);  // Print the customer's data



    return 0;
}




void fill_data(){

    printf("iam inside fill_data() \n");

    srand(time(NULL));   // for making random choices for each customer

    int r = rand() % 2;      // Returns a pseudo-random integer between 0 and 1.


    // Put the current time as id for the new person
    customer_info.id = getpid();
    customer_info.num_items = 0;
    customer_info.shopping_time= generate_shopping_time();


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


