#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define POLYNOMIAL "11011110" // порождающий полином
#define LENGTH 5 // длинна массива для последовательности голда
#define L 50 // длинна битов для имени и фамилии
#define M 50 // длинна бит для последовательности с CRC
#define G 50 // длинна бит для последовательности голда
#define N 6 // отчеты для 5 задания
#define RANDOM N*(L+M+G) //длинна переменной для рандомного ввода

// Функция для кодирования ASCII-символов в битовую последовательность
void encodeToBinary(char *input, char *binaryData) {
    int i, j, k;

    for (i = 0; input[i] != '\0'; ++i) {
        int asciiCode = input[i];

        for (j = 7; j >= 0; --j) {
            k = asciiCode >> j;
            if (k & 1) {
                *binaryData = '1';
            } else {
                *binaryData = '0';
            }
            ++binaryData;
        }
    }
}

// Функция для вычисления CRC
void calculateCRC(const char *data, const char *polynomial, char *result) {
    int dataLength = strlen(data);
    int polynomialLength = strlen(polynomial);

    char extendedData[dataLength + polynomialLength - 1];
    strcpy(extendedData, data);
    strcat(extendedData, "0000000");

    for (int i = 0; i < dataLength; i++) {
        if (extendedData[i] == '1') {
            for (int j = 0; j < polynomialLength; j++) {
                extendedData[i + j] ^= polynomial[j] - '0';
            }
        }
    }
    
    strncpy(result, extendedData + dataLength, dataLength);
    result[dataLength] = '\0';
}

// сдвиг по х из 4 лабы
void shift_register_x(int *register_state_x) {
    int feedback = (register_state_x[2] + register_state_x[3]) % 2;
    for (int i = LENGTH - 1; i > 0; i--) {
        register_state_x[i] = register_state_x[i - 1];
    }
    register_state_x[0] = feedback;
}

// сдвиг по у из 4 лабы
void shift_register_y(int *register_state_y) {
    int feedback = (register_state_y[1] + register_state_y[2]) % 2;
    for (int i = LENGTH - 1; i > 0; i--) {
        register_state_y[i] = register_state_y[i - 1];
    }
    register_state_y[0] = feedback;
}

// расчет последовательности голда
void generate_pseudo_random_sequence(char *result, int *register_state_x, int *register_state_y, int length) {
    printf("Последовательность Голда равняется: ");
    for (int i = 0; i < length; i++) {
        int bit = (register_state_x[4] + register_state_y[4]) % 2;
        printf("%d", bit);
        
        shift_register_x(register_state_x);
        shift_register_y(register_state_y);
        *result = bit + '0';
        ++result;
    }
    
    *result = '\0';
    printf("\n\n");
}

// поиск максимальной корреляции
double calculate_correlation(char *sequence1, char *sequence2, int offset) {
    double correlation = 0.0;
    int length = strlen(sequence1);
    int length2 = strlen(sequence2);


    for (int i = 0; i < length; i++) {
        correlation += (sequence1[i] - '0') * (sequence2[(i + offset) % length2] - '0');
    }

    return correlation / length;
}

// функция для уменьшения массива на N
void decreaseSequence(char *input, char *output, int length) {
    int inputLength = strlen(input);
    int outputIndex = 0;

    for (int i = 0; i < inputLength; i += length) {
        output[outputIndex++] = input[i];
    }

    output[outputIndex] = '\0';
}

// функция для поиска аналогичных последовательностей в массиве и удаления ее
void removeDuplicates(char *input, const char *pattern, char *output) {
    int inputLength = strlen(input);
    int patternLength = strlen(pattern);
    int outputIndex = 0;

    for (int i = 0; i < inputLength; i += patternLength) {
        // Сравниваем подстроку с шаблоном
        if (strncmp(input + i, pattern, patternLength) != 0) {
            strncpy(output + outputIndex, input + i, patternLength); // Если не совпадает, копируем в выходной массив
            outputIndex += patternLength;
        }
    }

    output[outputIndex] = '\0';
}

// функция преобразования битовой последовательности в ASCII-символы
void binaryToASCII(char *binaryData, char *asciiResult, int nameBit) {
    int binaryLength = strlen(binaryData);
    int asciiIndex = 0;

    for (int i = 0; i < nameBit; i += 8) {
        char byte[9];
        strncpy(byte, binaryData + i, 8);
        byte[8] = '\0';
        
        // Преобразуем бинарные данные в целое число
        int asciiCode = strtol(byte, NULL, 2);
        // Преобразуем целое число в ASCII-символ
        asciiResult[asciiIndex++] = (char)asciiCode;
    }

    asciiResult[asciiIndex] = '\0';
}

// функция для генерации шума по формуле
void generateNormalNoise(double *noise, int length, double mean, double stddev) {
    for (int i = 0; i < length; i++) {
        // Генерация нормально распределенного шума
        double u1 = ((double) rand() / RAND_MAX);
        double u2 = ((double) rand() / RAND_MAX);
        double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        
        // Применение среднего и стандартного отклонения
        noise[i] = mean + stddev * z;
    }
}

int main() {
    char firstName[L], lastName[L]; // имя и фамилия
    char binaryData[L*8]; // битовая последовательность
    char crcResult[M]; // результат срс
    char randomsequence[G]; // результат последовательности голда
    char resultfourth[2*(L+M+G)]; // общий результат всех данных 4 заданий
    char resultfifth[N*(L+M+G)]; // результат моей последовательности с домнажением ее на N отчетов
    char resultsixth[2*N*(L+M+G)]; // результат 6 задания
    char resultseventh[2 * RANDOM]; // результат 7 задания
    char resulteight[2 * RANDOM]; // результат 8 задания
    
    int register_state_x[LENGTH] = {0, 1, 0, 1, 0}; // данные которые подаются на х для расчета голда
    int register_state_y[LENGTH] = {1, 0, 0, 0, 1}; // данные которые подаются на у для расчета голда
    int cons = pow(2, LENGTH) - 1;
    
    printf("Введите ваше имя: ");
    scanf("%s", firstName);

    printf("Введите вашу фамилию: ");
    scanf("%s", lastName);

    encodeToBinary(firstName, binaryData); // преобразую в биты
    int nameBit = strlen(binaryData); // для дальнейшей расшифровки использую
    encodeToBinary(lastName, binaryData + 8 * strlen(firstName)); // преобразую в биты

    FILE *file = fopen("secondTask.txt", "w");

    if (file == NULL) {
        perror("Не удалось открыть файл");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s", binaryData);
    fclose(file);

    printf("Битовая последовательность: %s\n\n", binaryData);
    
    generate_pseudo_random_sequence(randomsequence, register_state_x, register_state_y, cons);// вызов функции для расчета последовательность голда
    strcat(resultfourth, randomsequence); // добавляю в результат последовательность голда
    strcat(resultfourth, binaryData); // добавляю в результат битовую последовательность
    
    calculateCRC(binaryData, POLYNOMIAL, crcResult); // вызов функции для расчета срс
    printf("CRC: %s\n\n", crcResult);
    
    strcat(resultfourth, crcResult); // добавляю в результат срс последовательность
    printf("Битовая последовательность с последовательностью голда: %s\n\n", resultfourth);
    
    // сохряню в файл
    file = fopen("thirdTask.txt", "w");
    if (file == NULL) {
        perror("Не удалось открыть файл");
        exit(EXIT_FAILURE);
    }
    printf("Битовая последовательность с последовательностью голда и CRC: %s\n\n", resultfourth);

    fprintf(file, "%s", resultfourth);
    fclose(file);

    // Увеличени битовой последовательности на N
    int index = 0;
    int resultNIndex = 0;
    while (resultfourth[index] != '\0') {
        for (int i = 0; i < N; i++) {
            resultfifth[resultNIndex++] = resultfourth[index];
        }
        ++index;
    }
    resultfifth[resultNIndex] = '\0';
    
    printf("Результат последовательности с отсчетами: %s\n", resultfifth);

    // Сохраняем временные отсчеты в файл
    file = fopen("fifthTask.txt", "w");
    if (file == NULL) {
        perror("Не удалось открыть файл");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s", resultfifth);
    fclose(file);
 
    // 6 пункт
    printf("Введите число от 0 до %d: ", RANDOM);

    // символ массива на который встанет массив с отчетами
    int element;
    scanf("%d", &element);

    // Проверка на корректность ввода
    if (element < 0 || element >= 2 * RANDOM) {
        printf("Введенное число не входит в заданный диапазон\n");
        return 1; // Возвращаем код ошибки
    }

    // Заполнение нулями до указанного индекса
    for (int i = 0; i < element; i++) {
        resultsixth[i] = '0';
    }

    // Вставка массива resultfifth начиная с указанного индекса
    strcpy(resultsixth + element, resultfifth);

    memset(resultsixth + element + strlen(resultfifth), '0', sizeof(resultsixth) - (element + strlen(resultfifth))); // заполняю оствашиеся элементы нулями
    
    file = fopen("sixthTask.txt", "w");
//    file = fopen("sixthTaskMinN.txt", "w");
//    file = fopen("sixthTaskAverageN.txt", "w");
//    file = fopen("sixthTaskHighN.txt", "w");
    if (file == NULL) {
        perror("Не удалось открыть файл");
        exit(EXIT_FAILURE);
    }
    
    // Вывод результата
    printf("Массив после вставки:\n");
    for (int i = 0; i < 2 * RANDOM; i++) {
        printf("%c", resultsixth[i]);
        fprintf(file, "%c", resultsixth[i]);
    }
    printf("\n");
    fclose(file);
    
    // 7 пункт
    file = fopen("seventhTask.txt", "w");
//    file = fopen("seventhTaskMinN.txt", "w");
//    file = fopen("seventhTaskAverageN.txt", "w");
//    file = fopen("seventhTaskHighN.txt", "w");
    if (file == NULL) {
        perror("Не удалось открыть файл");
        exit(EXIT_FAILURE);
    }
    
    double noise[2 * RANDOM];
    double mean = 0.0; // Среднее значение
    double stddev = 0.1; // Стандартное отклонение

    generateNormalNoise(noise, 2 * RANDOM, mean, stddev); // вызов функции которая создает шум

    // Создаем массив resultseventh с шумом
    for (int i = 0; i < (2*RANDOM); i++) {
        // Применяем шум к элементу resultsixth
        double noisyValue = resultsixth[i] - '0' + noise[i];
        fprintf(file, "%f ", noisyValue);
        
        if (noisyValue < 0) {
            noisyValue = 0;
                } else if (noisyValue > 0.6) {
                    noisyValue = 1;
                }
        resultseventh[i] = (char)('0' + noisyValue);
    }
    printf("\n");
    fclose(file);
    
    // 8 пункт
    char randomseq[RANDOM]; // для последовательности голда с N отчетами
    int resultsNIndex = 0;
    int indexs = 0;
    
    // увеличиваю последовательность голда
    while (randomsequence[indexs] != '\0') {
        for (int i = 0; i < N; i++) {
            randomseq[resultsNIndex++] = randomsequence[indexs];
        }
        ++indexs;
    }
    
    // Находим максимальную корреляцию
    double maxCorrelation = -1.0;
    int syncStartSample = 0;

    for (int offset = 0; offset < (2 * RANDOM); offset++) { // ищу последовательность голда в массиве
        double correlation = calculate_correlation(randomseq, resultseventh, offset);
        if (correlation > maxCorrelation) {
            maxCorrelation = correlation;
            syncStartSample = offset;
        }
    }
    
    printf("Начальный семпл синхросигнала: %d\n", syncStartSample);
    
    // избавляюсь от шума в начале
    int resulteightIndex = 0;
        for (int i = syncStartSample; i < (2 * RANDOM); i++) {
            resulteight[resulteightIndex++] = resultseventh[i];
        }
        resulteight[resulteightIndex] = '\0';

    int resultfifthLength = strlen(resultfifth); // узнаю длинну данных из пятого пункта

    // Создаем массив resultninth с размером, равным количеству элементов в resultfifth
    char resultninth[resultfifthLength + 1];
    // Узнаем, сколько элементов нужно скопировать из resulteight в resultninth
    int elementsToCopy = resultfifthLength;
    // Копируем элементы из resulteight в resultninth
    strncpy(resultninth, resulteight, elementsToCopy);
    resultninth[elementsToCopy] = '\0';

    // 9 пункт
    decreaseSequence(resultninth, resultninth, N); // вызываю функцию которая убирает отсчеты
    
    resultninth[elementsToCopy] = '\0';
    printf("Результат в массиве после удаления отсчетов:\n");
    printf("%s\n", resultninth);
    
    char resulttenth[2 * (L + M + G)]; // хранятся данные без отчетов
    char resulteleventh[L + M + G]; // данные без последователности голда
    removeDuplicates(resultninth, randomsequence, resulttenth); // удаляю последовательность голда  с помощью функции

    printf("Результат данных после удаления последовательности голда:\n");
    printf("%s\n", resulttenth);
    
    char checkCRC[M]; // для повторной проверки срс
    calculateCRC(resulttenth, POLYNOMIAL, checkCRC); // вызов функции для расчета срс на приеме
    printf("CRC на приеме равняется: %s\n", checkCRC);

    // Удаление повторяющихся элементов в resulteleventh на основе crcResult
    removeDuplicates(resulttenth, crcResult, resulteleventh);
    
    printf("Результат данных после удаления CRC:\n");
    printf("%s\n", resulteleventh);
    
    // расшифровка данных 12 пункт
    char asciiResultName[nameBit / 8 + 1];
    binaryToASCII(resulteleventh, asciiResultName, nameBit);

    // Вывод расшифровки только первых nameBit бит
    printf("Расшифрованное имя: %s\n", asciiResultName);

    int allBit = strlen(resulteleventh); // узнаю все биты для расшифровки имени и фамилии по отдельности
    int lastnameBit = allBit - nameBit;  // длинна фамилии

    int nameasciiSize = nameBit / 8 + 1;
    char nameasciiResultRest[nameasciiSize];
    binaryToASCII(resulteleventh + nameBit, nameasciiResultRest, lastnameBit);

    // Вывод оставшейся части в ASCII
    printf("Расшифрованная фамилия: %s\n", nameasciiResultRest);

    return 0;
    
}
