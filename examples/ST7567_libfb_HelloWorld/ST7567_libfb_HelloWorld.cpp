#include "ST7567_FB.h"
#include "user.h"
#include "../../fonts/c64enh_font.h"

extern ST7567_FB lcd;

void demo_setup() 
{
  lcd.init(4);
  lcd.cls();
  lcd.setFont(c64enh);
  lcd.printStr(ALIGN_CENTER, 28, "Hello World!", SET);
  lcd.drawRectD(0,0,128,64,1);
  lcd.drawRect(18,20,127-18*2,63-20*2,1);
  lcd.display();
}

void demo_loop() 
{
}
