 /******************************************************************************
  * �ļ�·��  ��   Hardware/usart/usart.h 
  * ����      ��   ����  
  * �̼���汾��   V1.0.2
  * ����      ��   2012-12-28
  * �ļ�����  ��   �ṩ�԰��ϴ��ڵ�����
  ******************************************************************************/
#ifndef __INTERFACE_H
#define __INTERFACE_H


//====================================================================================
//�궨��

//-----------------------------------------------------
//���޸�����
#define  SPEED_FRAME_X0				 110
#define  SPEED_FRAME_Y0				 82
#define  SPEED_EDIT_W					 230
#define  SPEED_EDIT_H					 180

#define  ANGLE_FRAME_X0				 450
#define  ANGLE_FRAME_Y0				 82
#define  ANGLE_EDIT_W					 230
#define  ANGLE_EDIT_H					 130

#define  OUT_BT_W						   107
#define  OUT_BT_H						   40
#define  ADDSUB_BT_W					 72
#define  ADDSUB_BT_H					 40
#define  SETSAVE_BT_W					 72
#define  SETSAVE_BT_H					 40

#define  FRAME_TEXT_H					 24
//-----------------------------------------------------
#define  BK_COLOR              GUI_LIGHTBLUE
#define  FRAME_COLOR					 GUI_DARKBLUE  //0x6b6b65,0x959592
#define  SPEED_FRAME_X1				 SPEED_FRAME_X0 + SPEED_EDIT_W + 20
#define  SPEED_FRAME_Y1				 ANGLE_FRAME_Y1

#define  ANGLE_FRAME_X1				 ANGLE_FRAME_X0 + ANGLE_EDIT_W + 20
#define  ANGLE_FRAME_Y1				 ADD_BT_Y + ADDSUB_BT_H + 20

#define  TEXT_BK_COLOR				 BK_COLOR
#define  SPEED_TEXT_X					 SPEED_FRAME_X0+80
#define  SPEED_TEXT_Y					 SPEED_FRAME_Y0+5
#define  ANGLE_TEXT_X					 ANGLE_FRAME_X0+80
#define  ANGLE_TEXT_Y					 ANGLE_FRAME_Y0+5

#define  EDIT_BK_COLOR				 GUI_WHITE
#define  EDIT_TEXT_COLOR			 GUI_RED
#define  SPEED_EDIT_X					 SPEED_FRAME_X0+10
#define  SPEED_EDIT_Y					 SPEED_FRAME_Y0+ FRAME_TEXT_H + 10

#define  ANGLE_EDIT_X					 ANGLE_FRAME_X0 + 10
#define  ANGLE_EDIT_Y					 ANGLE_FRAME_Y0 + FRAME_TEXT_H + 10


#define  BT_COLOR_UNPRESSED    GUI_LIGHTBLUE
#define  BT_COLOR_PRESSED      GUI_LIGHTGRAY
#define  ADD_BT_X							 ANGLE_FRAME_X0 + 25
#define  ADD_BT_Y							 ANGLE_EDIT_Y + ANGLE_EDIT_H + 30
#define  SUB_BT_X							 ADD_BT_X+ ADDSUB_BT_W + ANGLE_EDIT_W/5 //30
#define  SUB_BT_Y							 ADD_BT_Y


//====================================================================================
//���ݶ���

//====================================================================================
//�ⲿ����
void MainDlg(void);
void AppMainTask (void *p_arg);
void  SetMainCallback(void);
#endif//__INTERFACE_H







