#include <SoftwareSerial.h>

#define avoidPin A1  // задаем имя для порта с датчиком
#define ledPin 13    // задаем имя для порта со светодиодом
#define DIR_PIN 5    // Пин направления
#define STEP_PIN 6   // Пин шага

// encoder part:
#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

int avoid;
int prevAvoid = LOW;  // переменная для отслеживания предыдущего состояния
int counter = 0;      // счетчик

// encoder part:
volatile long encoderValue = 1;
long lastEncoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;

// Переменные для скорости и команд
int motorSpeed = 1000;  // начальная скорость вращения
char receivedChar;      // переменная для хранения полученных команд

// Настраиваем SoftwareSerial для HC-06
SoftwareSerial BTSerial(10, 11);  // RX, TX (соедините на пины 10 и 11)

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  Serial.begin(57600);    // инициализация монитора порта
  BTSerial.begin(9600);   // инициализация Bluetooth-модуля на скорости 9600

  pinMode(avoidPin, INPUT);
  pinMode(ledPin, OUTPUT);

  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);

  // Прерывания для пинов A и B энкодера
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), updateEncoder, CHANGE);
}

void loop() {
  // Работа с датчиком движения
  avoid = digitalRead(avoidPin);

  // Работа с энкодером
  if (lastEncoderValue != encoderValue) {
    Serial.println(encoderValue);
    lastEncoderValue = encoderValue;
  }

  // Работа с двигателем
  digitalWrite(DIR_PIN, HIGH);  // Задаем направление вращения
  for (int i = 0; i < 10000; i++) {
    if (counter < 40) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(motorSpeed);  // Задержка определяет скорость вращения
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(motorSpeed);

      avoid = digitalRead(avoidPin);  // Получаем данные с датчика препятствий

      Serial.print("Avoid Sensor - ");  // Выводим данные с датчика на монитор
      Serial.println(avoid);
      Serial.print("Encoder - ");       // Выводим данные с энкодера
      Serial.println(encoderValue);

      if (avoid == HIGH) {
        digitalWrite(ledPin, HIGH);
        if (prevAvoid == LOW) {  // Если предыдущее состояние было LOW, увеличиваем счетчик
          counter++;
          Serial.print("Counter: ");  // Выводим значение счетчика на монитор
          Serial.println(counter);
        }
      } else {
        digitalWrite(ledPin, LOW);
      }

      prevAvoid = avoid;  // Сохраняем текущее состояние как предыдущее для следующей итерации
      delay(encoderValue);
    } else {
      delay(1000);
      counter = 0;
    }
  }

  // Прием данных через Bluetooth
  if (BTSerial.available()) {
    receivedChar = BTSerial.read();
    if (receivedChar == 'S') {  // Остановка двигателя через Bluetooth
      motorSpeed = 0;
      Serial.println("Остановлено через Bluetooth");
    } else if (receivedChar == 'R') {  // Запуск двигателя через Bluetooth
      motorSpeed = 1000;
      Serial.println("Запущено через Bluetooth");
    } else if (receivedChar == '+') {  // Увеличение скорости через Bluetooth
      motorSpeed -= 100;  // Уменьшаем задержку, увеличивая скорость
      if (motorSpeed < 100) motorSpeed = 100;  // Ограничение минимальной скорости
      Serial.println("Увеличение скорости через Bluetooth");
    } else if (receivedChar == '-') {  // Уменьшение скорости через Bluetooth
      motorSpeed += 100;  // Увеличиваем задержку, уменьшая скорость
      if (motorSpeed > 3000) motorSpeed = 3000;  // Ограничение максимальной скорости
      Serial.println("Уменьшение скорости через Bluetooth");
    }
  }
}

// Функция обработки прерываний энкодера
void updateEncoder() {
  int MSB = digitalRead(ENCODER_A_PIN);
  int LSB = digitalRead(ENCODER_B_PIN);

  int encoded = (MSB << 1) | LSB;
  int sum = (lastMSB << 3) | (lastLSB << 2) | encoded;

  if (sum == 0b0001 || sum == 0b0111 || sum == 0b1110 || sum == 0b1000)
    encoderValue += 1;
  if (sum == 0b0010 || sum == 0b1011 || sum == 0b1101 || sum == 0b0100)
    encoderValue -= 1;

  lastMSB = MSB;
  lastLSB = LSB;
}
