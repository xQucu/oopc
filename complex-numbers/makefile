testcomplex: testcomplex.o complex.o
	g++ -g $^ -o $@

complex.o: complex.cpp complex.h
	g++ -c -g -Wall -pedantic $< -o $@

testcomplex.o: testcomplex.cpp complex.h
	g++ -c -g -Wall -pedantic $< -o $@

.PHONY: clean

clean:
	-rm complex.o test*.o
run:
	clear && $(MAKE) && valgrind --leak-check=full ./test*
