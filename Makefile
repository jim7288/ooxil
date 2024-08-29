CC = gcc
VPATH = src:include:build
CFLAGS = -fsanitize=address -g -pedantic

IDIR = ../include
ODIR = obj

_DEPS = types.h board.h eval.h movegen.h search.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = main.o board.o eval.o movegen.o search.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

ooxil: $(OBJ)
	$(CC) -o $@ $(CFLAGS) $(OBJ)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	-rm ooxil $(ODIR)/*.o
