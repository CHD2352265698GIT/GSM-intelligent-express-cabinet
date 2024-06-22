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
//��12864��д������
void LCD12864_Write_Com(unsigned char com);
//��LCD12864���г�ʼ��
void LCD12864_Init();
//LCD12864_Clear_Screen(0xFF)ȫ��
void LCD12864_Clear_Screen(unsigned char value);
void LCD12864_Display_Char(unsigned char x,unsigned char y,unsigned char dat);
//��ʾ����
void LCD12864_Display_Chars(unsigned char x,unsigned char y,const unsigned char *dat);
void LCD12864_Clear(void);

union Add{
	unsigned char Page_S_i;
	unsigned char Page_I_i;
	unsigned char Page_InAuth_i;
};
/*��д����*/
typedef struct Page{
	 char PAGE_NUM;               //ҳ��״̬
	 char Last_PAGE_NUM;          //�ϴ�ѭ��ҳ��״̬
   char Select_Box;             //�����ѡ��
	 char UnlockSign;             //�����ź�
	 char PassWord_Check;         //������
	 char Key_Value; 							//�����ؼ�
	 char Input_Enable;						//����ʹ��
	 char Refresh; 								//����ˢ��
	 char Sel_fun;								//����ѡ��
	 char Error_Add;							//�������
	 char BEEP_CallThePolice;     //����������
	 char Box[2];                 //�����״̬
	 char Correct_PassWord[6];    //��¼����
   char UserInAuthCode[7];      //�û�������֤��
	 char PassWord[7];            //����
	 char AuthCode[7];            //��֤��
	 char AuthCodeToUnicode[25];  //��֤��תUnicode��
}Page;
Page* p_PageData();
/* ����ѡ�� */
enum{
	Store=1, //��
	Fetch=2 //ȡ
};
/*���ҳ��*/
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

/*�󶨼�ֵ*/
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
//��ȡ�ṹ��ָ��
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
