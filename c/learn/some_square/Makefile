CC = gcc
CFLAGS = -std=c11 -g

ifeq ($(OS),Windows_NT)
	EXE = run.exe
	RM = del /Q
else
	EXE = run
	RM = rm -f
endif

SRC = main.c winsize_ctl.c
OBJ =  $(SRC:.c=.o)

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE)

# compile
%.o: %.c
	$(CC) $(CFLAGS) -c  $< -o $@

# run script
run: all
	./$(EXE)

# asm
asm: $(SRC)
	$(CC) $(CFLAGS) -S $(SRC)

# clean files
clean: 
	$(RM) $(OBJ) $(EXE) *.s

