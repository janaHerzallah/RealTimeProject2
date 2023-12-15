#include "header.h"
#include "constants.h"
#include "customer_header"
#include "cashier_header.h"

void fill_data();
void print_customer_data(const struct Customer *customer);
void fill_cart( struct Customer *customer);
struct Customer customer_info;
extern Config c;  // Declare or include the Config structure
extern Item items[MAX_LINE_LENGTH];
extern int num_items;
float calculate_total_price(const struct Customer *customer );
int find_max_score_index(float score1, float score2, float score3);

void send_customer_message(struct Customer *customer);
void send_customer_message2(struct Customer *customer);
void send_customer_message3(struct Customer *customer);

int main(int argc, char** argv ) {


    read_items("supermarket_items.txt");
    c = read_config("config.txt");



    fill_data();  // Populate customer_info with data

    printf("\n \n");
    print_customer_data(&customer_info);
    printf("\n \n");



    float  socre1 = get_score1_cashier1();
    float  socre2 = get_score2_cashier2();
    float  socre3 = get_score3_cashier3();

    printf("***********************************\n");
    printf("***********************************\n");
    printf("iam customer %d i see this in shaerd memory \n",customer_info.id);
    printf("score1 is %f \n",socre1);
    printf("score2 is %f \n",socre2);
    printf("score3 is %f \n",socre3);
    printf("***********************************\n");
    printf("***********************************\n");


   int  what_cashier = find_max_score_index(socre1,socre2,socre3);

  //  int what_cashier =  rand() % 3 + 1;      // Returns a pseudo-random integer between 1 and 3.

    switch (what_cashier) {
        case 1:
            send_customer_message(&customer_info);
            change_score1_cashier1(socre1-1);
            break;
        case 2:
            send_customer_message2(&customer_info);
            change_score2_cashier2(socre2-1);
            break;
        case 3:
            send_customer_message3(&customer_info);
            change_score3_cashier3(socre3-1);
            break;
        default:
            printf("error in finding the max score\n");

    }




    exit(0);




}




void fill_data(){


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

    customer_info.total_price = calculate_total_price(&customer_info);



}


void print_customer_data(const struct Customer *customer) {

    printf("customer with id %d info is : \n \n",customer->id);

    printf("Customer ID: %d\n", customer->id);
    printf("Number of Items: %d\n", customer->num_items);
    printf("Shopping Time: %d\n", customer->shopping_time);
    printf("Patience Degree: \n ");

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

    float total = 0.0;
    printf("Items in Cart: ");
    for (int i = 0; i < customer->num_items; ++i) {
        printf(" %s ", items[customer->cart[i]].name);
        total += items[customer->cart[i]].price;


    }

    printf("\nTotal Price: $%.2f\n", total);


}


float calculate_total_price(const struct Customer *customer ) {
    float total = 0.0;

    for (int i = 0; i < customer->num_items; ++i) {
        total += items[customer->cart[i]].price;
    }
    return total;
}

int find_max_score_index(float score1, float score2, float score3) {
    if (score1 >= score2 && score1 >= score3) {
        return 1;
    } else if (score2 >= score1 && score2 >= score3) {
        return 2;
    } else if(score3 >= score1 && score3 >= score2) {
        return 3;
    } else
        return 3;


}


void send_customer_message(struct Customer *customer) {


    write_score_att(1,1,1);
    write_score_att(customer->num_items,1,2);


    int queue = get_queue(C_KEY);

    customerQueue customer_data;
    customer_data.customer_id = customer->id;
    customer_data.item_count = customer->num_items;
    customer_data.total_price = customer->total_price;
    customer_data.pid= getpid();


    // mutex code

    customers_total_mutex = get_semaphore(queu1_key);

    lock_sem(customers_total_mutex);

    /* increase_cashier_queue_size(cashier_id);
     int size = get_cashier_queue_size(cashier_id);
     */


    sleep(get_total_cashiers(1));

    //  printf("Queue size for %d is %d after assigning customer\n", cashier_id, size);

    //fflush(stdout); // Flush the output buffer to ensure immediate display

    // Send the message
    if (msgsnd(queue, &customer_data, sizeof(customer_data), 0) == -1) {
        perror("customer_data");
        exit(EXIT_FAILURE);
    }




    unlock_sem(customers_total_mutex);
    close_semaphore(customers_total_mutex);

    //end mutex code



}

void send_customer_message2(struct Customer *customer) {

    write_score_att(1,2,1);
    write_score_att(customer->num_items,2,2);

    int queue = get_queue(C_KEY2);

    customerQueue customer_data;
    customer_data.customer_id = customer->id;
    customer_data.item_count = customer->num_items;
    customer_data.total_price = customer->total_price;


    // mutex code

    customers_total_mutex = get_semaphore(queu2_key);

    lock_sem(customers_total_mutex);

    /* increase_cashier_queue_size(cashier_id);
     int size = get_cashier_queue_size(cashier_id);
     */


    sleep(get_total_cashiers(2));

    //  printf("Queue size for %d is %d after assigning customer\n", cashier_id, size);

    //fflush(stdout); // Flush the output buffer to ensure immediate display

    // Send the message
    if (msgsnd(queue, &customer_data, sizeof(customer_data), 0) == -1) {
        perror("customer_data");
        exit(EXIT_FAILURE);
    }



    unlock_sem(customers_total_mutex);
    close_semaphore(customers_total_mutex);

    //end mutex code



}

void send_customer_message3(struct Customer *customer) {

    write_score_att(1,3,1);
    write_score_att(customer->num_items,3,2);


    int queue = get_queue(C_KEY3);

    customerQueue customer_data;
    customer_data.customer_id = customer->id;
    customer_data.item_count = customer->num_items;
    customer_data.total_price = customer->total_price;


    // mutex code

    customers_total_mutex = get_semaphore(queu3_key);

    lock_sem(customers_total_mutex);


    sleep(get_total_cashiers(3));


    // Send the message
    if (msgsnd(queue, &customer_data, sizeof(customer_data), 0) == -1) {
        perror("customer_data");
        exit(EXIT_FAILURE);
    }





    unlock_sem(customers_total_mutex);
    close_semaphore(customers_total_mutex);

    //end mutex code



}