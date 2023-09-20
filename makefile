# Makefile

TARGET = acharKMenores.out
CFLAGS = -g -O3 -lm -lpthread
VPATH = src
objs = acharKMenores.o utils.o
PROGRAMS = $(patsubst src/%.c, %, $(wildcard src/*.c))

.PHONY: all clean purge debug

all: $(PROGRAMS)
$(PROGRAMS) : $(objs) -lm

debug: CFLAGS += -DDEBUG
debug: all

# ligacao
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

# limpeza
clean:
	-rm -f $(objs) *~
purge: clean
	-rm -f $(PROGRAMS)
