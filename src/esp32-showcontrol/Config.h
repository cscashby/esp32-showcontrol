#ifndef Config_h
#define Config_h

// Display variables (all prefix LCD_)
#define LCD_SCL   14 //default / hard wired so unused
#define LCD_SDA   2  //default / hard wired so unused
#define LCD_CS    5  
#define LCD_DC    4 
#define LCD_RST   3

#define LCD_WIDTH 96
#define LCD_HEIGHT 64

#define LCD_BG_COLOR BLACK

const unsigned char LCD_HEART [] PROGMEM = {
  // 'LCD_HEART, 16x16px
  0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x42, 0x42, 0x83, 0xc3, 0x80, 0x03, 0xc0, 0x03, 0xc0, 0x03, 
  0x20, 0x04, 0x30, 0x0c, 0x18, 0x18, 0x04, 0x20, 0x02, 0x40, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00
};
#define LCD_HEART_WIDTH 16
#define LCD_HEART_HEIGHT 16
#define LCD_HEART_X LCD_WIDTH-LCD_HEART_WIDTH
#define LCD_HEART_Y 0
#define LCD_HEART_COLOR BLUE

#endif
