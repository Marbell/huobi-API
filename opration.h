#ifndef _OPRATION_H_
#define _OPRATION_H_



typedef enum{
BUY,
BUY_MARKET,
CANCEL_ORDER,
ACCOUNT_INFO,
NEW_DEAL_ORDERS,
ORDER_ID_BY_TRADE_ID,
GET_ORDERS,
ORDER_INFO,
SELL,
SELL_MARKET,
METHOD_NO_USE,

}method_t;

typedef enum{
ACCESS_KEY,
SECRET_KEY,
}usr_key_t;

typedef enum{
COIN_NO_USE,
COIN_BIT,
COIN_LTB
}coin_t;

typedef enum{
MARKET_CNY,
MARKET_USD,
MARKET_NO_USE,
}market_t;


extern const char * method_string[10];
extern const char * key_string[2];
extern const char *market_string[2];



typedef struct{
method_t method_vlaue;
char *method;

char *access_key;
char *secret_key;

coin_t coin_type_value;
char *coin_type;

unsigned long time_stamp;
char *created;

unsigned char *sign_value;
char *sign;

market_t market_value;
char *market;

double price_value;
char * price;

double amount_value;
char *amount;

unsigned long int trade_id_value;
char *trade_id;

unsigned long int id_value;
char *id;

char * data;

}post_data_t;

extern int build_post_data(method_t in_m,post_data_t * p_post_data,double in_price,double in_amount,unsigned long int in_id);
extern void free_post_data_type(post_data_t * p_post);

#endif
