CC  = gcc
SRC = ./src
#CFLAGS = -lm -I./include -L./lib -L. -Ofast -Wall
CFLAGS = -ltaup -L/opt/ttimes -L./lib -L. -I/opt/ttimes -I./include -lgfortran -lm -Ofast -Wall
BIN = ./bin
LIB = ./lib
INC = ./include

OBJ     = Sac_Ev.o Kmean.o
OBJLIB  = liberrmsg.a libtime.a libsph.a  libnumrec.a libsacio.a
OBJEXEC = GMTime DTime GCDis cut4Ev RpKmean clusterRPBaz NRootStack pl_derfmod
VPATH = ${SRC}:${INC}:${LIB}

.PHONY:all

all: ${OBJLIB} ${OBJEXEC}

.PHONY:lib

lib: ${OBJLIB}

.PHONY:exec

exec: ${OBJEXEC}

INSTALL: ${OBJLIB} ${OBJEXEC}
	mkdir ${BIN} 2>&- || rm ${BIN}/* -rf 2>&-
	mv ${OBJEXEC} ${BIN}
	mkdir ${LIB} 2>&- || rm ${LIB}/* -rf 2>&-
	mv ${OBJLIB} ${LIB}
	rm *.o

#Lib for error message
liberrmsg.a: liberrmsg.c liberrmsg.h
	${CC} -c ${SRC}/liberrmsg.c ${CFLAGS}
	ar cq  liberrmsg.a liberrmsg.o
	ranlib liberrmsg.a

#Lib for time processing
libtime.a: libtime.c libtime.h liberrmsg.h
	${CC} -c ${SRC}/libtime.c ${CFLAGS}
	ar cq libtime.a libtime.o
	ranlib libtime.a

#Lib for math calculation in spherical coordinates
libsph.a: libsph.c libsph.h liberrmsg.h
	${CC} -c ${SRC}/libsph.c ${CFLAGS}
	ar cq libsph.a libsph.o
	ranlib libsph.a

#Lib for numerical recipe
libnumrec.a: libnumrec.c libnumrec.h
	${CC} -c ${SRC}/libnumrec.c ${CFLAGS}
	ar cq libnumrec.a libnumrec.o
	ranlib libnumrec.a

#Lib for sac IO
libsacio.a: sacio.c sac.h
	${CC} -c ${SRC}/sacio.c ${CFLAGS}
	ar cq libsacio.a sacio.o
	ranlib libsacio.a


GMTime: GMTime.c libtime.h liberrmsg.h
	${CC} -o GMTime ${SRC}/GMTime.c -ltime -lerrmsg ${CFLAGS}

DTime:  DTime.c  libtime.h liberrmsg.h
	${CC} -o DTime ${SRC}/DTime.c -ltime -lerrmsg ${CFLAGS}

GCDis:  GCDis.c  libsph.h liberrmsg.h
	${CC} -o GCDis ${SRC}/GCDis.c -lsph -lerrmsg ${CFLAGS}

cut4Ev: cut4Ev_v2.c Sac_Ev.h Sac_Ev.o libsph.h liberrmsg.h libtime.h
	${CC} -o cut4Ev ${SRC}/cut4Ev_v2.c Sac_Ev.o  -lsph -lerrmsg -ltime ${CFLAGS}

RpKmean: RpKmean.c Kmean.o Kmean.h libnumrec.h liberrmsg.h libsph.h
	${CC} -o RpKmean ${SRC}/RpKmean.c Kmean.o -lnumrec -lerrmsg -lsph ${CFLAGS}

clusterRPBaz: clusterRPBaz.c liberrmsg.h
	${CC} -o clusterRPBaz ${SRC}/clusterRPBaz.c -lerrmsg ${CFLAGS}

NRootStack: NRootStack.c sac.h libnumrec.h
	${CC} -o NRootStack ${SRC}/NRootStack.c  -lnumrec -lsacio ${CFLAGS}

pl_derfmod: pl_derfmod.c liberrmsg.h
	${CC} -o pl_derfmod ${SRC}/pl_derfmod.c -lerrmsg ${CFLAGS}

.PHONY:clean

clean:
	-rm ${BIN}/* ${SRC}/*.o ${OBJLIB} ${OBJEXEC} ${LIB}/* 2>&-

