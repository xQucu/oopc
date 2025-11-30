
testMatrix: testMatrix.o matrix.o
	g++ -g $^ -o $@

matrix.o: matrix.cpp matrix.h
	g++ -c -g -Wall -pedantic $< -o $@

testMatrix.o: testMatrix.cpp matrix.h
	g++ -c -g -Wall -pedantic $< -o $@

.PHONY: clean

clean:
	-rm matrix.o test*.o
run:
	clear && $(MAKE) && valgrind --leak-check=full ./testMatrix*
