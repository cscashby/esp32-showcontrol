#ifndef DisplayConfig_h
#define DisplayConfig_h

// Display variables (all prefix LCD_)
#define LCD_SCL   14 //default / hard wired so unused
#define LCD_SDA   2  //default / hard wired so unused
#define LCD_CS    5  
#define LCD_DC    4 
#define LCD_RST   3

#define LCD_WIDTH 96
#define LCD_HEIGHT 64

#define TEXTAREA_X 0
#define TEXTAREA_Y 0
#define TEXTAREA_WIDTH 96
#define TEXTAREA_HEIGHT 64

#define LCD_BG_COLOR BLACK

#define LINE_LENGTH 14

const unsigned char LCD_HEART [] PROGMEM = {
  // 'h, 16x16px
  0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x42, 0x42, 0x83, 0xc3, 0x80, 0x03, 0xc0, 0x03, 0xc0, 0x03, 
  0x20, 0x04, 0x30, 0x0c, 0x18, 0x18, 0x04, 0x20, 0x02, 0x40, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00
};
#define LCD_HEART_WIDTH 16
#define LCD_HEART_HEIGHT 16
#define LCD_HEART_X LCD_WIDTH-LCD_HEART_WIDTH
#define LCD_HEART_Y 0
#define LCD_HEART_COLOR GREEN
const unsigned char LCD_BROKENHEART [] PROGMEM = {
  // 'bh, 16x16px
  0x00, 0x00, 0x1c, 0x78, 0x76, 0xde, 0x42, 0xca, 0x83, 0x47, 0x87, 0x63, 0x84, 0xe1, 0xc6, 0x83, 
  0x43, 0x82, 0x61, 0x86, 0x31, 0x8c, 0x18, 0x98, 0x0c, 0x30, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00
};
#define LCD_BROKENHEART_WIDTH 16
#define LCD_BROKENHEART_HEIGHT 16
#define LCD_BROKENHEART_X LCD_WIDTH-LCD_BROKENHEART_WIDTH
#define LCD_BROKENHEART_Y 0
#define LCD_BROKENHEART_COLOR RED

#endif
