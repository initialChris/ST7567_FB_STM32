#include "ST7567_FB.h"
#include "main.h"
#include <stdio.h>

extern ST7567_FB lcd;

// from PropFonts library
#include ".../../fonts/c64enh_font.h"
#include ".../../fonts/tiny3x7sq_font.h"

#define delay(x) HAL_Delay(x)

char buf[20];

void demo_setup() 
{
  lcd.init();
  lcd.cls();
  for(int j=1;j<17;j++) { lcd.setDither(j); lcd.fillRectD((j-1)*8,0,8,64,1); }
  lcd.display(); 
  HAL_Delay(5000);
}

void sample(int s)
{
  lcd.cls();
  lcd.setDither(s);
  lcd.setFont(c64enh);
  snprintf(buf,20,"Pattern: %02d",s);
  lcd.printStr(ALIGN_CENTER, 2, buf);
  lcd.fillRectD(4,13,68,32,1);
  lcd.fillCircleD(100,28,16,1);
  for(int j=1;j<17;j++) {
    lcd.setFont(Tiny3x7SqPL);
    snprintf(buf,20,"%2d",j);
    lcd.printStr((j-1)*8, 64-8*2, buf);
    lcd.setDither(j);
    lcd.fillRectD((j-1)*8,64-8,8,8,1);
  }
  lcd.display();
  HAL_Delay(500);
}

void demo_loop() 
{
  for(int i=0;i<=16;i++) sample(i);
  for(int i=16;i>0;i--) sample(i);
}

