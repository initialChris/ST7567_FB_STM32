#include "ST7567_FB.h"
#include "user.h"
#include <stdio.h>
#include "../../fonts/small4x7_font.h"

extern ST7567_FB lcd;
extern void Set_LCD_Brightness(uint8_t brightness);

void demo_setup() 
{
  lcd.init();
}

// --------------------------------------------------------------------------
char buf[40];

void demo_loop() 
{
  int i;
  int yt=(SCR_HT-8)/2;
  lcd.setFont(Small4x7PL);

  lcd.cls();
  lcd.drawRect(0,0,SCR_WD,SCR_HT,1);
  lcd.printStr(ALIGN_CENTER, yt, "ST7567_INVERT_OFF", SET);
  lcd.display();
  lcd.displayInvert(false);
  HAL_Delay(3000);
  lcd.fillRect(10,yt,SCR_WD-20,8,0);
  lcd.printStr(ALIGN_CENTER, yt, "ST7567_INVERT_ON", SET);
  lcd.display();
  lcd.displayInvert(true);
  HAL_Delay(3000);
  lcd.displayInvert(false);

  lcd.fillRect(10,yt,SCR_WD-20,8,0);
  lcd.printStr(ALIGN_CENTER, yt, "ST7567_DISPLAY_OFF", SET);
  lcd.display();
  HAL_Delay(1000);
  lcd.displayOn(false);
  HAL_Delay(1000);
  lcd.fillRect(10,yt,SCR_WD-20,8,0);
  lcd.printStr(ALIGN_CENTER, yt, "ST7567_DISPLAY_ON", SET);
  lcd.display();
  lcd.displayOn(true);
  HAL_Delay(2000);

  for(i=0;i<=20;i+=2) {
    snprintf(buf,40,"CONTRAST %d", i);
    lcd.fillRect(10,yt,SCR_WD-20,8,0);
    lcd.printStr(ALIGN_CENTER, yt, buf, SET);
    lcd.display();
    lcd.setContrast(i);
    HAL_Delay(1000);
  }
  lcd.fillRect(10,yt,SCR_WD-20,8,0);
  lcd.printStr(ALIGN_CENTER, yt, "CONTRAST 7", SET);
  lcd.display();
  lcd.setContrast(7);
  HAL_Delay(2000);

  for(i=0;i<100;i+=5) {
    snprintf(buf,40,"BACKLIGHT %d", i);
    lcd.fillRect(10,yt,SCR_WD-20,8,0);
    lcd.printStr(ALIGN_CENTER, yt, buf, SET);
    lcd.display();
    Set_LCD_Brightness(i);
    HAL_Delay(400);
  }
  Set_LCD_Brightness(100);
  lcd.fillRect(10,yt,SCR_WD-20,8,0);
  lcd.printStr(ALIGN_CENTER, yt, "BACKLIGHT MAX", SET);
  lcd.display();
  HAL_Delay(1000);

  lcd.cls();
  lcd.printStr(0, 0*8, "0", SET);
  lcd.printStr(0, 1*8, "1", SET);
  lcd.printStr(0, 2*8, "2", SET);
  lcd.printStr(0, 3*8, "3", SET);
  lcd.printStr(0, 4*8, "4", SET);
  lcd.printStr(0, 5*8, "5", SET);
  lcd.printStr(0, 6*8, "6", SET);
  lcd.printStr(0, 7*8, "7", SET);

  lcd.fillRect(10,yt,SCR_WD-10,8,0);
  lcd.printStr(ALIGN_CENTER, yt, " ROTATE 180 ", SET);
  lcd.setRotation(2);
  lcd.display();
  HAL_Delay(3000);
  lcd.fillRect(10,yt,SCR_WD-10,8,0);
  lcd.printStr(ALIGN_CENTER, yt, "   ROTATE 0  ", SET);
  lcd.setRotation(0);
  lcd.display();
  HAL_Delay(2000);

  for(i=0;i<64;i++) {
    lcd.fillRect(10,yt,SCR_WD-10,8,0);
    snprintf(buf,40,"    SCROLL %02d    ", i);
    lcd.printStr(ALIGN_CENTER, yt, buf, SET);
    lcd.display();
    lcd.setScroll(i);
    HAL_Delay(200);
  }
  lcd.fillRect(10,yt,SCR_WD-10,8,0);
  lcd.printStr(ALIGN_CENTER, 4, "  SCROLL 0  ", SET);
  lcd.display();
  lcd.setScroll(0);
  HAL_Delay(2000);
}

