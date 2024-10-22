import customtkinter as ctk
from tkinter import StringVar
import math

# Создаем главное окно
ctk.set_appearance_mode("dark")  # Темная тема
ctk.set_default_color_theme("green")  # Цветовая тема

# Функция для включения двигателя
def start_motor():
    print("Двигатель включен")

# Функция для остановки двигателя
def stop_motor():
    print("Двигатель остановлен")

# Функция для изменения скорости
def set_speed(value):
    speed = int(float(value))
    speed_var.set(f"{speed} км/ч")
    update_speedometer(speed)

# Функция для обновления спидометра
def update_speedometer(speed):
    angle = (speed / 100) * 180  # Преобразуем скорость в угол (макс. 100 км/ч)
    x = speedometer_center_x + speedometer_radius * math.cos(math.radians(180 - angle))
    y = speedometer_center_y - speedometer_radius * math.sin(math.radians(180 - angle))
    canvas.delete("arrow")  # Удаляем предыдущую стрелку
    canvas.create_line(speedometer_center_x, speedometer_center_y, x, y, fill="white", width=4, tags="arrow")

# Создаем главное окно
window = ctk.CTk()
window.title("Контроль вязальной машины")
window.geometry("500x500")

# Переменные для отслеживания количества петель и кругов
stitches_var = StringVar(value="0 петель")
rounds_var = StringVar(value="0 кругов")
speed_var = StringVar(value="0 км/ч")

# Параметры спидометра
speedometer_radius = 80
speedometer_center_x = 250
speedometer_center_y = 200

# Создаем холст для спидометра
canvas = ctk.CTkCanvas(window, width=500, height=400, bg="black")
canvas.pack(pady=20)

# Рисуем спидометр
canvas.create_oval(speedometer_center_x - speedometer_radius,
                    speedometer_center_y - speedometer_radius,
                    speedometer_center_x + speedometer_radius,
                    speedometer_center_y + speedometer_radius,
                    outline="white", width=2)

# Рисуем деления на спидометре
for i in range(0, 101, 10):
    angle = (i / 100) * 180
    x1 = speedometer_center_x + (speedometer_radius - 10) * math.cos(math.radians(180 - angle))
    y1 = speedometer_center_y - (speedometer_radius - 10) * math.sin(math.radians(180 - angle))
    x2 = speedometer_center_x + speedometer_radius * math.cos(math.radians(180 - angle))
    y2 = speedometer_center_y - speedometer_radius * math.sin(math.radians(180 - angle))
    canvas.create_line(x1, y1, x2, y2, fill="white", width=2)

# Создаем кнопку "Включить"
start_button = ctk.CTkButton(window, text="Включить", command=start_motor, width=200)
start_button.pack(pady=20)

# Создаем кнопку "Остановить"
stop_button = ctk.CTkButton(window, text="Остановить", command=stop_motor, width=200)
stop_button.pack(pady=20)

# Создаем ползунок для регулировки скорости
speed_slider = ctk.CTkSlider(window, from_=0, to=100, command=set_speed)
speed_slider.set(50)  # Устанавливаем начальную скорость
speed_slider.pack(pady=20)

# Метка для текущей скорости
speed_label = ctk.CTkLabel(window, textvariable=speed_var, font=("Arial", 18))
speed_label.pack(pady=10)

# Графа для отображения количества петель
stitches_label = ctk.CTkLabel(window, textvariable=stitches_var, font=("Arial", 18))
stitches_label.pack(pady=10)

# Графа для отображения количества кругов
rounds_label = ctk.CTkLabel(window, textvariable=rounds_var, font=("Arial", 18))
rounds_label.pack(pady=10)

# Запуск основного цикла Tkinter
window.mainloop()

