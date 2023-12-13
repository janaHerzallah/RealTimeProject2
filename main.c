
#include "header.h"
#include "constants.h"
#include "functions.h"
#include "customer_header"
#include "semphores.h"






void create_all_shared_memories();
void create_STM();
void create_CNM();

void clean_all_shared_memories();
void clean_STM();
void clean_CNM();


void create_all_semaphores();
void create_STM_semaphore();
void create_CNM_semaphore();
void create_cashier_semaphore();


void clean_all_semaphores();
void clean_STM_semaphore();
void clean_CNM_semaphore();
void clean_cashier_semaphore();


Config c;
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

    c =  read_config("config.txt");

    print_config(c); // Print the configuration variables funtion from functions.h


    printf("\n\n");

   // print config area end

   /***********************************************************************************************************************************************************/

    create_all_semaphores();
    create_all_shared_memories();



  /*  pid_t arr_customers_pid[5];

    for(int i = 0; i < 5; i++){
        arr_customers_pid[i] = fork();
        if(arr_customers_pid[i] < 0){
            perror("Error: Unable to fork customer process.\n");
            exit(EXIT_FAILURE);
        }
        else if(arr_customers_pid[i] == 0){
            execlp("./customer", "./customer", NULL);
        }
    }

    for(int i = 0; i <5; i++) {
        waitpid(arr_customers_pid[i], NULL, 0);
    }



     // create cashier process

        pid_t cashiers[3];

        for(int i = 0; i <3; i++){
            cashiers[i] = fork();
            if(cashiers[i] < 0){
                perror("Error: Unable to fork customer process.\n");
                exit(EXIT_FAILURE);
            }
            else if(cashiers[i] == 0){
                execlp("./cashier", "./cashier", NULL);
            }
        }

        for(int i = 0; i < 3; i++) {
            waitpid(cashiers[i], NULL, 0);
        }*/

        pid_t customer = fork();

        if(customer < 0){
            perror("Error: Unable to fork customer process.\n");
            exit(EXIT_FAILURE);
        }
        else if(customer == 0){
            execlp("./customer", "./customer", NULL);
        }

        pid_t cashier = fork();

        if(cashier < 0){
            perror("Error: Unable to fork customer process.\n");
            exit(EXIT_FAILURE);
        }
        else if(cashier == 0){
            execlp("./cashier", "./cashier", NULL);
        }




       waitpid(customer, NULL, 0);
        waitpid(cashier, NULL, 0);

    clean_all_semaphores();
    clean_all_shared_memories();

    return 0;
}

/** shared memory area start *************************************************************************************************************************/
void create_all_shared_memories(){
    create_STM();
    create_CNM();

}


void create_STM(){


    // Create the shared memory segment for items
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

    // Initialize shared memory with item data
    memcpy(shared_items, items, num_items * sizeof(Item));

    // Detach from the shared memory
    if (shmdt(shared_items) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}


void create_CNM() {
    int shmid = shmget(CUS_NUM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    shared_data->totalCustomers = 0;
    shared_data->totalCashiers= 0;

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

// clean all shared memories

void clean_all_shared_memories(){
    clean_STM();
    clean_CNM();

}

void clean_CNM() {
    int shmid = shmget(CUS_NUM_KEY, sizeof(SharedData), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
}

void clean_STM(){
    // Remove the shared memory segment for items
    int shmid = shmget(ITM_SMKEY, num_items * sizeof(Item), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
}




/** shared memory area end *************************************************************************************************************************/

/** semaphores area start *************************************************************************************************************************/

void create_all_semaphores(){
    create_STM_semaphore();
    create_CNM_semaphore();
    create_cashier_semaphore();


}

void create_cashier_semaphore(){
    sem_t *cashier_sem = sem_open(total_cashiers_key, O_CREAT, 0777, 0);
    if(cashier_sem == SEM_FAILED){
        perror("Cashier Semaphore Open Error\n");
        exit(-1);
    }

    // When return -1 then wrong issue happened
    if (sem_post(cashier_sem) < 0){
        perror("Cashier Semaphore Release Error\n");
        exit(-1);
    }
    sem_close(cashier_sem);

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

void create_CNM_semaphore(){
    sem_t *cnm_sem = sem_open(total_customers_key, O_CREAT, 0777, 0);
    if(cnm_sem == SEM_FAILED){
        perror("CNM Semaphore Open Error\n");
        exit(-1);
    }

    // When return -1 then wrong issue happened
    if (sem_post(cnm_sem) < 0){
        perror("CNM Semaphore Release Error\n");
        exit(-1);
    }
    sem_close(cnm_sem);
}




void clean_all_semaphores(){
    clean_STM_semaphore();
    clean_CNM_semaphore();
    clean_cashier_semaphore();

}

void clean_STM_semaphore(){
    if(sem_unlink(Pick_key) < 0){
        perror("STM Unlink Error\n");
        exit(-1);
    }
}

void clean_CNM_semaphore(){
    if(sem_unlink(total_customers_key ) < 0){
        perror("CNM Unlink Error\n");
        exit(-1);
    }


}

void clean_cashier_semaphore(){
    if(sem_unlink(total_cashiers_key ) < 0){
        perror("Cashier Unlink Error\n");
        exit(-1);
    }
}
/** semaphores area end *************************************************************************************************************************/