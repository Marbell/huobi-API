#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  

#include "sign_md5.h"

extern int sign_md5(unsigned char* input,unsigned char *out_put)
{
	MD5_CTX md5;
	MD5Init(&md5);         
	int i;
	//unsigned char encrypt[] ="admin";//21232f297a57a5a743894a0e4a801fc3
	unsigned char *encrypt = input;
	//unsigned char decrypt[16];    
	MD5Update(&md5,encrypt,strlen((char *)encrypt));
	//MD5Final(&md5,decrypt); 
	MD5Final(&md5,out_put); 
	//printf("pre-encode:%s\npost-enocde:",encrypt);
	for(i=0;i<16;i++)
	{
		//printf("%02x",decrypt[i]);
		//printf("%02x",out_put[i]);
	}
	//printf("\n");
	return 0;
}
