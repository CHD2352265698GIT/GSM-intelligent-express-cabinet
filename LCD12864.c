#include "LCD12864.H"
#include "stdio.H"
#include <string.h>
#include "gsm800c.h"
#include "config.h"

void delay_10us(unsigned char x) // 误差 -0.234375us
{
	unsigned char a, b;
	for (b = x; b > 0; b--)
		for (a = 3; a > 0; a--)
			;
}

void delay_1ms(void) // 误差 -0.651041666667us
{
	unsigned char a, b;
	for (b = 102; b > 0; b--)
		for (a = 3; a > 0; a--)
			;
}

void delay_ms(unsigned int x) // 误差 -0.651041666667us
{
	unsigned int a;
	for (a = x; a > 0; a--)
		delay_1ms();
}

// 向12864中写入数据
void LCD12864_Write_Data(unsigned char dat)
{
	LCD12864_E = 0;
	LCD12864_WR = 0; // 选择写模式
	LCD12864_RS = 1; // 选择数据模式

	// 延时
	delay_10us(1);

	// 将数据准备
	LCD12864_Data = dat;

	delay_10us(1);

	LCD12864_E = 1; // 表示开始写入数据,上升沿写入数据

	delay_10us(20); // 延时等待数据全部发送完成
}

// 向12864中写入命令
void LCD12864_Write_Com(unsigned char com)
{
	// 首先我们设置片选信号
	LCD12864_E = 0;	 // 片选信号一般设置为0表示片选
	LCD12864_WR = 0; // 选择写模式
	LCD12864_RS = 0; // 选择命令模式

	// 延时
	delay_10us(1);

	// 将数据准备
	LCD12864_Data = com;

	delay_10us(1);

	LCD12864_E = 1; // 表示开始写入数据,上升沿写入数据

	delay_10us(20);
}

// 对LCD12864进行初始化
void LCD12864_Init()
{
	// 首先对LCD进行复位
	LCD12864_REST = 0;
	delay_ms(30);
	LCD12864_REST = 1;
	delay_ms(20);

	/* 开始初始化的步骤 */
	LCD12864_Write_Com(0x30);
	//            LCD12864_Write_Com(0x08);
	//            LCD12864_Write_Com(0x06);
	LCD12864_Write_Com(0x0c);
	LCD12864_Write_Com(0x01);
}

unsigned char addresses[] = {0x80, 0x90, 0x88, 0x98};
void LCD12864_Display_Char(unsigned char x, unsigned char y, unsigned char dat)
{

	LCD12864_Write_Com(0x30);
	LCD12864_Write_Com(0x06);

	// 写入地址
	LCD12864_Write_Com(addresses[x - 1] + (y - 1));

	// 写入数据
	LCD12864_Write_Data(dat);
}

// 显示汉字
void LCD12864_Display_Chars(unsigned char x, unsigned char y, const unsigned char *dat)
{

	LCD12864_Write_Com(0x30);
	LCD12864_Write_Com(0x06);

	// 写入地址
	LCD12864_Write_Com(addresses[x - 1] + (y - 1));

	// 写入数据
	while (*dat != '\0')
	{
		LCD12864_Write_Data(*dat);
		dat++;
	}
}

void LCD12864_Clear(void)
{
	LCD12864_Display_Chars(1, 1, "                ");
	LCD12864_Display_Chars(2, 1, "                ");
	LCD12864_Display_Chars(3, 1, "                ");
	LCD12864_Display_Chars(4, 1, "                ");
}

/*
	页面
*/
static xdata Page PageData;
static xdata union Add A;

union Add *Get_A(void)
{
	return &A;
}

Page *p_PageData()
{
	return &PageData;
}
void Page_Control(Page *Data)
{
	switch (Data->PAGE_NUM)
	{
	case Theme:
		Page_Theme(Data);
		break;
	case Select_Box:
		Page_Select_Box(Data);
		break;
	case S:
		Page_S(Data);
		break;
	case I:
		Page_I(Data);
		break;
	case SendWait:
		Page_SendWait(Data);
		break;
	case InputNum:
		Page_InputNum(Data);
		break;
	case InputError:
		Page_InputError(Data);
		break;
	case AuthError:
		Page_AuthError(Data);
		break;
	case Access:
		Page_Access(Data);
		break;
	default:
#if Print_Debug
		printf("Error:Can not find page\n");
#endif
		break;
	}
	if (Data->Key_Value == ExitKey)
	{
		Data->PAGE_NUM = Access;
	}
}
void Refresh_Page(Page *Data)
{
	if (Data->Last_PAGE_NUM != Data->PAGE_NUM)
	{
#if Print_Debug
		printf("Refresh!\n");
#endif
		Data->Last_PAGE_NUM = Data->PAGE_NUM;
		Data->Refresh = 1;
	}
}
void Page_Theme(Page *Data)
{
	LCD12864_Write_Com(0x01);
	LCD12864_Display_Chars(1, 1, "基于单片机的GSM ");
	LCD12864_Display_Chars(2, 1, "    智能快递柜 ");
	LCD12864_Display_Chars(3, 1, "  控制系统设计 ");
	LCD12864_Display_Chars(4, 1, "               ");
	LCD12864_Write_Com(0x01);
	if (Data->Key_Value)
	{
		Data->PAGE_NUM = Access;
		Data->Input_Enable = 0;
	}
}
void Page_Select_Box(Page *Data)
{
	if (Data->Input_Enable)
	{
		if (Data->Sel_fun == Fetch)
		{
			if (Data->Key_Value == KeyNum1)
			{
				Data->Select_Box = 1;
				Data->Input_Enable = 0;
				if (Data->Box[0])
				{
					Data->PAGE_NUM = SendWait;
					p_GMS->Sel_Phone = p_GMS->phone1;
				}
				return;
			}
			else if (Data->Key_Value == KeyNum2)
			{
				Data->Select_Box = 2;
				Data->Input_Enable = 0;
				if (Data->Box[1])
				{
					Data->PAGE_NUM = SendWait;
					p_GMS->Sel_Phone = p_GMS->phone2;
				}
				return;
			}
		}
		else if (Data->Sel_fun == Store)
		{
			if (Data->Key_Value == KeyNum1)
			{
				Data->Select_Box = 1;
				Data->PAGE_NUM = I;
				p_GMS->Sel_Phone = p_GMS->phone1;
				Data->Input_Enable = 0;
				return;
			}
			else if (Data->Key_Value == KeyNum2)
			{
				Data->Select_Box = 2;
				Data->PAGE_NUM = I;
				p_GMS->Sel_Phone = p_GMS->phone2;
				Data->Input_Enable = 0;
				return;
			}
		}
	}
	LCD12864_Display_Chars(1, 1, "请选择货柜号码  :");
	LCD12864_Display_Chars(2, 1, "                ");

	if (Data->Box[0])
		LCD12864_Display_Chars(3, 1, "快递柜 1: 已存 ");
	else
		LCD12864_Display_Chars(3, 1, "快递柜 1: 未存 ");
	if (Data->Box[1])
		LCD12864_Display_Chars(4, 1, "快递柜 2: 已存 ");
	else
		LCD12864_Display_Chars(4, 1, "快递柜 2: 未存 ");
	Data->Error_Add = 0;
	Data->BEEP_CallThePolice = 0;
	BEEP = 1;
	LED = 1;
	Data->Input_Enable = 1;
}
void Clean_PassWord(Page *Data)
{
	memset(Data->PassWord, ' ', 6);
}
void Page_S(Page *Data)
{
	if (Data->Input_Enable)
	{
		if (Data->Key_Value <= KeyNum9 && Data->Key_Value >= KeyNum0)
		{
			sprintf(&Data->PassWord[A.Page_S_i++], "%d", (int)Data->Key_Value - 1);
#if Print_Debug
			printf("%s,%d\n", Data->PassWord, (int)A.Page_S_i);
#endif
		}
		else if (Data->Key_Value == CleanKey && A.Page_S_i)
		{
			Data->PassWord[--A.Page_S_i] = ' ';
#if Print_Debug
			printf("%s,%d\n", Data->PassWord, (int)A.Page_S_i);
#endif
		}
		if (A.Page_S_i == 6)
		{
			A.Page_S_i = 0;
			Data->PassWord_Check = 1;
		}
	}
	Data->PassWord[6] = '\0';
	LCD12864_Display_Chars(1, 1, "请登录您的账号 ");
	LCD12864_Display_Chars(2, 1, "登录密码：     ");
	LCD12864_Display_Chars(3, 1, Data->PassWord);
	LCD12864_Display_Chars(4, 1, "              ");
	Data->Input_Enable = 1;
	if (Data->PassWord_Check)
	{
		if (!memcmp(Data->PassWord, Data->Correct_PassWord, 6))
		{
#if Print_Debug
			printf("密码正确\n");
#endif
			Clean_PassWord(Data);
			A.Page_S_i = 0;
			Data->PAGE_NUM = Select_Box;
			Data->PassWord_Check = 0;
			Data->Input_Enable = 0;
			return;
		}
		else
		{
#if Print_Debug
			printf("密码错误！\n");
#endif
			Clean_PassWord(Data);
			A.Page_S_i = 0;
			Data->PAGE_NUM = InputError;
			Data->PassWord_Check = 0;
			Data->Input_Enable = 0;
			return;
		}
	}
}

void Page_I(Page *Data)
{
	if (Data->Input_Enable)
	{
		if (Data->Key_Value <= KeyNum9 && Data->Key_Value >= KeyNum0)
		{
			sprintf(&p_GMS->Sel_Phone[A.Page_I_i++], "%d", (int)Data->Key_Value - 1);
#if Print_Debug
			printf("%s,%d\n", p_GMS->Sel_Phone, (int)A.Page_I_i);
#endif
		}
		else if (Data->Key_Value == CleanKey && A.Page_I_i)
		{
			p_GMS->Sel_Phone[--A.Page_I_i] = ' ';
#if Print_Debug
			printf("%s,%d\n", p_GMS->Sel_Phone, (int)A.Page_I_i);
#endif
		}
		else if (Data->Key_Value == SendKey)
		{
			A.Page_I_i = 0;
			Data->PAGE_NUM = Select_Box;
			if (Data->Select_Box == 1)
			{
				Data->UnlockSign = 1;
				Data->Box[0] = !Data->Box[0];
			}
			else if (Data->Select_Box == 2)
			{
				Data->UnlockSign = 2;
				Data->Box[1] = !Data->Box[1];
			}
			Data->Input_Enable = 0;
			return;
		}
		if (A.Page_I_i > 10)
		{
			A.Page_I_i = 10;
		}
	}
	LCD12864_Display_Chars(1, 1, "请输入手机号码:  ");
	LCD12864_Display_Chars(2, 1, "                ");
	LCD12864_Display_Chars(2, 1, p_GMS->Sel_Phone);
	LCD12864_Display_Chars(3, 1, "                ");
	LCD12864_Display_Chars(4, 1, "                ");
	Data->Input_Enable = 1;
}
void Page_SendWait(Page *Data)
{
	LCD12864_Display_Chars(1, 1, "短信发送中...   ");
	LCD12864_Display_Chars(2, 1, "                ");
	LCD12864_Display_Chars(3, 1, "                ");
	LCD12864_Display_Chars(4, 1, "                ");
#if USE_GSM800C
	Set_Text_Mode();
	Send_AuTH(p_GMS->Sel_Phone);
#endif
	LCD12864_Write_Com(0x01);
	LCD12864_Display_Chars(1, 1, "短信发送成功    ");
	LCD12864_Display_Chars(2, 1, "                ");
	LCD12864_Display_Chars(3, 1, "                ");
	LCD12864_Display_Chars(4, 1, "                ");
	LCD12864_Write_Com(0x01);
	delay_ms(1000);
	Data->PAGE_NUM = InputNum;
	Data->Input_Enable = 0;
}

void Page_InputNum(Page *Data)
{
	if (Data->Input_Enable)
	{
		if (Data->Key_Value <= KeyNum9 && Data->Key_Value >= KeyNum0)
		{
			sprintf(&Data->UserInAuthCode[A.Page_InAuth_i++], "%d", (int)Data->Key_Value - 1);
#if Print_Debug
			printf("%s,%d\n", Data->UserInAuthCode, (int)A.Page_InAuth_i);
#endif
		}
		else if (Data->Key_Value == CleanKey && A.Page_InAuth_i)
		{
			Data->UserInAuthCode[--A.Page_InAuth_i] = ' ';
#if Print_Debug
			printf("%s,%d\n", Data->UserInAuthCode, (int)A.Page_InAuth_i);
#endif
		}
		else if (Data->Key_Value == SendKey)
		{
			A.Page_InAuth_i = 0;
			Data->PassWord_Check = 1;
		}
		if (A.Page_InAuth_i == 7)
		{
			A.Page_InAuth_i = 0;
			Data->PassWord_Check = 1;
		}
	}
	LCD12864_Write_Com(0x01);
	LCD12864_Display_Chars(1, 1, "请输入验证码:  ");
	LCD12864_Display_Chars(2, 1, "                ");
	LCD12864_Display_Chars(2, 1, Data->UserInAuthCode);
	LCD12864_Display_Chars(3, 1, "                ");
	LCD12864_Display_Chars(4, 1, "                ");
	LCD12864_Write_Com(0x01);
	Data->Input_Enable = 1;
	if (Data->PassWord_Check)
	{
		Data->PassWord_Check = 0;
		if (!memcmp(Data->UserInAuthCode, Data->AuthCode, 6))
		{
#if Print_Debug
			printf("Ture!\n");
#endif
			memset(Data->UserInAuthCode, ' ', sizeof(Data->UserInAuthCode));
			Data->Box[Data->Select_Box - 1] = !Data->Box[Data->Select_Box - 1];
			Data->UnlockSign = Data->Select_Box;
			Data->Input_Enable = 0;
			Data->PAGE_NUM = Select_Box;
			return;
		}
		else
		{
#if Print_Debug
			printf("Error!\n");
#endif
			memset(Data->UserInAuthCode, ' ', sizeof(Data->UserInAuthCode));
			Data->Input_Enable = 0;
			Data->PAGE_NUM = AuthError;
			return;
		}
	}
}

void Page_InputError(Page *Data)
{
	if (Data->Input_Enable)
	{
		if (Data->Key_Value)
		{
			Data->PAGE_NUM = S;
			Data->Input_Enable = 0;
			return;
		}
	}
	if (++Data->Error_Add >= Number_of_password_errors)
	{
		Data->Error_Add = 0;
		Data->BEEP_CallThePolice = 1;
	}
	LCD12864_Display_Chars(1, 1, "密码错误!!!   ");
	LCD12864_Display_Chars(2, 1, "请重新输入密码 ");
	LCD12864_Display_Chars(3, 1, "              ");
	LCD12864_Display_Chars(4, 1, "              ");
	Data->Input_Enable = 1;
}

void Page_AuthError(Page *Data)
{
	if (Data->Input_Enable)
	{
		if (Data->Key_Value)
		{
			Data->PAGE_NUM = InputNum;
			Data->Input_Enable = 0;
			return;
		}
	}
	if (++Data->Error_Add >= Number_of_password_errors)
	{
		Data->Error_Add = 0;
		Data->BEEP_CallThePolice = 1;
	}
	LCD12864_Display_Chars(1, 1, "验证码错误!!!  ");
	LCD12864_Display_Chars(2, 1, "请重新输入验证码");
	LCD12864_Display_Chars(3, 1, "              ");
	LCD12864_Display_Chars(4, 1, "              ");
	Data->Input_Enable = 1;
}

void Page_Access(Page *Data)
{
	if (Data->Input_Enable)
	{
		if (Data->Key_Value == KeyNum1)
		{
			Data->PAGE_NUM = S;
			Data->Input_Enable = 0;
			Data->Sel_fun = Store;
#if Print_Debug
			printf("Store\n");
#endif
			return;
		}
		else if (Data->Key_Value == KeyNum2)
		{
			Data->PAGE_NUM = Select_Box;
			Data->Input_Enable = 0;
			Data->Sel_fun = Fetch;
#if Print_Debug
			printf("Fetch\n");
#endif
			return;
		}
	}
	LCD12864_Write_Com(0x01);
	LCD12864_Display_Chars(1, 1, "请选择功能:    ");
	LCD12864_Display_Chars(2, 1, "存包裹          ");
	LCD12864_Display_Chars(3, 1, "取包裹          ");
	LCD12864_Display_Chars(4, 1, "                ");
	LCD12864_Write_Com(0x01);
	Data->Input_Enable = 1;
}