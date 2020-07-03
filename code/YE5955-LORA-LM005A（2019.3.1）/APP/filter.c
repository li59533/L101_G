#include "filter.h"
#include "paramaters.h"




//四阶抗混叠滤波器由两级二阶波特沃斯低通滤波器级联
//滤波系数                   3dB //8k                    4k                3.2k               1.6k                0.8k                      0.4k                   0.2k
//const int32_t b1[7][3] ={{16600,33201,16600},{5109,10218,5109},     {3422,6845,3422},      {940,1880,940},   {247,493,247},           {63,126,63},           {16,32,16}};           
//const int32_t b2[7][3] ={{12052,24105,12052},{4056,8111,4056},       {2805,5610,2805},    {837,1674,837},    {232,463,232},           {61,122,61},           { 16,31,16}};          

//const int32_t a1[7][3] ={{65536,-29696,30561},{65536,-86567,41467},{65536,-96980,45133},{65536,-115922,54146},{65536,-124087,59537},{65536,-127744,62460},{65536,-129451,63979}};
//const int32_t a2[7][3] ={{65536,-21560,4233},{ 65536,-68721,19408},{65536,-79483,25166},{65536,-103234,41047},{65536,-116544,51934},{65536,-123641,58349},{65536,-127313,61840}};


//因为不要用浮点数计算，所以所有系数均乘以65536	
const int32_t  b0[3] ={65536,-131072,65536};  //20K采样，4Hz高通滤波系数
const int32_t  b1[3] ={65536,131072,65536};   //20K采样，8KHz低通滤波系数
const int32_t  b2[3] ={65536,-131072,65536};  //40K采样，4Hz高通滤波系数
const int32_t  b3[3] ={65536,131072,65536};   //40K采样，8KHz低通滤波系数


const int32_t  a0[3] ={65536,-130958,65422};  //20K采样，4Hz高通滤波系数
const int32_t  a1[3] ={65536,69944,24947};    //20K采样，8KHz低通滤波系数
const int32_t  a2[3] ={65536,-131015,65478};  //40K采样，4Hz高通滤波系数
const int32_t  a3[3] ={65536,-26508,13149};   //40K采样，8KHz低通滤波系数


//static float  x_0,x_1,x_2;
//static float  y_0,y_1,y_2;
int32_t  x_0,x_1,x_2;
int32_t  y_0,y_1,y_2;


#define   GAIN0     0.9991326258127       //20K采样，二阶4Hz高通滤波增益   
#define   GAIN1     0.6119806329882       //20K采样，二阶8KHz低通滤波增益    
#define   GAIN2     0.9995662188233       //40K采样，二阶4Hz高通滤波增益  
#define   GAIN3     0.1990398655003       //40K采样，二阶8KHz低通滤波增益  


int32_t  Filter(int32_t  input_value)        //20K采样，4Hz一阶高通滤波
{
//	int32_t  middle_value = 0;
	float  middle_value = 0;
	
	x_1 = input_value;
	
//	middle_value =(x_1-x_0+y_0)*255;
	middle_value =(x_1-x_0+y_0)*0.9997;//截止频率4Hz    0.9987     //截止频率1Hz   0.9997

//	y_1 = middle_value>>8;        //除以65536，还原
	y_1 = middle_value;
	
	y_0 = y_1;
	x_0 = x_1;

	return	y_1;
}


float  Filter0(int32_t input_value)        //20K采样，4Hz高通滤波
{
//	float middle_value = 0;
	int32_t  middle_value = 0;
	x_0 = input_value;
	middle_value = b0[0]*x_0+b0[1]*x_1+b0[2]*x_2-a0[1]*y_1-a0[2]*y_2;
	y_0 = middle_value>>16;        //除以65536，还原(去除增益，近似1)
	y_2 = y_1;
	y_1 = y_0;
	x_2 = x_1;
	x_1 = x_0;

	return	y_0;
}


float  Filter1(int32_t input_value)        //20K采样，8KHz低通滤波    
{
	float middle_value = 0;
	
	x_0 = input_value;
	middle_value = b1[0]*x_0+b1[1]*x_1+b1[2]*x_2-a1[1]*y_1-a1[2]*y_2;
	y_0 = middle_value*GAIN1/65536;        //除以65536，还原
	y_2 = y_1;
	y_1 = y_0;
	x_2 = x_1;
	x_1 = x_0;

	return	y_0;
}


float  Filter2(int32_t input_value)        //40K采样，4Hz高通滤波
{
	float middle_value = 0;
	
	x_0 = input_value;
	middle_value = b2[0]*x_0+b2[1]*x_1+b2[2]*x_2-a2[1]*y_1-a2[2]*y_2;
	y_0 = middle_value*GAIN2/65536;        //除以65536，还原
	y_2 = y_1;
	y_1 = y_0;
	x_2 = x_1;
	x_1 = x_0;

	return	y_0;
}


float  Filter3(int32_t input_value)        //40K采样，8KHz低通滤波
{
	float middle_value = 0;
	
	x_0 = input_value;
	middle_value = b3[0]*x_0+b3[1]*x_1+b3[2]*x_2-a3[1]*y_1-a3[2]*y_2;
	y_0 = middle_value*GAIN3/65536;        //除以65536，还原
	y_2 = y_1;
	y_1 = y_0;
	x_2 = x_1;
	x_1 = x_0;

	return	y_0;
}

