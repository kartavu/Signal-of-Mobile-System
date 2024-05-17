all: clean main start

main: main.o
	clang++ -std=c++20 main.o -o main

main.o:
	clang++ -std=c++20 -c main.cpp -o main.o

start:
	./main

clean:
	-rm main *.o
