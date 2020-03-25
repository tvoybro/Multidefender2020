.PHONY: all clean run

NAME = MultiVitro2020.nes

CFLAGS = -C cnrom_horz.cfg -t nes -Oisr
CC = cl65
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

SRC = $(wildcard *.c)
SRC += $(wildcard *.s)

OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:.s=.o)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.s
	$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(CFLAGS) $(OBJ) -Ln labels.txt
	@echo $(NAME) was created.

$(OBJ): $(wildcard *.h *.sinc)

clean:
	rm -f $(NAME) *.o

run:
	@echo Launch ROM...
	@$(EMUL) $(shell pwd)/$(NAME)
