#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>


#include <time.h>/*for time_stamp*/
#include "sign_md5.h"/*for md5*/

#include "opration.h"
#include "build_raw_param.h"
#include "fill_post_data.h"


const char * method_string[10] = 
{
"buy",
"buy_market",
"cancel_order",
"get_account_info",
"get_new_deal_orders",
"get_order_id_by_trade_id",
"get_orders",
"order_info",
"sell",
"sell_market",
};

const char * key_string[2] =
{
"xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx",
"xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx",
};///apply on huobi.com

const char * market_string[2] =
{
"cny",
"usd",
};


void fill_method(method_t in_method,post_data_t * p_post)
{
p_post->method_vlaue= in_method;
p_post->method = (char*)method_string[in_method];
}


void fill_keys(post_data_t * p_post)
{
p_post->access_key=(char*)key_string[ACCESS_KEY];
p_post->secret_key=(char*) key_string[SECRET_KEY];
}


void fill_market(post_data_t * p_post)
{
    if(MARKET_CNY==p_post->market_value)
p_post->market=(char*)market_string[MARKET_CNY];
    if(MARKET_USD==p_post->market_value)
p_post->secret_key=(char*) market_string[MARKET_USD];
}

int fill_time_created(post_data_t * p_post)
{
    /* fill in the parameters */
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
	//printf("\007The current time is: %s\n",asctime(time_info));
	//printf("\007The current time is: %d\n",raw_time);
	char * time_creatd = NULL;
	//created has only 10 char
	time_creatd = (char *)malloc(sizeof(char)*10+1);
	if(NULL==time_creatd)
	{
	printf("malloc erro in fill_time_created.\n");
	return 0;
	}
	int tmp = 0;
	tmp = sprintf(time_creatd,"%li",raw_time);
	time_creatd[10]='\0';
	//printf("sprintf %d bite and result is %s.\n",tmp,time_creatd);

	p_post->created = time_creatd;
	p_post->time_stamp = raw_time;

	return 1;
}

int fill_coin_type(post_data_t * p_post)
{

if(COIN_NO_USE==p_post->coin_type_value)
{
printf("erro,input coin type is NO_USE.\n");
return 0;
}


char * coin_type = NULL;
coin_type = (char *)malloc(2*sizeof(char));

if(COIN_BIT==p_post->coin_type_value)
{coin_type[0] = '1';coin_type[1] = '\0';}
if(COIN_LTB==p_post->coin_type_value)
{coin_type[0] = '0';coin_type[1] = '\0';}

p_post->coin_type = coin_type;

return 1;
}

int fill_price(double in_price,post_data_t *p_post)
{
p_post->price_value = in_price;

char tmp[100];
memset(tmp,0,100);
sprintf(tmp,"%0.2f",p_post->price_value);
char * price = NULL;
unsigned int len = 0;
len = strlen(tmp);
tmp[len]='\0';
price =(char *) malloc((sizeof(char))*len+1);
if(NULL==price)
{
printf("erro, malloc memory for price fail in fill price.\n");
return 0;
}
strcpy(price,tmp);
p_post->price = price;
//printf("The price is %s.\n",p_post->price);

}

int fill_amount(double in_amount,post_data_t *p_post)
{
p_post->amount_value = in_amount;

char tmp[100];
memset(tmp,0,100);
sprintf(tmp,"%0.4f",p_post->amount_value);
char * amount = NULL;
unsigned int len = 0;
len = strlen(tmp);
tmp[len]='\0';
amount =(char *) malloc((sizeof(char))*len+1);
if(NULL==amount)
{
printf("erro, malloc memory for amount fail in fill amount.\n");
return 0;
}
strcpy(amount,tmp);
p_post->amount = amount;
//printf("The amount is %s.\n",p_post->amount);

}

int fill_id(unsigned long int in_id,post_data_t *p_post)
{
p_post->id_value = in_id;

char tmp[100];
memset(tmp,0,100);
sprintf(tmp,"%lu",p_post->id_value);
char * id = NULL;
unsigned int len = 0;
len = strlen(tmp);
tmp[len]='\0';
id =(char *) malloc(len*(sizeof(char))+1);
if(NULL==id)
{
printf("erro, malloc memory for price fail in fill id.\n");
return 0;
}
strcpy(id,tmp);
p_post->id = id;
//printf("The id is %s.\n",p_post->id);

}

int fill_trade_id(unsigned long int in_trade_id,post_data_t *p_post)
{
p_post->trade_id_value = in_trade_id;

char tmp[100];
memset(tmp,0,100);
sprintf(tmp,"%lu",p_post->trade_id_value);
char * trade_id = NULL;
unsigned int len = 0;
len = strlen(tmp);
tmp[len]='\0';
trade_id =(char *) malloc(len*(sizeof(char))+1);
if(NULL==trade_id)
{
printf("erro, malloc memory for price fail in fill trade_id.\n");
return 0;
}
strcpy(trade_id,tmp);
p_post->trade_id = trade_id;
printf("The trade_id is %s.\n",p_post->trade_id);

}


int build_md5_param(post_data_t *p_post)
{
unsigned char * sign_value = NULL;

sign_value = (unsigned char *)malloc(sizeof(unsigned char)*16);
if(NULL==sign_value)
{
printf("malloc erro in build_md5_param for sign.\n");
return 0;
}


char param[512];
memset(param,0,512);
//here we should check p_post&it's member not be null
if(NULL==p_post||NULL==p_post->method||NULL==p_post->created||NULL==p_post->access_key||NULL==p_post->secret_key)
{
printf("p_post erro, no NULL member should contain.\n");
return 0;
}

build_raw_param(param,p_post);

sign_md5((unsigned char *)param,sign_value);

p_post->sign_value = sign_value;

unsigned int sign_len = 0;
//xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx 32byte
sign_len = (sizeof(char))*32;
char *data = NULL;
data = (char *)malloc((sizeof(char))*(sign_len)+1);
if(NULL==data)
{
printf("malloc erro in build_md5_param for data.\n");
return 0;
}
char * tmp =data;
for(int i=0;i<16;i++)
{
	sprintf(tmp,"%02x",p_post->sign_value[i]);
	tmp = tmp+2;
}
data[sign_len] = '\0';
p_post->sign = data;

//printf("data is %s.\n",data);

return 1;

}


void fill_sign(post_data_t * p_post)
{
unsigned char *sign = NULL;
build_md5_param(p_post);

}




extern int build_post_data(method_t in_m,post_data_t * p_post_data,double in_price,double in_amount,unsigned long int in_id)
{
method_t in = in_m;
post_data_t * p_post = p_post_data;
fill_method(in,p_post);
if((BUY==p_post->method_vlaue)||(BUY_MARKET==p_post->method_vlaue)||(SELL==p_post->method_vlaue)||(SELL_MARKET==p_post->method_vlaue))
{
if((SELL==p_post->method_vlaue)||(SELL_MARKET==p_post->method_vlaue))
{
if((MIN_PRICE_SELL>=in_price)||(0==in_amount))
{
printf("erro when fill price or amount, price sell should be not little than MIN_PRICE_SELL price buy should little than MAX_PRICE_BUY, amount bigger than 0 .\n");
return 0;
}
}
if((BUY==p_post->method_vlaue)||(BUY_MARKET==p_post->method_vlaue))
{
if((MAX_PRICE_BUY<=in_price)||(0==in_amount))
{
printf("erro when fill price or amount, price buy should little than MAX_PRICE_BUY, amount bigger than 0 .\n");
return 0;
}

}
fill_price(in_price,p_post);
fill_amount(in_amount,p_post);
}
if((CANCEL_ORDER==p_post->method_vlaue)||(ORDER_ID_BY_TRADE_ID==p_post->method_vlaue)||(ORDER_INFO==p_post->method_vlaue))
{
if((0==in_id))
{
printf("erro when fill oder id which  should be not be 0 .\n");
return 0;
}
if((CANCEL_ORDER==p_post->method_vlaue)||(ORDER_INFO==p_post->method_vlaue))
fill_id(in_id,p_post);
if(ORDER_ID_BY_TRADE_ID==p_post->method_vlaue)
fill_trade_id(in_id,p_post);
}
fill_keys(p_post);
fill_time_created(p_post);
fill_market(p_post);
fill_coin_type(p_post);

fill_sign(p_post);

fill_post_data(p_post);


//printf("%s%s%s%s%s",p_post->method,p_post->coin_type,p_post->access_key,p_post->created,p_post->sign);

}

extern void free_post_data_type(post_data_t * p_post)
{
if(NULL!=p_post->coin_type)
free(p_post->coin_type);
if(NULL!=p_post->sign)
{
free(p_post->sign);
free(p_post->sign_value);
}
if(NULL!=p_post->created)
free(p_post->created);
if(NULL!=p_post->data)
free(p_post->data);
if(NULL!=p_post->price)
free(p_post->price);
if(NULL!=p_post->amount)
free(p_post->amount);
if(NULL!=p_post->id)
free(p_post->id);
if(NULL!=p_post->trade_id)
free(p_post->trade_id);

}

