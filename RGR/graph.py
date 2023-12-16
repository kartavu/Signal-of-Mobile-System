import numpy as np
import matplotlib.pyplot as plt

# Чтение битовой последовательности из файла
with open("secondTask.txt", "r") as file:
    second_task_binary_data = file.read()

# Преобразование битовой последовательности в массив нулей и единиц
second_task_binary_array = np.array([int(bit) for bit in second_task_binary_data]) # последовательность из второго задания

with open("thirdTask.txt", "r") as file:
    fourth_task_binary_data = file.read()

fourth_task_binary_array = np.array([int(bit) for bit in fourth_task_binary_data]) # последовательность из четвертого задания

with open("fifthTask.txt", "r") as file:
    fifth_task_binary_data = file.read()

fifth_task_binary_array = np.array([int(bit) for bit in fifth_task_binary_data]) # последовательность из пятого задания

with open("sixthTask.txt", "r") as file:
    sixth_task_binary_data = file.read()

sixth_task_binary_array = np.array([int(bit) for bit in sixth_task_binary_data if bit.strip()])  # фильтрация символов

with open("seventhTask.txt", "r") as file:
    seventh_task_data = [float(value) for value in file.read().split()]

# Построение двух графиков в одном окне
plt.subplot(4, 1, 1)
plt.plot(second_task_binary_array, drawstyle='steps-pre')
plt.title('Битовая последовательность')
plt.xlabel('Бит')
plt.ylabel('Значение')

plt.subplot(4, 1, 2)
plt.plot(fourth_task_binary_array, drawstyle='steps-pre')
plt.title('Измененная битовая последовательность')
plt.xlabel('Бит')
plt.ylabel('Значение')

plt.subplot(4, 1, 3)
plt.plot(fifth_task_binary_array, drawstyle='steps-pre')
plt.title('Битовая последовательность с N отчетами')
plt.xlabel('Бит')
plt.ylabel('Значение')

plt.subplot(4, 1, 4)
plt.plot(sixth_task_binary_array, drawstyle='steps-pre')
plt.title('Массив Signal')
plt.xlabel('Бит')
plt.ylabel('Значение')

plt.tight_layout()
plt.show()

# Визуализация массива отсчетов зашумленного принятого сигнала с использованием стем-графика
plt.plot(seventh_task_data, drawstyle='steps-pre')
plt.title('Визуализация зашумленного принятого сигнала ')
plt.xlabel('Отчеты')
plt.ylabel('Значение')
plt.grid(True)
plt.show()
