#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 

#include <pthread.h>


#include "process_response.h"
#include "get_data.h"
#include "misc.h"
#include "monitor_silding_window.h"
#include "log_buffer.h"



pthread_mutex_t update_info_mutex;
pthread_spinlock_t update_info_spinlock;

pthread_spinlock_t read_input_spinlock;

pthread_spinlock_t menu_spinlock;
static volatile int menu_value[8];


pthread_spinlock_t buy_sell_gap_spinlock;


static window_t windows_monitoring;

log_buffer_t * p_log_buffer;



static hq_response_t hq_response;
static update_ma_response_t update_ma_response;
static update_jx_response_t update_jx_response;

static account_info_response_t acount_info_response;
static get_new_deal_orders_response_t get_new_deal_orders_response;

static double asset_rmb = 0;
static double vol_pre = 0;
static double vol_delt = 0;


static double have_in_hand = 0;
static double reserved_in_hand = 0;
static double buy_under_ma_gap = 2;
static double sell_up_ma_gap = 5;

static double real_time_buy = 0;
static double real_time_sell = 0;

volatile int buy_trigerd = 0;

static volatile int keep_quite = 0;

char const * file_name = "data.csv";
FILE *fp_in;





char sever_data[1025] = {0};

void log_all_static_data(log_buffer_t * p_in);
void log_buy_sell_data(log_buffer_t * p_in,double in_price, double in_amount,log_buy_sell_t in_buy_sell);
void log_all_self_define_str(log_buffer_t * p_in,const char *in_string);



void *thread_dash_board_menu(void * args)
{
	unsigned long int pre_time = 0;
	while(1)
		{
		usleep(1);//too much cpu if we miss this
		if(hq_response.time==pre_time)
		{
		continue;
		}
		//pthread_mutex_lock(&update_info_mutex);

		update_window(hq_response.ticker.buy,hq_response.ticker.sell,update_ma_response.ma_sp,&windows_monitoring);
		process_window(&windows_monitoring);
				
		printf("\e[1;1H\e[2J");
		printf("Time:%lu\n",hq_response.time);
		printf("Buy:%f	 Sell:%f   Vol:%f   Delt-Vol:%f\n",hq_response.ticker.buy,hq_response.ticker.sell,hq_response.ticker.vol,vol_delt);
		if(1!=menu_value[2])
		printf("Bit:%f     Asset:%f\n",have_in_hand,asset_rmb);
		if(1==menu_value[3])
		{
		printf("MAZG is %Lf.\n",update_ma_response.ma_zg);
		printf("MAZD is %Lf.\n",update_ma_response.ma_zd);
		printf("MAJX is %Lf.\n",update_jx_response.jx);
		}
		printf("MASP is %Lf.\n",update_ma_response.ma_sp);

		//printf("the current index is %d.\n",windows_monitoring.current_index);
		/*printf("the newest buy is %Lf.\n",windows_monitoring.hq_window.hq_array_buy[windows_monitoring.current_index-1]);
		printf("the newest sell is %Lf.\n",windows_monitoring.hq_window.hq_array_sell[windows_monitoring.current_index-1]);

		printf("the newest jx is %Lf.\n",windows_monitoring.jx_window.jx_array[windows_monitoring.current_index-1]);

		if(CONFIG_WINDOW_SIZE==windows_monitoring.current_index)
		{
		printf("the oldest buy is %Lf.\n",windows_monitoring.hq_window.hq_array_buy[0]);
		printf("the oldest sell is %Lf.\n",windows_monitoring.hq_window.hq_array_sell[0]);
			
		printf("the oldest jx is %Lf.\n",windows_monitoring.jx_window.jx_array[0]);

		}
		else
		{
		printf("the oldest buy is %Lf.\n",windows_monitoring.hq_window.hq_array_buy[windows_monitoring.current_index]);
		printf("the oldest sell is %Lf.\n",windows_monitoring.hq_window.hq_array_sell[windows_monitoring.current_index]);

		printf("the oldest jx is %Lf.\n",windows_monitoring.jx_window.jx_array[windows_monitoring.current_index]);
		}*/
		if(long_double_equal_zero(windows_monitoring.hq_window.hq_array_buy[CONFIG_WINDOW_SIZE-1]))

		{
		//printf("the state is waiting update data,%d sample needed.\n",CONFIG_WINDOW_SIZE-windows_monitoring.current_index);
		printf("process data collectting:\n");
		for(int i = 0;i<=CONFIG_WINDOW_SIZE-windows_monitoring.current_index;i++)
		{
		printf(".");
		}
		printf("\n");
		}
		//else
		//{
		printf("the buy current state is %d.\n",windows_monitoring.buy_result);
		printf("the sell current state is %d.\n",windows_monitoring.sell_result);
		//}
		
		printf("Buy  trigger: %f,offset :%f\n",real_time_buy,hq_response.ticker.buy-real_time_buy);
		printf("Sell trigger: %f,offset :%f\n",real_time_sell,hq_response.ticker.sell-real_time_sell);
		printf("\n");
		
		printf("Menu:\n");
		printf("+-------------------------------------------------------------------------+\n");
		for(int i = 0 ;i<8;i++)
		{
		if(3==i||4==i)
		{
		printf("|    %d    ",menu_value[i]);
		continue;
		}
		printf("|   %d    ",menu_value[i]);
		}
		printf("|\n");
		printf("+-------------------------------------------------------------------------+\n");
		printf("|1.Buy:  1 price amount              |5.Enable/Disable data file          |\n");
		printf("|2.Sell: 2 price amount              |6.Enable/Disable socket output      |\n");
		printf("|3.Enable/Disable asset info         |7.New order info:7                  |\n");
		printf("|4.Enable/Disable more MA/JX info    |8.Quit out                          |\n");
		printf("+-------------------------------------------------------------------------+\n");
		
		
		if(1==menu_value[0])
				{
		//printf("+-------------------------------------------------------------------------+\n");
		printf("|>>>>>>1.Please input  price and amount:                                  |\n");
		printf("|      exp:[ 1500  0.8 ENTER]                                             |\n");	
		printf("|         \"| 1500 0.8\"                                                    |\n");

		double amount_buy = 0;
		double price_buy = 0;
		char enter_ok;
		printf("|");
		
		pthread_spin_lock(&read_input_spinlock);
		scanf("%lf %lf",&price_buy,&amount_buy);
		while(getchar() != '\n');
		pthread_spin_unlock(&read_input_spinlock);

		printf("+-------------------------------------------------------------------------+\n");
		printf("|>>>>>>You want to buy %lf with price %lf?                                 \n",amount_buy,price_buy);
		printf("|");
		
		pthread_spin_lock(&read_input_spinlock);
		scanf("%c",&enter_ok);
		while(getchar() != '\n');
		pthread_spin_unlock(&read_input_spinlock);
		
		if('\n'==enter_ok)
		printf("|>>>>>>Buy %lf with price %lf sucessfully.                                 \n",amount_buy,price_buy);
		else
		{
		printf("|>>>>>>Buy %lf with price %lf was canceled.                                \n",amount_buy,price_buy);
		}
		printf("+-------------------------------------------------------------------------+\n");
		pthread_spin_lock(&menu_spinlock);
		menu_value[0]= 0;
		pthread_spin_unlock(&menu_spinlock);
				}
		if(1==menu_value[1])

			{
		//printf("+-------------------------------------------------------------------------+\n");
		printf("|>>>>>>2.Please input  price and amount:                                  |\n");
		printf("|      exp:[ 3500  0.8 ENTER]                                             |\n");
		printf("|                                                                         |\n");
		printf("+-------------------------------------------------------------------------+\n");

			}
		//printf("log is %d item wirte pointer is %d and contend is :\n%s.\n",p_log_buffer->item,p_log_buffer->write_index,p_log_buffer->buffer);
		printf("\nLog:\n");
		printf("log is running,with %d item wirted and pointer is %d\n",p_log_buffer->item,p_log_buffer->write_index);
		if(menu_value[6])
		{
		printf("\nOrder:\n");
		//only show 5 item
		one_order_line_t *p_tmp = NULL;
		for(int i = 0;i<MAX_ORDER_INTEM_ONE_RESPONSE-5;i++)
		{
		
		p_tmp = &get_new_deal_orders_response.order_line_array[i];
		//{"id":XXXXXXXXX,"type":1,"order_price":"2728.20","order_amount":"0.1000","processed_amount":"0.1000","last_processed_time":XXXXXXXXX,"order_time":XXXXXXXXX,"status":2}
		//printf("the id is %lu,the type is %d.order price is %f.\n",get_new_deal_orders_response.order_line_array[i].id,get_new_deal_orders_response.order_line_array[i].type,get_new_deal_orders_response.order_line_array[i].order_price);
		//printf("the order_amount is %f,the processed_amount is %f.last_processed_time is %lu.\n",get_new_deal_orders_response.order_line_array[i].order_amount,get_new_deal_orders_response.order_line_array[i].processed_amount,get_new_deal_orders_response.order_line_array[i].last_processed_time);
		//printf("the order_time is %lu,the status is %d.\n",get_new_deal_orders_response.order_line_array[i].order_time,get_new_deal_orders_response.order_line_array[i].status);
		printf("Time:%lu type:%d price:%f processed amount:%f status:%d\n",p_tmp->order_time,p_tmp->type,p_tmp->order_price,p_tmp->processed_amount,p_tmp->status);
		}
		}
		
		printf("\e[?25l");
		pre_time = hq_response.time;
		//pthread_mutex_unlock(&update_info_mutex); 
		}
		
}

void *thread_monitor_key_press(void * args)
{	

	char sk_cmd;
	
while(1)
{
	pthread_spin_lock(&read_input_spinlock);
	sk_cmd = getchar();
	while (getchar() != '\n');
	pthread_spin_unlock(&read_input_spinlock);
	
	if(('q' == sk_cmd)||(1==menu_value[7]))
	{	
	fclose(fp_in);
	fp_in = NULL;
	exit(0);
	}
	int menu_value_tmp = sk_cmd - '0';
	if((0<menu_value_tmp)&&(menu_value_tmp<=8))
	{
	pthread_spin_lock(&menu_spinlock);
	if(0==menu_value[menu_value_tmp-1])
	menu_value[menu_value_tmp-1]= 1;
	else
	menu_value[menu_value_tmp-1]= 0;
	pthread_spin_unlock(&menu_spinlock);
	}
}


}



void *thread_query_hq(void * args)
{
	action_t action_tmp = QUERY_HQ;
	int rv = 0;
	while(1)
		{

		//pthread_mutex_lock(&update_info_mutex);
		//pthread_spin_lock(&update_info_spinlock);
		
		rv = sent_revc_data(action_tmp,NULL,(void *)&hq_response);

		//pthread_mutex_unlock(&update_info_mutex);
		//pthread_spin_unlock(&update_info_spinlock);
		
		if((rv)&&(0!=hq_response.ticker.sell)&&(0!=hq_response.ticker.buy))
		{
		
		
		vol_delt = hq_response.ticker.vol-vol_pre;
		
		vol_pre=hq_response.ticker.vol;
		
		real_time_buy=update_ma_response.ma_sp-buy_under_ma_gap;
	
		real_time_sell=update_ma_response.ma_sp+sell_up_ma_gap;

		 
		//only begin word data,they are both 0;
		if((update_ma_response.ma_zg==update_ma_response.ma_zd)||(-0.00001<update_jx_response.jx&&update_jx_response.jx<0.000001))
		continue;
		//out-put data to file and socket.
		if(1==menu_value[4])
		fprintf(fp_in,"%lu,%f,%f,%f,%f,%f,%Lf,%Lf,%Lf,%Lf\n",hq_response.time,hq_response.ticker.buy,hq_response.ticker.sell,hq_response.ticker.vol,vol_delt,have_in_hand,update_ma_response.ma_zg,update_ma_response.ma_zd,update_ma_response.ma_sp,update_jx_response.jx);
		if(1==menu_value[5])
		sprintf(sever_data,"%lu,%f,%f,%f,%f,%f,%Lf,%Lf,%Lf,%Lf\n",hq_response.time,hq_response.ticker.buy,hq_response.ticker.sell,hq_response.ticker.vol,vol_delt,have_in_hand,update_ma_response.ma_zg,update_ma_response.ma_zd,update_ma_response.ma_sp,update_jx_response.jx);
		}
		else
		{continue;}
		}

		

}


void *thread_update_ma(void * args)
{
	action_t action_tmp = UPDATE_MA;
	int rv = 0;
	while(1)
		{
		//update ma

		//pthread_mutex_lock(&update_info_mutex);
		rv = sent_revc_data(action_tmp,NULL,(void *)&update_ma_response);
		//pthread_mutex_unlock(&update_info_mutex);
		if((rv)&&(0!=update_ma_response.ma_sp)&&(0!=update_ma_response.ma_zd)&&(0!=update_ma_response.ma_zg))
		{
		//printf("ma_sp is %f ma_zd is %f,ma_zg is %f.\n",update_ma_response.ma_sp,update_ma_response.ma_zd,update_ma_response.ma_zg);
		//printf("ma_zg is %f, ma_zd is %f.\n",((update_ma_response.ma_zg+hq_response.ticker.buy)/2+0.1),((update_ma_response.ma_zd+hq_response.ticker.sell)/2-0.1));
		//printf("ma_zg is %f, ma_zd is %f,ma_pj is %f.\n",update_ma_response.ma_zg,update_ma_response.ma_zd,update_ma_response.ma_pj);
		}
		else
		{continue;}
		}
}


void *thread_update_jx(void * args)
{
	action_t action_tmp = UPDATE_JX;
	int rv = 0;
	while(1)
		{
		//update ma
		//pthread_mutex_lock(&update_info_mutex);
		rv = sent_revc_data(action_tmp,NULL,(void *)&update_jx_response);
		//pthread_mutex_unlock(&update_info_mutex);
		if((rv)&&(0!=update_jx_response.jx))
		{
		//printf("jx is %Lf.\n",update_jx_response.jx);
		}
		else
		{continue;}
		}
}

void *thread_get_new_deal_orders(void * args)
{	

	action_t action_tmp = DO_OPRATION;
	in_param_do_opration_t in_param_do_opration_get_new_deal_orders;
	do_opration_response_t do_opration_response_get_new_deal_orders;
	memset((char*)&in_param_do_opration_get_new_deal_orders,0,sizeof(in_param_do_opration_t));
	memset((char*)&do_opration_response_get_new_deal_orders,0,sizeof(do_opration_response_t));
	int rv = 0;
	//get_new_deal_orders
	in_param_do_opration_get_new_deal_orders.method=NEW_DEAL_ORDERS;
	in_param_do_opration_get_new_deal_orders.price=0;
	in_param_do_opration_get_new_deal_orders.amount=0;
	do_opration_response_get_new_deal_orders.method=NEW_DEAL_ORDERS;
	while(1)
		{
		if((get_new_deal_orders_response.order_line_array[0].id==0)||(menu_value[6]))
		{
		//memset((char*)&do_opration_response_get_new_deal_orders,0,sizeof(do_opration_response_t));
		rv = sent_revc_data(action_tmp,(void *)&in_param_do_opration_get_new_deal_orders,&do_opration_response_get_new_deal_orders);
		sleep(5);
		if(rv)
		{
		memcpy(&get_new_deal_orders_response,&do_opration_response_get_new_deal_orders.m_opration_response.get_new_deal_orders_response,sizeof(get_new_deal_orders_response_t));
		}
		}
		else
		{
		usleep(1000);
		continue;
		}
		}
}

void *thread_account_info(void * args)
{	

	action_t action_tmp = DO_OPRATION;
	in_param_do_opration_t in_param_do_opration_account_info;
	do_opration_response_t do_opration_response_account_info;
	memset((char*)&in_param_do_opration_account_info,0,sizeof(in_param_do_opration_t));
	memset((char*)&do_opration_response_account_info,0,sizeof(do_opration_response_t));
	int rv = 0;
	//get account info
	in_param_do_opration_account_info.method=ACCOUNT_INFO;
	in_param_do_opration_account_info.price=0;
	in_param_do_opration_account_info.amount=0;
	do_opration_response_account_info.method=ACCOUNT_INFO;
	while(1)
		{
		//pthread_mutex_lock(&update_info_mutex);
		rv = sent_revc_data(action_tmp,(void *)&in_param_do_opration_account_info,(void *)&do_opration_response_account_info);
		//pthread_mutex_unlock(&update_info_mutex);
		if(rv)
		{
		acount_info_response.available_btc_display = do_opration_response_account_info.m_opration_response.account_info_response.available_btc_display;
		acount_info_response.available_cny_display = do_opration_response_account_info.m_opration_response.account_info_response.available_cny_display;
		acount_info_response.net_asset = do_opration_response_account_info.m_opration_response.account_info_response.net_asset;
		have_in_hand = do_opration_response_account_info.m_opration_response.account_info_response.available_btc_display;
		asset_rmb=do_opration_response_account_info.m_opration_response.account_info_response.net_asset;
		//printf("Bit:%f.\n",have_in_hand);
		}
		else
		{continue;}
		}
}

void *thread_monitering_buy_sell(void * args)
{
	sleep(5);
	action_t action_tmp = DO_OPRATION;

	static unsigned long int tmp_order_id = 0;
	
	//---------------------------BUY-----------------------------//
	in_param_do_opration_t in_param_do_opration_buy;
	do_opration_response_t do_opration_response_buy;
	memset((char*)&in_param_do_opration_buy,0,sizeof(in_param_do_opration_t));
	memset((char*)&do_opration_response_buy,0,sizeof(do_opration_response_t));
	int rv_buy = 0;
	in_param_do_opration_buy.method=BUY;
	do_opration_response_buy.method=BUY;

	in_param_do_opration_buy.price=hq_response.ticker.sell;
	in_param_do_opration_buy.amount=0.001;
	
	//---------------------------SELL-----------------------------//
	in_param_do_opration_t in_param_do_opration_sell;
	do_opration_response_t do_opration_response_sell;
	memset((char*)&in_param_do_opration_sell,0,sizeof(in_param_do_opration_t));
	memset((char*)&do_opration_response_sell,0,sizeof(do_opration_response_t));
	int rv_sell = 0;
	in_param_do_opration_sell.method=SELL;
	do_opration_response_sell.method=SELL;

	in_param_do_opration_sell.price=hq_response.ticker.buy;
	in_param_do_opration_sell.amount=0.001;

	//--------------------------ORDER INFO-----------------------//
	in_param_do_opration_t in_param_do_opration_order_info;
	do_opration_response_t do_opration_response_order_info;
	memset((char*)&in_param_do_opration_order_info,0,sizeof(in_param_do_opration_t));
	memset((char*)&do_opration_response_order_info,0,sizeof(do_opration_response_t));
	int rv_order_info;
 	in_param_do_opration_order_info.method=ORDER_INFO;
 	in_param_do_opration_order_info.price=0;
 	in_param_do_opration_order_info.amount=0;
 	in_param_do_opration_order_info.id = 0;
 	do_opration_response_order_info.method=ORDER_INFO;
 	do_opration_response_order_info.m_opration_response.order_info_response.status = ORDER_UNUSED;
	
	//--------------------------ORDER CANCEL---------------------//
	in_param_do_opration_t in_param_do_opration_cancel_order;
	do_opration_response_t do_opration_response_cancel_order;
	memset((char*)&in_param_do_opration_cancel_order,0,sizeof(in_param_do_opration_t));
	memset((char*)&do_opration_response_cancel_order,0,sizeof(do_opration_response_t));
	int rv_order_cancel;
 	in_param_do_opration_cancel_order.method=CANCEL_ORDER;
 	in_param_do_opration_cancel_order.price=0;
 	in_param_do_opration_cancel_order.amount=0;
	do_opration_response_cancel_order.method=CANCEL_ORDER;
	//remeber to fit in
 	in_param_do_opration_cancel_order.id = 0;


	while(1)

{
	usleep(100);
	//if((update_ma_response.ma_pj+buy_under_ma_gap<=hq_response.ticker.sell)&&((have_in_hand-reserved_in_hand)>-0.0000001&&(have_in_hand-reserved_in_hand)<0.0000010))
	//if((real_time_buy>=hq_response.ticker.sell)&&((have_in_hand-reserved_in_hand)>-0.0000001)&&((have_in_hand-reserved_in_hand)<0.0000010))
	//if(((update_ma_response.ma_zd+hq_response.ticker.sell)/2-0.1>=hq_response.ticker.sell)&&(have_in_hand>-0.0000001&&have_in_hand<0.0000010))
	//if(((have_in_hand-reserved_in_hand)>-0.0000001)&&((have_in_hand-reserved_in_hand)<0.0000010))
	if(long_double_equal_zero(have_in_hand-reserved_in_hand))
	{
	//buy conditon
	//if(!(long_double_biger_zero(hq_response.ticker.buy-real_time_buy)&&long_double_biger_zero(hq_response.ticker.sell-real_time_sell)&&long_double_biger(hq_response.ticker.sell,update_ma_response.ma_sp)))
	//here we can define many condition here
	if(BELOW_TO_ABOVE!=windows_monitoring.sell_result)
		continue;


	in_param_do_opration_buy.price=hq_response.ticker.sell;
	//in_param_do_opration_buy.price=hq_response.ticker.buy;
	in_param_do_opration_buy.amount=0.1;
	rv_buy = sent_revc_data(action_tmp,(void *)&in_param_do_opration_buy,(void *)&do_opration_response_buy);

	log_all_static_data(p_log_buffer);
	
	printf("\e[1;1H\e[2J");
	printf("<-----Buy opration started:\n");
	printf("<-----Price:%f\n",in_param_do_opration_buy.price);
	printf("<-----Amount:%f\n",in_param_do_opration_buy.amount);

	log_buy_sell_data(p_log_buffer,in_param_do_opration_buy.price,in_param_do_opration_buy.amount,LOG_BUY);

	write_log_buffer_to_file(p_log_buffer);

	pthread_spin_unlock(&buy_sell_gap_spinlock);
	sleep(1);
	pthread_spin_lock(&buy_sell_gap_spinlock);

	
	if((!rv_buy))
	{
	printf("erro in sent_revc_data sent buy opration.\n");
	sleep(5);
	continue;
	}
	if(RESULT_FAIL==do_opration_response_buy.m_opration_response.sell_buy_response.result)
	{
	printf("The buy procedure result is fail with erro code %d.\n",do_opration_response_buy.m_opration_response.sell_buy_response.code);
	sleep(5);
	continue;
	}
	printf("The buy procedure reslut is sucess with id %lu.\n",do_opration_response_buy.m_opration_response.sell_buy_response.id);
	if(RESULT_SUCCSS==do_opration_response_buy.m_opration_response.sell_buy_response.result)
	{
	tmp_order_id = do_opration_response_buy.m_opration_response.sell_buy_response.id;

	in_param_do_opration_order_info.id = tmp_order_id;

 	rv_order_info= sent_revc_data(action_tmp,(void *)&in_param_do_opration_order_info,(void *)&do_opration_response_order_info);

	if(!rv_order_info)
	{
	printf("erro sent order_info opration.\n");
	break;
	//continue;
	}
 	printf("order %lu status is %d --%s.\n",in_param_do_opration_order_info.id,do_opration_response_order_info.m_opration_response.order_info_response.status,status_string_meaning[do_opration_response_order_info.m_opration_response.order_info_response.status]);
	
 	order_status_t tmp_status = do_opration_response_order_info.m_opration_response.order_info_response.status;
 
	if(ORDER_SETTLED!=tmp_status)
	{

 	in_param_do_opration_cancel_order.id = tmp_order_id;
 
 	printf("cancel order %lu because not settled.\n",tmp_order_id);
	rv_order_cancel= sent_revc_data(action_tmp,(void *)&in_param_do_opration_cancel_order,(void *)&do_opration_response_cancel_order);
	if(!rv_order_cancel)
	{
	printf("erro sent order cancel opration.\n");
	//continue;
	break;
	}
	}
	}
	printf("buy opration not success,code is %d ,casue:%s.\n",do_opration_response_buy.m_opration_response.sell_buy_response.code,erro_code_string[do_opration_response_buy.m_opration_response.sell_buy_response.code]);
	//sleep(5);
	}//for sucess buy


	//sleep(1);
	//if((update_ma_response.ma_pj>=hq_response.ticker.buy-0.5)&&((have_in_hand-reserved_in_hand)>0.0009999&&(have_in_hand-reserved_in_hand)<0.0010001))
	//if(((update_ma_response.ma_zg+hq_response.ticker.buy)/2+0.1<=hq_response.ticker.buy)&&(have_in_hand>0.0009999&&have_in_hand<0.0010010))
	//if((real_time_sell<=hq_response.ticker.buy)&&((have_in_hand-reserved_in_hand)>0.0009999)&&((have_in_hand-reserved_in_hand)<0.0010001))
	if(long_double_biger_zero(have_in_hand-reserved_in_hand))
	{
	//sell condition
	//if(!(long_double_little_zero(hq_response.ticker.buy-real_time_buy)&&long_double_little_zero(hq_response.ticker.sell-real_time_sell)&&long_double_little(hq_response.ticker.buy,update_ma_response.ma_sp)))
	//we can add so many condition here
	if(ABOVE_TO_BELOW!=windows_monitoring.buy_result)
	continue;
	in_param_do_opration_sell.price=hq_response.ticker.buy;
	//in_param_do_opration_sell.price=hq_response.ticker.sell;
	//in_param_do_opration_sell.price=cbj_tmp_sell;
	in_param_do_opration_sell.amount=0.1;
	rv_sell = sent_revc_data(action_tmp,(void *)&in_param_do_opration_sell,(void *)&do_opration_response_sell);

	log_all_static_data(p_log_buffer);
	
	printf("\e[1;1H\e[2J");
	printf("----->Sell opration started:\n");
	printf("----->Price:%f\n   ",in_param_do_opration_sell.price);
	printf("----->Amount:%f\n",in_param_do_opration_sell.amount);

	log_buy_sell_data(p_log_buffer,in_param_do_opration_sell.price,in_param_do_opration_sell.amount,LOG_SELL);
	write_log_buffer_to_file(p_log_buffer);

	pthread_spin_unlock(&buy_sell_gap_spinlock);
	sleep(1);
	pthread_spin_lock(&buy_sell_gap_spinlock);

	if((!rv_sell))
	{
	printf("erro in sent_revc_data sent sell opration.\n");
	log_all_self_define_str(p_log_buffer,"erro in sent_revc_data sent sell opration.\n");
	write_log_buffer_to_file(p_log_buffer);
	break;
	//continue;
	}
	if(RESULT_FAIL==do_opration_response_sell.m_opration_response.sell_buy_response.result)
	{
	printf("The sell procedure result is fail with erro code %d.\n",do_opration_response_sell.m_opration_response.sell_buy_response.code);
	//sleep(5);
	//continue;
	break;
	}
	printf("The sell procedure reslut is sucess with id %lu.\n",do_opration_response_sell.m_opration_response.sell_buy_response.id);

	}
}
}

int is_write_hearder_needed()
{
	long offset = 0;
	offset = ftell(fp_in);
	
	if(0==offset)
	{
	printf("New file,write header sucess.\n");
	return 1;
	}
	else
	{
	printf("No need wirte header.\n");
	return 0;
	}
}

inline void update_write_index(log_buffer_t * p_in,int rv)
{
	assert(rv>0);
	p_in->write_index =p_in->write_index+rv;
	
	if(LOG_BUFFER_SIZE<=p_in->write_index)
	{
	p_in->write_index = 0;
	p_in->item = 0;
	}
}


void log_buy_sell_data(log_buffer_t * p_in,double in_price, double in_amount,log_buy_sell_t in_buy_sell)
{

log_buffer_t * p_buffer = p_in;
double price = in_price;
double amount = in_amount;
log_buy_sell_t buy_sell = in_buy_sell;

int rv = 0;


if(LOG_BUY ==buy_sell)
{rv = sprintf(p_buffer->buffer+p_buffer->write_index,"\n<-----Buy opration started:\n");
update_write_index(p_buffer,rv);

rv = sprintf(p_buffer->buffer+p_buffer->write_index,"<-----Price:%f\n",price);
update_write_index(p_buffer,rv);

rv = sprintf(p_buffer->buffer+p_buffer->write_index,"<-----Amount:%f\n\n\n",amount);
update_write_index(p_buffer,rv);
p_buffer->item = p_buffer->item +1 ;

}

if(LOG_SELL==buy_sell)
{
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"\n----->Sell opration started:\n");
update_write_index(p_buffer,rv);

rv = sprintf(p_buffer->buffer+p_buffer->write_index,"----->Price:%f\n",price);
update_write_index(p_buffer,rv);

rv = sprintf(p_buffer->buffer+p_buffer->write_index,"----->Amount:%f\n\n\n",amount);
update_write_index(p_buffer,rv);
p_buffer->item = p_buffer->item +1 ;

}




}

void log_all_self_define_str(log_buffer_t * p_in,const char *in_string)
{
	
	log_buffer_t * p_buffer = p_in;
	
	int rv = 0;

	int string_len = strlen(in_string);

	rv = sprintf(p_buffer->buffer+p_buffer->write_index,"%s",in_string);
	if(rv==string_len)
	update_write_index(p_buffer,rv);
	else
	{
	assert(1);
	}



}




void log_all_static_data(log_buffer_t * p_in)
{

log_buffer_t * p_buffer = p_in;

int rv = 0;

rv = sprintf(p_buffer->buffer+p_buffer->write_index,"Write log file for item %d.\n",p_in->item);
update_write_index(p_buffer,rv);

rv = sprintf(p_buffer->buffer+p_buffer->write_index,"Time:%lu\n",hq_response.time);
update_write_index(p_buffer,rv);
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"Buy:%f	 Sell:%f   Vol:%f	Delt-Vol:%f\n",hq_response.ticker.buy,hq_response.ticker.sell,hq_response.ticker.vol,vol_delt);
update_write_index(p_buffer,rv);
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"Bit:%f	   Asset:%f\n",have_in_hand,asset_rmb);
update_write_index(p_buffer,rv);
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"MAZG is %Lf.\n",update_ma_response.ma_zg);
update_write_index(p_buffer,rv);
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"MAZD is %Lf.\n",update_ma_response.ma_zd);
update_write_index(p_buffer,rv);
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"MAJX is %Lf.\n",update_jx_response.jx);
update_write_index(p_buffer,rv);
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"MASP is %Lf.\n",update_ma_response.ma_sp);
update_write_index(p_buffer,rv);
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"the buy current state is %d.\n",windows_monitoring.buy_result);
update_write_index(p_buffer,rv);
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"the sell current state is %d.\n",windows_monitoring.sell_result);
update_write_index(p_buffer,rv);
//not need here
//p_buffer->item = p_buffer->item +1 ;
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"Buy:\n",p_in->item);
update_write_index(p_buffer,rv);

for(int i = 0;i<CONFIG_WINDOW_SIZE;i++)
{
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"%Lf,",windows_monitoring.hq_window.hq_array_buy[i]);
update_write_index(p_buffer,rv);
}
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"\nSell:\n",p_in->item);
update_write_index(p_buffer,rv);

for(int i = 0;i<CONFIG_WINDOW_SIZE;i++)
{
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"%Lf,",windows_monitoring.hq_window.hq_array_sell[i]);
update_write_index(p_buffer,rv);
}
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"\nJX:\n",p_in->item);
update_write_index(p_buffer,rv);

for(int i = 0;i<CONFIG_WINDOW_SIZE;i++)
{
rv = sprintf(p_buffer->buffer+p_buffer->write_index,"%Lf,",windows_monitoring.jx_window.jx_array[i]);
update_write_index(p_buffer,rv);
}


}



extern void* thread_server_send(void *args);

int main(int argc,char *argv[])
{

//hq_response_t hq_response;
//update_ma_response_t update_ma_response;
do_opration_response_t do_opration_response;
in_param_do_opration_t in_param_do_opration;

p_log_buffer = alloc_mem_for_buffer();


if((fp_in = fopen(file_name,"a")) == NULL)
{
	printf("The input file: %s is can not be open by write\n",file_name);
	return 0;
}

/*if((fp_in_log= fopen(file_name_log,"w")) == NULL)
{
	printf("The input file: %s is can not be open by write\n",file_name_log);
	return 0;
}*/


if(is_write_hearder_needed())
fprintf(fp_in,"time,buy,sell,vol,delt-vol,bit,mazg,mazd,masp,jx\n");
else
{};

int rv = pthread_mutex_init(&update_info_mutex, NULL); 

rv = pthread_spin_init(&update_info_spinlock, 0);
rv = pthread_spin_init(&read_input_spinlock, 0);
rv = pthread_spin_init(&buy_sell_gap_spinlock, 0);

if (rv != 0) 
{  
	return -1;
} 


pthread_t pid_dash_board_menu,pid_thread_monitor_key_press,pid_thread_query_hq,pid_thread_update_ma,pid_thread_account_info,pid_thread_get_new_deal_orders,pid_thread_monitering_buy_sell,pid_thread_update_jx,pid_thread_server_send; 
if(pthread_create(&pid_thread_monitor_key_press, NULL, &thread_monitor_key_press, NULL))  
{  
	return -1;	
}

if(pthread_create(&pid_thread_query_hq, NULL, &thread_query_hq, NULL))  
{  
	return -1;	
}
if(pthread_create(&pid_dash_board_menu, NULL, &thread_dash_board_menu, NULL))  
{  
	return -1;	
}

if(pthread_create(&pid_thread_update_ma, NULL, &thread_update_ma, NULL))  
{  
	return -1;	
} 
if(pthread_create(&pid_thread_update_jx, NULL, &thread_update_jx, NULL))  
{  
	return -1;	
} 


if(pthread_create(&pid_thread_account_info, NULL, &thread_account_info, NULL))  
{  
	return -1;	
} 

if(pthread_create(&pid_thread_get_new_deal_orders, NULL, &thread_get_new_deal_orders, NULL))	
{  
	return -1;	
} 

if(pthread_create(&pid_thread_server_send, NULL, &thread_server_send, sever_data))  
{  
	return -1;	
}

if(pthread_create(&pid_thread_monitering_buy_sell, NULL, &thread_monitering_buy_sell, NULL))  
{  
	return -1;	
}

while (1)
{	
	
	
	if((1==menu_value[7]))
	{
	fclose(fp_in);
	fp_in = NULL;
	exit(0);
	}
	usleep(100);
	/*char sk_cmd;
	
	pthread_spin_lock(&read_input_spinlock);
	sk_cmd = getchar();
	while (getchar() != '\n');
	pthread_spin_unlock(&read_input_spinlock);
	
	if('q' == sk_cmd)
	{	
	fclose(fp_in);
	fp_in = NULL;
	exit(0);
	}
	int menu_value_tmp = sk_cmd - '0';
	if((0<menu_value_tmp)&&(menu_value_tmp<=8))
	{
	pthread_spin_lock(&menu_spinlock);
	if(0==menu_value[menu_value_tmp-1])
	menu_value[menu_value_tmp-1]= 1;
	else
	menu_value[menu_value_tmp-1]= 0;
	pthread_spin_unlock(&menu_spinlock);
	}*/
	

}

}

