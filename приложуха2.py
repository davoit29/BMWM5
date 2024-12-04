import time
import customtkinter as ctk
import serial

# Настройка порта Bluetooth
bluetooth_port = "COM8"  # Замените на ваш COM-порт
baud_rate = 9600

# Подключение к Bluetooth
try:
    ser = serial.Serial(bluetooth_port, baud_rate)
    time.sleep(2)  # Даем время на подключение
    print("Bluetooth подключен!")
except serial.SerialException as e:
    print(f"Ошибка подключения: {e}")
    exit()

# Функция для изменения скорости
def set_speed():
    try:
        speed = int(speed_scale.get())  # Получаем значение из ползунка
        ser.write(f"{speed}".encode('utf-8'))  # Отправляем скорость на Arduino
        print(f"Отправлено: {speed} RPM")
        current_speed_label.configure(text=f"Текущая скорость: {speed} RPM")  # Обновляем метку
    except Exception as e:
        print(f"Ошибка отправки данных: {e}")

# Настройка окна приложения
ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")

app = ctk.CTk()
app.title("Управление шаговым двигателем")
app.geometry("400x300")

# Ползунок для выбора скорости
speed_scale = ctk.CTkSlider(app, from_=0, to=1000, number_of_steps=100, command=lambda _: set_speed())
speed_scale.pack(pady=20)

# Устанавливаем начальное значение скорости
speed_scale.set(100)  # Стартовая скорость

# Кнопка для установки скорости
set_speed_button = ctk.CTkButton(app, text="Установить скорость", command=set_speed)
set_speed_button.pack(pady=10)

# Метка для отображения текущей скорости
current_speed_label = ctk.CTkLabel(app, text="Текущая скорость: 100 RPM")
current_speed_label.pack(pady=20)

# Обработчик закрытия приложения
def on_closing():
    ser.close()  # Закрываем порт при выходе
    app.destroy()

app.protocol("WM_DELETE_WINDOW", on_closing)

# Запуск приложения
app.mainloop()
