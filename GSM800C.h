#ifndef __GSM800C_H
#define __GSM800C_H

#include <STC89C5xRC.H>
typedef   unsigned char  u8;
typedef   unsigned int   u16;
typedef   unsigned long  u32;

typedef struct GMS800C{
	char phone1[11];
	char phone2[11];
	char* Sel_Phone;
	char PhoneToUnicode[45];
	char Timer0_start;
}GMS800C;
GMS800C* GET_GMS();
#define p_GMS GET_GMS()

//static unsigned char *SMS_Num="\"+8613800816500\"";//短信中心号

#define Automatic_Startup 1     //定义自启动 V1.2版本起有自启动功能
#define Buf1_Max 60 					  //串口1缓存长度
//串口1发送回车换行
#define UART1_SendLR() UART1_SendData(0X0D);\
											 UART1_SendData(0X0A)


void Uart1Init(void);		//9600bps@11.0592MHz
void CLR_Buf1(void);    //清串口接收缓存
u8 Find(u8 *a);         //查找字符串
void Second_AT_Command(u8 *b,u8 *a,unsigned char Wait_Time);  //发送指令
void Wait_CREG(void);   //查询等待模块注册成功
void Set_Text_Mode(void); //设置短信为TEXT模式
void Send_AuTH(char* Phone);
void Make_AuthTo6(char* Unicode);
void Auth_Code(char* arr,char Length);
void ASCII_ToUnicode(char *arr,const char *str,char Length);


#endif
