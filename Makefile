BIN = build/engine
SRC = source/tb_db.c
CC = clang
CFLAGS = -Wall -Wextra -Wpedantic  -Wno-unused-function -g -Werror

$(BIN): $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $< -o $@

run:
	$(BIN)

.PHONY: clean
clean:
	rm -rf build
