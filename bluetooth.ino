#define avoidPin A1 // задаем имя для порта с датчиком
#define ledPin 13   // задаем имя для порта со светодиодом
#define DIR_PIN 5   // Пин направления
#define STEP_PIN 6  // Пин шага

int avoid;           // Переменная для хранения состояния датчика
int prevAvoid = LOW; // Переменная для отслеживания предыдущего состояния
int counter = 0;     // Счетчик для подсчета событий

long encoderValue = 1000; // Начальная скорость шагового двигателя

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  Serial.begin(9600);  // инициализация последовательного порта

  pinMode(avoidPin, INPUT); // Настройка порта для датчика препятствий
  pinMode(ledPin, OUTPUT);   // Настройка порта для светодиода
}

void loop() {
  // Чтение состояния датчика препятствий
  avoid = digitalRead(avoidPin);  
  Serial.print("Avoid Sensor - ");  
  Serial.println(avoid);
  
  // Чтение скорости из Bluetooth
  if (Serial.available() > 0) {
    encoderValue = Serial.parseInt(); // Получаем значение скорости
    Serial.print("New Speed: ");
    Serial.println(encoderValue);
  }

  digitalWrite(DIR_PIN, HIGH); // Задаем направление вращения

  // Делаем 200 шагов (полный оборот для двигателя с шагом 1.8 градуса)
  for (int i = 0; i < 10000; i++) {
    if (counter < 40) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(encoderValue); // Задержка определяет скорость вращения
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(encoderValue);
      
      // Проверка состояния датчика препятствий
      if (avoid == HIGH) {
        digitalWrite(ledPin, HIGH); // Включаем светодиод, если препятствие обнаружено
        if (prevAvoid == LOW) {  // Если предыдущее состояние было LOW, увеличиваем счетчик
          counter++;
          Serial.print("Counter: ");  
          Serial.println(counter);
        }
      } else {
        digitalWrite(ledPin, LOW); // Выключаем светодиод, если препятствия нет
      }
      
      prevAvoid = avoid; // Обновляем предыдущее состояние
    } else {
      delay(1000); // Задержка перед сбросом счетчика
      counter = 0; // Сброс счетчика
    }
  }
}
