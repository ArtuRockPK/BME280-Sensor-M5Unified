/*
    Привет с канала / Hi there from channel  
    https://youtube.com/@hobbysupport?si=pXP9KaTQCQTer35v
    Первым делом нужно установить библиотеку M5Unified.h её легко найти в Менеджере библиотек 
    После чего установить библиотеку BME280I2C.h by Tyler Glenn , её так же можно найти менеджере библиотек,
    но если не получилось найти она лежит в папке проекта. 
    First install M5Unified library via library manager. 
    After that, install the library BME280I2C.h by Tyler Glenn, it can also be found in the library manager, 
    but if you can't find it, it is located in the project folder.
*/

#include <BME280I2C.h>  // Подключение библиотеки для работы с датчиком BME280 через I2C. / Including the library for working with BME280 sensor via I2C.
#include <Wire.h>       // Подключение библиотеки для работы с шиной I2C. / Including the library for I2C bus communication.
#include <M5Unified.h>  // Подключение библиотеки для работы с устройствами M5Stack (дисплей, кнопки и т.д.). / Including the library for working with M5Stack devices (display, buttons, etc.).

#define SERIAL_BAUD 115200  // Установка скорости передачи данных для последовательного порта. / Setting the baud rate for the serial port.

BME280I2C bme;  // Создание объекта для работы с датчиком BME280. / Creating an object to work with the BME280 sensor.
byte lang = 0;  // Переменная для выбора языка (0 — английский, 1 — русский). / Variable to select the language (0 — English, 1 — Russian).

// Функция для отображения данных с датчика BME280 / Function to display data from the BME280 sensor
void printBME280Data(byte language) {
  float temp(NAN), hum(NAN), pres(NAN);  // Объявление переменных для хранения температуры, влажности и давления. / Declaring variables to store temperature, humidity, and pressure.

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);  // Установка единиц измерения температуры (градусы Цельсия). / Setting temperature units (Celsius).
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);       // Установка единиц измерения давления (паскали). / Setting pressure units (Pascals).
  bme.read(pres, temp, hum, tempUnit, presUnit);        // Считывание данных с датчика (давление, температура, влажность). / Reading data from the sensor (pressure, temperature, humidity).

  if (language % 2 == 0) {                                                // Если язык установлен на английский (lang == 0): / If the language is set to English (lang == 0):
    M5.Display.setFont(&fonts::efontJA_10);                               // Установка шрифта для дисплея. / Setting the font for the display.
    M5.Display.drawString("BME280 Sensor Test", 8, 0);                    // Вывод строки с заголовком на дисплей. / Displaying a string with a title on the screen.
    M5.Display.drawString("Temp: " + String(temp) + "°C", 8, 20);         // Вывод температуры. / Displaying the temperature.
    M5.Display.drawString("Humidity: " + String(hum) + "%", 8, 40);       // Вывод влажности. / Displaying the humidity.
    M5.Display.drawString("Pressure: " + String(pres) + "Pa", 8, 60);     // Вывод давления. / Displaying the pressure.
  } else {                                                                // Если язык установлен на русский (lang == 1): / If the language is set to Russian (lang == 1):
    M5.Display.setFont(&fonts::efontJA_10);                               // Установка шрифта для дисплея. / Setting the font for the display.
    M5.Display.drawString("BME280 тест сенсора", 8, 0);                   // Вывод строки с заголовком на дисплей. / Displaying a string with a title on the screen.
    M5.Display.drawString("Температура: " + String(temp) + "°C", 8, 20);  // Вывод температуры. / Displaying the temperature.
    M5.Display.drawString("Влажность: " + String(hum) + "%", 8, 40);      // Вывод влажности. / Displaying the humidity.
    M5.Display.drawString("Давление: " + String(pres) + "Па", 8, 60);     // Вывод давления. / Displaying the pressure.
  }
}

// Функция для настройки дисплея / Function to set up the display
void setUpDisplay() {
  M5.Display.init();                   // Инициализация дисплея устройства M5Stack. / Initializing the M5Stack display.
  M5.Display.setRotation(1);           // Установка ориентации дисплея. / Setting the display rotation.
  M5.Display.setTextSize(2);           // Установка размера текста. / Setting the text size.
  M5.Display.setTextColor(TFT_GREEN);  // Установка цвета текста (зеленый). / Setting the text color (green).
}

// Функция для настройки устройства / Function to set up the device
void setup() {
  auto cfg = M5.config();     // Получение конфигурации устройства M5Stack. / Getting the configuration of the M5Stack device.
  M5.begin(cfg);              // Инициализация устройства M5Stack с заданной конфигурацией. / Initializing the M5Stack device with the given configuration.
  Wire.begin();               // Инициализация шины I2C. / Initializing the I2C bus.
  setUpDisplay();             // Вызов функции настройки дисплея. / Calling the display setup function.
  Serial.begin(SERIAL_BAUD);  // Настройка последовательного порта для отладки. / Setting up the serial port for debugging.

  switch (bme.chipModel()) {  // Определение модели подключенного сенсора. / Determining the model of the connected sensor.
    case BME280::ChipModel_BME280:
      Serial.println("Found BME280 sensor! Success.");  // Если найден BME280, вывод сообщения в Serial. / If the BME280 is found, print the success message in Serial.
      break;
    case BME280::ChipModel_BMP280:
      Serial.println("Found BMP280 sensor! No Humidity available.");  // Если найден BMP280, предупреждение об отсутствии влажности. / If the BMP280 is found, a warning is printed about no humidity data.
      break;
    default:
      Serial.println("Found UNKNOWN sensor! Error!");  // Если сенсор не распознан, вывод ошибки. / If the sensor is not recognized, print an error message.
  }
}

// Основной цикл программы / Main program loop
void loop() {
  M5.update();  // Обновление состояния устройства M5Stack. / Updating the state of the M5Stack device.

  printBME280Data(lang);  // Вызов функции для отображения данных с датчика. / Calling the function to display data from the sensor.

  if (M5.BtnA.wasPressed()) {  // Проверка, была ли нажата кнопка A: / Checking if button A was pressed:
    if (lang == 0) {           // Если текущий язык английский: / If the current language is English:
      lang = 1;                // Переключить язык на русский. / Switch the language to Russian.
    } else {                   // Если текущий язык русский: / If the current language is Russian:
      lang = 0;                // Переключить язык на английский. / Switch the language to English.
    }
  }

  delay(1000);                       // Задержка на 1 секунду. / Delay for 1 second.
  M5.Display.fillScreen(TFT_BLACK);  // Очистка экрана (заливка черным цветом). / Clear the screen (fill with black).
}
