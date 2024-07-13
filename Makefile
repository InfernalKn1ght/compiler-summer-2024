all:
	@cmake -S . -B build
	@make -C build
	@./build/compiler
