#ifndef __SCREEN_APP_H
#define __SCREEN_APP_H

#include "main.h"

extern const u16 POINT_COLOR_TBL[10];
void ctp_test(void);
void Button_App(void);
void ipsDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color, u16 style);

void INIT_Action(void);
void AUTO1_Action(void);
void MANUAL_Action(void);

void Draw_INIT_Screen(void);
void Draw_MAUAL_Screen(void);
void Draw_AUTO1_Screen(void);

extern u16 pointer_x1, pointer_y1, pointer_x2, pointer_y2;
extern int ipsInputNum;
extern int manualInputNum1;
extern int manualInputNum2;


#endif
