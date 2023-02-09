CC = gcc

CPPFLAGS = `pkg-config gtk+-3.0 --cflags sdl` -MMD
CFLAGS = -Werror -Wextra -std=c99 -g -Wall
LDLIBS = `pkg-config gtk+-3.0 --libs sdl` -lSDL_image -lSDL_gfx -lm

BIN = pencil

SRC = pencil.c pixel_operations.c
OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: ${BIN}

${BIN}: $(OBJ)

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} ${BIN}
	${RM} ./*.o
