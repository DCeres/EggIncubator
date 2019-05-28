#include <MenuSystem.h>
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

#define PCD8544_CHAR_HEIGHT 8

class MyRenderer : public MenuComponentRenderer {
public:
    void render(Menu const& menu) const {
        display.clearDisplay();
        menu.render(*this);
        menu.get_current_component()->render(*this);
        display.display();
    }

    void render_menu_item(MenuItem const& menu_item) const {
        display.setCursor(0, 1 * PCD8544_CHAR_HEIGHT);
        display.print(menu_item.get_name());
    }

    void render_back_menu_item(BackMenuItem const& menu_item) const {
        display.setCursor(0, 1 * PCD8544_CHAR_HEIGHT);
        display.print(menu_item.get_name());
    }

    void render_numeric_menu_item(NumericMenuItem const& menu_item) const {
        display.setCursor(0, 1 * PCD8544_CHAR_HEIGHT);
        display.print(menu_item.get_name());
    }

    void render_menu(Menu const& menu) const {
        display.setCursor(0, 0 * PCD8544_CHAR_HEIGHT);
        display.print(menu.get_name());
    }
};
MyRenderer my_renderer;

// Menu callback function

void on_item1_selected(MenuComponent* p_menu_component) {
    display.setCursor(0, 2 * PCD8544_CHAR_HEIGHT);
    display.print("Item1 Selectd");
    display.display();
    delay(1500); // so we can look the result on the LCD
}

void on_item2_selected(MenuComponent* p_menu_component) {
    display.setCursor(0, 2 * PCD8544_CHAR_HEIGHT);
    display.print("Item2 Selectd");
    display.display();
    delay(1500); // so we can look the result on the LCD
}

void on_item3_selected(MenuComponent* p_menu_component) {
    display.setCursor(0, 2 * PCD8544_CHAR_HEIGHT);
    display.print("Item3 Selectd");
    display.display();
    delay(1500); // so we can look the result on the LCD
}


// Menu variables

MenuSystem ms(my_renderer);
MenuItem mm_mi1("Lvl1-Item1(I)", &on_item1_selected);
MenuItem mm_mi2("Lvl1-Item2(I)", &on_item2_selected);
Menu mu1("Lvl1-Item3(M)");
MenuItem mu1_mi1("Lvl2-Item1(I)", &on_item3_selected);

void serial_print_help() {
    Serial.println("***************");
    Serial.println("w: go to previus item (up)");
    Serial.println("s: go to next item (down)");
    Serial.println("a: go back (right)");
    Serial.println("d: select \"selected\" item");
    Serial.println("?: print this help");
    Serial.println("h: print this help");
    Serial.println("***************");
}

void serial_handler() {
    char inChar;
    if((inChar = Serial.read())>0) {
        switch (inChar) {
            case 'w': // Previus item
                ms.prev();
                ms.display();
                break;
            case 's': // Next item
                ms.next();
                ms.display();
                break;
            case 'a': // Back pressed
                ms.back();
                ms.display();
                break;
            case 'd': // Select pressed
                ms.select();
                ms.display();
                break;
            case '?':
            case 'h': // Display help
                serial_print_help();
                break;
            default:
                break;
        }
    }
}


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

  display.setTextSize(1);
  display.clearDisplay();
  
  serial_print_help();

  ms.get_root_menu().add_item(&mm_mi1);
  ms.get_root_menu().add_item(&mm_mi2);
  ms.get_root_menu().add_menu(&mu1);
  mu1.add_item(&mu1_mi1);
  ms.display();

  displayTempHumid();
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
  serial_handler();
  //displayTempHumid();
  display.display();
}
