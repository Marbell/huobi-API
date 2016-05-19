#include <stdio.h> 
#include <stdlib.h> 

#include "misc.h"

/*
for long double we think 0.000001 = 0; -0.000001 = 0;
long double a;
if(0==a)
erro may happended.
*/
extern bool long_double_equal_zero(long double a_in)

{
if((0.000001>=a_in)&&(-0.000001<=a_in))
//if(0==a_in)
return true;
else 
return false;
}

extern bool long_double_biger_zero(long double a_in)
{	
	if(long_double_equal_zero(a_in))
		return false;
	if((0.000001>=a_in))
		return false;
	else
		return true;


}
extern bool long_double_little_zero(long double a_in)
{
	if(long_double_equal_zero(a_in))
		return false;
	if((-0.000001>=a_in))
		return true;
	else
		return false;

}



extern bool long_double_equal(long double a_in,long double b_in)
{
	long double tmp = 0;
	tmp = a_in - b_in;
	if(long_double_equal_zero(tmp))
	return true;
	else
	return false;
}

extern bool long_double_biger(long double a_in,long double b_in)
{
	long double tmp = 0;
	tmp = a_in - b_in;
	if(long_double_equal_zero(tmp))
	return false;
	if(long_double_biger_zero(tmp))
	return true;
	else
	return false;
}

extern bool long_double_little(long double a_in,long double b_in)
{
	long double tmp = 0;
	tmp = a_in - b_in;
	if(long_double_equal_zero(tmp))
	return false;
	if(long_double_biger_zero(tmp))
	return false;
	else
	return true;
}


//use for test
/*int main(int agrc, char *argv[])

{
long double in_put_a = 0.000000;
long double in_put_b = -0.000002;

printf("input a is %Lf.\n",in_put_a);

printf("input b is %Lf.\n",in_put_b);



if(long_double_equal_zero(in_put_a))
printf("%Lf is equal zero.\n",in_put_a);
else
printf("%Lf is not equal zero.\n",in_put_a);

if(long_double_equal_zero(in_put_b))
printf("%Lf is equal zero.\n",in_put_b);
else
printf("%Lf is not equal zero.\n",in_put_b);

if(long_double_biger_zero(in_put_a))
printf("%Lf is big zero.\n",in_put_a);

if(long_double_little_zero(in_put_b))
printf("%Lf is little zero.\n",in_put_b);

if(long_double_equal(in_put_a,in_put_b))
printf("equal.\n");
else
printf("not equal.\n");





}*/

