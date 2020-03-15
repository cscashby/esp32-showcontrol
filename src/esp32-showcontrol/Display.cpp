#include "Arduino.h"
#include "Display.h"

Display::Display() {
}

void Display::begin() {
  tft = new SSD_13XX(LCD_CS, LCD_DC, LCD_RST);
  tft->begin();
}

void Display::loop() {
  
}

void Display::heartbeat() {
  uint16_t col;
  if( heartbeatStatus ) {
    col = LCD_HEART_COLOR;
  } else {
    col = LCD_BG_COLOR;
  }
  tft->fillRect(LCD_HEART_X, LCD_HEART_Y, LCD_HEART_WIDTH, LCD_HEART_HEIGHT, LCD_BG_COLOR);
  tft->drawBitmap(LCD_HEART_X, LCD_HEART_Y, LCD_HEART, LCD_HEART_WIDTH, LCD_HEART_HEIGHT, col);
  heartbeatStatus = !heartbeatStatus;
}
void Display::brokenHeart() {
  uint16_t col;
  if( heartbeatStatus ) {
    col = LCD_BROKENHEART_COLOR;
  } else {
    col = LCD_BG_COLOR;
  }
  tft->fillRect(LCD_BROKENHEART_X, LCD_BROKENHEART_Y, LCD_BROKENHEART_WIDTH, LCD_BROKENHEART_HEIGHT, LCD_BG_COLOR);
  tft->drawBitmap(LCD_BROKENHEART_X, LCD_BROKENHEART_Y, LCD_BROKENHEART, LCD_BROKENHEART_WIDTH, LCD_BROKENHEART_HEIGHT, col);
  heartbeatStatus = !heartbeatStatus;
}

void Display::clearTextArea() {
  // Left of heart
  tft->fillRect(TEXTAREA_X, TEXTAREA_Y, TEXTAREA_WIDTH - LCD_BROKENHEART_WIDTH, TEXTAREA_HEIGHT, LCD_BG_COLOR);
  // Below heart
  tft->fillRect(LCD_BROKENHEART_X, TEXTAREA_Y + LCD_BROKENHEART_HEIGHT, LCD_BROKENHEART_WIDTH, TEXTAREA_HEIGHT - LCD_BROKENHEART_HEIGHT, LCD_BG_COLOR);
  tft->setCursor(TEXTAREA_X, TEXTAREA_Y);
}

String Display::truncateString(String in, long maxChars) {
  if( in.length() > maxChars) {
    return in.substring(0, maxChars) + "...";
  } else {
    return in;
  }
}
