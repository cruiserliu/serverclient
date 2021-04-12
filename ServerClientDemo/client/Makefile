CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lm

all: clean Client.o main.o myprogram

CoderDecipher.o : CoderDecipher.h contain.h
	${CC} ${CFLAGS} -c CoderDecipher.cpp -std=c++11

Client.o : Client.h DataCenter.h CoderDecipher.h contain.h
	${CC} ${CFLAGS} -c Client.cpp DataCenter.cpp CoderDecipher.cpp -std=c++11

DataCenter.o : CoderDecipher.h DataCenter.h contain.h
	${CC} ${CFLAGS} -c CoderDecipher.cpp DataCenter.cpp -std=c++11

main.o : DataCenter.h Client.h contain.h
	${CC} ${CFLAGS} -c DataCenter.cpp Client.cpp main.cpp -std=c++11

myprogram : Client.o CoderDecipher.o DataCenter.o main.o
	${CC} ${CFLAGS} Client.o CoderDecipher.o DataCenter.o main.o ${LDFLAGS} -o myprogram
clean:
	rm -rf *.o
	rm -rf myprogram