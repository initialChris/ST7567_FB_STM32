#include "ST7567_FB.h"
#include "main.h"
#include <stdio.h>
#include <math.h>


extern ST7567_FB lcd;
#include ".../../fonts/c64enh_font.h"

#define random(max) (rand() % (max))

char buf[20];

void demo_setup() 
{
  lcd.init();
}

float t=0;

void animSinLines()
{
  lcd.cls();
  float x;
  for(int i=0;i<64;i++) {
    x=20.0*(2.0+sin(t/10.0+i/18.0)*sin(i/9.0));
    lcd.drawLineHfastD(64-x,64+x,i,1);
    x=10.0*(2.0+sin(t/8.0+i/15.0)*sin(i/5.0));
    lcd.drawLineHfast(64-x,64+x,i,1);
  }
  t+=0.7;
  lcd.display();
}

const char *banner="ST7567 LCD Library";

void animTextSin()
{
  lcd.cls();
  lcd.setFont(c64enh);
  const char *txt=banner;
  int x=4,i=0;
  float y;
  while(*txt) {
    y=20+10.0*(1.0+sin(t/10.0+i/6.0)*sin(t/18.0+i/9.0));
    x+=lcd.printChar(x,(int)y,*txt++);
    i++;
  }
  t+=0.7;
  lcd.display();
}

void animTextCircle()
{
  lcd.cls();
  lcd.setFont(c64enh);
  const char *txt=banner;
  int x=4;
  float y,y2,xr;
  float r=10.0+6.0*(1.0+sin(t/28.0));
  float xo=38.0*(0.0+sin(t/20.0));
  float yt=24;
  while(*txt) {
    xr=2+x-64-xo;
    y2=(r+3)*(r+3)-(xr)*(xr);
    y=yt;
    if(y2>0) y=32-sqrt(y2)-5;
    if(y<0) y=0;
    if(y>yt) y=yt;
    x+=lcd.printChar(x,(int)y,*txt++);
  }
  
  lcd.fillCircleD(64+xo,32,r,1);
  //lcd.drawCircle(64+xo,32,r+1,1);
  t+=0.4;
  lcd.display();
}

void rects()
{
  int x,y;
  x = random(128);
  y = random(40);
  lcd.setDither(random(12));
  lcd.fillRect(x,y,30,30,0); 
  lcd.fillRectD(x,y,30,30,1); 
  lcd.drawRect(x,y,30,30,1);
  lcd.display();
//HAL_Delay(100);
}

void circles()
{
  int x,y;
  x = 15+random(128-30);
  y = 15+random(64-30);
  lcd.setDither(random(12));
  lcd.fillCircle(x,y,15,0);
  lcd.fillCircleD(x,y,15,1);
  lcd.drawCircle(x,y,16,1);
  lcd.display();
//HAL_Delay(100);
}

int x=40,y=0;
int dx=6,dy=5;
int x2=80,y2=40;
int dx2=9,dy2=8;

void animRect()
{
  x+=dx;
  y+=dy;
  if(x>64 || x<0) { dx=-dx; x+=dx; }
  if(y>32 || y<0) { dy=-dy; y+=dy; }
  lcd.fillRect(x,y,64,32,2);
  lcd.display();
  lcd.fillRect(x,y,64,32,2);
  HAL_Delay(40);
}

#define MAX_LINES 10
int8_t lx0[MAX_LINES];
int8_t lx1[MAX_LINES];
int8_t ly0[MAX_LINES];
int8_t ly1[MAX_LINES];
int8_t curLine=0;

void animLines()
{
  x+=dx;
  y+=dy;
  x2+=dx2;
  y2+=dy2;
  if(x>127) { dx=-dx; x=127; }
  if(x<1) { dx=-dx; x=0; }
  if(y>63) { dy=-dy; y=63; }
  if(y<1) { dy=-dy; y=0; }
  if(x2>127) { dx2=-dx2; x2=127; }
  if(x2<1) { dx2=-dx2; x2=0; }
  if(y2>63) { dy2=-dy2; y2=63; }
  if(y2<1) { dy2=-dy2; y2=0; }
  lx0[curLine]=x;
  lx1[curLine]=x2;
  ly0[curLine]=y;
  ly1[curLine]=y2;
  if(++curLine>=MAX_LINES) curLine=0;
  lcd.cls();
  for(int i=0;i<MAX_LINES;i++) {
    lcd.drawLine(lx0[i],ly0[i],lx1[i],ly1[i],1);
    lcd.drawLine(127-lx0[i],ly0[i],127-lx1[i],ly1[i],1);
    lcd.drawLine(lx0[i],63-ly0[i],lx1[i],63-ly1[i],1);
    lcd.drawLine(127-lx0[i],63-ly0[i],127-lx1[i],63-ly1[i],1);
  }
  lcd.display();
  HAL_Delay(50);
}

unsigned long tm,demoTime=14000;

void demo_loop() 
{
  tm = HAL_GetTick();
  lcd.cls();
  while(HAL_GetTick()-tm<demoTime) rects();

  tm = HAL_GetTick();
  lcd.cls();
  while(HAL_GetTick()-tm<demoTime) circles();

  lcd.setDither(8);
  tm = HAL_GetTick();
  while(HAL_GetTick()-tm<demoTime) animSinLines();

  tm = HAL_GetTick();
  while(HAL_GetTick()-tm<demoTime) animTextSin();

  tm = HAL_GetTick();
  while(HAL_GetTick()-tm<demoTime) animTextCircle();

  tm = HAL_GetTick();
  while(HAL_GetTick()-tm<demoTime) animLines();

  lcd.setFont(c64enh);
  lcd.fillRect(0,27,128,9,0);
  lcd.printStr(ALIGN_CENTER, 28, banner);
  x=40; y=0;
  dx=2; dy=1;
  tm = HAL_GetTick();
  while(HAL_GetTick()-tm<demoTime) animRect();
}

