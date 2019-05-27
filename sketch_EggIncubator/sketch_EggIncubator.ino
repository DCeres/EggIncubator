#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define DHTPIN 2     // контакт, к которому подключаемся
#define DHTTYPE DHT11   // для датчика DHT11
// контакт для данных подключаем к цифровому контакту 3 на Arduino:
#define ONE_WIRE_BUS 3

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// создаем экземпляр класса для датчика DHT11:
DHT dht(DHTPIN, DHTTYPE);

// создаем экземпляр класса OneWire
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() 
{
  Serial.begin(115200);
  Serial.println(F("SSD1306 Start"));
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  Wire.begin();
  dht.begin(); // инициализируем объект «dht»  
  sensors.begin();  
  display.clearDisplay();
}

void displayTempHumid()
{
  delay(2000);
  // считывание данных о температуре и влажности
  // занимает около 250 мс; кроме того, это считывание
  // может запаздывать примерно на 2 секунды
  // (это очень медленный датчик):
  float h = dht.readHumidity();
  // считываем температуру в градусах Цельсия:
  float t = dht.readTemperature();

  sensors.requestTemperatures();
  float t2 = sensors.getTempCByIndex(0);
  // проверяем, корректны ли данные, и если нет,
  // то выходим и пробуем снова:
  if (isnan(h) || isnan(t) || isnan(t2)) 
  {
    display.clearDisplay();  //  очищаем дисплей
    display.setTextColor(WHITE);  //  задаем цвет
    display.setTextSize(1);  //  задаем шрифт
    display.setCursor(5,0);  //  задаем координаты курсора
    display.print("Failed to read from DHT sensor!");
    //  "Не удалось прочесть данные с датчика DHT!"
    return;
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Humidity: ");  //  "Влажность: "
  display.print(h);
  display.print(" %");
  display.setCursor(0,10);
  display.print("Temperature: ");  //  "Температура: "
  display.print(t);
  display.print(" C");
  display.setCursor(0,20);
  display.print("Temperature2: ");  //  "Температура: "
  display.print(t2);
  display.print(" C");
}

void loop() 
{
  displayTempHumid();
  display.display();
}
