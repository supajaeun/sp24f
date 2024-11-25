# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I. -D_POSIX_C_SOURCE=200809L

# Source files
BASE_SRCS = main.c open.c close.c put.c get.c snapshot_baseline.c
CUSTOM_SRCS = main.c open.c close.c put.c get.c snapshot_custom.c

BASE_OBJS = $(BASE_SRCS:.c=.o)
CUSTOM_OBJS = $(CUSTOM_SRCS:.c=.o)

# Targets
BASE_TARGET = kvs_baseline
CUSTOM_TARGET = kvs_custom

# Default rule: build both Baseline and Custom
all: $(BASE_TARGET) $(CUSTOM_TARGET)

# Baseline build
$(BASE_TARGET): $(BASE_OBJS)
	$(CC) $(CFLAGS) -o $@ $(BASE_OBJS)

# Custom build
$(CUSTOM_TARGET): $(CUSTOM_OBJS)
	$(CC) $(CFLAGS) -o $@ $(CUSTOM_OBJS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(BASE_OBJS) $(CUSTOM_OBJS) $(BASE_TARGET) $(CUSTOM_TARGET)

# Run Baseline version
run_baseline: $(BASE_TARGET)
	./$(BASE_TARGET)

# Run Custom version
run_custom: $(CUSTOM_TARGET)
	./$(CUSTOM_TARGET)
