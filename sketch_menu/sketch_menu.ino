#include <MenuSystem.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing O

void draw()
{
  u8g2.setFont(u8g2_font_9x15_tr);
  u8g2.userInterfaceSelectionList("Title", 2, "line 1\Node 2\Item 3");
  
  // u8g2.drawStr( 0, 16, "20\xB0 33%");
  // u8g2.setFont(u8g2_font_9x15_tr);
  // u8g2.drawStr( 0, 32, "(3) 22:35");
  // u8g2.setFont(u8g2_font_open_iconic_arrow_1x_t);
  // for (int i = 0; i < 16; i++) 
  //   u8g2.drawGlyph(8 * i, 32, 0x48 + i);
  
}

void setup() 
{
  Serial.begin(115200);
  Serial.println(F("LCD Test Start"));
  //u8g2.begin();
}

void loop() {
  //u8g2.clearBuffer();
  //draw();
  //u8g2.sendBuffer();
  delay(1500);
  Serial.println(F("Next draw1"));
}
