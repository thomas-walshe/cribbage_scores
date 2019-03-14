build:
	gcc -Wall -std=c11 -o cc_output main.c

run:
	./cc_output

buildrun: build run
