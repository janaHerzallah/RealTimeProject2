
#include "header.h"
#include "constants.h"
#include "functions.h"
#include "customer_header"
#include "semphores.h"


void create_all_semaphores();
void create_STM_semaphore();


void clean_all_semaphores();
void clean_STM_semaphore();

Config c;
int main(int argc, char** argv){


    create_all_semaphores();


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


    wait(NULL);

    clean_all_semaphores();


    return 0;
}


void create_all_semaphores(){
    create_STM_semaphore();

}

void create_STM_semaphore(){
    sem_t *stm_sem = sem_open(Pick_key, O_CREAT, 0777, 0);
    if(stm_sem == SEM_FAILED){
        perror("STM Semaphore Open Error\n");
        exit(-1);
    }

    // When return -1 then wrong issue happened
    if (sem_post(stm_sem) < 0){
        perror("STM Semaphore Release Error\n");
        exit(-1);
    }
    sem_close(stm_sem);
}

void clean_all_semaphores(){
    clean_STM_semaphore();

}

void clean_STM_semaphore(){
    if(sem_unlink(Pick_key) < 0){
        perror("STM Unlink Error\n");
        exit(-1);
    }
}