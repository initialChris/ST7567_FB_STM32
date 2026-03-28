#include "main.h"
#include "user.h"

#include <ST7567_FB.h>

#include <cstdlib>

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;

extern void demo_setup(void);
extern void demo_loop(void);


ST7567_FB lcd(&hspi1, LCD_DC_GPIO_Port, LCD_DC_Pin, LCD_CS_GPIO_Port, LCD_CS_Pin, LCD_RST_GPIO_Port, LCD_RST_Pin);

void Set_LCD_Brightness(uint8_t brightness) {
    if (brightness > 100) brightness = 100;
    uint32_t pulse = (brightness * (__HAL_TIM_GET_AUTORELOAD(&htim3))) / 100;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pulse);
}

void setup(void){
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    Set_LCD_Brightness(100);
    demo_setup();
}

void loop(void){

    demo_loop();
}

