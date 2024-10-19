TARGET = main

# DIRECTORIES.
BUILD_DIR = build
INCS_DIR = include
SRCS_DIR = srcs
OBJS_DIR = objs

# FILES.
SRCS = main.c utils.c screen.c board.c
OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))

# COMPILER OPTIONS.
CC = gcc
FLAGS = -Wall -Wextra -Werror -I$(INCS_DIR) 


all: $(BUILD_DIR)/$(TARGET)


$(BUILD_DIR)/$(TARGET): $(OBJS) | build_dir
	@echo "LINKING $^"
	@$(CC) -g $^ -o $(BUILD_DIR)/$(TARGET) $(FLAGS)


$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | objs_dir
	@echo "COMPILING $@"
	@$(CC) $^ -c -o $@ $(FLAGS)


$(OBJS_DIR)/$(TARGET).o: $(TARGET).c | objs_dir
	@echo "COMPILING $@"  
	$(CC) $^ -c -o $@ $(FLAGS)


run: all
	./$(BUILD_DIR)/$(TARGET)


build_dir: 
	@mkdir -p $(BUILD_DIR)/


objs_dir: 
	@mkdir -p $(OBJS_DIR)/


clean:
	@echo "CLEANING DIRECTORY"
	@rm -rf $(BUILD_DIR)/$(TARGET)
	@rm -rf $(OBJS_DIR)


.PHONY: all clean run build_dir objs_dir 