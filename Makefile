
### directories
OUT_DIR = bin
OBJ_DIR = obj
INC_DIRS = utils
SERVER_DIRS = tcpServer utils
CLIENT_DIRS = tcpClient utils
UTILS_DIRS = utils

### compiler info
CC = gcc
CFLAGS = -Wall -Wextra -g -I $(INC_DIRS)

### specify code files for server and client applications
SERVER_FILES = $(foreach f, $(SERVER_DIRS), $(wildcard $(f)/*.c))
CLIENT_FILES = $(foreach f, $(CLIENT_DIRS), $(wildcard $(f)/*.c))
UTILS_FILES = $(foreach f, $(UTILS_DIRS), $(wildcard $(f)/*.c))

### specify object files
SERVER_OBJ = $(patsubst %.c, %.o, $(SERVER_FILES))
CLIENT_OBJ = $(patsubst %.c, %.o, $(CLIENT_FILES))
UTILS_OBJ = $(patsubst %.c, %.o, $(UTILS_FILES))
OBJ = $(SERVER_OBJ) $(CLIENT_OBJ) $(UTILS_OBJ)

### targets to build
TARGETS = server client

all: $(TARGETS) clean

server: $(SERVER_OBJ)
	$(CC) -o $@ $^

client: $(CLIENT_OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf *.dSYM $(OBJ)