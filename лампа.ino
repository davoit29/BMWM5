#include <SoftwareSerial.h>

// Пины для подключения HC-06
const int rxPin = 0;
const int txPin = 1;

// Пины для подключения светодиода
const int ledPin = 11; //  Выберите свободный пин для управления светодиодом

// Создание объекта SoftwareSerial
SoftwareSerial bluetooth(rxPin, txPin);

const int baudRate = 9600;
int brightness = 0;

void setup() {
  Serial.begin(baudRate); // Для отладки через Serial Monitor
  bluetooth.begin(baudRate); 
  pinMode(ledPin, OUTPUT); // Устанавливаем пин светодиода в режим вывода
}

void loop() {
  // Чтение данных с компьютера
  if (bluetooth.available() > 0) {
    String data = bluetooth.readStringUntil('\n');
    if (data.startsWith("b:")) {
      brightness = data.substring(2).toInt();
      analogWrite(ledPin, brightness); // Управление яркостью светодиода
    }
  }

  delay(100); 
}
