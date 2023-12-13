


#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"


void read_items(const char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        num_items = 0;
        return;
    }

    num_items = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        num_items++;
    }

    rewind(file);

    for (int i = 0; i < num_items; ++i) {
        fgets(line, sizeof(line), file);
        sscanf(line, "%[^,],%d,%f", items[i].name, &items[i].quantity, &items[i].price);
    }

    fclose(file);
}

// items area end -------------------------------------------------------------------------------------------------------


// Function to read config from file
Config read_config(const char *filename) {
    Config config = {0};
    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        fscanf(file, "customers_per_interval=%d\n", &config.customers_per_interval);
        fscanf(file, "interval_seconds=%d\n", &config.interval_seconds);
        fscanf(file, "SHOPPING_TIME_Max=%d\n", &config.shopping_time_max);
        fscanf(file, "SCANNING_TIME=%d\n", &config.scanning_time);
        fscanf(file, "BEHAVIOR_DECAY_RATE=%d\n", &config.behavior_decay_rate);
        fscanf(file, "MAX_WAIT_TIME=%d\n", &config.max_wait_time);
        fscanf(file, "CASHIER_BEHAVIOR_THRESHOLD=%d\n", &config.cashier_behavior_threshold);
        fscanf(file, "CUSTOMER_PATIENCE_THRESHOLD=%d\n", &config.customer_patience_threshold);
        fscanf(file, "INCOME_THRESHOLD=%d\n", &config.income_threshold);
        fscanf(file, "num_cashiers=%d\n", &config.num_cashiers);

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
    printf("customers_per_interval=%d\n", config.customers_per_interval);
    printf("interval_seconds=%d\n", config.interval_seconds);
    printf("SHOPPING_TIME_Max=%d\n", config.shopping_time_max);
    printf("SCANNING_TIME=%d\n", config.scanning_time);
    printf("BEHAVIOR_DECAY_RATE=%d\n", config.behavior_decay_rate);
    printf("MAX_WAIT_TIME=%d\n", config.max_wait_time);
    printf("CASHIER_BEHAVIOR_THRESHOLD=%d\n", config.cashier_behavior_threshold);
    printf("CUSTOMER_PATIENCE_THRESHOLD=%d\n", config.customer_patience_threshold);
    printf("INCOME_THRESHOLD=%d\n", config.income_threshold);
    printf("num_cashiers=%d\n", config.num_cashiers);
}
// print config area end ------------------------------------------------------------------------------------------------

#endif