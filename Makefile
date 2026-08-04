.PHONY: all build run clean
all: build
build:
	@cmake -B build
	@cmake --build build
run: build
	@if [ -f build/f77_transpiler ]; then ./build/f77_transpiler; else ./build/f77_transpiler.exe; fi
clean:
	@cmake -E remove_directory build
