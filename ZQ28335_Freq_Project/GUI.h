/*
 * GUI.h
 *
 *  Created on: 2019年5月25日
 *      Author: sunxiaobing
 */

#ifndef GUI_H_
#define GUI_H_
#include "LCD_24TFT.h"

u16 LCD_BGR2RGB(u16 c);
void Gui_Circle(u16 X,u16 Y,u16 R,u16 fc);
void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color);
void Gui_Draw_DotLine(unsigned int x0,
                      unsigned int y0,
                      unsigned int x1,
                      unsigned int y1,
                      unsigned int line_color);
void GUIcircle(unsigned int xc,
               unsigned int yc,
               unsigned int r,
               unsigned int yc_colour);

void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc);
void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode);
void GUITable(unsigned int x0,
              unsigned int y0,
              unsigned int x1,
              unsigned int y1,
              unsigned char TH,
              unsigned char TL,
              unsigned int color,
              unsigned char temp);

void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2);
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2);
void ClearButtonUp(u16 x1,u16 y1,u16 x2,u16 y2,u16 bcolour);

void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
void Gui_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
void Gui_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num) ;
void Show_Num(Uint16 x0,Uint16 y0,Uint32 Num,Uint16 ccolor,Uint16 bcolor);

void Show_Image(unsigned int x0,
                unsigned int y0,
                unsigned int x1,
                unsigned int y1,
                const unsigned char *p);
void GUICoordinate(u16 x0,u16 y0,u16 xm,u16 ym,u8 pix,u16 ccolor,u16 bcolor);  //绘制一个坐标轴，以用于ADC显示电压为参考，可自行更改
/******************************************************/

void Draw_Mainmenu(void);    //主菜单界面
void Draw_Submenu1(void);    //子菜单1界面
void Draw_Submenu2(void);    //子菜单2界面
void Draw_Submenu3(void);    //子菜单3界面
void Draw_Submenu4(void);    //子菜单4界面
void WavePicture(void);      //波形显示界面框架


void Num_Test(void);
void Font_Test(void);
void Color_Test(void);
void showimage(const unsigned char *p);
void QDTFT_Test_Demo(void);

#endif /* GUI_H_ */
