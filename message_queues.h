#ifndef QUEUES
#define QUEUES

#include "header.h"
#define MAX_ITEMS 10
#define C_KEY 0x1234
#define C_KEY2 0x1235
#define C_KEY3 0x1236
#include "functions.h"

// Modified structure to include customer ID and multiple items
struct queue {
    int customer_id; // Unique identifier for each customer
    int item_count; // Number of items in the message
    Item items[MAX_ITEMS]; // Array of items
    char customer_name[50];
    float total_price;
    pid_t pid;
};

typedef struct queue customerQueue;




/* For access the Queue */
int get_queue(int key){


    int queue;

    if ( (queue = msgget(key, IPC_CREAT | 0777)) == -1 ) {
        perror("Queue Access Error\n");
        exit(-1);
    }

    return queue;
}


int check_queue_empty(int queue){

    if(msgrcv(queue, NULL, 0, 0, IPC_NOWAIT) == -1){
        if(errno == E2BIG)
            return 0; // There is data

    }
    return 1; // Empty queue
}


#endif
