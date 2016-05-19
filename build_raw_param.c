#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>


#include "opration.h"
#include "build_raw_param.h"


const char * str_access_key= "access_key";
const char * str_created= "created";
const char * str_coin_type = "coin_type";
const char * str_price = "price";
const char * str_amount = "amount";
const char * str_method= "method";
//please noted that we have two id for orders, one is id and the other is trade_id....confusing me,but ......
const char * str_id = "id";
const char * str_trade_id  = "trade_id";

const char * str_secret_key= "secret_key";
const char * str_link_and = "&";
const char * str_link_euqal = "=";

void build_access_key_no_and_symbol(char * param,post_data_t* p_post)
{
strcat(param,str_access_key);
strcat(param,str_link_euqal);
strcat(param,p_post->access_key);
}

extern void build_created(char * param,post_data_t* p_post)
{
strcat(param,str_link_and);
	
strcat(param,str_created);
strcat(param,str_link_euqal);
strcat(param,p_post->created);

}

extern void build_coin_type(char * param,post_data_t* p_post)
{
strcat(param,str_link_and);
	
strcat(param,str_coin_type);
strcat(param,str_link_euqal);
strcat(param,p_post->coin_type);

}

void build_method_with_and_symbol(char * param,post_data_t* p_post)
{
strcat(param,str_link_and);
	
strcat(param,str_method);
strcat(param,str_link_euqal);
strcat(param,p_post->method);
}

extern void build_secret_key(char * param,post_data_t* p_post)
{
strcat(param,str_link_and);
	
strcat(param,str_secret_key);
strcat(param,str_link_euqal);
strcat(param,p_post->secret_key);

}

extern void build_price(char * param,post_data_t* p_post)
{
strcat(param,str_link_and);
	
strcat(param,str_price);
strcat(param,str_link_euqal);
strcat(param,p_post->price);
}

extern void build_amount(char * param,post_data_t* p_post)
{
strcat(param,str_link_and);
	
strcat(param,str_amount);
strcat(param,str_link_euqal);
strcat(param,p_post->amount);
}

extern void build_id(char * param,post_data_t* p_post)
{
strcat(param,str_link_and);
strcat(param,str_id);
strcat(param,str_link_euqal);
strcat(param,p_post->id);
}

extern void build_trade_id(char * param,post_data_t* p_post)
{
strcat(param,str_link_and);
strcat(param,str_trade_id);
strcat(param,str_link_euqal);
strcat(param,p_post->trade_id);
}

void build_param_get_account_info(char * param,post_data_t* p_post)
{
//"access_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&created=1386844119&method=get_account_info&secret_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx"

build_access_key_no_and_symbol(param,p_post);
build_created(param,p_post);
build_method_with_and_symbol(param,p_post);
build_secret_key(param,p_post);





}

void build_param_get_orders(char * param,post_data_t* p_post)
{
//"access_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&coin_type=1&created=1386844119&method=get_orders&secret_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx"
build_access_key_no_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);
build_method_with_and_symbol(param,p_post);
build_secret_key(param,p_post);


}

void build_param_get_new_deal_orders(char * param,post_data_t* p_post)
{
//"access_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&coin_type=1&created=1386844119&method=get_new_deal_orders&secret_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx"
build_access_key_no_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);
build_method_with_and_symbol(param,p_post);
build_secret_key(param,p_post);

}

void build_param_sell(char * param,post_data_t * p_post)
{
//access_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&amount=10&coin_type=1&created=1386844119&method=sell&price=5000&secret_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx
build_access_key_no_and_symbol(param,p_post);
build_amount(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);
build_method_with_and_symbol(param,p_post);
build_price(param,p_post);
build_secret_key(param,p_post);
}

void build_param_buy(char * param,post_data_t * p_post)
{
//access_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&amount=10&coin_type=1&created=1386844119&method=buy&price=5000&secret_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx
build_access_key_no_and_symbol(param,p_post);
build_amount(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);
build_method_with_and_symbol(param,p_post);
build_price(param,p_post);
build_secret_key(param,p_post);
}




void build_param_get_order_id_by_trade_id(char *param,post_data_t * p_post)
{
//access_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&coin_type=1&created=1386844119&method=get_order_id_by_trade_id&secret_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&trade_id=1
build_access_key_no_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);
build_method_with_and_symbol(param,p_post);
build_secret_key(param,p_post);
build_trade_id(param,p_post);

}

void build_param_cancel_order(char *param,post_data_t * p_post)
{
//access_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&coin_type=1&created=1386844119&id=1&method=cancel_order&secret_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx
build_access_key_no_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);
build_id(param,p_post);
build_method_with_and_symbol(param,p_post);
build_secret_key(param,p_post);
}

void build_param_order_info(char *param,post_data_t * p_post)
{
//access_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx&coin_type=1&created=1386844119&id=2&method=order_info&secret_key=xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx
build_access_key_no_and_symbol(param,p_post);
build_coin_type(param,p_post);
build_created(param,p_post);
build_id(param,p_post);
build_method_with_and_symbol(param,p_post);
build_secret_key(param,p_post);
}

int extern build_raw_param(char * param,post_data_t* p_post)
{

if(METHOD_NO_USE == p_post->method_vlaue)
{

printf("erro here, the method is NO_USE.\n");

return 0;

}

method_t m_tmp = p_post->method_vlaue;

switch (m_tmp)
{
	case ACCOUNT_INFO:
	{
		build_param_get_account_info(param,p_post);
		break;
	}
	case GET_ORDERS:
	{
		if(COIN_NO_USE== p_post->coin_type_value)
	{

	printf("erro here, the coin_t is COIN_NO_USE.\n");
	return 0;
	}
	build_param_get_orders(param,p_post);
	break;
	}
	case NEW_DEAL_ORDERS:
	{
	if(COIN_NO_USE== p_post->coin_type_value)
	{

	printf("erro here, the coin_t is COIN_NO_USE.\n");
	return 0;
	}
	build_param_get_new_deal_orders(param,p_post);
	break;
	}
	case SELL:
	{
	if(MIN_PRICE_SELL>= p_post->price_value)
	{
	printf("erro here, the price is little than MIN_PRICE.\n");
	return 0;
	}
	build_param_sell(param,p_post);
	break;
	}
	case BUY:
	{
	if(MAX_PRICE_BUY<= p_post->price_value)
	{
	printf("erro here, the price is  bigger than MAX_PRICE_BUY.\n");
	return 0;
	}
	build_param_buy(param,p_post);
	break;
	}
	case ORDER_ID_BY_TRADE_ID:
	{
	if(0== p_post->id_value)
	{
	printf("erro here, the order id is 0.\n");
	return 0;
	}
	build_param_get_order_id_by_trade_id(param,p_post);
	break;
	}
	case CANCEL_ORDER:
	{
	if(0== p_post->id_value)
	{
	printf("erro here, the cancel order id is 0.\n");
	return 0;
	}
	build_param_cancel_order(param,p_post);
	break;
	}
	case ORDER_INFO:
	{
	if(0== p_post->id_value)
	{
	printf("erro here, the order info id is 0.\n");
	return 0;
	}
	build_param_order_info(param,p_post);
	break;
	}
	default:
	{
	}


}

}
