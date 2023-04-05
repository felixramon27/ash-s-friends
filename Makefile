COMP = gcc -c -ansi
LINK = gcc

.PHONY: all clean

all: fameChecker

fameChecker: main.o\
		utils.o
	$(LINK) *.o -o fameChecker

main.o: main.c
	$(COMP) main.c

utils.o: utils.c
	$(COMP) utils.c

clean:
	rm -rf *.o fameChecker