
#ifndef __CONSTANTS__
#define __CONSTANTS__
#define MAX_LINE_LENGTH 100


// common area ------------------------------------------------------------------------------------------------------------

// Structure for items in the supermarket
typedef struct {
    char name[MAX_LINE_LENGTH];
    int quantity;
    float price;
} Item;

// Config structure
typedef struct {
    int customers_per_interval;
    int interval_seconds;
    int shopping_time_max;
    int scanning_time;
    int behavior_decay_rate;
    int max_wait_time;
    int cashier_behavior_threshold;
    int customer_patience_threshold;
    int income_threshold;
    int num_cashiers;
} Config;


// Function to read config from file
Config read_config(const char *filename);


// common area end --------------------------------------------------------------------------------------------------------

 int num_items;
 Item items[MAX_LINE_LENGTH];


 // customer area ---------------------------------------------------------------------------------------------------------

int elapseTime = 0;

sem_t *mutex;


// customer area end -----------------------------------------------------------------------------------------------------


#endif
