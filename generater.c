#include "header.h"
#include "shared_memories.h"
#include "message_queues.h"
#include "cashier_header.h"
#include "customer_header"

void terminate_handler(int signo);
void terminate_simulation();

Config c;
int pid_cashier[3];

int main(int argc, char *argv[ ])
{
    c =  read_config("config.txt");


    // signal area
    // Register the signal handler using sigaction for better signal handling
    struct sigaction sa;
    sa.sa_handler = terminate_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }


    // cashiers area start
    pid_cashier[0] = fork();
    if(pid_cashier[0] == 0){
        execlp("./cashier", "cashier", "1", NULL);
    }
    sleep(1);

    pid_cashier[1] = fork();
    if(pid_cashier[1] == 0){
        execlp("./cashier", "cashier", "2", NULL);
    }
    sleep(1);

    pid_cashier[2] = fork();
    if( pid_cashier[2] == 0 ){
        execlp("./cashier", "cashier", "3", NULL);
    }
    sleep(1);

    for(int i = 0 ; i < 3 ; i++){
        waitpid(pid_cashier[i], NULL, 0);
    }

    if (kill(pid_cashier[0], SIGTERM) == -1) {
         perror("Error sending SIGTERM");
         exit(EXIT_FAILURE);
     } if (kill(pid_cashier[1], SIGTERM) == -1) {
         perror("Error sending SIGTERM");
         exit(EXIT_FAILURE);
     } if (kill(pid_cashier[2], SIGTERM) == -1) {
         perror("Error sending SIGTERM");
         exit(EXIT_FAILURE);
     }






        terminate_simulation();
        exit(EXIT_SUCCESS);





}



void terminate_handler(int signo) {
    if (signo == SIGUSR1) {
        printf("Received signal: Cashier's behavior below threshold. Stopping the program.\n");
        exit_program = 1;
        totalSalesAboveThreshold = 1;
        // Send SIGTERM to the process group (including children)

        kill(pid_cashier[0], SIGKILL);
        kill(pid_cashier[1], SIGKILL);
        kill(pid_cashier[2], SIGKILL);
        kill(0, SIGKILL);


    }
}

void terminate_simulation() {
    while (!exit_program) {
        // Optional: Add a sleep to avoid tight-looping and reduce CPU usage
        sleep(1);

        // Check if any cashier's behavior fell below the threshold
        if (totalSalesAboveThreshold) {
            printf("Exiting the main program because a cashier's behavior is below the threshold.\n");
            fflush(stdout);

            _exit(EXIT_SUCCESS);
        }
    }
}
