CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L
FILE ?= main

TARGET_FILE = $(FILE)
SRCS_FILE = $(FILE).c

all: $(TARGET_FILE)

$(TARGET_FILE): $(SRCS_FILE)
	$(CC) $(CFLAGS) $(SRCS_FILE) -o $(TARGET_FILE)

clean: 
	rm $(TARGET_FILE)

.PHONY: all clean
