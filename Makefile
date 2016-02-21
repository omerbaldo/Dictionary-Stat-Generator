all: first

first: first.c
	gcc -Wall -Werror -o first first.c

clean:
	rm -f first //deletes the binary