#include "ST7567_FB.h"
#include "main.h"
#include "math.h"
#include "../../fonts/c64enh_font.h"

extern ST7567_FB lcd;
#define PI 3.14159265358979323846

void HariChord(int frame);

void demo_setup() 
{
  lcd.init(4);
}

int nFrames = 36;

void demo_loop() 
{
  for (int i=0; i<nFrames; i++) HariChord(i);
  for (int i=(nFrames-1); i >= 0; i--) HariChord(i);
}

// by Hari Wiguna
void HariChord(int frame)
{
  lcd.clearDisplay();
  int n = 7;
  int r = frame * 64 / nFrames;
  float rot = frame * 2*PI / nFrames;
  for (int i=0; i<(n-1); i++) {
    float a = rot + i * 2*PI / n;
    int x1 = SCR_WD/2 + cos(a) * r;
    int y1 = SCR_HT/2 + sin(a) * r;
    for (int j=i+1; j<n; j++) {
      a = rot + j * 2*PI / n;
      int x2 = SCR_WD/2 + cos(a) * r;
      int y2 = SCR_HT/2 + sin(a) * r;
      lcd.drawLine(x1,y1, x2,y2, SET);
    }
  }
  lcd.display();
  HAL_Delay(50);
}

