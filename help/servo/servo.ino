//Температура
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Подключаем датчики температуры
#include <OneWire.h>
#include <DallasTemperature.h>

// Датчки подключены к 1 порту
OneWire oneWire(A0);
DallasTemperature sensors(&oneWire);

DeviceAddress Thermometer1 = {
    0x28, 0xFF, 0xA2, 0xD2, 0x01, 0x17, 0x04, 0xB6
    // адрес датчика DS18B20 28FFA2D2011704B6
};

// Подключаем библиотеку Севродвигателя
#include <Servo.h>
Servo iv_srv_1;

// Подключаем ИК
#include <IRremote.h>
IRrecv irrecv(2);
decode_results results;

// Подключаем экран
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // Задаем адрес и размерность дисплея.

// Обявляем реле
int iv_light_pin = 8; // Подсветка 220В
int iv_fan1_pin = 9;  // Обдув лампочек
int iv_fan2_pin = 10; // Обдув короба

// Обяъвляем переменные
int iv_serv_0 = 90;     // Угол поворота.
bool iv_back_light = 1; // Подсветка экрана
int iv_day = 1;         // Текущий день

unsigned long iv_timer = 0;      // Таймер дня
unsigned long iv_srv_timer = 0;  // Таймер переворота
unsigned long iv_cool_timer = 0; // Таймер обдува
unsigned long iv_temp_timer = 0; // Таймер опроса датчиков
unsigned long lcd_time = 0;      // Переменная времени для выключения экрана

float iv_temp = 37.9; // Желаемая температура
float iv_temp_1 = 20; // Показания первого датчика

int dht_h;
int dht_t;

void setup()
{
  //
  // Инициализируем все устройства
  //

  // Запуск температурных датчиков
  dht.begin();

  sensors.begin();
  sensors.setResolution(Thermometer1, 12);

  iv_srv_1.attach(7); // Пин сервы

  irrecv.enableIRIn(); // ИК пульт

  lcd.init();      // Инициализация lcd
  lcd.backlight(); // Включаем подсветку
  lcd.setCursor(0, 0);
  lcd.print("Hello!");

  iv_timer = millis(); // Таймер дня
  iv_srv_timer = millis();
  iv_temp_timer = millis();

  // Активируем реле
  // Реле лампочек
  pinMode(iv_light_pin, OUTPUT);
  digitalWrite(iv_light_pin, HIGH);
  // Реле кулера 1
  pinMode(iv_fan1_pin, OUTPUT);
  digitalWrite(iv_fan1_pin, HIGH);
  // Реле кулера 2
  pinMode(iv_fan2_pin, OUTPUT);
  digitalWrite(iv_fan2_pin, HIGH);
}

void loop()
{

  //
  // День 24*60*60*1000 = 86400000
  //
  if ((millis() - iv_timer) > 86400000)
  {
    // Если прошли сутки
    iv_day++;
    iv_timer = millis();
    iv_days();
  }

  //
  // Термостат
  //
  if ((millis() - iv_temp_timer) > 5000)
  {
    // Опрашиваем датчики раз в 5 секунд
    get_temp(); // Получем температуру
    iv_temp_timer = millis();
  }

  if ((digitalRead(iv_light_pin) == LOW) && (iv_temp_1 > (iv_temp + 0.2)))
  {
    // Если пошел перегрев
    // Все отключаем
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Light OFF");

    digitalWrite(iv_light_pin, HIGH);
    delay(10000); // Задержка 10 секунд
    digitalWrite(iv_fan1_pin, HIGH);
  }

  if ((digitalRead(iv_light_pin) == HIGH) && (iv_temp_1 < (iv_temp - 0.2)))
  {
    // Если пошло охлаждение
    // Все включаем
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Light ON");

    digitalWrite(iv_light_pin, LOW);
    digitalWrite(iv_fan1_pin, LOW);
    delay(10000); // Задержка 10 секунд
  }

  //
  // Крутилка
  //
  if (iv_day < 20)
  {
    // После 20 дня переворачивать не надо
    if ((millis() - iv_srv_timer) > 10800000)
    {
      // Переворачиваем каждые три часа 3*60*60*1000 = 10800000
      iv_srv_timer = millis();
      // Переворот
      if (iv_serv_0 > 90)
      {
        srv_set(60);
      }
      else
      {
        srv_set(120);
      }
    }
  }

  //
  // Обдув
  //
  if (iv_day > 11)
  {
    // Начинаем на 12 день
    if ((millis() - iv_cool_timer) > 43200000)
    {
      // Два раза в день: 12*60*60*1000 = 43200000
      iv_cool_timer = millis();

      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("Vent");

      // Все выключаем
      digitalWrite(iv_light_pin, HIGH);
      digitalWrite(iv_fan1_pin, HIGH);

      // Включаем обдув на 5 минут: 5*60*1000 = 300000
      digitalWrite(iv_fan2_pin, LOW);
      digitalWrite(iv_fan1_pin, LOW);
      delay(300000);
      digitalWrite(iv_fan2_pin, HIGH);
      digitalWrite(iv_fan1_pin, HIGH);
    }
  }

  // Выключаем экран
  if (iv_back_light)
  {
    if ((millis() - lcd_time) > 60000)
    { // Если прошла минута
      lcd_off();
    }
  }

  //
  // Обрабатываем нажатие кнопок
  //

  if (irrecv.decode(&results))
  {

    // При нажатии включаем экран и выставляем счетчик времени
    if (iv_back_light)
    {
      lcd_time = millis();
    }
    else
    {
      lcd_on();
      lcd_time = millis();
    }

    switch (results.value)
    {
    // #
    case 0xFF52AD:
      // переключаем подсветку
      lcd_off();
      break;

    // ок
    case 0xFF02FD:
      get_temp();
      // Серва на 90
      //srv_set(90);
      // Включаем подсветку
      //digitalWrite (iv_light_pin, LOW);
      //digitalWrite (iv_fan1_pin, LOW);
      //delay (10000);
      //digitalWrite (iv_light_pin, HIGH);
      //digitalWrite (iv_fan1_pin, HIGH);
      break;

    // 1
    case 0xFF6897:
      // Серва в 60
      srv_set(60);
      break;

    // 2
    case 0xFF9867:
      // Серва на 90
      srv_set(90);
      break;

    // 3
    case 0xFFB04F:
      // Серва на 120
      srv_set(120);
      break;

    // 4
    case 0xFF30CF:
      // Тест свет
      digitalWrite(iv_light_pin, LOW); // Включаем
      delay(15000);
      digitalWrite(iv_light_pin, HIGH); // Все выключаем
      break;

    // 5
    case 0xFF18E7:
      // Тест обдува света
      digitalWrite(iv_fan1_pin, LOW);
      delay(15000);
      digitalWrite(iv_fan1_pin, HIGH); // Все выключаем
      break;

    // 6
    case 0xFF7A85:
      // Тест вентиляции
      digitalWrite(iv_fan2_pin, LOW);
      delay(15000);
      digitalWrite(iv_fan2_pin, HIGH); // Все выключаем
      break;

    // вверх
    case 0xFF629D:
      // плюс день
      iv_day++;
      iv_days();
      break;

    // вниз
    case 0xFFA857:
      // минус день
      iv_day--;
      iv_days();
      break;
    }

    lcd_print();
    // Сбрасываем кнопки
    irrecv.resume();
  }
}

void lcd_print()
{
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("D:");
  lcd.print(iv_day);
  lcd.print(" T:");
  lcd.print(iv_temp_1);
  lcd.print(" H:");
  lcd.print(dht_h);
}

void srv_set(int pos)
{
  //
  // Функция поворачивает лотки медленно
  //
  if (pos > iv_serv_0)
  {
    for (int i = iv_serv_0; i <= pos; i++)
    {
      iv_srv_1.write(i);
      delay(40);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("Rotate: ");
      lcd.print(i);
    }
  }
  else
  {
    for (int i = iv_serv_0; i >= pos; i--)
    {
      iv_srv_1.write(i);
      delay(40);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("Rotate: ");
      lcd.print(i);
    }
  }
  iv_serv_0 = pos;
}

void get_temp()
{
  //
  // Функция считывает температуру
  //
  float iv_temp_0 = 0;

  // Запрашиваем температуру
  sensors.requestTemperatures();

  dht_h = dht.readHumidity();
  dht_t = dht.readTemperature();

  // Сохраняем температуру
  iv_temp_0 = sensors.getTempC(Thermometer1);
  // проверка на ошибку
  if (iv_temp_0 > -100)
    iv_temp_1 = iv_temp_0;
  //delay(5000); // Задержка 5 секунд
  lcd_print();
}

void iv_days()
{
  iv_temp = 37.9;
  if (iv_day > 11)
    iv_temp = 37.3;
  if (iv_day > 19)
    iv_temp = 37.0;
  if (iv_day == 20)
    srv_set(60); // На 20 день перестаем крутить
  lcd_print();
}

void lcd_on()
{
  iv_back_light = 1;
  lcd.backlight();
}

void lcd_off()
{
  iv_back_light = 0;
  lcd.noBacklight();
}
