.PHONY: all


all:
	gcc -g -o game -lncurses main.c 
	./game 

