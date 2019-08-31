#include "Arduino.h"
#include "Config.h"
#include "Display.h"

Display::Display() {
  tft = new SSD_13XX(LCD_CS, LCD_DC, LCD_RST);
  tft->begin();
}

void Display::heartBeat() {
  uint16_t col;
  if( heartBeatStatus ) {
    col = LCD_HEART_COLOR;
  } else {
    col = LCD_BG_COLOR;
  }
  tft->drawBitmap(LCD_HEART_X, LCD_HEART_Y, LCD_HEART, LCD_HEART_WIDTH, LCD_HEART_HEIGHT, col);
  heartBeatStatus = !heartBeatStatus;
}
void Display::brokenHeart() {
  uint16_t col;
  if( heartBeatStatus ) {
    col = LCD_BROKENHEART_COLOR;
  } else {
    col = LCD_BG_COLOR;
  }
  tft->drawBitmap(LCD_BROKENHEART_X, LCD_BROKENHEART_Y, LCD_BROKENHEART, LCD_BROKENHEART_WIDTH, LCD_BROKENHEART_HEIGHT, col);
  heartBeatStatus = !heartBeatStatus;
}
