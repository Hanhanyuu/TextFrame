/**
 ******************************************************************************
 * @file    screen_int.c
 * @author  hanhanyuu
 * @date    2023/11/27 16:55:09
 * @brief   为显示屏应用提供接口
 ******************************************************************************
 */
#include "screen_int.h"
#include "LCD_Driver.h"

/**
 * @brief:     1.输入TextFrame指针，可以在屏幕上显示自定义宽度的文本框。
 *         生成文本框的长度会返回到输入的TextFrame结构体中.
 *             2.屎山代码，单词相对过长可能会导致空格大小变量数值异常.
 */
void GeneTextFrame(TextFrame *pTextFrame)
{
    uint16_t AnchorX = pTextFrame->Anchor.X;
    uint16_t AnchorY = pTextFrame->Anchor.Y;
    uint16_t CharWidth = pTextFrame->CharSize.width;
    uint16_t CharHeight = pTextFrame->CharSize.height;
    uint16_t CharDistance = pTextFrame->ColDistance;
    uint16_t FrameWidth = pTextFrame->FrameSize.width;
    Color BackgroundColor = pTextFrame->BackgroundColor;

    uint16_t SurPix;
    uint16_t GapPix;

    uint8_t *String = pTextFrame->Text;

    uint16_t SpaceLength = CharWidth;
    uint16_t AccLineLength = 0;
    uint16_t LastAccLineLength = 0;
    uint16_t WordNumInLine = 0;

    uint16_t RenderBottom = 0; // 渲染的字符最底端
    uint16_t RenderTop;        // 渲染最顶端

    uint16_t i = 0;
    uint16_t j = 0;
    int16_t k = 0;
    uint16_t ColorChangeIndex = 0;
    uint16_t ColorChangePosition = pTextFrame->ColorChangePosition[ColorChangeIndex];
    Color TextColor = pTextFrame->ColorList[ColorChangeIndex];
    uint16_t LastWordEnd;

    uint16_t SpaceLengthModified = SpaceLength;
    uint16_t CharDistanceModified = CharDistance;

    for (;; i++)
    {

        if (i == pTextFrame->TextLength)
        {
            PadColor(AnchorX, AnchorY, CharHeight, FrameWidth, BackgroundColor);
            SpaceLengthModified = SpaceLength;
            CharDistanceModified = CharDistance;
            RenderTop = i;
            for (j = RenderBottom; j <= RenderTop; j++)
            {
                if (j == ColorChangePosition)
                {
                    ColorChangeIndex++;
                    TextColor = pTextFrame->ColorList[ColorChangeIndex];
                    ColorChangePosition = pTextFrame->ColorChangePosition[ColorChangeIndex];
                }
                if (String[j] == ' ')
                    AnchorX += SpaceLength;
                else
                {
                    LCD_DisplayChar(AnchorX, AnchorY, String[j], BackgroundColor, TextColor);
                    AnchorX += CharWidth;
                }
                AnchorX += CharDistance;
            }
            break;
        }
        if (String[i] != ' ')
            AccLineLength += (CharWidth + CharDistance);
        else
        {
            WordNumInLine++; // 循环中未更新
            if (AccLineLength <= FrameWidth)
            {
                LastAccLineLength = AccLineLength - CharDistance;
                AccLineLength += (SpaceLength + CharDistance);
                LastWordEnd = i - 1; // 上个单词的结尾
                continue;
            }
            else
            {
                AccLineLength -= CharDistance;
                SurPix = AccLineLength - FrameWidth;
                GapPix = FrameWidth - LastAccLineLength;
                if (SurPix > GapPix)
                {
                    RenderTop = LastWordEnd;
                    WordNumInLine -= 1;
                    if ((RenderTop - RenderBottom) < GapPix)
                    {
                        CharDistanceModified = CharDistance + 1;
                        SpaceLengthModified = SpaceLength + (GapPix - (RenderTop - RenderBottom)) / (WordNumInLine - 1);
                        k = (int16_t)(GapPix - (RenderTop - RenderBottom)) % (WordNumInLine - 1);
                    }
                    else
                    {

                        SpaceLengthModified = SpaceLength + GapPix / (WordNumInLine - 1);
                        k = (int16_t)GapPix % (WordNumInLine - 1);
                    }

                    for (j = RenderBottom; j <= RenderTop; j++)
                    {
                        if (j == ColorChangePosition)
                        {
                            ColorChangeIndex++;
                            TextColor = pTextFrame->ColorList[ColorChangeIndex];
                            ColorChangePosition = pTextFrame->ColorChangePosition[ColorChangeIndex];
                        }
                        if (String[j] == ' ')
                        {
                            if (k > 0)
                            {
                                PadColor(AnchorX, AnchorY, CharHeight, SpaceLengthModified + 1, BackgroundColor);
                                AnchorX += SpaceLengthModified + 1;
                                k--;
                            }
                            else
                            {
                                PadColor(AnchorX, AnchorY, CharHeight, SpaceLengthModified, BackgroundColor);
                                AnchorX += SpaceLengthModified;
                            }
                        }
                        else
                        {
                            LCD_DisplayChar(AnchorX, AnchorY, String[j], BackgroundColor, TextColor);
                            AnchorX += CharWidth;
                        }
                        if (j == RenderTop)
                            break;
                        PadColor(AnchorX, AnchorY, CharHeight, CharDistanceModified, BackgroundColor);
                        AnchorX += CharDistanceModified;
                    }
                    AnchorX = pTextFrame->Anchor.X;
                    AnchorY += pTextFrame->CharSize.height;
                    PadColor(AnchorX, AnchorY, pTextFrame->RowDistance, FrameWidth, BackgroundColor);
                    AnchorX = pTextFrame->Anchor.X;
                    AnchorY += pTextFrame->RowDistance;
                    WordNumInLine = 0;
                    AccLineLength = 0;
                    RenderBottom = RenderTop + 2;
                    i = RenderTop + 1;
                    CharDistanceModified = CharDistance;
                }
                else
                {
                    RenderTop = i - 1;
                    if ((RenderTop - RenderBottom) < SurPix)
                    {
                        CharDistanceModified = CharDistance - 1;
                        SpaceLengthModified = SpaceLength - (SurPix - (LastWordEnd - RenderBottom)) / (WordNumInLine - 1);
                        k = (int16_t)(SurPix - (LastWordEnd - RenderBottom)) % (WordNumInLine - 1);
                    }
                    else
                    {
                        SpaceLengthModified = SpaceLength - SurPix / (WordNumInLine - 1);
                        k = (int16_t)SurPix % (WordNumInLine - 1);
                    }

                    for (j = RenderBottom; j <= RenderTop; j++)
                    {
                        if (j == ColorChangePosition)
                        {
                            ColorChangeIndex++;
                            TextColor = pTextFrame->ColorList[ColorChangeIndex];
                            ColorChangePosition = pTextFrame->ColorChangePosition[ColorChangeIndex];
                        }
                        if (String[j] == ' ')
                        {
                            if (k > 0)
                            {
                                PadColor(AnchorX, AnchorY, CharHeight, SpaceLengthModified - 1, BackgroundColor);
                                AnchorX += SpaceLengthModified - 1;
                                k--;
                            }
                            else
                            {
                                PadColor(AnchorX, AnchorY, CharHeight, SpaceLengthModified, BackgroundColor);
                                AnchorX += SpaceLengthModified;
                            }
                        }
                        else
                        {
                            LCD_DisplayChar(AnchorX, AnchorY, String[j], BackgroundColor, TextColor);
                            AnchorX += CharWidth;
                        }
                        if (j == RenderTop)
                            break;
                        PadColor(AnchorX, AnchorY, CharHeight, CharDistanceModified, BackgroundColor);
                        AnchorX += CharDistanceModified;
                    }
                    AnchorX = pTextFrame->Anchor.X;
                    AnchorY += pTextFrame->CharSize.height;
                    PadColor(AnchorX, AnchorY, pTextFrame->RowDistance, FrameWidth, BackgroundColor);
                    AnchorX = pTextFrame->Anchor.X;
                    AnchorY += pTextFrame->RowDistance;
                    WordNumInLine = 0;
                    AccLineLength = 0;
                    RenderBottom = RenderTop + 2;
                    i = RenderTop + 1;
                    CharDistanceModified = CharDistance;
                }
            }
        }
    }
}
