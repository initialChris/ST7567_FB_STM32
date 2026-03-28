#include "ST7567_FB.h"
#include "main.h"

extern ST7567_FB lcd;

void demo_setup() 
{
  lcd.init();
  lcd.cls();
  lcd.drawRectD(0,0,SCR_WD,SCR_HT,1);

  int x=16,y=10;
  lcd.drawRect(x+8,y-5,20,20,1);
  lcd.fillRect(x+8+30,y-5,20,20,1);
  lcd.fillRectD(x+8+60,y-5,20,20,1);

  lcd.drawCircle(x+5+12,48,y,1);
  lcd.fillCircle(x+5+30+12,y+36,12,1);
  lcd.fillCircleD(x+5+60+12,y+36,12,1);

  lcd.display();
}

void demo_loop() 
{
}

