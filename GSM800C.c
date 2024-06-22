#include "gsm800c.h"
#include "string.h"
#include "stdio.h"
#include "LCD12864.H"
#include <stdlib.h>  
#include "config.h"
idata u8 Uart1_Buf[Buf1_Max];
idata u8 Flag;//定时器标志位
u8 First_Int = 0;

idata GMS800C GMS;
GMS800C* GET_GMS()
{
	return &GMS;
}

code const char authcode[]="667a80fd5feb901267dcff0c9a8c8bc17801ff1a"; //验证码：Unicode

char Rand_AuthCode(char max,char min)
{
  return rand() % (max - min + 1) + min;
}

void Auth_Code(char* arr,char Length)
{
	char i=0;
	for(i=0;i<Length;i++)
	arr[i] = Rand_AuthCode(9,0)+'0';
}
void ASCII_ToUnicode(char *arr,const char *str,char Length) 
{		
	  char i=0;
	  char temp[4]="003";
    while (i != Length) {  
      temp[3] = str[i]; 
			memcpy(arr+(i*4),temp,4);
      i++;  
    }  
} 

void Auth_CodeToUnicode(char* Unicode,const char* arr,char Length)
{
		ASCII_ToUnicode(Unicode,arr,Length);
}
	
void Make_AuthTo6(char* Unicode)
{
	Auth_Code(p_PageDATA->AuthCode,6);
	Auth_CodeToUnicode(Unicode,p_PageDATA->AuthCode,6);
}

void delay1ms(void)   //误差 -0.651041666667us
{
    unsigned char a,b;
    for(b=102;b>0;b--)
        for(a=3;a>0;a--);
}

void delayms(unsigned int time)   //误差 -0.651041666667us
{
	u16 b;
    for(b=time;b>0;b--)
       delay1ms();
}


void Uart1Init(void)		//9600bps@11.0592MHz
{
    TMOD |= 0x20;
    SCON = 0x50;
    TH1 = 0xFD;
    TL1 = TH1;
    PCON = 0x00;
    EA = 1;
    ES = 1;
    TR1 = 1;
}
/*----------------------------
UART1 发送串口数据
-----------------------------*/
void UART1_SendData(u8 dat)
{
	ES=0;					//关串口中断
	SBUF=dat;			
	while(TI!=1);	//等待发送成功
	TI=0;					//清除发送中断标志
	ES=1;					//开串口中断
}

char putchar(char c){
	//输出重定向到串口
	UART1_SendData(c);
	return c;  //返回给函数的调用者printf
}

void UART1_SendString(char *s)
{
	while(*s)//检测字符串结束符
	{
		UART1_SendData(*s++);//发送当前字符
	}
}


void CLR_Buf1(void)
{
	u16 k;
	for(k=0;k<Buf1_Max;k++)      //将缓存内容清零
	{
		Uart1_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}


u8 Find(u8 *a)
{ 
  if(strstr(Uart1_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

void Second_AT_Command(u8 *b,u8 *a,unsigned char Wait_Time)         
{
	u8 i;
	u8 *c;
	c = b;										//保存字符串地址到c
	CLR_Buf1(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a))           //查找需要应答的字符
		{
			if(p_GMS->Timer0_start == 0)//超时重新发送命令
			{
				b = c;						 //将字符串地址给b
				for (b; *b!='\0';b++)
				{
					UART1_SendData(*b);
				}
				UART1_SendLR();	
				delayms(Wait_Time);
				p_GMS->Timer0_start = 1;  //开始计时
		   }
    }
		else
		{
				p_GMS->Timer0_start = 0;  //停止计时
				i = 1;//退出等待
		}
	}
	CLR_Buf1(); 
}
void Set_Text_Mode(void)
{
	unsigned char temp[50]="AT+CSCA=";
	Second_AT_Command("ATE0","OK",0);										  //取消回显	
	Second_AT_Command("AT+CMGF=1","OK",0);								//TEXT模式	
	Second_AT_Command("AT+CSMP=17,167,1,8","OK",100);
	Second_AT_Command("AT+CSCS=\"UCS2\"","OK",0);         //设置字符集
}

void Send_AuTH(char* Phone)
{
	unsigned char temp[55]="AT+CMGS=\"";
	ASCII_ToUnicode(p_GMS->PhoneToUnicode,Phone,11);       //电话转Unicode
	strcat(temp,p_GMS->PhoneToUnicode);										 //字符串拼接函数（库函数）
	strcat(temp,"\""); 																		 //字符串拼接函数（库函数）
	Second_AT_Command(temp,">",0); 												 //等待接收到“>”才发送短信内容
  Make_AuthTo6(p_PageDATA->AuthCodeToUnicode);					 //生成6位验证码
	printf("%s%s",authcode,p_PageDATA->AuthCodeToUnicode);//验证码随机数
	UART1_SendData(0X1A);          //发送结束符
}
	
void Wait_CREG(void)
{
	u8 i;
	u8 k;
	i = 0;
	CLR_Buf1();
  while(i == 0)        			
	{
		CLR_Buf1();
		UART1_SendString("AT+CREG?");//查询模块网络注册状态
		UART1_SendLR();
		delayms(1500);
		 LED = !LED;	 
	    for(k=0;k<Buf1_Max;k++)      			
    	{
			if(Uart1_Buf[k] == ':')
			{
				if((Uart1_Buf[k+2] == '0')||(Uart1_Buf[k+4] == '1')) //表明网络注册成功
				{
					i = 1;
				  return;
				}
			}
		}
	}
}
void Uart() interrupt 4
{
    if (RI)
    {
      RI = 0;                 //清除RI位
			Uart1_Buf[First_Int++] = SBUF;  	  //将接收到的字符串存到缓存中
			if(First_Int == Buf1_Max-1)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			}
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
    }
}

