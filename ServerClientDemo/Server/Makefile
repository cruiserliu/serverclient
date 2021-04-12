CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lm

all: clean Receiver.o Process.o Errorlog.o Contact.o Sender.o Server.o Mediator.o main.o myprogram

Errorlog.o : Errorlog.h contain.h
	${CC} ${CFLAGS} -c Errorlog.cpp -std=c++11

CoderDecipher.o : CoderDecipher.h contain.h
	${CC} ${CFLAGS} -c CoderDecipher.cpp -std=c++11

Sender.o : CoderDecipher.h Sender.h contain.h Errorlog.h
	${CC} ${CFLAGS} -c CoderDecipher.cpp Sender.cpp Errorlog.cpp -std=c++11

Server.o : Server.h Errorlog.h contain.h
	${CC} ${CFLAGS} -c Server.cpp Errorlog.cpp -std=c++11

Receiver.o : Receiver.h Errorlog.h Sender.h Server.h
	${CC} ${CFLAGS} -c Receiver.cpp Errorlog.cpp Sender.cpp Server.cpp -std=c++11

Contact.o : Contact.h Server.h Receiver.h contain.h Errorlog.h
	${CC} ${CFLAGS} -c Server.cpp Receiver.cpp Errorlog.cpp Contact.cpp -std=c++11

Process.o : Process.h Receiver.h Contact.h Errorlog.h
	${CC} ${CFLAGS} -c Process.cpp Receiver.cpp Contact.cpp Errorlog.cpp -std=c++11

Mediator.o : Mediator.h Process.h contain.h
	${CC} ${CFLAGS} -c Mediator.cpp Process.cpp -std=c++11

main.o : Server.h Mediator.h Contact.h Receiver.h CoderDecipher.h
	${CC} ${CFLAGS} -c Server.cpp Mediator.cpp Contact.cpp Receiver.cpp CoderDecipher.cpp main.cpp -std=c++11

myprogram : CoderDecipher.o Receiver.o Process.o Errorlog.o Contact.o Sender.o Server.o Mediator.o main.o 
	${CC} ${CFLAGS} CoderDecipher.o Receiver.o Process.o Errorlog.o Contact.o Sender.o Server.o Mediator.o main.o -g ${LDFLAGS} -o myprogram -lpthread -std=c++11

clean:
	rm -rf *.o
	rm -rf *.txt
	rm -rf myprogram