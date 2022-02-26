#include <tim.h>
#include "WS2812.h"
#include "BMP.h"
#include "main.h"
#include "usart.h"
#include "math.h"

extern BMP_INFOHEADER INFO;
extern uint32_t send_Buf[NUM];
extern BMP_24 img_bmp24[IMG_WIDTH];


void LEDShow(void)
{
    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_2, (uint32_t *)send_Buf, NUM);
    HAL_Delay(100);
    HAL_TIM_PWM_Stop_DMA(&htim2,TIM_CHANNEL_2);
}

void LEDCloseAll(void)
{
    uint32_t i;
    for (i = 0; i < LED_NUM * 24; i++)
        send_Buf[i] = OFF;
    for (i = LED_NUM * 24; i < NUM; i++)
        send_Buf[i] = 0;
    LEDShow();
}

uint32_t WS281x_Color(uint8_t red, uint8_t green, uint8_t blue)
{
    return green << 16 | red << 8 | blue;
}

/* Color order is 'GRB' for WS2812 */
void WS281x_SetPixelColor(uint16_t n, uint32_t GRBColor)
{
    uint8_t i;
    if (n < LED_NUM)
        for (i = 0; i < 24; i++)
            send_Buf[24 * n + i] = (((GRBColor << i) & 0X800000) ? ON : OFF);
}

void WS281x_SetPixelRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t i;
    if (n <= LED_NUM)
        for (i = 0; i < 24; i++)
            send_Buf[24 * n + i] = (((WS281x_Color(red, green, blue) << i) & 0X800000) ? ON : OFF);
}

/* RGB to GRB */
uint32_t Change(uint32_t RGB)
{
    uint8_t R,G,B;
    R = RGB >> 16;
    G = (RGB >> 8) & 0xff;
    B = RGB & 0xff;
    return G << 16 | R << 8 | B;
}

uint8_t ReadShow(FIL* fp, BITMAPFILEHEADER* Header, BMP_INFOHEADER* INFO,BMP_24 bmp24[IMG_WIDTH])
{
    if (ImgReadHeader(Header, fp) == 0)
        return 0;
    ImgReadInfo(INFO,fp);
    f_lseek(fp,Header->bfOffBits);
    uint32_t byteswritten;

    if (INFO->biBitCount == 24)
    {
        LEDCloseAll();      //Close all WS2812 lights
        uint8_t buffer[3];
        for (int i = INFO->biHeight - 1; i >= 0; i--)       // i 为图像高度
        {
            for (int j = INFO->biWidth - 1; j >= 0; j--)         // j 为图像宽度（灯带数目）
            {
                f_read(fp,buffer,3*sizeof(uint8_t),&byteswritten);
                // fread(buffer, sizeof(uint8_t), 3, fp);
                img_bmp24[j].r_val = buffer[2];
                img_bmp24[j].g_val = buffer[1];
                img_bmp24[j].b_val = buffer[0];
                WS281x_SetPixelRGB(j,(img_bmp24[j].r_val)>>3,(img_bmp24[j].g_val)>>3, (img_bmp24[j].b_val)>>3);
//                UART_printf(&huart1,"r=%d,g=%d,b=%d  i=%d j=%d \r\n",img_bmp24[j].r_val,img_bmp24[j].g_val,img_bmp24[j].b_val,i,j);       //用于调试
            }
            LEDShow();
        }
    }
    else
    {
        return 0;       //Read error (IMG Bitcount is not 24)
    }
    LEDCloseAll();
    return 1;
}

uint32_t Wheel(uint8_t WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return WS281x_Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return WS281x_Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return WS281x_Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void ShowRainbow(uint8_t wait)
{
    uint32_t timestamp = HAL_GetTick();
    uint16_t i;
    static uint8_t j;
    static uint32_t next_time = 0;

    uint32_t flag = 0;
    if (next_time < wait)
    {
        if ((uint64_t)timestamp + wait - next_time > 0)
            flag = 1;
    }
    else if (timestamp > next_time)
    {
        flag = 1;
    }
    if (flag) // && (timestamp - next_time < wait*5))
    {
        j++;
        next_time = timestamp + wait;
        for (i = 0; i < LED_NUM; i++)
        {
            WS281x_SetPixelColor(i, Wheel((i + j) & 255));
        }
    }
    LEDShow();
}