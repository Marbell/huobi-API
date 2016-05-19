#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include  "process_response.h"

const char * status_string_meaning[9] = 
{
	"ORDER_UNSETTLED",
	"ORDER_PART_SETTLED",
	"ORDER_SETTLED",
	"ORDER_CANCELED",
	"ORDER_DIACARD",
	"ORDER_UNNORMAL",
	"ORDER_PART_SETTLED_PART_CANCELED",
	"ORDER_INQUNE",
	"ORDER_UNUSED"

};

const char * erro_code_string[] = 
{
	"no use",
	"Server is busy",
	"Insufficient RMB",
	"Account is locked",
	"Unable to operate liquidating system order",
	"Abnormal assets, please contact customer service",
	"User does not exist",
	"Insufficient USD",
	"Insufficient BTC",
	"Insufficient LTC",
	"The order does not exist",
	"Asset is locked, unable to trade",
	"Cancellation of order is failed",
	"Unable to modify market order",
	"Unable to cancel market order",
	"The order has ended, can not be canceled or modified",
	"The order has been canceled, can not be canceled or modified",
	"The order has been filled, can not be canceled or modified",
	"Purchase amount can not be less than 1 yuan",
	"Purchase price of LTC can not be higher than 110% of current price",
	"BTC transactions can not be less than 0.001",
	"LTC transactions can not be less than 0.01",
	"Sell price of LTC can not be 10% lower than current price",
	"Buy price of BTC can not be 10% higher than current price",
	"Sell price of BTC can not be 10% lower than current price",
	"Buy price can not be 5% higher than current price/td>",
	"Sell price can not be 5% lower than current price",
	"Request is missing necessary parameters",
	"Request parameter is not correct",
	"Invalid request",
	"Invalid method",
	"Private key verification is failed",
	"Invalid price",
	"Invalid amount",
	"Invalid request time",
	"Too many requests",
	"Invalid order type",
	"Invalid transaction type",
	"Request information is not supported",
	"Invalid asset type",
	"Account type does not match the currency type",
	"Invalid trading market",
	"Trading amount is less than 0.1 BTC, the purchase price can not be 1% higher than the market price",
	"Trading amount is less than 0.1 BTC, the selling price can not be 1% lower than the market price",
	"Trading amount is less than 0.1 LTC, the purchase price can not be 1% higher than the market price",
	"Trading amount is less than 0.1 LTC, the selling price can not be 1% lower than the market price",
	"Invalid currency type",
	"Password is wrong {0} times. Account will be frozen for 24 hours if password is wrong for continuing 5 times, please try again",
	"Password is wrong more than 5 times, account has been frozen for 24 hours",
	"Please enter password",
	"User-defined order number already exists",
	"Transaction price of limit price order is rounded to 2 decimal places",
	"Trading volume of limit price order is rounded to 4 decimal places",
	"Purchase amount of market order is rounded to 2 decimal places",
	"Selling volume of market order is rounded to 4 decimal places",
	"Trading price can not be more than 100 thousand yuan",
	"Submit order failed",
	"BTC transactions can not be more than 1000",
	"Purchase price of market order can not be more than 100,000 USD",
	"BTC transaction price of limit price order can not be less than 0.01",
	"LTC transaction price of limit price order can not be less than 0.01",
	"Purchase amount can not be more than 1,000,000 RMB",
	"BTC withdrawal is rounded to 4 decimal places",
	"LTC withdrawal is rounded to 4 decimal places/td>",
	"Minimum transfer is 0.0001",
	"Transfer amount is rounded to 4 decimal places",
	"ip error",
	"Invalid user",
	"Transfer out account error",
	"Transfer in account error",
	"Transfer amount error",
	"Transfer error",
	"RMB Spot assets error",
	"USD Spot assets error",
	"Insufficient available assets",
	"Transfer in account and transfer out account can not be the same account",
	"System does not support LTC transfers for now",
	"Institutional account is not allowed to transfer",
	"LTC transactions can not be more than 20,000",
	"Please use POST to submit requests",
	"Withdrawals are not allowed",
	"No access",
	"User locked",
	"Abnormal assets, please contact customer service",
	"Withdrawal address does not exists",
	"Non security authenticated withdrawal address",
	"Insufficient balance",
	"Please contact customer service or account manager to make an appointment when you want to withdraw BTC more than 1,000 or LTC more than 10,000",
	"Minimum BTC withdrawal amount is 0.0100, Minimum LTC withdrawal amount is 0.1",
	"Lower than withdrawal fee",
	"Beyond daily amount",
	"Withdrawal address error",
	"Withdrawal record does not exist",
	"User information error",
	"This withdrawal can not be cancelled",
	"System is busy",
	"Unable to meet the requirements of withdrawal security standard",
	"Too many withdrawals",
	"Institutional accounts are not allowed to withdraw",    
	"Account can't apply margin",
	"Fail to get interest rate",
	"Margin amount error",
	"Fail to get latest price",
	"Fail to get reserved margin amount",
	"Fail to get VIP margin amount",
	"Fail to get available amount for application",
	"Fail to get total margin amount",
	"Insufficient system balance",
	"Asset error",
	"Your account is not allowed for margin application",
	"Application amount is less than minimum amount available",
	"Application amount is more than maximum amount available",
	"Fail to calculate the interest",
	"Fail to change the asset",
	"Fail to change system asset",
	"Fail to change account asset",
	"Fail to add margin records",
	"Fail to get margin list",    
	"No corresponding record",
	"Insufficient available balance",
	"Incorrect repayment amount",
	"Fail to lock margin record",
	"Fully repaid",
	"Repayment amount exceeds the amount owing",
	"Fail to add repayment record",
	"Fail to add system asset",
	"Insufficient available asset",    
	"Invalid margin type",
	"Invalid repayment type",
	"Amount of application or repayment should be rounded to 2 decimal places",
	"Volume of application or repayment should be rounded to 4 decimal places",
};


extern int pre_process_response(char *response)
{
//printf("Response:\n%s\n",response);
const char * str_ok_200 = "200 OK";
char * tmp_ok = NULL;

tmp_ok = strstr(response, str_ok_200);
if((NULL==tmp_ok))
{
printf("erro here, no 200 OK string in respponse message:%s.\n",response);
return 0;
}
return 1;

}


extern int process_hq_response(char * response, hq_response_t * p_hq)
{
//printf("Response:\n%s\n",response);

const char * str_buy = "buy";
const char * str_sell = "sell";
const char * str_time = "time";
const char * str_vol = "vol";


char * tmp_b = NULL;
char * tmp_s = NULL;
char * tmp_time = NULL;
char * tmp_vol = NULL;




tmp_b = strstr ( response, str_buy);
tmp_s = strstr ( response, str_sell);
tmp_time = strstr( response, str_time);
tmp_vol = strstr( response, str_vol);


if((NULL==tmp_b)||(NULL==tmp_s))
{
printf("erro here, no buy /sell string in respponse message:%s.\n",response);
return 0;
}
double buy = 0;
double sell = 0;
unsigned long int time = 0;
double vol = 0;
char ** end_tmp_b = NULL;
char ** end_tmp_s =NULL;
char ** end_tmp_time = NULL;
char ** end_tmp_vol = NULL;


buy = strtod((tmp_b+5),end_tmp_b);
p_hq->ticker.buy = buy;
sell = strtod((tmp_s+6),end_tmp_s);
p_hq->ticker.sell = sell;
time = strtoul(tmp_time+7,end_tmp_time,10);
p_hq->time = time;
vol = strtod((tmp_vol+5),end_tmp_vol);
p_hq->ticker.vol = vol;

//printf("the buy price is %f .\n",p_hq->ticker.buy);
//printf("the sell price is %f .\n",p_hq->ticker.sell);

return 1;

}

void process_one_order_line(one_order_line_t *p_in)
{
	unsigned int len = p_in->len-2;
	char * in = p_in->begin+2;
	
	char * p_id = NULL;
	char * p_type= NULL;
	char * p_order_price= NULL;
	char * p_order_amount= NULL;
	char * p_processed_amount= NULL;
	char * p_last_processed_time= NULL;
	char * p_order_time= NULL;
	char * p_status= NULL;

	
	//{"id":XXXXXXXXX,"type":1,"order_price":"2728","order_amount":"0.1000","processed_amount":"0.1000","last_processed_time":XXXXXXXXX,"order_time":XXXXXXXXX,"status":2}
	
	int count = 0;
	
	for (int i =0;i<len;i++)
	{
	//printf("%c",*(in+i));
	if(':'==*(in+i))
	count++;
	if ((1==count)&&(':'==*(in+i)))
	{
	p_id = in+i+1;

	char ** end_tmp_id = NULL;
	unsigned long int id = 0;
	id = strtoul(p_id,end_tmp_id,10);
	p_in->id = id;
	p_in->p_id = p_id;
	}
	if((2==count)&&(':'==*(in+i)))
	{
	p_type = in+i+1;
	
	char ** end_tmp_type = NULL;
	int type = 0;
	type = (unsigned int)strtol(p_type,end_tmp_type,10);
	p_in->type= type;
	p_in->p_id = p_type;
	}
	
	if((3==count)&&(':'==*(in+i)))
	{
	p_order_price = in+i+1+1;
	char ** end_tmp_order_price = NULL;
	double a = 0;
	a = strtod(p_order_price,end_tmp_order_price);
	p_in->order_price = a;
	}

	if((4==count)&&(':'==*(in+i)))
	{
	p_order_amount = in+i+1+1;
	char ** end_tmp = NULL;
	double a = 0;
	a = strtod(p_order_amount,end_tmp);
	p_in->order_amount = a;
	}

	if((5==count)&&(':'==*(in+i)))
	{
	p_processed_amount = in+i+1+1;
	char ** end_tmp = NULL;
	double a = 0;
	a = strtod(p_processed_amount,end_tmp);
	p_in->processed_amount = a;
	}
	if ((6==count)&&(':'==*(in+i)))
	{
	p_last_processed_time = in+i+1;

	char ** end_tmp = NULL;
	unsigned long int last_processed_time = 0;
	last_processed_time = strtoul(p_last_processed_time,end_tmp,10);
	p_in->last_processed_time = last_processed_time;
	p_in->p_last_processed_time = p_last_processed_time;
	}
	if ((7==count)&&(':'==*(in+i)))
	{
	p_order_time = in+i+1;

	char ** end_tmp = NULL;
	unsigned long int order_time = 0;
	order_time = strtoul(p_order_time,end_tmp,10);
	p_in->order_time = order_time;
	p_in->p_order_time = p_order_time;
	}
	if((8==count)&&(':'==*(in+i)))
	{
	p_status = in+i+1;
	
	char ** end_tmp = NULL;
	int status = 0;
	status = (unsigned int)strtol(p_status,end_tmp,10);
	p_in->status= status;
	p_in->p_status = p_status;
	}
		
	}
	//printf("the id is %lu,the type is %d.\n",p_in->id,p_in->type);

}



void process_ma_minutes_line(minutes_line_t * p_in)
{
unsigned int len = p_in->len-2;
char * in = p_in->begin+2;

char * p_time = NULL;
char * p_kp = NULL;
char * p_zg = NULL;
char * p_zd = NULL;
char * p_sp = NULL;
char * p_cjl =NULL;


p_time = in;
int count = 0;

for (int i =0;i<len;i++)
{
//printf("%c",*(in+i));
if(','==*(in+i))
count++;
if ((1==count)&&(','==*(in+i)))
p_kp = in+i+1;
if((2==count)&&(','==*(in+i)))
{
p_zg = in+i+1;

char ** end_tmp_zg = NULL;
double a = 0;
a = strtod(p_zg,end_tmp_zg);
p_in->zg = a;


}

if((3==count)&&(','==*(in+i)))
{
p_zd = in+i+1;
char ** end_tmp_zd = NULL;
double a = 0;
a = strtod(p_zd,end_tmp_zd);
p_in->zd = a;
/*for(int j = 0;j<10;j++)
{
printf("%c",*(p_zd+j));
}
printf("\n");*/
}
if((4==count)&&(','==*(in+i)))
{
p_sp = in+i+1;

char ** end_tmp_sp = NULL;
double a = 0;
a = strtod(p_sp,end_tmp_sp);
p_in->sp = a;
/*for(int j = 0;j<10;j++)
{
printf("%c",*(p_sp+j));
}
printf("\n");*/
}
if((5==count)&&(','==*(in+i)))
{
p_cjl = in+i+1;

char ** end_tmp_cjl = NULL;
double a = 0;
a = strtod(p_cjl,end_tmp_cjl);
p_in->cjl = a;


}

}
//printf("\n");
//printf("sp is %f.\n",p_in->sp);

}



extern int process_ma_response(char *response, update_ma_response_t * p_ma)
{
	//printf("Response:\n%s\n",response);
		long double average_sp = 0;
		long double average_zd = 0;
		long double average_zg = 0;

		long double all_cjje_sp = 0;
		long double all_cjje_zg = 0;
		long double all_cjje_zd = 0;
		long double all_cjje_pj = 0;
		long double all_cjl = 0;
		
		const char * str_begin = "[\"";
		char * p_begin = NULL;
		char * p_end = NULL;
		const char * str_end = "]";
		char * p_tmp = response;
		//char * p_tmp_end = response;
		int count = 0;
		for(int i = 0 ;i <MAX_ITEM_IN_ONE_RESPONSE;i++)
		{
			p_begin = strstr(p_tmp,str_begin);
			p_tmp = p_begin;
			p_end = strstr(p_tmp,str_end);
			if((NULL!=p_begin)&&(NULL!=p_end))
			//if((NULL!=p_begin))
			{
			count++;
			p_tmp = p_end;
			p_ma->fs_array[i].begin = p_begin;
			p_ma->fs_array[i].end = p_end;
			p_ma->fs_array[i].len = p_end - p_begin;
			//process_len(fs_array[i].begin+2,fs_array[i].len-2);
			process_ma_minutes_line(&p_ma->fs_array[i]);
			//printf("index %d ptr is %x len is %d.\n",i,p_tmp,fs_array[i].len);
			}
		}
	for(int i = MAX_ITEM_IN_ONE_RESPONSE-PRE_MINUTES_FOR_MA;i < MAX_ITEM_IN_ONE_RESPONSE;i++)
	{
	//printf("%f\n",p_ma->fs_array[i].zd);
	average_sp = average_sp+p_ma->fs_array[i].sp/(PRE_MINUTES_FOR_MA);
	average_zd = average_zd+p_ma->fs_array[i].zd/(PRE_MINUTES_FOR_MA);
	average_zg = average_zg+p_ma->fs_array[i].zg/(PRE_MINUTES_FOR_MA);
	all_cjje_sp = all_cjje_sp + p_ma->fs_array[i].sp*p_ma->fs_array[i].cjl;
	all_cjje_zg = all_cjje_zg + p_ma->fs_array[i].zg*p_ma->fs_array[i].cjl;
	all_cjje_zd = all_cjje_zd + p_ma->fs_array[i].zd*p_ma->fs_array[i].cjl;
	all_cjje_pj = all_cjje_pj + ((p_ma->fs_array[i].zd+p_ma->fs_array[i].zg)/2)*p_ma->fs_array[i].cjl;
	all_cjl = all_cjl + p_ma->fs_array[i].cjl;
	}
	//p_ma->ma_sp=all_cjje_sp/all_cjl;
	p_ma->ma_sp=average_sp;
	p_ma->ma_zd=all_cjje_zd/all_cjl;
	p_ma->ma_zg=all_cjje_zg/all_cjl;
	p_ma->ma_pj=all_cjje_pj/all_cjl;

	
	//printf("the ma_sp is %f ma_zd is %f ma_zg is %f,ma_pj is %f.\n",p_ma->ma_sp,p_ma->ma_zd,p_ma->ma_zg,p_ma->ma_pj);
}

extern int process_jx_response(char *response, update_jx_response_t * p_jx)
{
//printf("Response:\n%s\n",response);
	
	const char * str_huobi = "huobi";
	const char * str_dh = ",";
	
	
	char * tmp_ptr = NULL;
	tmp_ptr = strstr(response,str_huobi);
	tmp_ptr = strstr(tmp_ptr,str_dh);
	tmp_ptr = tmp_ptr-6;
	
	//printf("data is \n%s",tmp_ptr);
	
	int count = 0;
	
	char * data_begin = tmp_ptr;
	char * p_dh = NULL;
	do
	{
	p_dh = strstr(data_begin,str_dh);
	if(NULL!=p_dh)

	{
	count++;
	int fsjx_array_index = 0;
	fsjx_array_index = (count-1)/3;
	int sub_index = 0;
	sub_index = count%3;
	if(1==sub_index)
	{
	p_jx->fsjx_array[fsjx_array_index].p_time = p_dh-6;
	p_jx->fsjx_array[fsjx_array_index].p_dq_jg = p_dh+1;
	}
	if(2==sub_index)
	p_jx->fsjx_array[fsjx_array_index].p_cjl = p_dh+1;
	if(0==sub_index)
	p_jx->fsjx_array[fsjx_array_index].p_cjje = p_dh+1;
	}
	else
	break;
	data_begin = p_dh+1;
	}while(1);
	
	//printf("all have %d dh.\n",count);
	
	for(int i = 0;i <MAX_ITEM_IN_ONE_RESPONSE;i++)
	{
	char * tmp_time = NULL;
	char * tmp_dqjg = NULL;
	char * tmp_cjl = NULL;
	char * tmp_cjje = NULL;
	
	char ** end_tmp_time = NULL;
	char ** end_tmp_dqjg = NULL;
	char ** end_tmp_cjl = NULL;
	char ** end_tmp_cjje = NULL;
	
	tmp_time = p_jx->fsjx_array[i].p_time;
	tmp_dqjg = p_jx->fsjx_array[i].p_dq_jg;
	tmp_cjl = p_jx->fsjx_array[i].p_cjl;
	tmp_cjje = p_jx->fsjx_array[i].p_cjje;
	
	double a = 0;
	a = strtod(tmp_dqjg,end_tmp_dqjg);
	
	double b = 0;
	b= strtod(tmp_cjl,end_tmp_cjl);
	
	double c = 0;
	c= strtod(tmp_cjje,end_tmp_cjje);
	
	unsigned int long d = 0;
	d = strtoul(tmp_time,end_tmp_time,10);
	
	//printf("d:%lu,a:%f,b:%f,c:%f,\n",d,a,b,c);
	p_jx->fsjx_array[i].time = d;
	p_jx->fsjx_array[i].dq_jg = a;
	p_jx->fsjx_array[i].cjl = b;
	p_jx->fsjx_array[i].cjje = c;
	
	}
	
	long double all_cjl = 0;
	long double all_cjje = 0;

	unsigned int start_index = 0;

	for(int j = 0;j<MAX_ITEM_IN_ONE_RESPONSE;j++)
	{
	if(180000==p_jx->fsjx_array[j].time)
	start_index = j;

	}

	long double pjjg_jx = 0;
	
	for(int i = MAX_ITEM_IN_ONE_RESPONSE-PRE_MINUTES_FOR_MA;i < MAX_ITEM_IN_ONE_RESPONSE;i++)
	//for(int i = 0;i < MAX_ITEM_IN_ONE_RESPONSE;i++)
	//for(int i = start_index+1;i < MAX_ITEM_IN_ONE_RESPONSE;i++)
	{
	all_cjl = all_cjl+p_jx->fsjx_array[i].cjl;
	all_cjje = all_cjje+p_jx->fsjx_array[i].cjje;
	pjjg_jx = pjjg_jx+(p_jx->fsjx_array[i].cjje/p_jx->fsjx_array[i].cjl);
	//printf("the cjl is %f.\n",p_jx->fsjx_array[i].cjl);
	//printf("the cje is %f.\n",p_jx->fsjx_array[i].cjje);
	}
	long double jx = 0;
	jx = (all_cjje/all_cjl);
	pjjg_jx = pjjg_jx/PRE_MINUTES_FOR_MA;
	
	//printf("the all_cjl is %Lf.\n",all_cjl);
	//printf("the all_cjje is %Lf.\n",all_cjje);
	//printf("%s",response);
	
	//printf("the fsjx_ma is %Lf.\n",jx);
	p_jx->jx=jx;
	

}



int process_buy_sell_response(char * response, do_opration_response_t* p_do_response)
{
	printf("Response:\n%s\n",response);

	const char * str_sucess = "success";
	const char * str_code = "code";
	const char * str_id = "id";

	
	method_t tmp_method = p_do_response->method;
	
	char * tmp_s = NULL;
	char * tmp_c = NULL;
	char * tmp_i = NULL;

	char ** end_tmp_code = NULL;
	char ** end_tmp_id = NULL;
	
	
	
	tmp_s = strstr ( response, str_sucess);
	tmp_c = strstr ( response, str_code);
	tmp_i = strstr (response,str_id);
	if(NULL!=tmp_s&&NULL==tmp_c&&NULL!=tmp_i)
	{
	p_do_response->m_opration_response.sell_buy_response.result= RESULT_SUCCSS;

	unsigned long int id_tmp = 0;
	
	id_tmp = strtoul(tmp_i+4,end_tmp_id,10);

	p_do_response->m_opration_response.sell_buy_response.id = id_tmp;
	
	
	//printf("the response is sucess,and id is %d,and msg is %s.\n",p_do_response->m_opration_response.sell_buy_response.id,erro_code_string[p_do_response->m_opration_response.sell_buy_response.id]);
	
	return 1;
	}
	
	
	if(NULL==tmp_s&&NULL!=tmp_c)
	{
	p_do_response->m_opration_response.sell_buy_response.result = RESULT_FAIL;

	unsigned long int code_tmp = 0;
	
	code_tmp = strtoul(tmp_c+6,end_tmp_code,10);

	p_do_response->m_opration_response.sell_buy_response.code= code_tmp;
	
	//printf("the response is fail,and code is %d.\n",p_do_response->m_opration_response.sell_buy_response.code);
	
	return 1;
	}
	
	return 0;

}
int process_account_info_response(char * response, do_opration_response_t* p_do_response)
{
	//printf("Response:\n%s\n",response);

	const char * str_net_asset = "net_asset";
	const char * str_available_cny_display = "available_cny_display";
	const char * str_available_btc_display = "available_btc_display";

	method_t tmp_method = p_do_response->method;

	char * tmp_net_asset = NULL;
	char * tmp_available_cny_display = NULL;
	char * tmp_available_btc_display = NULL;

	tmp_net_asset = strstr ( response, str_net_asset);
	tmp_available_cny_display = strstr ( response, str_available_cny_display);
	tmp_available_btc_display = strstr (response,str_available_btc_display);

	if((NULL!=tmp_net_asset)&&(NULL!=tmp_available_cny_display)&&(NULL!=tmp_available_btc_display))
	{
	char ** end_tmp_net = NULL;
	char ** end_tmp_acny =NULL;
	char ** end_tmp_abtc = NULL;

	double a = 0;
	double b = 0;
	double c = 0;

	a = strtod((tmp_net_asset+12),end_tmp_net);
	b = strtod((tmp_available_cny_display+24),end_tmp_acny);
	c = strtod((tmp_available_btc_display+24),end_tmp_abtc);

	p_do_response->m_opration_response.account_info_response.net_asset =a;
	p_do_response->m_opration_response.account_info_response.available_cny_display=b;
	p_do_response->m_opration_response.account_info_response.available_btc_display=c;

	/*printf("Acount Info:\n");
	printf("Asset:%f\n",p_do_response->m_opration_response.account_info_response.net_asset);
	printf("CNY:%f\n",p_do_response->m_opration_response.account_info_response.available_cny_display);
	printf("BIT:%f\n",p_do_response->m_opration_response.account_info_response.available_btc_display);*/
	return 1;
	}
	
	return 0;

}

int process_get_new_deal_orders_response(char * response, do_opration_response_t* p_do_response)

{
	//printf("Response:\n%s\n",response);
	
	unsigned int long len = 0;
	len = strlen(response);
		
	const char * str_begin = "{\"";
	char * p_begin = NULL;
	char * p_end = NULL;
	const char * str_end = "}";
	char * p_tmp = response;
		//char * p_tmp_end = response;
	int count = 0;
	for(int i = 0 ;i <MAX_ORDER_INTEM_ONE_RESPONSE;i++)
		{
			p_begin = strstr(p_tmp,str_begin);
			p_tmp = p_begin;
			p_end = strstr(p_tmp,str_end);
			if((NULL!=p_begin)&&(NULL!=p_end))
			//if((NULL!=p_begin))
			{
			count++;
			p_tmp = p_end;			
			p_do_response->m_opration_response.get_new_deal_orders_response.order_line_array[i].begin = p_begin;
			p_do_response->m_opration_response.get_new_deal_orders_response.order_line_array[i].end = p_end;
			p_do_response->m_opration_response.get_new_deal_orders_response.order_line_array[i].len = p_end - p_begin;
			
			process_one_order_line(&p_do_response->m_opration_response.get_new_deal_orders_response.order_line_array[i]);
			
			}
		}
}



int process_order_info_response(char * response, do_opration_response_t* p_do_response)
{
	//printf("Response:\n%s\n",response);
	//
	const char * str_status = "status";
	const char * str_oder_id = "id";

	char * tmp_status = NULL;
	char * tmp_id = NULL;
	tmp_status = strstr ( response, str_status);
	tmp_id = strstr(response,str_oder_id);

	
	if((NULL!=tmp_status)&&(NULL!=tmp_id))
	{
	char ** end_tmp_status = NULL;
	char ** end_tmp_id = NULL;

	unsigned int a = 0;
	unsigned long int b = 0;

	a = (unsigned int)strtol((tmp_status+8),end_tmp_status,10);
	b = (unsigned long int)strtoul((tmp_id+4),end_tmp_id,10);

	p_do_response->m_opration_response.order_info_response.status=(order_status_t)a;
	p_do_response->m_opration_response.order_info_response.id = b;

	printf("Order Info:\n");
	printf("Status:%d --%s\n",p_do_response->m_opration_response.order_info_response.status,status_string_meaning[p_do_response->m_opration_response.order_info_response.status]);
	printf("Id:%lu \n",p_do_response->m_opration_response.order_info_response.id);
	return 1;
	}
	
	return 0;

}


extern int process_do_opration_response(char * response, do_opration_response_t* p_do_response)
{
	method_t tmp_method = p_do_response->method;

	switch (tmp_method)

		{	
			case SELL:
			case BUY:
			{
				process_buy_sell_response(response,p_do_response);
				break;

			}
			case ACCOUNT_INFO:
			{
				process_account_info_response(response,p_do_response);
				break;
			}
			case ORDER_INFO:
			{
				process_order_info_response(response,p_do_response);
				break;
			}
			case NEW_DEAL_ORDERS:
			{
				process_get_new_deal_orders_response(response,p_do_response);
				break;
			}
		}


}


