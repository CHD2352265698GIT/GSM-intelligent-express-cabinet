#include <STC89C5xRC.H>
#include "key.h"
#include "string.h"
#include "stdio.h"
#include "lcd12864.h"
#include "GSM800C.h"
#include "config.h"




static char Key_value=0;
static char Read_Key=0;
char Get_ReadKeyFlag(void)
{
	 return Read_Key;
}
char Get_KeyValue(void)
{
	 return Key_value;
}
void Clean_ReadKeyFlag(void)
{
  Read_Key = 0;
}
char Check_Column(void)
{
	char value=0;
	KeyPort = 0xf0;
	if(KeyPort!= 0xf0)
	{
		switch (KeyPort)
		{
			case  0x70: value = 1;
				break;
			case  0xB0: value = 2;
				break;
			case  0xD0: value = 3;
				break;
			case  0xE0: value = 4;
				break;
			default:	value =0; 
				break;
		}
	#if PrintLC
		printf("Column:%d\n",(int)value);
	#endif
	}
	return value;
}
char Check_Line(void)
{
	char value=0;
	KeyPort = 0x0f;
	if(KeyPort!= 0x0f)
	{
		switch (KeyPort)
		{
			case  0x07: value = 1;
				break;   
			case  0x0B: value = 2;
				break;   
			case  0x0D: value = 3;
				break;   
			case  0x0E: value = 4;
				break;
			default:	value =0; 
				break;
		}
	#if PrintLC
	printf("Line:%d\n",(int)value);
	#endif
	}
	return value;
}


void KeyScan(void)
{
  char	column = Check_Column();
	char  line =   Check_Line();
	if(column && line)
	{
		switch (column)
		{
				case 4: switch (line)
								{
									case 1: Key_value=13;
										break;
									case 2: Key_value=14;
										break;
									case 3: Key_value=15;
										break;
									case 4: Key_value=16;
										break;
									default: Key_value = 0;
										break;
								}
				break;
			case 3: switch (line)
								{
									case 1: Key_value=9;
										break;
									case 2: Key_value=10;
										break;
									case 3: Key_value=11;
										break;
									case 4: Key_value=12;
										break;
									default: Key_value = 0;
										break;
								}
				break;
			case 2:  switch (line)
								{
									case 1: Key_value=5;
										break;
									case 2: Key_value=6;
										break;
									case 3: Key_value=7;
										break;
									case 4: Key_value=8;
										break;
									default:Key_value = 0;
										break;
								}
				break;
			case 1: switch (line)
								{
									case 1: Key_value=1;
										break;
									case 2: Key_value=2;
										break;
									case 3: Key_value=3;
										break;
									case 4: Key_value=4;
										break;
									default: Key_value = 0;
										break;
								}
				break;
			default: Key_value = 0;
				break;
		}
	}
	else  Key_value = 0;
		
}
static unsigned char time1=0;
static unsigned char time2=0;
static unsigned int time3=0;
void Timer0_Isr(void) interrupt 1
{
    TH0 = 0x0FC;
    TL0 = 0x66;
 if(time1++==Buffeting_time)
	{
	 Read_Key=1;
	 time1=0;
  }
	if(p_GMS->Timer0_start)
	{
		time2++;
		if(time2==250)
		p_GMS->Timer0_start=0;
	}
	if(p_PageDATA->BEEP_CallThePolice)
	{
		if(time3++ == Beep_frequency)
		{
			time3 = 0;
			BEEP = !BEEP;
			LED =!LED;
		}
	}
}

void Timer0_Init(void)		//1000Œ¢√Î@11.0592MHz
{
    TMOD |= 0x01;
    TH0 = 0x0FC;
    TL0 = 0x66;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}


void Set_Val(char* value,bit Enable)
{
	if(Enable)
	{
		switch (*value)
		{
			case 1:
				break;
			case 2: 
				break;
			case 3:
				break;
			case 4: 
				break;
			case 5: 
				break;
			case 6: 
				break;
			case 7: 
				break;
			case 8: 
				break;
			case 9: 
				break;
			case 10:
				break;
			case 11:
				break;
			case 12:
				break;
			case 13: 
				break;
			case 14:
				break;
			case 15: 
				break;
			case 16: 
				break;
			default:
				break;
		}
		*value=0;
	}
}
