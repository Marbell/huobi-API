#ifndef _GET_DATA_H_
#define _GET_DATA_H_

#include "opration.h"

typedef enum
{
ACTION_NO_USE,
QUERY_HQ,
UPDATE_MA,
UPDATE_JX,
DO_OPRATION
}
action_t;

typedef struct
{
method_t method;
double price;
double amount;
unsigned long int id;

}in_param_do_opration_t;


extern int sent_revc_data(action_t in_action,void * in_ptr,void *out_ptr);

#endif