BIN = build/engine
SRC = source/engine/engine_entry.c
CC = clang
CFLAGS = -Wall -Wextra -Wfloat-equal -Wswitch-default -Wswitch-enum \
         -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-unused-function -g -Werror

$(BIN): $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $< -o $@

run:
	$(BIN)

.PHONY: clean
clean:
	rm -rf build
