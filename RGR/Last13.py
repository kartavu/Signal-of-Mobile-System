import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft, fftfreq

# Чтение битовой последовательности из файла
with open("seventhTaskMinN.txt", "r") as file:
    seventhMin_binary_data = np.array([float(value) for value in file.read().split()])

# Преобразование битовой последовательности в массив нулей и единиц
# seventhMin = np.array([float(bit) for bit in seventhMin_binary_data]) # последовательность которую мы принимаем с N = 2

with open("sixthTaskMinN.txt", "r") as file:
    sixthMin_binary_data = file.read()

sixthMin = np.array([int(bit) for bit in sixthMin_binary_data])  # последовательность которую мы передаем на передачу с N = 2

with open("seventhTaskAverageN.txt", "r") as file:
    seventhAverage_binary_data = np.array([float(value) for value in file.read().split()])

# seventhAverage = np.array([float(bit) for bit in seventhAverage_binary_data])  # последовательность которую мы принимаем с N = 6

with open("sixthTaskAverageN.txt", "r") as file:
    sixthAverage_binary_data = file.read()

sixthAverage = np.array([int(bit) for bit in sixthAverage_binary_data if bit.strip()])  # последовательность которую мы передаем на передачу с N = 6

with open("seventhTaskHighN.txt", "r") as file:
    seventhHigh_binary_data = np.array([float(value) for value in file.read().split()])

# seventhHigh = np.array([float(bit) for bit in seventhHigh_binary_data])  # последовательность которую мы принимаем с N = 10

with open("sixthTaskHighN.txt", "r") as file:
    sixthHigh_binary_data = file.read()

sixthHigh = np.array([int(bit) for bit in sixthHigh_binary_data if bit.strip()])  # последовательность которую мы передаем на передачу с N = 10

# сигнал на передаче
sendingMinN = np.fft.fft(sixthMin)
sendingAvgN = np.fft.fft(sixthAverage)
sendingHighN = np.fft.fft(sixthHigh)

# сигнал на приеме
receptionMinN = np.fft.fft(seventhMin_binary_data)
receptionAvgN = np.fft.fft(seventhAverage_binary_data)
receptionHighN = np.fft.fft(seventhHigh_binary_data)

kM = np.arange(1200)
kA = np.arange(2700)
kH = np.arange(4200)

plt.plot(kH, np.abs(receptionHighN), label='Сигнал на приеме с N=10', color='red')
plt.plot(kA, np.abs(receptionAvgN), label='Сигнал на приеме с N=10', color='blue')
plt.plot(kM, np.abs(receptionMinN), label='Сигнал на приеме с N=2', color='orange')

plt.title('Передаваемый сигнал')
plt.xlabel('Бит')
plt.ylabel('Значение')
plt.legend()
plt.grid(True)
plt.show()

plt.plot(kH, np.abs(sendingHighN), label='Сигнал на передаче с N=10', color='red')
plt.plot(kA, np.abs(sendingAvgN), label='Сигнал на передаче с N=6', color='blue')
plt.plot(kM, np.abs(sendingMinN), label='Сигнал на передаче с N=2', color='orange')

plt.title('Принимаемый сигнал')
plt.xlabel('Бит')
plt.ylabel('Значение')
plt.legend()
plt.grid(True)
plt.show()





