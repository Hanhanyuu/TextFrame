/**
  ******************************************************************************
  * @file    screen_int.h
  * @author  hanhanyuu
  * @date    2023/11/26 18:28:51
  * @brief   为显示屏应用提供接口
  ******************************************************************************
  */

#ifndef __SCREEN_INT_H
#define __SCREEN_INT_H

#define Color uint16_t
/**
  * @brief  LCD color
  */
#define WHITE          0xFFFF
#define BLACK          0x0000
#define GREY           0xF7DE
#define BLUE           0x001F
#define BLUE2          0x051F
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0


#include "stdint.h"

/**
  * @brief: .--------->   Y
  *         |          
  *         |
  *         |
  *         V
  * 
  *         X
  *           纵向轴X，横向Y轴
  */
typedef struct 
{
  uint16_t X;
  uint16_t Y;
}Position;

typedef struct 
{
  uint16_t height;
  uint16_t width;
}Size;



typedef struct                          
{
  uint8_t* Text;                      //文本内容   
  uint16_t TextLength;                //文本长度      
  Color* ColorList;                   //显示的颜色列表  
  uint16_t* ColorChangePosition;      //变色节点
  Color BackgroundColor;              //背景色
  Size FrameSize;                     //文本框尺寸
  Size CharSize;                      //字符尺寸    
  Position Anchor;                    //文本框的左上端锚点
  uint16_t RowDistance;               //行间距
  uint16_t ColDistance;               //字符间距
  //TextFrame* LastTextframe;          //上一个文本框
  //TextFrame* NextTextframe;          //下一个文本框
}TextFrame;                           //用于描述一个文本框

void GeneTextFrame(TextFrame* pTextFrame);//不需要设定高度，函数根据文本自动生成并填充

#endif
