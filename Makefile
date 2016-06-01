PREFIX = /opt/SFW
PREFIX_CTTIMES = ${PREFIX}/c_ttimes

export sfw_home_tmp=${PREFIX}
export cttimes_home_tmp=${PREFIX_CTTIMES}

SUBS = C_ttimes SFW

all: ${SUBS}

C_ttimes:
	-mkdir ${PREFIX_CTTIMES} || 2>&-
	cd src/C_ttimes ; ./configure --prefix=${PREFIX_CTTIMES}
	make -C src/C_ttimes
	make -C src/C_ttimes install

SFW:
	make -C src all
	make -C src install

clean:
	-make clean -C src/C_ttimes
	-make clean -C src
