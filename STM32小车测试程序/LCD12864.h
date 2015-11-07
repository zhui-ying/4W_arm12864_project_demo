#ifndef  __LCD1602_H_
#define  __LCD1602_H_
#include "stm32f10x.h"

void LCD12864Init(void);
unsigned char LcdWriteString(unsigned char x, unsigned char y, unsigned char *cn);
void ShowEnString(unsigned int x,unsigned int y,unsigned char *str);
void LcdClearScreen(void);
void LCD12864WriteCommand(char comm);
void LCD12864ShowRedRay(char void_l , char search_l , char search_m , char search_r , char void_r);
#endif

