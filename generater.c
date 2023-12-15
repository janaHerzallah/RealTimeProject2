#include "header.h"
#include "shared_memories.h"
#include "message_queues.h"
#include "cashier_header.h"
#include "customer_header.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void cashier_handler(int signo);
void terminate_simulation();

Config c;
int pid_cashier[3];
int terminated_cashiers = 0;  // Counter for terminated cashiers



int main(int argc, char *argv[ ])
{
    c = read_config("config.txt");


    struct sigaction sa;
    sa.sa_handler = cashier_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1 ||
        sigaction(SIGUSR2, &sa, NULL) == -1 ||
        sigaction(SIGRTMIN, &sa, NULL) == -1 ||
        sigaction(SIGQUIT, &sa, NULL) == -1) {  // Add SIGQUIT
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

        terminate_simulation();
        exit(EXIT_SUCCESS);

}


void cashier_handler(int signo) {
    if (signo == SIGQUIT) {
        printf("Immediate termination signal received. Shutting down all processes.\n");
        for (int i = 0; i < 3; i++) {
            kill(pid_cashier[i], SIGKILL);  // Immediately terminate all cashier processes
        }
        exit(EXIT_SUCCESS);  // Exit the main program
    }

    printf("Received termination signal for a cashier.\n");
    terminated_cashiers++;

    // Terminate specific cashier based on the signal
    if (signo == SIGUSR1) {
        kill(pid_cashier[0], SIGKILL);  // Terminate cashier 1
        printf("Cashier 1 terminated.\n");
    } else if (signo == SIGUSR2) {
        kill(pid_cashier[1], SIGKILL);  // Terminate cashier 2
        printf("Cashier 2 terminated.\n");
    } else if (signo == SIGRTMIN) {
        kill(pid_cashier[2], SIGKILL);  // Terminate cashier 3
        printf("Cashier 3 terminated.\n");
    }

    // If two cashiers have been terminated, end everything
    if (terminated_cashiers >= 2) {
        for (int i = 0; i < 3; i++) {
            kill(pid_cashier[i], SIGKILL);  // Terminate remaining cashiers
        }
        printf("Two cashiers terminated. Shutting down all processes.\n");
        exit(EXIT_SUCCESS);  // Exit the main program
    }
}

void terminate_simulation() {
    // Wait for child processes to finish
    for (int i = 0; i < 3; i++) {
        waitpid(pid_cashier[i], NULL, 0);  // Wait for each cashier process to complete
    }
    printf("Exiting the main program after all cashiers have finished.\n");
}