
#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES


#define ITM_SMKEY 0x1333// for items shared memory
#define CUS_NUM_KEY 0x1235
#define MAX_LINE_LENGTH 100


char *STM_SEM = "stm_mutex196652"; // Semaphore for timer

struct shared_time_memory{
    time_t current_hours;
    time_t current_minutes;

};

typedef struct shared_time_memory* STM;


typedef struct {
    int totalCustomers;
} SharedData;



//-----------------------------------------------------------------------------------------------
// Shared Time Memory Key
#define STM_KEY 0x7341


#endif