NAME:=libexpress
ENTRY:=express

LD_LIBRARY_PATH:=/usr/local/lib
INCLUDE:=/usr/local/include

SRC_DIR:=src
OBJ_DIR:=obj
DEP_DIR:=$(OBJ_DIR)
HEAD_DIR:=include
DOCS_DIR:=docs

INC_DIR:=.

TEST:=test

SRCFILES:=$(shell find $(SRC_DIR) -type f -name "*.c")
OBJFILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCFILES))
DEPFILES := $(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d,$(SRCFILES))

CC:=gcc
CFLAGS:=-std=gnu18 $(addprefix -I,$(INC_DIR))
INSTALL:=install

all: $(NAME).so

.PHONY: all run docs clean

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
	@ulimit -c unlimited
	@$(CC) $(CFLAGS) -o ./examples/$(TEST).o ./examples/$(TEST).c -lexpress -lcol -lasync -ltpool -lqueue -lhmap -lvector
	@echo "------------"
	@./examples/$(TEST).o
	@ulimit -c 0

install: $(NAME).so $(HEAD_DIR)/$(ENTRY).h
	@echo "Installing..."
	@sudo mkdir -p $(INCLUDE)/$(NAME)
	@sudo ldconfig $(LD_LIBRARY_PATH)
	@sudo cp -a $(HEAD_DIR)/. $(INCLUDE)/$(NAME)/
	@sudo cp $(NAME).so $(LD_LIBRARY_PATH)
	@echo "Done!"

docs:
	@doxygen 1> /dev/null
	@echo "Documentation created!"
	@echo ""
	@echo "Copy the following link and open in a browser:"
	@echo file://$(shell pwd)/$(DOCS_DIR)/html/index.html

clean:
	@rm -rf $(NAME).so $(TEST).o $(OBJ_DIR) $(DEP_DIR) ./examples/$(TEST).o $(DOCS_DIR)

-include $(DEPFILES)