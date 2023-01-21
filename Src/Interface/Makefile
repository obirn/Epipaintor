# Basic Makefile Template

CC= gcc
CPPFLAGS= -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS=

SRC= # Insert your c files here
OBJ=${SRC:.c=.o}
DEP=${SRC:.c=.d}

all : main

main: ${OBJ}
	${CC} ${CFLAGS} ${CPPFLAGS} ${OBJ} ${LDLIBS} -o main

.PHONY: clean

clean:
	${RM} ${OBJ} ${DEP} *~
	${RM} main
	${RM} *.d

-include ${DEP}

# End
