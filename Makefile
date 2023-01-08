NAME:=libexpress
ENTRY:=express

LD_LIBRARY_PATH:=/usr/local/lib
INCLUDE:=/usr/local/include

SRC_DIR:=src
OBJ_DIR:=obj
DEP_DIR:=$(OBJ_DIR)
HEAD_DIR:=include

INC_DIR:=.

TEST:=test

SRCFILES:=$(shell find $(SRC_DIR) -type f -name "*.c")
OBJFILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCFILES))
DEPFILES := $(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d,$(SRCFILES))

CC:=gcc
CFLAGS:=-std=c11 $(addprefix -I,$(INC_DIR))
INSTALL:=install

all: $(NAME).so

.PHONY: all run

$(NAME).so: $(OBJFILES)
	@echo "Building Final Shared Object"
	@$(CC) $(CFLAGS) $(OBJ_DIR)/*.o -shared -o $@
	@echo "Done"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile $(OBJ_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -fPIC -c $< -o $@
	@echo "Compiling $< file"

$(OBJ_DIR):
	@mkdir $(OBJ_DIR) 2> /dev/null

run: install
	@$(CC) $(CFLAGS) -o ./examples/$(TEST).o ./examples/$(TEST).c -lexpress -lcol -lasync -ltpool -lqueue -lhmap -lvector
	@echo "------------"
	@./examples/$(TEST).o

install: $(NAME).so $(HEAD_DIR)/$(ENTRY).h
	@echo "Installing..."
	@sudo mkdir -p $(INCLUDE)/$(NAME)
	@sudo ldconfig $(LD_LIBRARY_PATH)
	@sudo cp -a $(HEAD_DIR)/. $(INCLUDE)/$(NAME)/
	@sudo cp $(NAME).so $(LD_LIBRARY_PATH)
	@echo "Done!"

clean:
	@rm -rf $(NAME).so $(TEST).o $(OBJ_DIR) $(DEP_DIR) ./examples/$(TEST).o

-include $(DEPFILES)