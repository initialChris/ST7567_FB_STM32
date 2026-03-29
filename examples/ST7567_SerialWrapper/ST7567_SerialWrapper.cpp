#include "ST7567_FB.h"
#include "user.h"
#include "serial.h"
#include "../../fonts/c64enh_font.h"

extern ST7567_FB lcd;
extern UART_HandleTypeDef huart2;

extern void parseSerialCommand();

void demo_setup() 
{
  serialBegin(&huart2);
  lcd.init(4);
  lcd.cls();
  lcd.setFont(c64enh);
  lcd.printStr(ALIGN_CENTER, 28, "Waiting!", SET);
  lcd.drawRectD(0,0,128,64,1);
  lcd.drawRect(18,20,127-18*2,63-20*2,1);
  lcd.display();
}

void demo_loop() 
{
    if (serialAvailable()) {
        parseSerialCommand();
    }
}
