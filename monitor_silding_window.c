#include <stdio.h>
#include <stdlib.h>

#include "monitor_silding_window.h"
#include "misc.h"


window_result_t get_result_of_two_array(long double * in_array_source,long double * in_array_target,int newest_index)

{

int equal_count = 0;
int above_count = 0;
int little_count = 0;

bool rv = false;

bool rv_test = false;
rv_test = long_double_equal_zero(*(in_array_source+CONFIG_WINDOW_SIZE-1));
rv_test = long_double_equal_zero(*(in_array_target+CONFIG_WINDOW_SIZE-1));
if(rv_test)
return WINDOW_INITAL_STATE;


//we start form the index 0
for(int i = 0; i< CONFIG_WINDOW_SIZE-1;i++)
{
rv = long_double_equal(*(in_array_source+i),*(in_array_target+i));
if(true ==rv)
{
equal_count++;
continue;
}

rv = long_double_biger(*(in_array_source+i),*(in_array_target+i));
if(true ==rv)
{
above_count++;
continue;
}
rv = long_double_little(*(in_array_source+i),*(in_array_target+i));
if(true ==rv)
{
little_count++;
continue;
}
}
if(CONFIG_WINDOW_SIZE==equal_count+1)
{
return WINDOW_RESULT_EQUAL;
}

if(CONFIG_WINDOW_SIZE==little_count+1)
return ALL_BELOW_JX;

if(CONFIG_WINDOW_SIZE==above_count+1)
return ALL_ABOVE_JX;

//comes to here we need to find out wether up or down
//printf("equal count is %d\n",equal_count);
//printf("above count is %d\n",above_count);
//printf("little count is %d\n",little_count);



long double newest_value;
long double oldest_value;

newest_value = *(in_array_source+newest_index);
oldest_value = *(in_array_source+newest_index+1);

long double target_newest_value;
long double target_oldest_value;

target_newest_value = *(in_array_target+newest_index);
target_oldest_value = *(in_array_target+newest_index+1);


/*if(long_double_biger(newest_value,oldest_value))
return BELOW_TO_ABOVE;
if(long_double_little(newest_value,oldest_value))
return ABOVE_TO_BELOW;

else
{
if(little_count==above_count)
//printf("this can not be handled now,just update later.\n");
return WINDOW_RESULT_EQUAL;
if(little_count>=above_count)
return BELOW_TO_ABOVE;
else
return ABOVE_TO_BELOW;
*/

if((CONFIG_WINDOW_SIZE-1==little_count+1)&&(long_double_biger(newest_value,target_newest_value)))
return BELOW_TO_ABOVE;

if(long_double_little(newest_value,target_newest_value))
return ABOVE_TO_BELOW;


}




extern int process_window(window_t *p_in_put)
{
long double * p_source_buy = NULL;
long double * p_source_sell = NULL;

long double * p_target =NULL;
int current_index = 0;


p_source_buy = &p_in_put->hq_window.hq_array_buy[0];
p_source_sell = &p_in_put->hq_window.hq_array_sell[0];

p_target = &p_in_put->jx_window.jx_array[0];

current_index = p_in_put->current_index-1;

p_in_put->buy_result = get_result_of_two_array(p_source_buy,p_target,current_index);
p_in_put->sell_result = get_result_of_two_array(p_source_sell,p_target,current_index);



}


extern int update_window(long double buy,long double sell,long double jx,window_t *p_in_put)
{

if(CONFIG_WINDOW_SIZE==p_in_put->current_index)

p_in_put->current_index = 0;

int index = p_in_put->current_index;

p_in_put->hq_window.hq_array_buy[index] = buy;
p_in_put->hq_window.hq_array_sell[index] = sell;
p_in_put->jx_window.jx_array[index]= jx;


p_in_put->current_index++;


}



