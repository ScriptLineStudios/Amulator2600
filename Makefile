main:
	gcc -Wall -Wextra -O3 -o out main.c `sdl2-config --cflags --libs` 
	./out
	rm out