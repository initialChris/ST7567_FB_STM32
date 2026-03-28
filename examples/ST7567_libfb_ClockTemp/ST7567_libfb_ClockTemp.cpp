#include "ST7567_FB.h"
#include "main.h"
#include <stdio.h>

extern ST7567_FB lcd;

// from PropFonts library
#include "../../fonts/c64enh_font.h"
#include "../../fonts/term8x10_font.h"
#include "../../fonts/term11x24bolddig_font.h"

void demo_setup() 
{
  lcd.init();
  lcd.cls();
  /*
  lcd.setFont(c64enh);
  lcd.printStr(ALIGN_CENTER, 28, "Hello World!");
  lcd.drawRect(18,20,127-18*2,63-20*2,1);
  */
  /*
  lcd.setFont(Term11x24b);
  lcd.setDigitMinWd(12);
  lcd.setCharMinWd(5);
  lcd.printStr(0, 0, "12:345678");
  */
  lcd.display();
}

int hour=13,minute=56,second=25,day=20,month=6;
float temperature=23.6;
int humidity=54;
char buf[30];

void demo_loop() 
{
  int x;
  lcd.cls();

  lcd.setFont(Term11x24b);
  lcd.setDigitMinWd(12); lcd.setCharMinWd(5);
  snprintf(buf,25,"%d:%02d",hour,minute);
  char secs[3];
  int xp,wd = lcd.strWidth(buf);
  snprintf(secs,3,"%02d",second);
  lcd.setFont(Term8x10);
  lcd.setDigitMinWd(12); lcd.setCharMinWd(5);
  wd+=lcd.strWidth(secs);
  xp = 4+(SCR_WD-wd)/2;

  lcd.setFont(Term11x24b);
  lcd.setDigitMinWd(12);
  lcd.setCharMinWd(5);
  x=lcd.printStr(xp, 2*8, buf);
  lcd.setFont(Term8x10);
  lcd.printStr(x, 2*8, secs);

  lcd.setFont(Term8x10);
  int t1 = temperature;
  int t10 =temperature*10-t1*10;
  snprintf(buf,25,"%d.%d' ",t1,t10); lcd.printStr(1, 6*8, buf);
  snprintf(buf,25," %d%%",humidity); lcd.printStr(ALIGN_RIGHT, 6*8, buf);
  
  lcd.setFont(c64enh);
  snprintf(buf,25,"%02d.%02d ",day,month); lcd.printStr(ALIGN_LEFT, 0, buf);
  snprintf(buf,25," %s", "Thu");   lcd.printStr(ALIGN_RIGHT, 0, buf);
  lcd.display();
  HAL_Delay(1000);
  second++;
  if(second>=60) { second=0;minute++; }
  if(minute>=60) { minute=0;hour++; }
}

