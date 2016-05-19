#ifndef _BUILD_RAW_PARAM_H_
#define _BUILD_RAW_PARAM_H_

#define MIN_PRICE_SELL 2700
#define MAX_PRICE_BUY 3100

extern const char * str_access_key;
extern const char * str_created;
extern const char * str_coin_type;
extern const char * str_method;
extern const char * str_secret_key;
extern const char * id;
extern const char * trade_id;
extern const char * str_link_and;
extern const char * str_link_euqal;


extern void build_created(char * param,post_data_t* p_post);
extern void build_coin_type(char * param,post_data_t* p_post);
extern void build_secret_key(char * param,post_data_t* p_post);
extern void build_price(char * param,post_data_t* p_post);
extern void build_amount(char * param,post_data_t* p_post);

extern void build_id(char * param,post_data_t* p_post);
extern void build_trade_id(char * param,post_data_t* p_post);


int extern build_raw_param(char * param,post_data_t* p_post);


#endif
