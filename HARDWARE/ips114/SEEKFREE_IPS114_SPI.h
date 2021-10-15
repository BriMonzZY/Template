/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.24
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-11-15
 * @note		
					���߶��壺
					------------------------------------ 
					1.14��IPSģ��ܽ�       ��Ƭ���ܽ�
					SCL                 	�鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_SCL		�궨��     Ӳ��SPI���Ų��������л�
					SDA                 	�鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_SDA		�궨��     Ӳ��SPI���Ų��������л�
					RES                 	�鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_REST_PIN�궨��    
					DC                  	�鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_DC_PIN	�궨��  
					CS                  	�鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_CS		�궨��     Ӳ��SPI���Ų��������л�
					BL  					�鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_BL_PIN	�궨��
					
					��Դ����
					VCC 3.3V��Դ
					GND ��Դ��
					���ֱ���135*240
					------------------------------------ 
 ********************************************************************************************************************/
 

#ifndef _SEEKFREE_IPS114_H
#define _SEEKFREE_IPS114_H

#include "SEEKFREE_FONT.h"
#include "main.h"

#define uint8 		uint8_t
#define uint16      uint16_t
#define uint32      uint32_t
#define uint64      uint64_t
 
#define int8        int8_t
#define int16       int16_t
#define int32       int32_t
#define int64       int64_t

//-----------------���Ŷ���------------------------------      


#define	IPS114_SCL(x)	HAL_GPIO_WritePin(IPS114_SCL_GPIO_Port,IPS114_SCL_Pin,(GPIO_PinState)x)
#define IPS114_SDA(x)	HAL_GPIO_WritePin(IPS114_SDA_GPIO_Port,IPS114_SDA_Pin,(GPIO_PinState)x)
#define IPS114_REST(x)	HAL_GPIO_WritePin(IPS114_REST_GPIO_Port,IPS114_REST_Pin,(GPIO_PinState)x)
#define IPS114_DC(x)	HAL_GPIO_WritePin(IPS114_DC_GPIO_Port,IPS114_DC_Pin,(GPIO_PinState)x)
#define IPS114_CS(x)	HAL_GPIO_WritePin(IPS114_CS_GPIO_Port,IPS114_CS_Pin,(GPIO_PinState)x)
#define IPS114_BL(x)	HAL_GPIO_WritePin(IPS114_BL_GPIO_Port,IPS114_BL_Pin,(GPIO_PinState)x)
			


#define IPS114_W   135
#define IPS114_H   240



//-------������ɫ----------
#define RED          	0xF800	//��ɫ
#define BLUE         	0x001F  //��ɫ
#define YELLOW       	0xFFE0	//��ɫ
#define GREEN        	0x07E0	//��ɫ
#define WHITE        	0xFFFF	//��ɫ
#define BLACK        	0x0000	//��ɫ 
#define GRAY  			0X8430 	//��ɫ
#define BROWN 			0XBC40 	//��ɫ
#define PURPLE    		0XF81F	//��ɫ
#define PINK    		0XFE19	//��ɫ



//����д�ֱʵ���ɫ
#define IPS114_PENCOLOR    RED

//���屳����ɫ
#define IPS114_BGCOLOR     WHITE



//������ʾ����
//0 ����ģʽ
//1 ����ģʽ  ��ת180
//2 ����ģʽ
//3 ����ģʽ  ��ת180
#define IPS114_DISPLAY_DIR 3

#if (0==IPS114_DISPLAY_DIR || 1==IPS114_DISPLAY_DIR)
#define	IPS114_X_MAX	IPS114_W	//Һ��X������
#define IPS114_Y_MAX	IPS114_H   	//Һ��Y������
     
#elif (2==IPS114_DISPLAY_DIR || 3==IPS114_DISPLAY_DIR)
#define	IPS114_X_MAX	IPS114_H	//Һ��X������
#define IPS114_Y_MAX	IPS114_W   	//Һ��Y������
     
#else
#error "IPS114_DISPLAY_DIR �������"
     
#endif


void ips114_init(void);
void ips114_clear(uint16 color);
void ips114_drawpoint(uint16 x,uint16 y,uint16 color);
void ips114_showchar(uint16 x,uint16 y,uint8 dat);
void ips114_showstr(uint16 x,uint16 y,uint8 dat[]);
void ips114_showint8(uint16 x,uint16 y,int8 dat);
void ips114_showuint8(uint16 x,uint16 y,uint8 dat);
void ips114_showint16(uint16 x,uint16 y,int16 dat);
void ips114_showuint16(uint16 x,uint16 y,uint16 dat);
void ips114_displayimage032(uint8 *p, uint16 width, uint16 height);
void ips114_displayimage032_zoom(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void ips114_displayimage7725(uint8 *p, uint16 width, uint16 height);
void ips114_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color);

#endif