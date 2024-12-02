#include <AccelStepper.h>

#define DIR_PIN 5   // Пин направления
#define STEP_PIN 6  // Пин шага

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

float rpm = 100;     // Начальная скорость в оборотах в минуту
float stepsPerRevolution = 200.0; // Шагов на один оборот двигателя (1.8°)

void setup() {
  Serial.begin(9600); // Инициализация последовательного порта

  // Настройка шагового двигателя
  stepper.setMaxSpeed(stepsPerRevolution * 1000 / 60); // Максимальная скорость в шагах/сек
  stepper.setAcceleration(stepsPerRevolution * 1000 / 60); // Ускорение
}

void loop() {
  // Чтение данных от Bluetooth
  if (Serial.available() > 0) {
    rpm = Serial.parseFloat(); // Читаем скорость в RPM
    if (rpm < 0) rpm = 0;      // Ограничение минимальной скорости
    if (rpm > 1000) rpm = 1000; // Ограничение максимальной скорости

    // Перевод RPM в скорость шагов в секунду
    float stepsPerSecond = (rpm * stepsPerRevolution) / 60.0;
    stepper.setSpeed(stepsPerSecond); // Устанавливаем новую скорость

    // Вывод в монитор порта
    Serial.print("New RPM: ");
    Serial.println(rpm);
  }

  // Управление двигателем
  stepper.runSpeed(); // Запуск двигателя с заданной скоростью
}
