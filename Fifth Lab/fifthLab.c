#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DATA_SIZE 100  // Количество бит в данных
#define POLYNOMIAL "1100110"  // Порождающий полином

// Функция для генерации случайных данных
void generateRandomData(char *data, int dataSize) {
    srand(time(NULL));  // Инициализация генератора случайных чисел

    for (int i = 0; i < dataSize; i++) {
        data[i] = rand() % 2 + '0';  // Генерация случайного бита (0 или 1)
    }
    data[dataSize] = '\0';  // Добавляем символ конца строки
}

// Функция для вычисления CRC
void calculateCRC(const char *data, const char *polynomial, char *result) {
    int dataLength = strlen(data);
    int polynomialLength = strlen(polynomial);

    // Выделяем память под расширенные данные (данные + (polynomialLength - 1) нулей)
    char extendedData[dataLength + polynomialLength - 1];
    strcpy(extendedData, data);
    strcat(extendedData, "000000");  // Добавляем нули в конец расширенных данных

    for (int i = 0; i < dataLength; i++) {
        if (extendedData[i] == '1') {
            for (int j = 0; j < polynomialLength; j++) {
                extendedData[i + j] ^= polynomial[j] - '0';  // Выполняем XOR с соответствующим битом полинома
            }
        }
    }

    // Получаем CRC, который представляет собой последние dataLength бит расширенных данных
    strncpy(result, extendedData + dataLength, dataLength);
    result[dataLength] = '\0';
}

// Функция для вычисления расширенного CRC
void calculateExtendedCRC(const char *data, const char *polynomial, char *result) {
    char crcResult[DATA_SIZE + 1];  // Результат CRC
    calculateCRC(data, polynomial, crcResult);

    int dataLength = strlen(data);
    int crcResultLength = strlen(crcResult);
    
    char extendedData[crcResultLength + dataLength];
    strcpy(extendedData, crcResult);
    strcat(extendedData, data);  // Добавляем данные в конец расширенных данных

    for (int i = 0; i < crcResultLength; i++) {
        if (extendedData[i] == '1') {
            for (int j = 0; j < crcResultLength; j++) {
                extendedData[i + j] ^= polynomial[j] - '0';  // Выполняем XOR с соответствующим битом полинома
            }
        }
    }

    // Получаем CRC, который представляет собой последние crcResultLength бит расширенных данных
    strncpy(result, extendedData + crcResultLength, crcResultLength);
    result[crcResultLength] = '\0';
}

int main() {
    char data[DATA_SIZE + 1];  // Данные
    char crcResult[DATA_SIZE + 1];  // Результат CRC
    char extendedCRCResult[DATA_SIZE + 1];  // Расширенный CRC

    generateRandomData(data, DATA_SIZE);

    printf("Сгенерированные данные: %s\n", data);

    calculateCRC(data, POLYNOMIAL, crcResult);

    printf("Вычисленное значение CRC: %s\n", crcResult);

    int detectedErrors = 0;
    int undetectedErrors = 0;

    for (int bitToFlip = 0; bitToFlip < DATA_SIZE + strlen(crcResult); bitToFlip++) {
        char corruptedData[DATA_SIZE + 1];
        strcpy(corruptedData, data);

        if (bitToFlip < DATA_SIZE) {
            // Искажаем бит в данных
           corruptedData[bitToFlip] = (corruptedData[bitToFlip] == '0') ? '1' : '0';
        } else {
            
        }

        calculateExtendedCRC(corruptedData, POLYNOMIAL, extendedCRCResult);

        int errors = 0;
        for (int i = 0; i < strlen(extendedCRCResult); i++) {
            if (extendedCRCResult[i] != '0') {
                errors = 1;
                break;
            }
        }

        if (errors) {
            detectedErrors++;
        } else {
            undetectedErrors++;
        }
    }

    printf("Найдено ошибок: %d\n", detectedErrors);
    printf("НЕ обнаружено ошибок: %d\n", undetectedErrors);

    return 0;
}
