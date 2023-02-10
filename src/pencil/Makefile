CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`

SRC = main.c
OBJ = ${SRC:.c=.o}

BIN = main
LIBS = `pkg-config --libs gtk+-3.0` 

all: ${BIN}

${BIN}: ${OBJ}
	${CC} ${CFLAGS} ${LIBS} $^

%.o: %.c
	${CC} ${CFLAGS} ${LIBS} -c $^

clean: 
	rm ${BIN}
	rm ${OBJ}