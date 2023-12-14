
#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES


//-----------------------------------------------------------------------------------------------
//iteams shared memory


#define ITM_SMKEY 0x1333// for items shared memory
#define CUS_NUM_KEY 0x1235

#define cashier1_score_key 0x1236
#define cashier2_score_key 0x1237
#define cashier3_score_key 0x1238

typedef struct{

    float score1;
} cashier_score1_shared_memory;

typedef struct{

    float score2;
} cashier_score2_shared_memory;

typedef struct{

    float score3;
} cashier_score3_shared_memory;

typedef struct cashier_score1_shared_memory * SC1SM;
typedef struct cashier_score2_shared_memory * SC2SM;
typedef struct cashier_score3_shared_memory * SC3SM;

#define MAX_LINE_LENGTH 100

struct items_shared_memory{

    char name[MAX_LINE_LENGTH];
    int quantity;
    float price;

};

typedef struct {
    int totalCustomers;
    int totalCashiers;
} SharedData;

typedef struct items_shared_memory* ISM;





//-----------------------------------------------------------------------------------------------
// Shared Time Memory Key
#define STM_KEY 0x7341


#endif