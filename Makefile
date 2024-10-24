CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Wvla
CPPFLAGS = -Isrc

BIN = src/vm

OBJ = ${BIN}.o

all: release

release: CFLAGS += -Werror
release: ${BIN}

debug: CFLAGS += -g3 -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: ${BIN}

${BIN}: ${OBJ}

clean:
	${RM} ${BIN} ${OBJ}

.PHONY: all release debug
