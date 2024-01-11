


#ifndef __FUNCTIONS__
#define __FUNCTIONS__


#include "header.h"
#include "constants.h"



// Config structure
typedef struct {
    int customerCapacity;
    int interval_seconds;
    int shopping_time_max;
    int numberOfProducts;
    int amountPerProductOnShelves;
    int amountPerProductInStorage;
    int numberOfShelvingTeams;
    int numberOfEmployeesPerTeam;
    int productRestockThreshold;
    int minCustomerArrivalRate;
    int maxCustomerArrivalRate;
    int maxItemsPerCustomer;
    int maxQuantityPerItem;
    int startHour;
    int startMinute;
    int endHour;
    int endMinute;
} Config;

// Function to read config from file
Config read_config(const char *filename);
pid_t arr_customers_pid[MAXIMUM_NUM_OF_CUSTOMERS];

// Structure for items in the supermarket
typedef struct {
    char name[MAXIMUM_NUM_OF_PRODUCTS];
    int  storageAmount;
    int shelfAmount;
    int restockThreshold;
    pthread_mutex_t lock; // Mutex for synchronizing access to the product
    char semName[30];     // Name of the semaphore for each product


} Product;


int num_of_products;
Product products[MAXIMUM_NUM_OF_PRODUCTS];
Config c_values;


void read_items(const char *filename) {

    c_values = read_config("config.txt");
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        num_of_products = 0;
        return;
    }

    num_of_products = c_values.numberOfProducts;
    char line[MAXIMUM_NUM_OF_PRODUCTS];

    rewind(file);

    for (int i = 0; i < c_values.numberOfProducts; ++i) {
        fgets(line, sizeof(line), file);

        // Initialize storageAmount and shelfAmount with default values
        int storageAmountFromFile, shelfAmountFromFile;

        // Read from file first, regardless of the condition
        sscanf(line, "%[^,],%d,%d", products[i].name, &storageAmountFromFile, &shelfAmountFromFile);

        // Determine storage amount
        if (c_values.amountPerProductInStorage == 0) {
            products[i].storageAmount = storageAmountFromFile;
        } else {
            products[i].storageAmount = c_values.amountPerProductInStorage;
        }

        // Determine shelf amount
        if (c_values.amountPerProductOnShelves == 0) {
            products[i].shelfAmount = shelfAmountFromFile;
        } else {
            products[i].shelfAmount = c_values.amountPerProductOnShelves;
        }
    }

    fclose(file);
}

// items area end -------------------------------------------------------------------------------------------------------


// Function to read config from file
Config read_config(const char *filename) {
    Config config = {0};
    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        fscanf(file, "customerCapacity=%d\n", &config.customerCapacity);
        fscanf(file, "interval_seconds=%d\n", &config.interval_seconds);
        fscanf(file, "SHOPPING_TIME_Max=%d\n", &config.shopping_time_max);
        fscanf(file, "numberOfProducts=%d\n", &config.numberOfProducts);
        fscanf(file, "amountPerProductOnShelves=%d\n", &config.amountPerProductOnShelves);
        fscanf(file, "amountPerProductInStorage=%d\n", &config.amountPerProductInStorage);
        fscanf(file, "numberOfShelvingTeams=%d\n", &config.numberOfShelvingTeams);
        fscanf(file, "numberOfEmployeesPerTeam=%d\n", &config.numberOfEmployeesPerTeam);
        fscanf(file, "productRestockThreshold=%d\n", &config.productRestockThreshold);
        fscanf(file, "minCustomerArrivalRate=%d\n", &config.minCustomerArrivalRate);
        fscanf(file, "maxCustomerArrivalRate=%d\n", &config.maxCustomerArrivalRate);
        fscanf(file, "maxItemsPerCustomer=%d\n", &config.maxItemsPerCustomer);
        fscanf(file, "maxQuantityPerItem=%d\n", &config.maxQuantityPerItem);
        fscanf(file, "startHour=%d\n", &config.startHour);
        fscanf(file, "startMinute=%d\n", &config.startMinute);
        fscanf(file, "endHour=%d\n", &config.endHour);
        fscanf(file, "endMinute=%d\n", &config.endMinute);





        fclose(file);
    } else {
        fprintf(stderr, "Error: Unable to open the configuration file.\n");
        exit(EXIT_FAILURE);
    }

    return config;
}


// Print configuration variables

void print_config(Config config) {
    printf("\nConfiguration Variables:\n");
    printf("customerCapacity=%d\n", config.customerCapacity);
    printf("interval_seconds=%d\n", config.interval_seconds);
    printf("SHOPPING_TIME_Max=%d\n", config.shopping_time_max);
    printf("numberOfProducts=%d\n", config.numberOfProducts);
    printf("amountPerProductOnShelves=%d\n", config.amountPerProductOnShelves);
    printf("amountPerProductInStorage=%d\n", config.amountPerProductInStorage);
    printf("numberOfShelvingTeams=%d\n", config.numberOfShelvingTeams);
    printf("numberOfEmployeesPerTeam=%d\n", config.numberOfEmployeesPerTeam);
    printf("productRestockThreshold=%d\n", config.productRestockThreshold);
    printf("minCustomerArrivalRate=%d\n", config.minCustomerArrivalRate);
    printf("maxCustomerArrivalRate=%d\n", config.maxCustomerArrivalRate);
    printf("maxItemsPerCustomer=%d\n", config.maxItemsPerCustomer);
    printf("maxQuantityPerItem=%d\n", config.maxQuantityPerItem);
    printf("startHour=%d\n", config.startHour);
    printf("startMinute=%d\n", config.startMinute);
    printf("endHour=%d\n", config.endHour);
    printf("endMinute=%d\n", config.endMinute);
}
// print config area end ------------------------------------------------------------------------------------------------



#endif