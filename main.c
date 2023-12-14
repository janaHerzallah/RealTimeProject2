
#include "header.h"
#include "constants.h"
#include "functions.h"
#include "customer_header"
#include "semphores.h"
#include "customer_header"
#include "cashier_header.h"







void create_all_shared_memories();
void create_STM();
void create_CNM();
void create_SC1SM(); // shared memory for cashier score
void create_SC2SM(); // shared memory for cashier2 score
void create_SC3SM(); // shared memory for cashier3 score
void create_SHTCSH(); // shared memory for total number of cashiers
void create_SCORA(); // shared memory for score attributes

void clean_all_shared_memories();
void clean_STM();
void clean_CNM();
void clean_SC1SM();
void clean_SC2SM();
void clean_SC3SM();
void clean_SHTCSH();
void clean_SCORA();



void create_all_semaphores(); // create all semaphores

void create_STM_semaphore();
void create_CNM_semaphore();
void create_cashier_semaphore();
void create_q1_semaphore();
void create_q2_semaphore();
void create_q3_semaphore();
void create_score1_semaphore();
void create_score2_semaphore();
void create_score3_semaphore();
void score_attributes_semaphore();



void clean_all_semaphores();// clean all semaphores

void clean_STM_semaphore();
void clean_CNM_semaphore();
void clean_cashier_semaphore();
void clean_q1_semaphore();
void clean_q2_semaphore();
void clean_q3_semaphore();
void clean_score1_semaphore();
void clean_score2_semaphore();
void clean_score3_semaphore();
void clean_score_attributes_semaphore();


void create_all_message_queues();
void create_Q1();
void create_Q2();
void create_Q3();

void clean_all_message_queues();
void clean_O1();
void clean_Q2();
void clean_Q3();


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



    c =  read_config("config.txt");

    print_config(c); // Print the configuration variables funtion from functions.h


    printf("\n\n");

   // print config area end

   /***********************************************************************************************************************************************************/
    create_all_shared_memories();
    create_all_semaphores();
    create_all_message_queues();


   // cashiers area start
    pid_t cashier1 = fork();
    if(cashier1 == 0){
        execlp("./cashier", "cashier", "1", NULL);
    }
    sleep(1);

    pid_t cashier2 = fork();
    if(cashier2 == 0){
        execlp("./cashier", "cashier", "2", NULL);
    }
    sleep(1);

    pid_t cashier3 = fork();
    if(cashier3 == 0){
        execlp("./cashier", "cashier", "3", NULL);
    }
    sleep(1);

    // cashiers area end

   // customers area start
   pid_t arr_customers_pid[9];

    for(int i = 0; i < 9; i++){
        arr_customers_pid[i] = fork();

        if(arr_customers_pid[i] < 0){
            perror("Error: Unable to fork customer process.\n");
            exit(EXIT_FAILURE);
        }
        else if(arr_customers_pid[i] == 0){
            execlp("./customer", "./customer", NULL);
        }

    }

    for(int i = 0; i <9; i++) {
        waitpid(arr_customers_pid[i], NULL, 0);
    }

    // customers area end


    // clean area start
    sleep(40);

    if (kill(cashier1, SIGTERM) == -1) {
        perror("Error sending SIGTERM");
        exit(EXIT_FAILURE);
    } if (kill(cashier2, SIGTERM) == -1) {
        perror("Error sending SIGTERM");
        exit(EXIT_FAILURE);
    } if (kill(cashier3, SIGTERM) == -1) {
        perror("Error sending SIGTERM");
        exit(EXIT_FAILURE);
    }


    clean_all_message_queues();
    clean_all_semaphores();
    clean_all_shared_memories();

    // clean area end


    return 0;
}

/** shared memory area start *************************************************************************************************************************/
void create_all_shared_memories(){
    create_STM();
    create_CNM();
    create_SC1SM();
    create_SC2SM();
    create_SC3SM();
    create_SHTCSH();
    create_SCORA();

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

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

void create_SC1SM(){
    int shmid = shmget(cashier1_score_key, sizeof(cashier_score1_shared_memory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    cashier_score1_shared_memory *shared_data = (cashier_score1_shared_memory *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    shared_data->score1 = 0;

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

void create_SC2SM(){
    int shmid = shmget(cashier2_score_key, sizeof(cashier_score2_shared_memory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    cashier_score2_shared_memory *shared_data = (cashier_score2_shared_memory *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    shared_data->score2 = 0;

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

void create_SC3SM(){
    int shmid = shmget(cashier3_score_key, sizeof(cashier_score3_shared_memory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    cashier_score3_shared_memory *shared_data = (cashier_score3_shared_memory *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    shared_data->score3 = 0;

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

void create_SHTCSH(){
    int shmid = shmget(total_num_cashiers_key, sizeof(total_cashiers), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    total_cashiers *shared_data = (total_cashiers *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    shared_data->totalCashiers = 0;

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

void create_SCORA(){
    int shmid = shmget(score_atrributes_key, sizeof(score_atrributes), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    score_atrributes *shared_data = (score_atrributes *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    shared_data->total_waiting_customers[0] = 0;
    shared_data->total_waiting_customers[1] = 0;
    shared_data->total_waiting_customers[2] = 0;

    shared_data->total_num_of_items[0] = 0;
    shared_data->total_num_of_items[1] = 0;
    shared_data->total_num_of_items[2] = 0;

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

// clean all shared memories

void clean_all_shared_memories(){
    clean_STM();
    clean_CNM();
    clean_SC1SM();
    clean_SC2SM();
    clean_SC3SM();
    clean_SHTCSH();
    clean_SCORA();

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

void clean_SC1SM(){
    int shmid = shmget(cashier1_score_key, sizeof(cashier_score1_shared_memory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
}

void clean_SC2SM(){
    int shmid = shmget(cashier2_score_key, sizeof(cashier_score2_shared_memory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
}

void clean_SC3SM(){
    int shmid = shmget(cashier3_score_key, sizeof(cashier_score3_shared_memory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
}

void clean_SHTCSH(){
    int shmid = shmget(total_num_cashiers_key, sizeof(total_cashiers), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
}

void clean_SCORA(){
    int shmid = shmget(score_atrributes_key, sizeof(score_atrributes), IPC_CREAT | 0666);
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

    create_STM_semaphore(); // for itmeas
    create_CNM_semaphore(); // for customers total num
    create_cashier_semaphore(); // for cashier total num
    create_q1_semaphore(); // for queue 1
    create_q2_semaphore(); // for queue 2
    create_q3_semaphore(); // for queue 3
    create_score1_semaphore(); // for cashier 1 score
    create_score2_semaphore(); // for cashier 2 score
    create_score3_semaphore(); // for cashier 3 score
    score_attributes_semaphore(); // for score attributes


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

void create_q1_semaphore(){


    sem_t *cnm_sem = sem_open(queu1_key, O_CREAT, 0777, 0);
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

void create_q2_semaphore(){
    sem_t *cnm_sem = sem_open(queu2_key, O_CREAT, 0777, 0);
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

void create_q3_semaphore(){

    sem_t *cnm_sem = sem_open(queu3_key, O_CREAT, 0777, 0);
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

void create_score1_semaphore(){


    sem_t *cnm_sem = sem_open(cashSem1_score_key, O_CREAT, 0777, 0);
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

void create_score2_semaphore(){
    sem_t *cnm_sem = sem_open(cashSem2_score_key, O_CREAT, 0777, 0);
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

void create_score3_semaphore(){

    sem_t *cnm_sem = sem_open(cashSem3_score_key, O_CREAT, 0777, 0);
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

void score_attributes_semaphore(){

    sem_t *cnm_sem = sem_open(score_atrributes_key_sem, O_CREAT, 0777, 0);
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
    clean_STM_semaphore();
    clean_CNM_semaphore();
    clean_cashier_semaphore();
    clean_q1_semaphore();
    clean_q2_semaphore();
    clean_q3_semaphore();
    clean_score1_semaphore();
    clean_score2_semaphore();
    clean_score3_semaphore();
    clean_score_attributes_semaphore();

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
        perror("total cashier Unlink Error\n");
        exit(-1);
    }
}

void clean_q1_semaphore(){
    if(sem_unlink(queu1_key ) < 0){
        perror("q1 Unlink Error\n");
        exit(-1);
    }
}

void clean_q2_semaphore(){
    if(sem_unlink(queu2_key ) < 0){
        perror("q2 Unlink Error\n");
        exit(-1);
    }
}

void clean_q3_semaphore(){
    if(sem_unlink(queu3_key ) < 0){
        perror("q3 Unlink Error\n");
        exit(-1);
    }
}

void clean_score1_semaphore(){
    if(sem_unlink(cashSem1_score_key ) < 0){
        perror("score1 Unlink Error\n");
        exit(-1);
    }
}

void clean_score2_semaphore(){
    if(sem_unlink(cashSem2_score_key ) < 0){
        perror("soc2 Unlink Error\n");
        exit(-1);
    }
}

void clean_score3_semaphore(){
    if(sem_unlink(cashSem3_score_key ) < 0){
        perror("scor3 Unlink Error\n");
        exit(-1);
    }
}

void clean_score_attributes_semaphore(){
    if(sem_unlink(score_atrributes_key_sem ) < 0){
        perror("score attributes Unlink Error\n");
        exit(-1);
    }
}


/** semaphores area end *************************************************************************************************************************/

/** message queues area start *************************************************************************************************************************/


void create_all_message_queues(){
    create_Q1();
    create_Q2();
    create_Q3();

}

void create_Q1(){
    if ( msgget(C_KEY, IPC_CREAT | 0777) == -1 ) {
        perror("OQ Create Error\n");
        exit(-1);
    }
}
void create_Q2(){
    if ( msgget(C_KEY2, IPC_CREAT | 0777) == -1 ) {
        perror("OQ Create Error\n");
        exit(-1);
    }
}

void create_Q3(){
    if ( msgget(C_KEY3, IPC_CREAT | 0777) == -1 ) {
        perror("OQ Create Error\n");
        exit(-1);
    }
}

void clean_all_message_queues(){
    clean_O1();
    clean_Q2();
    clean_Q3();

}

void clean_O1(){
    int queue;
    if ( (queue = msgget(C_KEY, IPC_CREAT | 0777)) == -1 ) {
        perror("OQ Get Error\n");
        exit(-1);
    }
    msgctl(queue, IPC_RMID, 0);/* remove */
}

void clean_Q2(){
    int queue;
    if ( (queue = msgget(C_KEY2, IPC_CREAT | 0777)) == -1 ) {
        perror("OQ Get Error\n");
        exit(-1);
    }
    msgctl(queue, IPC_RMID, 0);/* remove */
}

void clean_Q3(){
    int queue;
    if ( (queue = msgget(C_KEY3, IPC_CREAT | 0777)) == -1 ) {
        perror("OQ Get Error\n");
        exit(-1);
    }
    msgctl(queue, IPC_RMID, 0);/* remove */
}

/** message queues area end *************************************************************************************************************************/