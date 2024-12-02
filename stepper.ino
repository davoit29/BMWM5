#include <Stepper.h>

// Константы для шагового двигателя
const int stepsPerRevolution = 200; // Количество шагов на один оборот двигателя
const int DIR_PIN = 5;   // Пин направления
const int STEP_PIN = 6;  // Пин шага

// Создаем объект для управления шаговым двигателем
Stepper stepper(stepsPerRevolution, DIR_PIN, STEP_PIN);

// Переменные для управления скоростью
int rpm = 100; // Начальная скорость (оборотов в минуту)

void setup() {
  Serial.begin(9600);  // Инициализация последовательного порта
  Serial.println("Arduino готово к работе!");

  stepper.setSpeed(rpm); // Устанавливаем начальную скорость
}

void loop() {
  // Проверяем, есть ли входящие данные из Serial (от Python)
  if (Serial.available() > 0) {
    // Читаем переданное значение
    int newRpm = Serial.parseInt(); // Получаем новое значение скорости в RPM
    if (newRpm >= 0 && newRpm <= 1000) { // Проверяем корректность значения
      rpm = newRpm;
      stepper.setSpeed(rpm); // Устанавливаем новую скорость
      Serial.print("Установленная скорость: ");
      Serial.print(rpm);
      Serial.println(" RPM");
    }
  }

  // Вращаем двигатель
  stepper.step(stepsPerRevolution); // Один полный оборот
}
