#ifndef __CONFIG_H
#define __CONFIG_H


/*
	ģ��
	#if Print_Debug
	printf("\n");
	#endif
*/
#define Print_Debug    0     						//������Ϣ���ڴ�ӡ
#define USE_GSM800C    1     						//����GSM���Ͷ��Ų���
#define PrintLC        0     						//��ӡ��ֵ
#define Buffeting_time 150   						//��������ʱ��(ms)
#define Beep_frequency  1000 						//������ʾ��Ƶ��(ms)
#define Number_of_password_errors 3     //�����������

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