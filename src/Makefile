#home for sfw
install_sfw_home = ${sfw_home_tmp}
install_libdir   = ${sfw_home_tmp}/lib
install_bindir   = ${sfw_home_tmp}/bin
install_incdir   = ${sfw_home_tmp}/inc

#home for c_ttimes
cttims_home = ${cttimes_home_tmp}
lib_cttims_home = ${cttimes_home_tmp}/lib
inc_cttims_home = ${cttimes_home_tmp}/inc

CC = gcc
AR = ar
CFLAGS = -I../include -I${inc_cttims_home} -L. -L${lib_cttims_home} -Ofast

OBJ = clusterRPBaz.o cut4Ev.o cut4Ev_v2.o \
	 DTime.o GCDis.o GMTime.o \
	 NRootStack.o \
	 pl_derfmod.o RpKmean.o SelectEq_ISC.o spDis.o \
	 liberrmsg.o libnumrec.o libsph.o libtime.o libsacio.o libKmean.o libSac_Ev.o

LIBS = liberrmsg.a libtime.a libsph.a  libnumrec.a libsacio.a libKmean.a libSac_Ev.a

LIB_ALL = libsfw.a

EXEC_PART1 = GMTime DTime GCDis spDis RpKmean clusterRPBaz NRootStack pl_derfmod SelectEq_ISC
EXEC_PART2 = cut4Ev

.PHONY:all

all: ${OBJ} ${LIBS} ${LIB_ALL} ${EXEC_PART1} ${EXEC_PART2}

install:
	-mkdir ${install_libdir} ${install_bindir} ${install_incdir} 2>&-
	cp ${EXEC_PART1} ${EXEC_PART2} ${install_bindir}
	cp ${LIB_ALL} ${install_libdir}
	cp ../include/*.h  ${install_incdir}

${OBJ}:%.o:%.c
	${CC} -o $@ $< -c  ${CFLAGS}

${LIBS}:%.a:%.o
	${AR} cq $@ $<
	ranlib $@

libsfw.a: $(subst .a,.o, ${LIBS})
	${AR} cq $@ $^
	ranlib $@

${EXEC_PART1} :%:%.o
	${CC} -o $@ $< -lsfw -lm ${CFLAGS}

cut4Ev: cut4Ev_v2.o
	${CC} -o $@ $< -lsfw -lctau -lgfortran -lm  ${CFLAGS}

.PHONY:clean

clean:
	-rm ${OBJ} ${LIBS} ${LIB_ALL} ${EXEC_PART1} ${EXEC_PART2} -f 2>&-

