#Compiler and flags
CC = gcc
CFLAGS =  -Wno-implicit-function-declaration -Wno-packed-bitfield-compat 
# Directories
SRC_DIRS = src/snail src/snail/comm_dev
BUILD_DIR = build
BIN= server client

# Source files
SERVER_SRCS = $(wildcard $(addsuffix /*.c, $(SRC_DIRS))) src/server.c src/game.c src/objetos.c
CLIENT_SRCS = $(wildcard $(addsuffix /*.c, $(SRC_DIRS))) src/client.c src/game.c src/objetos.c
SERVER_OBJS = $(SERVER_SRCS:%.c=$(BUILD_DIR)/%.o)
CLIENT_OBJS = $(CLIENT_SRCS:%.c=$(BUILD_DIR)/%.o)

server: $(SERVER_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

client: $(CLIENT_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

# Object file build rule
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -rf $(BUILD_DIR)

# Remove binary and objects
purge: clean
	rm -f $(BIN)

