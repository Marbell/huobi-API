#include <stdio.h>
#include <stdlib.h>

#include "log_buffer.h"


char const * file_name_log = "data_log.log";


extern log_buffer_t *alloc_mem_for_buffer()
{

log_buffer_t * p = NULL;

p = (log_buffer_t *)malloc(sizeof(log_buffer_t));

if(NULL==p)
{
printf("alloc memory for log buffer failed.\n");
return p;
}
p->item = 0;
p->write_index = 0;
return p;
}

extern void destory_log_buffer(log_buffer_t * p_in)
{
assert(p_in);
free(p_in);
}

extern void write_log_buffer_to_file(log_buffer_t *p_in)
{
assert(p_in);
FILE * fp_in_log = NULL;
//assert(fp_in_log);
if((fp_in_log= fopen(file_name_log,"w")) == NULL)
{
	printf("The input file: %s is can not be open by write\n",file_name_log);
	assert(1);
}

unsigned count  = p_in->write_index;
fwrite(p_in->buffer,count,1,fp_in_log);

fclose(fp_in_log);
}




