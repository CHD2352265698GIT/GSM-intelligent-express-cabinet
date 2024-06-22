#ifndef __LCD12864_H
#define __LCD12864_H
#include <STC89C5xRC.H>
sbit LCD12864_WR=P3^5;
sbit LCD12864_RS=P3^4;
sbit LCD12864_E =P3^6;
sbit LCD12864_REST = P3^3;
sbit LCD12864_BK =  P3^2;
#define LCD12864_Data  P1
#define CallThePolice  3



void delay_ms(unsigned int x);  
void LCD12864_Write_Data(unsigned char dat);
//向12864中写入命令
void LCD12864_Write_Com(unsigned char com);
//对LCD12864进行初始化
void LCD12864_Init();
//LCD12864_Clear_Screen(0xFF)全灭
void LCD12864_Clear_Screen(unsigned char value);
void LCD12864_Display_Char(unsigned char x,unsigned char y,unsigned char dat);
//显示汉字
void LCD12864_Display_Chars(unsigned char x,unsigned char y,const unsigned char *dat);
void LCD12864_Clear(void);

union Add{
	unsigned char Page_S_i;
	unsigned char Page_I_i;
	unsigned char Page_InAuth_i;
};
/*读写数据*/
typedef struct Page{
	 char PAGE_NUM;               //页面状态
	 char Last_PAGE_NUM;          //上次循环页面状态
   char Select_Box;             //快递箱选择
	 char UnlockSign;             //开锁信号
	 char PassWord_Check;         //密码检查
	 char Key_Value; 							//按键控件
	 char Input_Enable;						//输入使能
	 char Refresh; 								//立即刷新
	 char Sel_fun;								//功能选择
	 char Error_Add;							//错误次数
	 char BEEP_CallThePolice;     //蜂鸣器报警
	 char Box[2];                 //快递箱状态
	 char Correct_PassWord[6];    //登录密码
   char UserInAuthCode[7];      //用户输入验证码
	 char PassWord[7];            //密码
	 char AuthCode[7];            //验证码
	 char AuthCodeToUnicode[25];  //验证码转Unicode码
}Page;
Page* p_PageData();
/* 功能选择 */
enum{
	Store=1, //存
	Fetch=2 //取
};
/*添加页面*/
enum{
	Theme,
	Select_Box,
	S,
	I,
	SendWait,
	InputNum,
	InputError,
	AuthError,
	Access,
	Max_Flag
};

/*绑定键值*/
enum{
	BackLightKey = 14,
	ExitKey = 13,
	SendKey = 15,
	CleanKey = 16,
	KeyNum0 =1,
	KeyNum1 =2,
	KeyNum2 =3,
	KeyNum3 =4,
	KeyNum4 =5,
	KeyNum5 =6,
	KeyNum6 =7,
	KeyNum7 =8,
	KeyNum8 =9,
	KeyNum9 =10,	
};
//获取结构体指针
#define p_PageDATA  p_PageData()  
union Add* Get_A(void);
void Page_Control(Page* Data);
void Page_Theme(Page* Data);
void Page_Select_Box(Page* Data);
void Page_S(Page* Data);
void Page_I(Page* Data);
void Page_SendWait(Page* Data);
void Page_InputNum(Page* Data);
void Page_InputError(Page* Data);
void Page_AuthError(Page* Data);
void Page_Access(Page* Data);
void Clean_PassWord(Page* Data);
void Refresh_Page(Page* Data);

#endif
