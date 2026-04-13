BIN = build/tb_db
SRC = source/tb_db/tb_db.c

# CC = gcc
CC = clang

COMPILER := $(shell $(CC) --version | grep -o "gcc\|clang" | head -1)

# check for compile optimizations per compiler
ifeq ($(COMPILER),gcc)
    CFLAGS = -Wall -Wextra -Wpedantic -Wno-unused-function -g -Werror -O0
else ifeq ($(COMPILER),clang)
    CFLAGS = -Wall -Wextra -Wpedantic -Wno-unused-function -g -O0
else
    CFLAGS = -Wall -Wextra -Wpedantic -Wno-unused-function -g -O0
endif

$(BIN): $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $< -o $@

run: $(BIN)
	$(BIN)

.PHONY: clean
clean:
	rm -rf build
