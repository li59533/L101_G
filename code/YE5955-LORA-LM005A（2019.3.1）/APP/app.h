#ifndef APPH
#define APPH
#define  CloseAllInt() { __disable_irq();}
#define OpenAllInt() { __enable_irq();}
#
#include <stdio.h>  
#include <stdint.h>  
#include <string.h>  
#include <stdbool.h>
#include <stdarg.h>

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#define RX_BUF_MAX_LEN     96                                     //最大接收缓存字节数

#define     macLM005A_Usart( fmt, ... )           USART_CMD_LM005A(fmt, ##__VA_ARGS__  )
#define     macLM005A_Usart_data( fmt, ... )           USART_CMD_LM005A_data(fmt, ##__VA_ARGS__  )


typedef struct CONFIG				 // 配置信息
{
	uint16_t vaildsign;

	uint16_t baundrate;    /* =0:4800    =1:9600 		=2:38400 		=3:115200  		 */
	uint16_t addr; 
	uint16_t ParityType;
	uint16_t DataLength;
	uint16_t StopBit;
	uint16_t SNnumber; 
	uint8_t parity;		// =0 : n,8,1   =1: o,8,1  =2: e,8,1	 数据格式
	
	uint8_t DisplayMode;  // 显示模式　=0　固定　=1 循环
	uint8_t interface_type[12]; // 输入类型
	uint8_t unit[12];  // 单位
	uint32_t scale[12]; // 转换系数
	uint32_t adjust[12]; // 修正值
	uint16_t interface_addr[12]; // modbus 地址 上传
	uint8_t abs[12]; // 绝对值
	uint8_t means	;// 均值类型
	uint16_t means_times; // 均值积算周期
	uint16_t freq;  // 采样频率 Hz
	uint16_t avr_count;
	uint8_t yellow_ch,blue_ch,green_ch,red_ch; // 显示波形通道
	uint8_t reflash; // 刷新时间 
	uint16_t din_addr;  //  开关量输入寄存器地址
	uint16_t dout_addr; //  开关量输出寄存器地址
	uint16_t gate0,gate1,gate2,gate3;  // 动作阈值
	uint32_t backlash0,backlash1,backlash2,backlash3; // 动作回差
	uint32_t vibrate_gate,vibrate_backlash;  // 振动启动阈值，回差
	uint32_t force_gate,force_backlash;  // 应变启动阈值， 回差。
	uint16_t max_addr0,max_addr1;        //
	uint16_t vlpsseconds;            //  
	uint16_t vlprseconds;           //
	uint16_t runseconds;           //
	uint16_t pga;                 //
	uint16_t workcycleminutes;   //  工作周期
//	uint16_t fangda;            //  放大倍数
	uint16_t boardset;         // 发射功率设置
	uint16_t ADtime;          //AD单次采样时间
	uint16_t ADfrequence;    //AD采样频率
	  
	uint64_t alarmminutetime;  //开始报警时间
	uint32_t FLASH_WRADDR;
	uint32_t current_boardtime;
	
	uint8_t  displacement;
	uint8_t  netack;
	
	float    ADCscale;     //AD系数
	float    LMDscale;     //灵敏度系数
	float    CRTscale;     //修正系数
}CONFIG;
extern  struct CONFIG  config;  //配置信息


extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	volatile uint16_t FramHeadIndex    ;
  union {
    volatile uint16_t InfAll;
    struct {
		  volatile uint16_t FramLength       :15;                               // 14:0 	  
			volatile uint16_t FramFinishFlag   :1;                              // 15 
	  } InfBit;
	
  }; 
	
} strLM005A_Fram_Record;


/* TYPE_HBR 全桥 */
typedef enum {TYPE_NONE=0,TYPE_MA=1,TYPE_V=2, TYPE_IEPE=3,TYPE_PT=4,TYPE_HBR=5,TYPE_AC_V=6,} INTERFACE_TYPE;
typedef enum {UNIT_NONE=0,UNIT_V=1,UNIT_A=2,UNIT_KV=3,UNIT_TEMP=4,UNIT_M=5,UNIT_M_S=6,UNIT_M_S2=7,UNIT_G=8,UNIT_MA=9,UNIT_DB=10,UNIT_KG=11,} INTERFACE_UNIT;
typedef enum {MEANS_NONE=0,
							MEANS_MEANS, // 均值
							MEANS_RMS, // 均方根值
							MEANS_P,  // 峰值
							MEANS_PP, // 峰峰值
} MEANS_TYPE; 

typedef enum {BAUND2400=0,BAUND4800,BAUND9600,BAUND19200,BAUND38400,} BAUND_RATE;
typedef enum {Coordinator_Mode=0,Normal_Mode,LowPower_Mode,Sleep_Mode,} E70_MODE;
//typedef enum {BAM4E33=0,BAM4E31=1, BAM4I33=2,BAM4I31=3,BAM4U33=4,BAM4U31=5,BAM4H33=6,BAM4H31=7,BAM4P33=8,BAM4P31=9,BAM4Q33=10,BAM4Q31=11}MODEL ;
extern uint8_t 	brightness; 
	
typedef struct PARAMETER				 // 所有参数
{
	uint16_t vaildsign;
	int32_t int_v[12];
	int64_t s[12]; // 累积
	float v[12];
	float adate;                //加速度
	float vdate;                //速度
	float xdate;                //位移
	float pdate;                //温度
	float scale_v[12];
	int32_t gate[12];
	int32_t backlash[12];
	uint8_t alarm[12]; // 报警标志
	uint16_t din;  // 低8位对应 8个通道输入
	uint16_t dout; // 低4位对应 4个输出通道 
	uint16_t status;
	int32_t ch1_int_max,ch2_int_max;
	int32_t ch1_int_max1,ch2_int_max2;
	float ch1_max,ch2_max;
	int32_t vibrate_gate1,vibrate_gate2,vibrate_backlash1,vibrate_backlash2;
	int32_t force_gate,force_backlash;
	int16_t selfpgaallow;
	uint16_t daytime;
	uint16_t minutetime;
	uint16_t selffangda;
	uint16_t alarmminutetime;
	uint16_t battery;
} PARAMETER;




				
struct ADJUSTDATA					 // 校准数据
{	uint16_t vaildsign;
	uint32_t  UaSub,UbSub,UcSub;
	uint32_t IaSub,IbSub,IcSub;
	int32_t PhaseA,PhaseB,PhaseC;
	uint32_t  CVSub,OVSub;
};

extern  struct PARAMETER Parameter;


#define  COMMAND_REPLY_DISPLACEMENT       0x00
#define  COMMAND_SET_DISPLACEMENT         0x01
#define  COMMAND_REPLY_ACK                0x02
#define  COMMAND_SET_ACK                  0x03
#define  COMMAND_REPLY_RATE               0x04
#define  COMMAND_SET_RATE                 0x05
#define  COMMAND_REPLY_MEASURE            0x06
#define  COMMAND_REPLY_ADC                0x08
#define  COMMAND_SET_ADC                  0x07
#define  COMMAND_REPLY_LMD                0x0a
#define  COMMAND_SET_LMD                  0x09
#define  COMMAND_REPLY_CRT                0x0c
#define  COMMAND_SET_CRT                  0x0b
#define  COMMAND_REPLY_ADDR               0x0e
#define  COMMAND_SET_ADDR                 0x0d



#define isRunning() ((Parameter.status&0x01)!=0)
#define setRunning() {Parameter.status|=0x01;}
#define clrRunning() {Parameter.status&=~0x01;}

#define isForce() ((Parameter.status&0x02)!=0)
#define setForce() {Parameter.status|=0x02;}
#define clrForce() {Parameter.status&=~0x02;}

#define isPreheat() ((Parameter.status&0x04)!=0)
#define setPreheat() {Parameter.status|=0x04;}
#define clrPreheat() {Parameter.status&=~0x04;}

#endif

