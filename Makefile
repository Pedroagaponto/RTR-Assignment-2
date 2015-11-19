CC=g++
EXE = assignment2
OBJS = main.o core.o draw.o calc.o io.o shaders.o

CFLAGS = -g
LFLAGS = -g

all: ${EXE}

${EXE}: ${OBJS}
	${LINK.cpp} ${LFLAGS} -o $@ ${OBJS} -lm -lglut -lGL -lGLU

clean:
	-rm -f ${OBJS}

clobber: clean
	-rm -f ${EXE}

