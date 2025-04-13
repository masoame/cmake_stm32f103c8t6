#pragma once
#include"oledfront.hpp"
#include "common.hpp"

#define OLED_SCL_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)
#define OLED_SCL_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET)

#define OLED_SDA_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)
#define OLED_SDA_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET)



#define OLED_CMD  0	
#define OLED_DATA 1	

namespace oled {
    
    using u8 = unsigned char;
    using u16 = unsigned short;
    using u32 = unsigned int;

    extern void OLED_ClearPoint(u8 x,u8 y);
    extern void OLED_ColorTurn(u8 i);
    extern void OLED_DisplayTurn(u8 i);
    // extern void I2C_Start(void);
    // extern void I2C_Stop(void);
    // extern void I2C_WaitAck(void);
    // extern void Send_Byte(u8 dat);
    extern void OLED_WR_Byte(u8 dat,u8 mode);
    extern void OLED_DisPlay_On(void);
    extern void OLED_DisPlay_Off(void);
    extern void OLED_Refresh(void);
    extern void OLED_Clear(void);
    extern void OLED_DrawPoint(u8 x,u8 y,u8 t);
    extern void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
    extern void OLED_DrawCircle(u8 x,u8 y,u8 r);
    extern void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
    extern void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
    extern void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
    extern void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
    extern void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
    extern void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
    extern void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,const u8 BMP[],u8 mode);
    extern void OLED_Init(void);
    
}