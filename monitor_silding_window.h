#ifndef _MONITOR_SLIDING_WINDOW_
#define _MONITOR_SLIDING_WINDOW_

#include <stdio.h> 
#include <stdlib.h> 

#include "process_response.h"


#define CONFIG_WINDOW_SIZE 60

typedef enum{

WINDOW_RESULT_EQUAL,
ALL_ABOVE_JX,
ALL_BELOW_JX,
ABOVE_TO_BELOW,
BELOW_TO_ABOVE,
WINDOW_INITAL_STATE
}window_result_t;

typedef struct{

long double jx_array[CONFIG_WINDOW_SIZE];

}jx_window_t;

typedef struct{

long double hq_array_buy[CONFIG_WINDOW_SIZE];
long double hq_array_sell[CONFIG_WINDOW_SIZE];

}hq_window_t;

typedef enum 
{
HQ_NO_USE,
HQ_VOLATILE,
HQ_SMOTH_DOWN_UP
}hq_type_t;

typedef struct{

jx_window_t  jx_window;
hq_window_t  hq_window;
int current_index;
window_result_t buy_result;
window_result_t sell_result;

}window_t;

extern int process_window(window_t *p_in_put);
extern int update_window(long double buy,long double sell,long double jx,window_t *p_in_put);



#endif

