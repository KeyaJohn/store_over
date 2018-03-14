DIR=.
SRC=$(wildcard $(DIR)/*.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
CFLAG= -g -Wall -I./ -I../inc
LFLAG= -L../lib -lrt -lpthread -lconfig -lzlog
BIN=./test

all:$(BIN)
$(DIR)/%.o:$(DIR)/%.c
	gcc -c $< -o $@ $(CFLAG)
$(BIN):$(OBJ)
	gcc -o $@ $^ $(LFLAG)
.PHONY:clean rebuild
clean:
	rm -rf $(BIN) $(OBJ)
rebuild: clean all
