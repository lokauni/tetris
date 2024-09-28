compile: src/main.c
	mkdir -p build
	gcc src/main.c -o build/tetris -lSDL2
run: compile
	build/tetris

