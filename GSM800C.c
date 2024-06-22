#include "gsm800c.h"
#include "string.h"
#include "stdio.h"
#include "LCD12864.H"
#include <stdlib.h>  
#include "config.h"
idata u8 Uart1_Buf[Buf1_Max];
idata u8 Flag;//��ʱ����־λ
u8 First_Int = 0;

idata GMS800C GMS;
GMS800C* GET_GMS()
{
	return &GMS;
}

code const char authcode[]="667a80fd5feb901267dcff0c9a8c8bc17801ff1a"; //��֤�룺Unicode

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

void delay1ms(void)   //��� -0.651041666667us
{
    unsigned char a,b;
    for(b=102;b>0;b--)
        for(a=3;a>0;a--);
}

void delayms(unsigned int time)   //��� -0.651041666667us
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
UART1 ���ʹ�������
-----------------------------*/
void UART1_SendData(u8 dat)
{
	ES=0;					//�ش����ж�
	SBUF=dat;			
	while(TI!=1);	//�ȴ����ͳɹ�
	TI=0;					//��������жϱ�־
	ES=1;					//�������ж�
}

char putchar(char c){
	//����ض��򵽴���
	UART1_SendData(c);
	return c;  //���ظ������ĵ�����printf
}

void UART1_SendString(char *s)
{
	while(*s)//����ַ���������
	{
		UART1_SendData(*s++);//���͵�ǰ�ַ�
	}
}


void CLR_Buf1(void)
{
	u16 k;
	for(k=0;k<Buf1_Max;k++)      //��������������
	{
		Uart1_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
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
	c = b;										//�����ַ�����ַ��c
	CLR_Buf1(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a))           //������ҪӦ����ַ�
		{
			if(p_GMS->Timer0_start == 0)//��ʱ���·�������
			{
				b = c;						 //���ַ�����ַ��b
				for (b; *b!='\0';b++)
				{
					UART1_SendData(*b);
				}
				UART1_SendLR();	
				delayms(Wait_Time);
				p_GMS->Timer0_start = 1;  //��ʼ��ʱ
		   }
    }
		else
		{
				p_GMS->Timer0_start = 0;  //ֹͣ��ʱ
				i = 1;//�˳��ȴ�
		}
	}
	CLR_Buf1(); 
}
void Set_Text_Mode(void)
{
	unsigned char temp[50]="AT+CSCA=";
	Second_AT_Command("ATE0","OK",0);										  //ȡ������	
	Second_AT_Command("AT+CMGF=1","OK",0);								//TEXTģʽ	
	Second_AT_Command("AT+CSMP=17,167,1,8","OK",100);
	Second_AT_Command("AT+CSCS=\"UCS2\"","OK",0);         //�����ַ���
}

void Send_AuTH(char* Phone)
{
	unsigned char temp[55]="AT+CMGS=\"";
	ASCII_ToUnicode(p_GMS->PhoneToUnicode,Phone,11);       //�绰תUnicode
	strcat(temp,p_GMS->PhoneToUnicode);										 //�ַ���ƴ�Ӻ������⺯����
	strcat(temp,"\""); 																		 //�ַ���ƴ�Ӻ������⺯����
	Second_AT_Command(temp,">",0); 												 //�ȴ����յ���>���ŷ��Ͷ�������
  Make_AuthTo6(p_PageDATA->AuthCodeToUnicode);					 //����6λ��֤��
	printf("%s%s",authcode,p_PageDATA->AuthCodeToUnicode);//��֤�������
	UART1_SendData(0X1A);          //���ͽ�����
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
		UART1_SendString("AT+CREG?");//��ѯģ������ע��״̬
		UART1_SendLR();
		delayms(1500);
		 LED = !LED;	 
	    for(k=0;k<Buf1_Max;k++)      			
    	{
			if(Uart1_Buf[k] == ':')
			{
				if((Uart1_Buf[k+2] == '0')||(Uart1_Buf[k+4] == '1')) //��������ע��ɹ�
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
      RI = 0;                 //���RIλ
			Uart1_Buf[First_Int++] = SBUF;  	  //�����յ����ַ����浽������
			if(First_Int == Buf1_Max-1)       		//���������,������ָ��ָ�򻺴���׵�ַ
			{
				First_Int = 0;
			}
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
    }
}

