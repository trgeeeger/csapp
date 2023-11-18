CFLAGS = -Wall -g  -Werror

EXE = program

SRC = ../src1/

CODE = ./cpu/mmu.c ./memory/instruction.c ./memory/dram.c ./disk/code.c ./main.c

.PHONY: program
main:
	gcc  $(CODE) -I$(SRC) -o $(EXE) $(CFLAGS)

run:
	./$(EXE)