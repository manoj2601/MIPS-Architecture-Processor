all: main.cpp functions.h
	g++ main.cpp

clean:
	rm -f a.out

run: all
	./a.out < tests/input.txt
run_large:
	./a.out < tests/inputlarge.txt > outputlarge.txt
