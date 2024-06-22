#ifndef __CONFIG_H
#define __CONFIG_H


/*
	模版
	#if Print_Debug
	printf("\n");
	#endif
*/
#define Print_Debug    0     						//调试信息串口打印
#define USE_GSM800C    1     						//编译GSM发送短信部分
#define PrintLC        0     						//打印键值
#define Buffeting_time 150   						//按键消抖时间(ms)
#define Beep_frequency  1000 						//报警提示音频率(ms)
#define Number_of_password_errors 3     //报警错误次数

sbit RELAY1=P2^4;
sbit RELAY2=P2^5;
sbit RELAY3=P2^6;
sbit KEYL1=P0^3;
sbit KEYL2=P0^2;
sbit KEYL3=P0^1;
sbit KEYL4=P0^0;
sbit KEYH=P0^7;
sbit BEEP=P2^7;
sbit LED = P3^7;
#define ON 0;
#define OFF 1;
#define KeyPort P0


#endif