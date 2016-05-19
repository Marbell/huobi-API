#ifndef _LOG_BUFFER_H
#define _LOG_BUFFER_H


#include<assert.h>

#define LOG_BUFFER_SIZE 40960

typedef struct
{
//define 200 bytes more if over flow
char buffer[LOG_BUFFER_SIZE+200];
unsigned int item;
unsigned int write_index;

}log_buffer_t;

typedef enum
{
LOG_BUY,
LOG_SELL
}log_buy_sell_t;

extern void destory_log_buffer(log_buffer_t *);
extern log_buffer_t *alloc_mem_for_buffer();

extern void write_log_buffer_to_file(log_buffer_t *p_in);




#endif
