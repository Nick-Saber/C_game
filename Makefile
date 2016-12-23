.PHONY: all


all:
	gcc -o game -lncurses main.c
	./game
