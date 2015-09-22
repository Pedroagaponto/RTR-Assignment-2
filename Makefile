CC=g++
EXE = assignment2
OBJS = main.o

CFLAGS = -g -Wall
LFLAGS = -g

all: ${EXE}

${EXE}: ${OBJS}
	${LINK.cpp} ${LFLAGS} -o $@ ${OBJS} -lm -lglut -lGL -lGLU

clean:
	-rm -f ${OBJS}

clobber: clean
	-rm -f ${EXE}

