// Fast ST7567 128x64 LCD graphics library (with frambuffer)
// (C) 2020 by Pawel A. Hernik

/*
 128x64 ST7567 connections in SPI mode (only 5-6 wires between LCD and MCU):

 #01 LED  -> D6, GND or any pin via resistor
 #02 RST  -> D9 or any pin
 #03 CS   -> D10 or any pin
 #04 DC   -> D8 or any pin
 #05 SCK  -> D13/SCK
 #06 SDI  -> D11/MOSI
 #07 3V3  -> VCC (3.3V)
 #08 GND  -> GND
*/

#ifndef _ST7567_FB_H
#define _ST7567_FB_H


#include "stm32f4xx_hal.h"

#define SCR_WD  128
#define SCR_HT  64
#define SCR_HT8 8  // SCR_HT/8

#define ALIGN_LEFT    0
#define ALIGN_RIGHT  -1
#define ALIGN_CENTER -2

#define SET 1
#define CLR 0
#define XOR 2

struct _propFont
{
  const uint8_t* font;
  int8_t xSize;
  uint8_t ySize;
  uint8_t firstCh;
  uint8_t lastCh;
  uint8_t minCharWd;
  uint8_t minDigitWd;
};

// ---------------------------------
class ST7567_FB {
public:
  ST7567_FB(SPI_HandleTypeDef *hspi, GPIO_TypeDef *dc_port, uint16_t dc_pin, GPIO_TypeDef *cs_port, uint16_t cs_pin,  GPIO_TypeDef *rst_port, uint16_t rst_pin);

  inline void sendSPI(uint8_t v) __attribute__((always_inline)); // costs about 350B of flash
  inline void sendCmd(uint8_t cmd);
  inline void sendData(uint8_t data);
  void init(uint8_t contrast=4);
  void begin() { init(); }
  void initCmds();
  void display();
  void copy(uint8_t x, uint8_t y8, uint8_t wd, uint8_t ht8);
  void gotoXY(uint8_t x, uint8_t y);
  void sleep(bool mode=true);
  void setContrast(uint8_t val);
  void setScroll(uint8_t val);
  void displayInvert(bool mode);
  void displayOn(bool mode);
  void displayMode(uint8_t val);
  void setRotation(uint8_t mode);

  void cls();
  void clearDisplay() { cls(); }
  void drawPixel(uint8_t x, uint8_t y, uint8_t col);
  void drawLine(int8_t x0, int8_t y0, int8_t x1, int8_t y1, uint8_t col);
  void drawLineH(uint8_t x0, uint8_t x1, uint8_t y, uint8_t col);
  void drawLineV(uint8_t x,  uint8_t y0, uint8_t y1, uint8_t col);
  void drawLineVfast(uint8_t x, uint8_t y0, uint8_t y1, uint8_t col);
  void drawLineVfastD(uint8_t x, uint8_t y0, uint8_t y1, uint8_t col);
  void drawLineHfast(uint8_t x0, uint8_t x1, uint8_t y, uint8_t col);
  void drawLineHfastD(uint8_t x0, uint8_t x1, uint8_t y, uint8_t col);
  void drawRect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t col);
  void drawRectD(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t col);
  void fillRect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t col);
  void fillRectD(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t col);
  void drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t col);
  void fillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t col);
  void fillCircleD(uint8_t x0, uint8_t y0, uint8_t r, uint8_t col);
  void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void fillTriangleD(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void setDither(int8_t s);
  int16_t drawBitmap(const uint8_t *bmp, int16_t x, int16_t y, uint8_t w, uint8_t h);
  int16_t drawBitmap(const uint8_t *bmp, int16_t x, int16_t y);

  void setFont(const uint8_t* f);
  void setCR(uint8_t _cr) { cr = _cr; }
  void setInvert(uint8_t _inv) { invertCh = _inv; }
  void setFontMinWd(uint8_t wd) { cfont.minCharWd = wd; }
  void setCharMinWd(uint8_t wd) { cfont.minCharWd = wd; }
  void setDigitMinWd(uint8_t wd) { cfont.minDigitWd = wd; }
  int16_t printChar(int16_t xpos, int16_t ypos, unsigned char c, uint8_t col = SET);
  int16_t printStr(int16_t xpos, int16_t ypos, const char *str, uint8_t col = SET);
  int16_t charWidth(uint8_t _ch, bool last=true);
  int16_t fontHeight();
  int16_t strWidth(const char *txt);
  unsigned char convertPolish(unsigned char _c);
  static bool isNumber(uint8_t ch);
  static bool isNumberExt(uint8_t ch);
  void setIsNumberFun(bool (*fun)(uint8_t)) { isNumberFun=fun; }

public:
  static uint8_t scr[SCR_WD*SCR_HT8];
  SPI_HandleTypeDef * _spi = nullptr;
  uint16_t _dcPin, _csPin, _rstPin;
  GPIO_TypeDef *_dcPort, *_csPort, *_rstPort;

  uint8_t scrWd = SCR_WD;
  uint8_t scrHt = SCR_HT8;
  int8_t rotation;

  static uint8_t ystab[8];
  static uint8_t yetab[8];
  static uint8_t pattern[4];
  static const uint8_t ditherTab[4*17];

//private:
  bool (*isNumberFun)(uint8_t ch);
  _propFont  cfont;
  uint8_t cr;  // carriage return mode for printStr
  uint8_t dualChar;
  uint8_t invertCh;
  uint8_t spacing = 1;
};
#endif
