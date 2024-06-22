#ifndef __KEY_H
#define __KEY_H
#include <STC89C5xRC.H>


void KeyScan(void);
void Timer0_Init(void);		//1000Œ¢√Î@11.0592MHz
void Clean_ReadKeyFlag(void);
char Get_ReadKeyFlag(void);
char Get_KeyValue(void);
void Set_Val(char* value,bit Enable);


#endif
