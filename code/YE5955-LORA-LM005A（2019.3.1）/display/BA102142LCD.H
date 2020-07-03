#ifndef BA215102LCDH
#define  BA215102LCDH
#define DISPBUFLEN	512

#include "conf.h"

#define dispU() {SetBit(DispBuf[47],6);}
#define dispI() {SetBit(DispBuf[0],5);}
#define dispP() {SetBit(DispBuf[0],4);}
#define dispPF() {SetBit(DispBuf[20],2);}
#define dispQ() {SetBit(DispBuf[47],5);}

#define disp3P3L() {SetBit(DispBuf[0],6);}
#define disp3P4L() {SetBit(DispBuf[0],7);}

#define dispZC() {SetBit(DispBuf[20],3);}
#define dispZL() {SetBit(DispBuf[47],7);}

#define dispKA() {SetBit((DispBuf[9]),4);SetBit((DispBuf[9]),5);}		 //
#define dispKW() {SetBit((DispBuf[10]),1);}			//
#define dispMW() {SetBit((DispBuf[10]),0);}			//
#define dispRATE() {SetBit((DispBuf[37]),3);}
#define dispMVAR() {SetBit((DispBuf[9]),7);}	   //
#define dispKVAR() {SetBit((DispBuf[9]),6);}	   //
#define dispA() {SetBit((DispBuf[9]),5);}
#define dispKV() {SetBit((DispBuf[10]),3);SetBit((DispBuf[10]),2);}	  //
#define dispV() {SetBit((DispBuf[10]),2);}						//

#define dispKWH() {SetBit((DispBuf[10]),4);}			//
#define dispMWH() {}			//
#define dispHz() {SetBit((DispBuf[10]),6);}			//

#define dispKVARH() {SetBit((DispBuf[10]),5)}
#define dispKVAH() {}

#define dispES() {echoChar(L_S,8);}   //  显示视在电能标志   ///////////
#define dispEP() {echoChar(L_P,8);}   // 显示 有功电能标志   //////////
#define dispEQ() {echoChar(L_Q,8);}  // 显示无功电能标志     ///////////

#define dispSignLine1() {SetBit((DispBuf[19]),4);}
#define dispSignLine2() {SetBit((DispBuf[22]),3);}
#define dispSignLine3() {SetBit((DispBuf[20]),4);}

#define disp485COM() {}	// 显示 RS485 通讯标志
#define hide485COM() {}	// 停止显示 RS485 通讯标志

#define dispMENU() {}
#define hideMENU() {}

extern void unFlashByte(uint8 lines,uint8 pos);
extern void flashByte(uint8 lines,uint8 pos);


#endif
