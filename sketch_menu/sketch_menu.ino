#include <MenuSystem.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing O

void draw()
{
  u8g2.setFont(u8g2_font_9x15_tf);
  u8g2.drawStr( 0, 16, "20\xB0 33%");
  u8g2.setFont(u8g2_font_9x15_tf);
  u8g2.drawStr( 0, 32, "(3) 22:35");
}

void setup() 
{
  u8g2.begin();
}

void loop() {
  u8g2.clearBuffer();
  draw();
  u8g2.sendBuffer();
  delay(500);
}
