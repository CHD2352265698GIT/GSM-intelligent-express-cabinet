#include <STC89C5xRC.H>
#include "LCD12864.H"
#include "gsm800c.h"
#include "key.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>  
#include "config.h"


#define Relay_Delay	{LED = 0;BEEP = 0;delay_ms(200);BEEP = 1;	LED = 1;}
char i=0;
void main(){
	RELAY1 = OFF;
	RELAY2 = OFF;
	RELAY3 = OFF;
	KEYH=0;
		p_PageDATA->Input_Enable=0;
		p_PageDATA->UnlockSign=0;
		p_PageDATA->Box[0] =0;
		p_PageDATA->Box[1] =0;
		p_PageDATA->Refresh=0;
		p_PageDATA->Sel_fun=0;
		p_PageDATA->Error_Add = 0;
		p_PageDATA->BEEP_CallThePolice = 0;
		LCD12864_BK =0;
    //液晶初始化
    LCD12864_Init();
	  Uart1Init();		//9600bps@11.0592MHz
		Timer0_Init();
	  LCD12864_Display_Chars(2,1,"初始化中... ");
		#if USE_GSM800C
		Wait_CREG();    //查询等待模块注册成功
		Relay_Delay;
		#endif
	  LCD12864_Display_Chars(2,1,"初始化成功   ");
    LCD12864_Write_Com(0x01);  
  	delay_ms(1000);
	  memcpy(p_PageData()->Correct_PassWord,"543322",6);
	  memset(p_GMS->phone1,' ',11);
	  memset(p_GMS->phone2,' ',11);
	  memset(p_PageDATA->UserInAuthCode,' ',6);
		p_PageDATA->UserInAuthCode[6]='\0';
  	p_GMS->PhoneToUnicode[44] = '\0';
		p_PageDATA->AuthCodeToUnicode[24] = '\0';
  	Clean_PassWord(p_PageDATA);
		p_PageDATA->PAGE_NUM = Theme;
		p_PageDATA->PassWord_Check =0;
		Get_A()->Page_S_i=0;
		while(1)
		{
			//扫描按键
			if(Get_ReadKeyFlag())
			{
				KeyScan();
				p_PageDATA->Key_Value = Get_KeyValue();
				Clean_ReadKeyFlag();
			}
			//按键按下更新屏幕或特殊条件刷新
		  if(p_PageDATA->Key_Value || p_PageDATA->Refresh)
			{
				 p_PageDATA->Refresh = 0;
				Page_Control(p_PageDATA);
				#if Print_Debug
				printf("NUM:%d,Key:%d,p_Input_En:%d\n",(int)p_PageDATA->PAGE_NUM,(int)p_PageDATA->Key_Value,(int)p_PageDATA->Input_Enable);
				#endif				
				//开关背光
				(p_PageDATA->Key_Value == BackLightKey)? LCD12864_BK=!LCD12864_BK:LCD12864_BK;
				 p_PageDATA->Key_Value =0;
			}				
			Refresh_Page(p_PageDATA);
			(p_PageDATA->PAGE_NUM>Max_Flag)? p_PageDATA->PAGE_NUM=Theme:p_PageDATA->PAGE_NUM;
			//继电器开关
			if(p_PageDATA->UnlockSign == 1)
			{
				p_PageDATA->UnlockSign=0;
				RELAY2 = !RELAY2;
				RELAY3 = !RELAY3;
				Relay_Delay
				RELAY2 = !RELAY2;
				RELAY3 = !RELAY3;
			}
			else if(p_PageDATA->UnlockSign == 2)
			{
				p_PageDATA->UnlockSign=0;
				RELAY1 = !RELAY1;
				RELAY3 = !RELAY3;
				Relay_Delay
				RELAY1 = !RELAY1;
				RELAY3 = !RELAY3;				
			}
    }
}
