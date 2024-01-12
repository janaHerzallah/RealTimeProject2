
#include "header.h"
#include "functions.h"
#include "customer_header.h"
#include "semphores.h"
#include "shared_memories.h"
#include "processing_times.h"


int check_finish();
void terminate_processes(pid_t *team_pids ,pid_t drawer, pid_t timer, pid_t *arr_customers_pid, int customerCapacity);

void create_all_shared_memories();
void create_TimerSHM();
void create_STM();
void create_CNM();


void clean_all_shared_memories();
void clean_TimerSHM();
void clean_STM();
void clean_CNM();



void create_all_semaphores(); // create all semaphores
void create_TimerSHM_semaphore();
void create_STM_semaphore();
void create_CNM_semaphore();




void clean_all_semaphores();// clean all semaphores
void clean_TimerSHM_semaphore();
void clean_STM_semaphore();
void clean_CNM_semaphore();


volatile sig_atomic_t terminate_flag = 0;

void simple_sig_handler(int sig) {
    terminate_flag = 1;
}

Config c;


int main( int argc, char *argv[] ){


    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = simple_sig_handler;
    sigaction(SIGINT, &sa, NULL);


    // print items area start
    read_items("supermarket_items.txt");

    // Print items
    printf("Items in the Supermarket:\n");
    printf("----------------------------\n");
    for (int i = 0; i < num_of_products; ++i) {
        printf("Product: %s, Storage amount : %d, Shelf Amount: %d\n", products[i].name, products[i].storageAmount , products[i].shelfAmount );
    }

    // print items area end

   /***********************************************************************************************************************************************************/


    c =  read_config("config.txt");

    print_config(c); // Print the configuration variables funtion from functions.h


    printf("\n\n");

   // print config area end

   /***********************************************************************************************************************************************************/


    // Fork a child process to run the main program loop

   /**************************************/
    create_all_shared_memories();
    create_all_semaphores();



    // creating teams starts **********************************************************************************************************************

    pid_t team_pids [MAXIMUM_NUM_OF_TEAMS]; // Dynamic array for child process IDs

    // Allocate memory for the pid array


    for (int i = 0; i < c.numberOfShelvingTeams; i++) {
        team_pids[i] = fork(); // Create a new process

        if (team_pids[i] < 0) {
            // Error handling
            perror("Failed to fork.");
            exit(EXIT_FAILURE);
        } else if (team_pids[i] == 0) {
            // Child process
            char index_str[10]; // Buffer for index string
            snprintf(index_str, sizeof(index_str), "%d", i+1); // Convert index to string

            execlp("./shelving_team", "shelving_team", index_str, (char *) NULL);

            // If execlp returns, there was an error
            perror("execlp");
            exit(EXIT_FAILURE);
        }

        sleep(c.numberOfShelvingTeams + 0.01 *c.numberOfEmployeesPerTeam - 0.4 *c.numberOfShelvingTeams);

    }

    // creating teams  end  **********************************************************************************************************************

    sleep(3);


    pid_t timer;

    /* Create the timer */
    if((timer = fork()) == -1){
        perror("The timer fork error\n");
        exit(-1);
    }

    // To let the timer leave the main code and go to timer.c
    if(!timer){
        execlp("./timer", "timer", (char *) NULL);

        // If the program not have enough memory then will raise error
        perror("exec Failure\n");
        exit(-1);
    }

    /* End Create the timer */


    // drawer area start
    pid_t drawer;

    if((drawer = fork()) == -1){
        perror("The drawer fork error\n");
        exit(-1);
    }

    // To let the drawer leave the main code and go to drawer.c
    if(!drawer){
        execlp("./drawer", "drawer", (char *) NULL);

        // If the program not have enough memory then will raise error
        perror("exec Failure\n");
        exit(-1);
    }




  // Set your desired customer capacity


    int j = 0;
    while (get_total_customers() <= c.customerCapacity) {

        // Generate a random number of customers for this batch within the min-max range
        int customersInBatch = c.minCustomerArrivalRate + rand() % (c.maxCustomerArrivalRate - c.minCustomerArrivalRate + 1);


        for (int i = 0; i < customersInBatch  ; i++) {

            if( get_total_customers() >= c.customerCapacity*0.8){ // this to ensure to read the right value of total customers
                sleep(1);
            }

            if(get_total_customers() >= c.customerCapacity){
                break;
            }

            if((get_total_customers() < c.customerCapacity)){

                arr_customers_pid[j] = fork();

                if (arr_customers_pid[j] < 0) {
                    perror("Error: Unable to fork customer process.\n");
                    exit(EXIT_FAILURE);
                } else if (arr_customers_pid[j] == 0) {
                    increment_total_customers(1);
                    execlp("./customer", "./customer", NULL);
                    perror("Error: Unable to execute customer process.\n");
                    exit(EXIT_FAILURE);
                }

            }

            usleep(customersInBatch*100000); // Sleep for 10ms to ensure different seed for each customer

            j++;


        }

        if (terminate_flag) {
            terminate_processes(team_pids ,drawer, timer, arr_customers_pid, j); // Ensure 'j' is the actual count of customer processes.
            terminate_flag = 0; // Reset the flag after handling termination
        }

        sleep(c.interval_seconds);

        if(!terminate_flag){
            printf("**sleeping interval is :%d\n",c.interval_seconds);
        }

    }

    printf("Entering  main loop\n");

    // safe wait until have a signal to terminate.
    while (1){

        if (terminate_flag) {
            printf(" Main Process End\n");
            terminate_processes(team_pids,drawer, timer, arr_customers_pid, j); // Ensure 'j' is the actual count of customer processes.
            terminate_flag = 0; // Reset the flag after handling termination
        }

    }



}

void terminate_processes(pid_t *team_pids  ,pid_t drawer, pid_t timer, pid_t *arr_customers_pid, int customerCapacity) {

    printf(" End of Program\n");


    if (drawer > 0) {
        printf("Killing drawer process\n");
        kill(drawer, SIGKILL);
    }

    if (timer > 0) {
        printf("Killing timer process\n");
        kill(timer, SIGKILL);
    }


    for(int i = 0; i < c.numberOfShelvingTeams; i++) {
        kill(team_pids[i], SIGTERM);
    }

    sleep(3);

    for(int i = 0; i < customerCapacity; i++) {

        if (arr_customers_pid[i] > 0) { // Validate PID
            kill(arr_customers_pid[i], SIGKILL); // Use SIGTERM for graceful termination
        }

    }


    sleep(3);
    clean_all_semaphores();
    clean_all_shared_memories();
    exit(0);

}




/** shared memory area start *************************************************************************************************************************/
void create_all_shared_memories(){

    create_TimerSHM();
    create_STM();
    create_CNM();


}

void create_TimerSHM(){

    // For create the STM
    int stm_id = shmget(STM_KEY, sizeof(struct shared_time_memory), IPC_CREAT | 0777);
    if (stm_id == -1){
        perror("STM Creation Error\n");
        exit(-1);
    }

    // For access the STM
    STM stm_mem =  shmat(stm_id, NULL, 0);


    if (stm_mem == (void *) -1) {
        perror("STM Attached Error\n");
        exit(-1);
    }

    // For initialize the first value of the time
    stm_mem->current_hours = c.startHour; // Start from 5AM
    stm_mem->current_minutes = c.startMinute;


    if (shmdt(stm_mem) == -1) {
        perror("STM Detached Error\n");
        exit(-1);
    }
}


void create_STM(){


    // Create the shared memory segment for items
    int shmid = shmget(ITM_SMKEY, num_of_products * sizeof(Product), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    Product *shared_items = (Product *)shmat(shmid, NULL, 0);
    if (shared_items == (Product *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }


// Initialize shared memory with item data and semaphores
    for (int i = 0; i < num_of_products; i++) {
        shared_items[i] = products[i]; // Assume products[] is already filled with product data

        // Create a unique name for each semaphore based on the product index or name
        snprintf(shared_items[i].semName, sizeof(shared_items[i].semName), "SEM_PRODUCT_%d", i);

        // Initialize the semaphore for each product
        sem_t *sem = sem_open(shared_items[i].semName, O_CREAT, 0644, 1);
        if (sem == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }
        sem_close(sem); // Close the semaphore as it's not needed in the parent process immediately
    }


    for (int i = 0; i < c.numberOfProducts; ++i) {
        pthread_mutex_init(&shared_items[i].lock, NULL);
    }


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

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

// clean all shared memories

void clean_all_shared_memories(){
    clean_TimerSHM(); // for timer
    clean_STM(); // for  items
    clean_CNM(); // for customers total num


}

void clean_TimerSHM(){

    int stm_id = shmget(STM_KEY, sizeof(struct shared_time_memory), IPC_CREAT | 0777);
    if (stm_id == -1){
        perror("STM Get Error\n");
        exit(-1);
    }


    if(shmctl(stm_id,IPC_RMID, 0) == -1){
        perror("STM Remove Error\n");
        exit(-1);
    }
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

    int shmid = shmget(ITM_SMKEY, num_of_products * sizeof(Product), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    Product *shared_items = (Product *)shmat(shmid, NULL, 0);
    if (shared_items == (Product *)-1) {
        perror("shmat");
        // If shmat fails, consider if you need to handle removing the shared memory segment created by shmget
        exit(EXIT_FAILURE);
    }


// Unlink semaphores when they are no longer needed
    for (int i = 0; i < num_of_products; i++) {
        if (sem_unlink(shared_items[i].semName) == -1) {
            perror("sem_unlink");
            // Consider continuing instead of exiting, so all semaphores get a chance to be unlinked
        }
    }


// Detach from the shared memory
    if (shmdt(shared_items) == -1) {
        perror("shmdt");
        // Handle error appropriately
    }

// Now remove the shared memory segment since it's no longer needed
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        // Handle error appropriately
    }


}



/** shared memory area end *************************************************************************************************************************/

/** semaphores area start *************************************************************************************************************************/

void create_all_semaphores(){

    create_TimerSHM_semaphore(); // for timer
    create_STM_semaphore(); // for  items
    create_CNM_semaphore(); // for customers total num


}

void create_TimerSHM_semaphore(){
    sem_t *stm_sem = sem_open(STM_SEM, O_CREAT, 0777, 0);
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




// clean area ==============================================================================================================
void clean_all_semaphores(){
    clean_TimerSHM_semaphore();
    clean_STM_semaphore();
    clean_CNM_semaphore();

}

void clean_TimerSHM_semaphore(){
    if(sem_unlink(STM_SEM) < 0){
        perror("STM Unlink Error\n");
        exit(-1);
    }
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

/** semaphores area end *************************************************************************************************************************/

/** message queues area start *************************************************************************************************************************/


/** message queues area end *************************************************************************************************************************/