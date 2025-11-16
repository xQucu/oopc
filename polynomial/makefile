
testPoly: testPoly.o poly.o
	g++ -g $^ -o $@

poly.o: poly.cpp poly.h
	g++ -c -g -Wall -pedantic $< -o $@

testPoly.o: testPoly.cpp poly.h
	g++ -c -g -Wall -pedantic $< -o $@

.PHONY: clean

clean:
	-rm poly.o test*.o
run:
	clear && $(MAKE) && valgrind --leak-check=full ./test*
