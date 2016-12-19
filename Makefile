# Makefile

CCFLAGS=-lncurses

mysql:mysql.c functions.h fileio.h
	clear
	gcc -o mysql mysql.c -I /usr/include/mysql $(CCFLAGS) -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -ldl
