.PHONY: all


all:
	gcc -o game -lncurses main.c 
	./game 


:main.c
	touch fds.c
