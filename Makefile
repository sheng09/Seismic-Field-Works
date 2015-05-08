CC = gcc
SRC = ./src
CFLAGS = -lm -Ofast
BIN = ./bin
all: GCDis GMTime

GCDis: ${SRC}/GCDis.o
	${CC} ${SRC}/GCDis.o -o ${BIN}/GCDis ${CFLAGS}

GMTime: ${SRC}/GMTime.o
	${CC} ${SRC}/GMTime.o -o ${BIN}/GMTime ${CFLAGS}

.PHONY:clean
clean:
	-rm ${BIN}/* ${SRC}/*.o

