
testPolyWithFunctionTemplate: testPolyWithFunctionTemplate.o poly.o
	g++ -g $^ -o $@

poly.o: poly.cpp poly.h
	g++ -c -g -Wall -pedantic $< -o $@

testPolyWithFunctionTemplate.o: testPolyWithFunctionTemplate.cpp poly.h
	g++ -c -g -Wall -pedantic $< -o $@

.PHONY: clean

clean:
	-rm poly.o test*.o
run:
	clear && $(MAKE) && valgrind --leak-check=full ./testPolyW*
