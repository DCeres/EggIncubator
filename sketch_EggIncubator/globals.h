#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#include <DHT.h>

#include <OneWire.h>
#include <DallasTemperature.h>



enum pinAdress
{
    // матриця кнопок керування 
    pin_NTC1_s = A0,    
    // Аналогові температурний сенсор
    pin_NTC2_s = A1,
    // рівень освітлення 
    pin_Light_s = A2,
    // вімірювання напруги
    pin_Volt_s = A3,

    // Сенсор температури на рівні досить точний
    pin_DT_s = 2,
    // Сенсор вологості
    pin_DHT_s = 4,    
    // Вхідна заслока
    pin_servo = 3,
    // Підігрів жарівками
    pin_Light1 = 5,
    pin_Light2 = 6,
    pin_Light3 = 9,
    
    // потік повітря
    pin_FanT = 7,
    pin_FanH = 8,

    // Шаговий двигун
    pin_engine11 = 10,
    pin_engine12 = 11,
    pin_engine21 = 12,
    pin_engine22 = 13,

};