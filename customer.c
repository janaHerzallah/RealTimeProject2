#include "header.h"
#include "customer_header.h"
#include "shelving_team.h"
#include "functions.h"

void fill_data();
void print_customer_data(const struct Customer *customer);
void fill_cart( struct Customer *customer);
struct Customer customer_info;
 Config c;  // Declare or include the Config structure
 Product products[MAX_LINE_LENGTH];
 int num_of_products;

void sigterm_handler();

int main(int argc, char** argv ) {

    read_items("supermarket_items.txt");
    c = read_config("config.txt");

    fill_data();  // Populate customer_info with data

   // printf("\n \n");
   // print_customer_data(&customer_info);
   // printf("\n \n");

    exit(0);
}




void fill_data(){


    srand(time(NULL));   // for making random choices for each customer

    int r = rand() % 2;      // Returns a pseudo-random integer between 0 and 1.

    // Put the current time as id for the new person
    customer_info.id = get_total_customers();
    customer_info.num_items = 0;
    customer_info.shopping_time= generate_shopping_time();

    fill_cart(&customer_info);

    printf(" ------------------ \n");
    printf(" customer id is %d is done  \n",customer_info.id);
    printf(" ------------------ \n");

    increment_total_customers(-1);


}


void print_customer_data(const struct Customer *customer) {

    printf("customer with id %d info is : \n \n",customer->id);

    printf("Customer ID: %d\n", customer->id);
    printf("Number of Items: %d\n", customer->num_items);
    printf("Shopping Time: %d\n", customer->shopping_time);


    printf("Products in Cart: ");
    for (int i = 0; i < customer->num_items; ++i) {
        printf(" %s ", products[customer->cart[i]].name);

    }



}



