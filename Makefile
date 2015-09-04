CC  = gcc
SRC = ./src
CFLAGS = -lm -I./include -L./lib -Ofast -Wall
CFLAGSTAUP = -ltaup -L/opt/ttimes -I/opt/ttimes -lgfortran
BIN = ./bin
LIB = ./lib
OBJLIB = liberrmsg.a libtime.a libsph.a libSac_Ev.a libnumrec.a libKmean.a libsacio.a
OBJEXE = GMTime DTime GCDis cut4Ev RpKmean NRootMerge
VPATH = src:include

.PHONY:all


all: ${OBJLIB} ${OBJEXE}

INSTALL: ${OBJLIB} ${OBJEXE}
	mkdir ${BIN} 2>&- || rm ${BIN}/* -rf 2>&-
	mv ${OBJEXE} ${BIN}
	mkdir ${LIB} 2>&- || rm ${LIB}/* -rf 2>&-
	mv ${OBJLIB} ${LIB}
	rm *.o

liberrmsg.a: liberrmsg.c liberrmsg.h
	${CC} -c ${SRC}/liberrmsg.c ${CFLAGS}
	ar cq  liberrmsg.a liberrmsg.o
	ranlib liberrmsg.a

libtime.a: libtime.c libtime.h liberrmsg.h
	${CC} -c ${SRC}/libtime.c ${CFLAGS}
	ar cq libtime.a libtime.o
	ranlib libtime.a

libsph.a: libsph.c libsph.h liberrmsg.h
	${CC} -c ${SRC}/libsph.c ${CFLAGS}
	ar cq libsph.a libsph.o
	ranlib libsph.a

libSac_Ev.a: libSac_Ev.c libSac_Ev.h liberrmsg.h libtime.h
	${CC} -c ${SRC}/libSac_Ev.c ${CFLAGSTAUP} ${CFLAGS}
	ar cq libSac_Ev.a libSac_Ev.o
	ranlib libSac_Ev.a

libnumrec.a: libnumrec.c libnumrec.h
	${CC} -c ${SRC}/libnumrec.c ${CFLAGS}
	ar cq libnumrec.a libnumrec.o
	ranlib libnumrec.a

libKmean.a: libKmean.c libKmean.h liberrmsg.h libsph.h libnumrec.h
	${CC} -c ${SRC}/libKmean.c ${CFLAGS}
	ar cq libKmean.a libKmean.o
	ranlib libKmean.a

libsacio.a: sacio.c sac.h
	${CC} -c ${SRC}/sacio.c ${CFLAGS}
	ar cq libsacio.a sacio.o
	ranlib libsacio.a


GMTime: GMTime.c libtime.a liberrmsg.a libtime.h liberrmsg.h
	${CC} -o GMTime ${SRC}/GMTime.c libtime.a liberrmsg.a ${CFLAGS}

DTime:  DTime.c libtime.a liberrmsg.a libtime.h liberrmsg.h
	${CC} -o DTime ${SRC}/DTime.c libtime.a liberrmsg.a ${CFLAGS}

GCDis:  GCDis.c libsph.a liberrmsg.a libsph.h liberrmsg.h
	${CC} -o GCDis ${SRC}/GCDis.c libsph.a liberrmsg.a ${CFLAGS}

cut4Ev: cut4Ev_v2.c libSac_Ev.a libsph.a liberrmsg.a libtime.a libSac_Ev.h libsph.h liberrmsg.h libtime.h
	${CC} -o cut4Ev ${SRC}/cut4Ev_v2.c libSac_Ev.a libsph.a liberrmsg.a libtime.a  ${CFLAGSTAUP} ${CFLAGS}

RpKmean: RpKmean.c libKmean.a libnumrec.a liberrmsg.a libsph.a libKmean.h libnumrec.h liberrmsg.h libsph.h
	${CC} -o RpKmean ${SRC}/RpKmean.c libKmean.a libnumrec.a liberrmsg.a libsph.a ${CFLAGS}

NRootMerge: NRootMerge.c sac.h libsacio.a
	${CC} -o NRootMerge ${SRC}/NRootMerge.c libsacio.a ${CFLAGS}
#all: GCDis GMTime DTime cut4Ev
#
#GCDis: ${SRC}/GCDis.o ${SRC}/libtime.c
#	${CC} ${SRC}/GCDis.o ${SRC}/libtime.c -o ${BIN}/GCDis ${CFLAGS}
#
#GMTime: ${SRC}/GMTime.o ${SRC}/libtime.c
#	${CC} ${SRC}/GMTime.o ${SRC}/libtime.c -o ${BIN}/GMTime ${CFLAGS}
#
#DTime:  ${SRC}/DTime.o ${SRC}/libtime.c
#	${CC} ${SRC}/DTime.o ${SRC}/libtime.c -o ${BIN}/DTime ${CFLAGS}
#
#cut4Ev:  ${SRC}/cut4Ev.o
#	${CC} ${SRC}/cut4Ev.o -o ${BIN}/cut4Ev ${CFLAGS}
#
.PHONY:clean

clean:
	-rm ${BIN}/* ${SRC}/*.o ${OBJLIB} ${OBJEXE} ${LIB}/*

