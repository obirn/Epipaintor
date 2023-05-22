CC = gcc  -fsanitize=address

CPPFLAGS = `pkg-config gtk+-3.0 --cflags sdl2 SDL2_image` -MMD
CFLAGS =  -Wextra -std=c99 -g -Wall -Werror
LDLIBS = `pkg-config gtk+-3.0 --libs sdl2 SDL2_image` -lm -D__NO_INLINE__ -rdynamic -lssl -lcrypto -lcurl			

BIN = epipaintor

SRC =   epipaintor.c gui/gui.c auxiliary/auxiliary.c data_structs/queue.c data_structs/shared_stack.c \
        data_structs/stack.c data_structs/p_queue.c image_utils/tool_bucket.c network/network.c \
        network/base64.c \
	    $(wildcard filters/filter_*.c) $(wildcard shapes/*.c)

OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: ${BIN}

${BIN}: $(OBJ)

clean: 
	rm ${OBJ}
	rm ${DEP}
	rm ${BIN}
