include .env
# In my case, I store my CRITERION_PATH so that the compiledb
# can find it

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Wvla
CPPFLAGS = -Isrc

BIN = src/vm

OBJ = src/machine.o     \
      src/opcode.o

TEST_OBJ = tests/unit/test_helpers.o
TEST_BIN = tests/test

all: debug

release: CFLAGS += -Werror
release: ${BIN}

debug: CFLAGS += -g3 -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: ${BIN}

${TEST_BIN}: CPPFLAGS += -I${CRITERION_PATH}
${TEST_BIN}: CFLAGS += -g3 -fsanitize=address
${TEST_BIN}: LDFLAGS += -fsanitize=address
${TEST_BIN}: LDLIBS += -lcriterion
${TEST_BIN}: ${OBJ} ${TEST_OBJ}
	${CC} ${LDLIBS} ${LDFLAGS} $^ -o $@

${BIN}: ${BIN}.o ${OBJ}

check: ${TEST_BIN}
	./${TEST_BIN}

clean:
	${RM} ${BIN} ${OBJ} ${TEST_OBJ} ${TEST_BIN}

.PHONY: all release debug clean check
