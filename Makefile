CC = gcc
SRC = ./src
CFLAGS = -lm -Ofast -I./include
BIN = ./bin
OBJ = 
all: GCDis GMTime DTime

GCDis: ${SRC}/GCDis.o ${SRC}/libtime.c
	${CC} ${SRC}/GCDis.o ${SRC}/libtime.c -o ${BIN}/GCDis ${CFLAGS}

GMTime: ${SRC}/GMTime.o ${SRC}/libtime.c
	${CC} ${SRC}/GMTime.o ${SRC}/libtime.c -o ${BIN}/GMTime ${CFLAGS}

DTime:  ${SRC}/DTime.o ${SRC}/libtime.c
	${CC} ${SRC}/DTime.o ${SRC}/libtime.c -o ${BIN}/DTime ${CFLAGS}

.PHONY:clean

clean:
	-rm ${BIN}/* ${SRC}/*.o

