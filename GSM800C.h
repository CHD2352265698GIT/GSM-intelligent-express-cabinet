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

//static unsigned char *SMS_Num="\"+8613800816500\"";//�������ĺ�

#define Automatic_Startup 1     //���������� V1.2�汾��������������
#define Buf1_Max 60 					  //����1���泤��
//����1���ͻس�����
#define UART1_SendLR() UART1_SendData(0X0D);\
											 UART1_SendData(0X0A)


void Uart1Init(void);		//9600bps@11.0592MHz
void CLR_Buf1(void);    //�崮�ڽ��ջ���
u8 Find(u8 *a);         //�����ַ���
void Second_AT_Command(u8 *b,u8 *a,unsigned char Wait_Time);  //����ָ��
void Wait_CREG(void);   //��ѯ�ȴ�ģ��ע��ɹ�
void Set_Text_Mode(void); //���ö���ΪTEXTģʽ
void Send_AuTH(char* Phone);
void Make_AuthTo6(char* Unicode);
void Auth_Code(char* arr,char Length);
void ASCII_ToUnicode(char *arr,const char *str,char Length);


#endif
