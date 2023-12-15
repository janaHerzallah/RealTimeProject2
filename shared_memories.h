
#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES


//-----------------------------------------------------------------------------------------------
//iteams shared memory


#define ITM_SMKEY 0x1333// for items shared memory
#define CUS_NUM_KEY 0x1235

#define cashier1_score_key 0x1236
#define cashier2_score_key 0x1237
#define cashier3_score_key 0x1238

#define total_num_cashiers_key 0x1239

#define score_atrributes_key 0x1240

#define total_sales_key 0x1241

#define happiest_cashier_key 0x1242

typedef struct {

    int happy_cashier_1;
    int happy_cashier_2;
    int happy_cashier_3;

} cashier_happiness_Shared_Memory;

typedef struct {

    float total_sales1;
    float total_sales2;
    float total_sales3

} total_sales;

typedef struct {
    int totalCashiers;
    int scanning_time1;
    int scanning_time2;
    int scanning_time3;


} total_cashiers;

typedef struct{

    float score1;

} cashier_score1_shared_memory;

typedef struct{

    float score2;
} cashier_score2_shared_memory;

typedef struct{

    float score3;
} cashier_score3_shared_memory;

typedef struct{

    int total_waiting_customers[3];
    int total_num_of_items[3];

} score_atrributes;


typedef struct cashier_score1_shared_memory * SC1SM;
typedef struct cashier_score2_shared_memory * SC2SM;
typedef struct cashier_score3_shared_memory * SC3SM;
typedef struct total_cashiers * TC;
typedef struct score_atrributes * SA;
typedef struct total_sales * TS;

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