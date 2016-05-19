#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>


#include "opration.h"
#include "build_raw_param.h"

const char * str_market = "market";
const char * str_sign = "sign";


void build_method_no_and_symbol(char * param,post_data_t* p_post)
{
	
strcat(param,str_method);
strcat(param,str_link_euqal);
strcat(param,p_post->method);
}

void build_access_key_with_and_symbol(char * param,post_data_t* p_post)
{
strcat(param,str_link_and);
strcat(param,str_access_key);
strcat(param,str_link_euqal);
strcat(param,p_post->access_key);
}



extern void build_data_sign(char * param,post_data_t* p_post)
{
strcat(param,str_link_and);
	
strcat(param,str_sign);
strcat(param,str_link_euqal);
strcat(param,p_post->sign);
}

extern void build_data_market(char * param,post_data_t* p_post)

{
strcat(param,str_link_and);
		
strcat(param,str_market);
strcat(param,str_link_euqal);
strcat(param,p_post->market);



}
void build_data_get_account_info(char * param,post_data_t* p_post)
{
//method=get_account_info&access_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&created=1451621432&sign=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&market=cny
	
build_method_no_and_symbol(param,p_post);
build_access_key_with_and_symbol(param,p_post);
build_created(param,p_post);
build_data_sign(param,p_post);	
build_data_market(param,p_post);


}

void build_data_sell(char * param,post_data_t* p_post)
{

build_method_no_and_symbol(param,p_post);
build_access_key_with_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);
build_price(param,p_post);
build_amount(param,p_post);
build_data_sign(param,p_post);
build_data_market(param,p_post);


}

void build_data_buy(char * param,post_data_t* p_post)
{

build_method_no_and_symbol(param,p_post);
build_access_key_with_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_price(param,p_post);
build_amount(param,p_post);
build_created(param,p_post);
build_data_sign(param,p_post);
build_data_market(param,p_post);


}

void build_data_get_new_deal_orders(char * param,post_data_t* p_post)
{

build_method_no_and_symbol(param,p_post);
build_access_key_with_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);

build_data_sign(param,p_post);
build_data_market(param,p_post);


}

void build_data_get_orders(char * param,post_data_t* p_post)
{

build_method_no_and_symbol(param,p_post);
build_access_key_with_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);

build_data_sign(param,p_post);
build_data_market(param,p_post);


}

void build_data_get_order_id_by_trade_id(char *param,post_data_t* p_post)
{
build_method_no_and_symbol(param,p_post);
build_access_key_with_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);
build_trade_id(param,p_post);
build_data_sign(param,p_post);
build_data_market(param,p_post);

}

void build_data_cancel_order(char *param,post_data_t* p_post)
{
build_method_no_and_symbol(param,p_post);
build_access_key_with_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_id(param,p_post);
build_created(param,p_post);
build_data_sign(param,p_post);
build_data_market(param,p_post);

}

void build_data_order_info(char *param,post_data_t* p_post)
{
build_method_no_and_symbol(param,p_post);
build_access_key_with_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_id(param,p_post);
build_created(param,p_post);
build_data_sign(param,p_post);
build_data_market(param,p_post);

}


int extern fill_post_data(post_data_t* p_post)
{

if(METHOD_NO_USE == p_post->method_vlaue)
{

printf("erro here, the method is NO_USE.\n");

return 0;

}

char tmp[1024];
memset(tmp,0,1024);

method_t m_tmp = p_post->method_vlaue;

switch (m_tmp)
{
	case ACCOUNT_INFO:
	{
		build_data_get_account_info(tmp,p_post);
		break;
	}
	case GET_ORDERS:
	{
		if(COIN_NO_USE== p_post->coin_type_value)
	{

	printf("erro here, the coin_t is COIN_NO_USE.\n");
	return 0;
	}
	build_data_get_orders(tmp,p_post);
	break;
	}
	case NEW_DEAL_ORDERS:
	{
	if(COIN_NO_USE== p_post->coin_type_value)
	{

	printf("erro here, the coin_t is COIN_NO_USE.\n");
	return 0;
	}
	build_data_get_new_deal_orders(tmp,p_post);
	break;
	}
	case SELL:
	{
	if(MIN_PRICE_SELL>= p_post->price_value)
	{
	printf("erro here, the price is little than MIN_PRICE.\n");
	return 0;
	}
	build_data_sell(tmp,p_post);
	break;
	}
	case BUY:
	{
	if(MAX_PRICE_BUY<= p_post->price_value)
	{
	printf("erro here, the price is  bigger than MAX_PRICE_BUY.\n");
	return 0;
	}
	build_data_buy(tmp,p_post);
	break;
	}
	case ORDER_ID_BY_TRADE_ID:
	{
	if(0== p_post->id_value)
	{
	printf("erro here, the order id is 0.\n");
	return 0;
	}
	build_data_get_order_id_by_trade_id(tmp,p_post);
	break;
	}
	case CANCEL_ORDER:
	{
	if(0== p_post->id_value)
	{
	printf("erro here, the cancel order id is 0.\n");
	return 0;
	}
	build_data_cancel_order(tmp,p_post);
	break;
	}
	case ORDER_INFO:
	{
	if(0== p_post->id_value)
	{
	printf("erro here, the order info id is 0.\n");
	return 0;
	}
	build_data_order_info(tmp,p_post);
	break;
	}
	default:
	{
	}


}

unsigned int len = 0;
len = strlen(tmp);
tmp[len]='\0';
char * data = NULL;
data = (char *)malloc((sizeof(char))*len+1);
if(NULL==data)
{
printf("malloc for data in build_post_data.\n");
return 0;
}

strcpy(data,tmp);

p_post->data = data;

//printf("\nthe fill data is %s.\n",p_post->data);

}


