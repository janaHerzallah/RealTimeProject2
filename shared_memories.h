
#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES


//-----------------------------------------------------------------------------------------------
//iteams shared memory


#define ITM_SMKEY 0x1333
#define MAX_LINE_LENGTH 100

struct items_shared_memory{

    char name[MAX_LINE_LENGTH];
    int quantity;
    float price;

};

typedef struct items_shared_memory* ISM;

// Semaphore for STM
const char *ITEM_SEM = "stm_mutex196444";

//-----------------------------------------------------------------------------------------------
// Shared Time Memory Key
#define STM_KEY 0x7341


#endif