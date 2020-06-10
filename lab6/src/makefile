.PHONY: all clean
# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=gcc
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-L. -llibrary

all : server client

server : server.c liblibrary.so
	$(CC) -pthread -o server server.c $(CFLAGS)

client : client.c liblibrary.so
	$(CC) -o client client.c $(CFLAGS)

liblibrary.so : library.o
	$(CC) -shared -o liblibrary.so library.o

library.o : library.c
	$(CC) -fPIC -c library.c

# Цель clean. Она традиционно используется для быстрой очистки всех результатов сборки проекта.
# Очистка запускается так: make -f makefile clean
clean :
	rm server client library.o liblibrary.so