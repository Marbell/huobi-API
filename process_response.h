#ifndef _PROCESS_RESPONSE_H_
#define _PROCESS_RESPONSE_H_

#include "opration.h"

typedef struct{
double open;
double vol;
double last;
double buy;
double sell;
double high;
double low;

}hq_ticker_t;

typedef struct {
//{"time":"1451806498","ticker":{"open":2836.93,"vol":702938.5956,"last":2832.8,"buy":2832.8,"sell":2833.23,"high":2865.43,"low":2818} }
unsigned long int time;
hq_ticker_t ticker;
}
hq_response_t;

extern const char * status_string_meaning[9];

typedef enum
{
ORDER_UNSETTLED,
ORDER_PART_SETTLED,
ORDER_SETTLED,
ORDER_CANCELED,
ORDER_DIACARD,
ORDER_UNNORMAL,
ORDER_PART_SETTLED_PART_CANCELED,
ORDER_INQUNE,
ORDER_UNUSED
}order_status_t;


typedef struct
{
unsigned long int id;
double order_price;
double order_amount;
double processed_price;
double processed_amount;

order_status_t status;

}order_info_response_t;

typedef struct 
{
char * begin;

unsigned long int id;
char * p_id;
unsigned int type;
char * p_type;
double order_price;
char * p_order_price;
double order_amount;
char * p_order_amount;
double processed_amount;
char * p_processed_amount;
unsigned long int last_processed_time;
char * p_last_processed_time;
unsigned long int order_time;
char * p_order_time;
unsigned int status;
char * p_status;

char * end;
unsigned int len;
}one_order_line_t;

#define MAX_ORDER_INTEM_ONE_RESPONSE 10

typedef struct
{
one_order_line_t order_line_array[MAX_ORDER_INTEM_ONE_RESPONSE];

}get_new_deal_orders_response_t;

typedef struct 
{
char * begin;
unsigned long int time_index;
char * p_time;
double kp;
char * p_kp;
double zg;
char * p_zg;
double zd;
char * p_zd;
double sp;
char * p_sp;
double cjl;
char * p_cjl;
char * end;
unsigned int len;
}minutes_line_t;

typedef struct 
{
unsigned long int time;
char * p_time;

double dq_jg;
char * p_dq_jg;

double cjl;
char * p_cjl;

double cjje;
char *p_cjje;

}jx_info_line_t;


#define MAX_ITEM_IN_ONE_RESPONSE 300
#define PRE_MINUTES_FOR_MA 60

typedef struct
{
minutes_line_t fs_array[MAX_ITEM_IN_ONE_RESPONSE];
long double ma_sp;
long double ma_zg;
long double ma_zd;
long double ma_pj;
}update_ma_response_t;

typedef struct
{
jx_info_line_t fsjx_array[MAX_ITEM_IN_ONE_RESPONSE];
long double jx;
}update_jx_response_t;


typedef enum
{
RESULT_SUCCSS,
RESULT_FAIL,
}result_t;

typedef enum{
ERRO_CODE_0,  
ERRO_CODE_1,  
ERRO_CODE_2,  
ERRO_CODE_5 = 5,	
ERRO_CODE_6,	
ERRO_CODE_7,	
ERRO_CODE_8,	
ERRO_CODE_9,	
ERRO_CODE_10,
ERRO_CODE_11,
ERRO_CODE_26 =26,
ERRO_CODE_34 =34,
ERRO_CODE_38 =38,
ERRO_CODE_39,
ERRO_CODE_40,
ERRO_CODE_41,
ERRO_CODE_42,
ERRO_CODE_43,
ERRO_CODE_48 =44,
ERRO_CODE_50 =45,
ERRO_CODE_52 =46,
ERRO_CODE_53,
ERRO_CODE_54,
ERRO_CODE_55,
ERRO_CODE_56,
ERRO_CODE_58 =58,
ERRO_CODE_59,
ERRO_CODE_61 =61,
ERRO_CODE_63 =63,
ERRO_CODE_64,
ERRO_CODE_65,
ERRO_CODE_67 =67,
ERRO_CODE_68,
ERRO_CODE_69,
ERRO_CODE_70,
ERRO_CODE_71,
ERRO_CODE_72,
ERRO_CODE_73,
ERRO_CODE_74,
ERRO_CODE_76 =76,
ERRO_CODE_77,
ERRO_CODE_78,
ERRO_CODE_87,
ERRO_CODE_88,
ERRO_CODE_89,
ERRO_CODE_90,
ERRO_CODE_91,
ERRO_CODE_94,
ERRO_CODE_95,
ERRO_CODE_97,
ERRO_CODE_107,
ERRO_CODE_110,
ERRO_CODE_111,
ERRO_CODE_112,
ERRO_CODE_113,
ERRO_CODE_114,
ERRO_CODE_118,
ERRO_CODE_119,
ERRO_CODE_120,
ERRO_CODE_122,
ERRO_CODE_123,
ERRO_CODE_124,
ERRO_CODE_140,
ERRO_CODE_141,
ERRO_CODE_201,
ERRO_CODE_202,
ERRO_CODE_203,
ERRO_CODE_204,
ERRO_CODE_205,
ERRO_CODE_206,
ERRO_CODE_207,
ERRO_CODE_208,
ERRO_CODE_209,
ERRO_CODE_210,
ERRO_CODE_211,
ERRO_CODE_212,
ERRO_CODE_213,
ERRO_CODE_214,
ERRO_CODE_220,
ERRO_CODE_300,
ERRO_CODE_302,
ERRO_CODE_303,
ERRO_CODE_304,
ERRO_CODE_305,
ERRO_CODE_306,
ERRO_CODE_307,
ERRO_CODE_308,
ERRO_CODE_309,
ERRO_CODE_310,
ERRO_CODE_311,
ERRO_CODE_312,
ERRO_CODE_313,
ERRO_CODE_314,
ERRO_CODE_315,
ERRO_CODE_316,
ERRO_CODE_317,
ERRO_CODE_318,
ERRO_CODE_319,
ERRO_CODE_320,
             
ERRO_CODE_321,
ERRO_CODE_322,
ERRO_CODE_323,
ERRO_CODE_324,
ERRO_CODE_325,
ERRO_CODE_326,
ERRO_CODE_327,
ERRO_CODE_328,
ERRO_CODE_329,
ERRO_CODE_330,
ERRO_CODE_331,
ERRO_CODE_332,
ERRO_CODE_333,
ERRO_CODE_334,
ERRO_CODE_335,
ERRO_CODE_336,
ERRO_CODE_337,
ERRO_CODE_338,
ERRO_CODE_339,
             
ERRO_CODE_350,
ERRO_CODE_351,
ERRO_CODE_352,
ERRO_CODE_353,
ERRO_CODE_354,
ERRO_CODE_355,
ERRO_CODE_356,
ERRO_CODE_357,
ERRO_CODE_358,
             
             
ERRO_CODE_370,
ERRO_CODE_371,
ERRO_CODE_372,
ERRO_CODE_373
}erro_code_t;

extern const char *erro_code_string[];
/*BUY,
BUY_MARKET,
CANCEL_ORDER,
ACCOUNT_INFO,
NEW_DEAL_ORDERS,
ORDER_ID_BY_TRADE_ID,
GET_ORDERS,
ORDER_INFO,
SELL,
SELL_MARKET,
METHOD_NO_USE,*/


typedef struct
{
result_t result;
unsigned long int id;
int code;
}sell_buy_response_t;

typedef struct
{
double net_asset;
double available_cny_display;
double available_btc_display;

}account_info_response_t;


typedef struct
{
method_t method;

union {
account_info_response_t account_info_response;
sell_buy_response_t sell_buy_response;
order_info_response_t order_info_response;
get_new_deal_orders_response_t get_new_deal_orders_response;


}m_opration_response;

}do_opration_response_t;

extern int pre_process_response(char *response);
extern int process_hq_response(char * response, hq_response_t * p_hq);
extern int process_ma_response(char *response, update_ma_response_t * p_ma);
extern int process_jx_response(char *response, update_jx_response_t * p_jx);

extern int process_do_opration_response(char * response, do_opration_response_t* p_do_response);


#endif
