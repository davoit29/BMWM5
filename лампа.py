import customtkinter as ctk
import serial

# Настройки соединения с HC-06 через Bluetooth
port = "COM8"  # Замените на ваш порт COM
baudrate = 9600  # Стандартная скорость для HC-06

# Инициализация соединения с HC-06
ser = serial.Serial(port, baudrate)

# Создание окна приложения
app = ctk.CTk()
app.title("Arduino LED Brightness Control")

# Создание фрейма для ползунка
slider_frame = ctk.CTkFrame(app)
slider_frame.pack(pady=20, padx=20)

# Создание ползунка
brightness_slider = ctk.CTkSlider(slider_frame, from_=0, to=255, command=lambda value: set_brightness(value))
brightness_slider.pack()

# Отображение значения яркости
brightness_label = ctk.CTkLabel(slider_frame, text="Яркость: 0")
brightness_label.pack(pady=10)

# Функция для установки яркости
def set_brightness(value):
    global brightness_label
    brightness_label.configure(text=f"Яркость: {value}")
    ser.write(f"b:{value}\n".encode())

# Запуск основного цикла приложения
app.mainloop()
