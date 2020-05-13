.PHONY: all clean
CC=gcc
CFLAGS=-I.

all : parallel zombie process_memory parallel_sum

zombie : zombie.c
	$(CC) -o zombie zombie.c

process_memory : process_memory.c
	$(CC) -o process_memory process_memory.c

parallel : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)
	
parallel_sum.o : parallel_sum.c
	$(CC) -o parallel_sum.o -c parallel_sum.c $(CFLAGS)
	
parallel_sum : parallel_sum.o utils.o utils.h sum.h
	$(CC) -pthread -o parallel_sum parallel_sum.o utils.o -I. -L. -lsum

# Цель clean. Она традиционно используется для быстрой очистки всех результатов сборки проекта.
# Очистка запускается так: make -f makefile clean
clean :
	rm zombie process_memory parallel utils.o find_min_max.o parallel_sum parallel_sum.o