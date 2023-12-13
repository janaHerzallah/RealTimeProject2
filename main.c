
#include "header.h"
#include "constants.h"
#include "functions.h"




int main(int argc, char** argv){


    // print items area start
    read_items("supermarket_items.txt");

    // Print items
    printf("Items in the Supermarket:\n");
    printf("----------------------------\n");
    for (int i = 0; i < num_items; ++i) {
        printf("Item: %s, Quantity: %d, Price: $%.2f\n", items[i].name, items[i].quantity, items[i].price);
    }

    // print items area end

   /***********************************************************************************************************************************************************/

    // Print configuration variables

    //Config config = read_config("config.txt");
    Config c;
    c =  read_config("config.txt");

    print_config(c); // Print the configuration variables funtion from functions.h


    printf("\n\n");

   // print config area end

   /***********************************************************************************************************************************************************/

    pid_t customer_pid = fork();

    if(customer_pid < 0){
        perror("Error: Unable to fork customer.c process.\n");
        exit(EXIT_FAILURE);
    }
    else if(customer_pid == 0){
        execlp("./customer", "customer", NULL);
    }


//good




    return 0;
}