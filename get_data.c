#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

#include "opration.h"
#include "process_response.h"
#include "get_data.h"


void error(const char *msg) { perror(msg); exit(0); }

extern int sent_revc_data(action_t in_action,void * in_ptr,void *out_ptr)

{

	hq_response_t * p_hq_response = NULL;
	update_ma_response_t * p_ma_response = NULL;
	update_jx_response_t * p_jx_response = NULL;
	do_opration_response_t * p_do_op_response= NULL;
	in_param_do_opration_t * p_in_param = NULL;

	post_data_t tmp;
  	memset((char*)&tmp,0,sizeof(post_data_t));
	post_data_t * p_post = NULL;
	
	if(QUERY_HQ==in_action)
	{
	p_hq_response = (hq_response_t *)out_ptr;
	}

	if(UPDATE_MA==in_action)
	{
	p_ma_response = (update_ma_response_t *)out_ptr;
	}
	if(UPDATE_JX==in_action)
	{
	p_jx_response = (update_jx_response_t *)out_ptr;
	}
	if(DO_OPRATION==in_action)
	{

	p_do_op_response= (do_opration_response_t  *)out_ptr;
	}

	if(NULL!=in_ptr)
	{
	p_in_param=(in_param_do_opration_t *)in_ptr;

	}

	
    /* first what are we going to send and where are we going to send it? */
	//!!!!!!!remember to change port whatever you wanted to do somthing!!!!
    int portno =  8000;
   const char *host =        "xxx.xxx.xxx.xxx";//http proxy,use your own
   //const char *host =        "api.huobi.com";
   // const char *api_adress = "POST https://api.huobi.com/apiv3?";

   //const char *request_url = "Request URL: ";
   // const char *http_method = "Request Method:GET\r\n";
   const char * http_method_post = "POST";
   const char * http_method_get = "GET";
    const char *api_adress = " https://api.huobi.com/apiv3?";
    const char *hq_adress = " http://api.huobi.com/staticmarket/ticker_btc_json.js";
    const char *ma_adress = " http://api.huobi.com/staticmarket/btc_kline_001_json.js";
	const char *jx_adress = " http://api.huobi.com/staticmarket/td_btc.js";

   // const char *api_adress = "POST https://api.huobi.com/api.php?";

    const char *http_type = " HTTP/1.0\n";
    const char *host_name =   "Host: api.huobi.com\n";
    const char *connection =   "Connection:keep-alive\n";
    const char *accept  =          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0\n";
    const char *accept_language = "Accept-Language: zh-CN,zh;q=0.8\n";
    const char *content_type = "Content-Type:application/x-www-form-urlencoded\r\n";

	const char *post_end = "\r\n";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char message[1024],response[20480];

	
if(DO_OPRATION==in_action)
{
 //method_t method_tmp = GET_ORDERS;
// method_t method_tmp = ACCOUNT_INFO;
//method_t method_tmp = NEW_DEAL_ORDERS;
//method_t method_tmp =SELL;
//method_t method_tmp =ORDER_ID_BY_TRADE_ID;
//method_t method_tmp =CANCEL_ORDER;
//method_t method_tmp=ORDER_INFO;
//method_t method_tmp=BUY;
  method_t method_tmp = p_in_param->method;
  tmp.method_vlaue = METHOD_NO_USE;
  tmp.market_value = MARKET_CNY;
  tmp.coin_type_value= COIN_BIT;
//553157649
   if(!build_post_data(method_tmp,&tmp,p_in_param->price,p_in_param->amount,p_in_param->id))
{
	printf("build_post_data failed.\n");
	return 0;
}
   p_post = &tmp;
}

   memset(message,0,1024);
 //  strcat(message,request_url);
if((DO_OPRATION==in_action)||(QUERY_HQ==in_action))
{
strcat(message,http_method_post);
}
if((UPDATE_MA==in_action)||(UPDATE_JX==in_action))
{
strcat(message,http_method_get);
}

if(DO_OPRATION==in_action)
{
 strcat(message,api_adress);
 strcat(message,p_post->data);
}

if(QUERY_HQ==in_action)
{
strcat(message,hq_adress);
}
if(UPDATE_MA==in_action)
{
strcat(message,ma_adress);
}
if(UPDATE_JX==in_action)
{
strcat(message,jx_adress);
}



strcat(message,http_type);
//   strcat(message,http_method_post);
//  strcat(message,host_name);
if(DO_OPRATION==in_action)
{
strcat(message,connection);
strcat(message,accept);
strcat(message,accept_language);
strcat(message,content_type);
}
strcat(message,post_end);
strcat(message,post_end);
//printf("\nRequest:\n%s\n",message);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    memset(response,0,sizeof(response));
    total = sizeof(response)-1;
    received = 0;
    do {
        bytes = read(sockfd,response+received,total-received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket");

    /* close the socket */
    

    /* process response */
    //printf("Response:\n%s\n",response);
    if(!pre_process_response(response))
	{
	printf("erro, not get sucess while do HTTP request.\n");
	return 0;
	}
	
	if(QUERY_HQ==in_action)
	{
    process_hq_response(response, p_hq_response);
	}

	if(DO_OPRATION==in_action)
	{
	process_do_opration_response(response,p_do_op_response);
	}
    if(UPDATE_MA==in_action)
    {
	process_ma_response(response, p_ma_response);
	}
	if(UPDATE_JX==in_action)
    {
	process_jx_response(response, p_jx_response);
	}
    free_post_data_type(&tmp);

	close(sockfd);
    return 1;
}
