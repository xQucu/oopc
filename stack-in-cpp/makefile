teststack: teststack.o stack.o
	g++ -g $^ -o $@

stack.o: stack.cpp stack.h
	g++ -c -g -Wall -pedantic $< -o $@

teststack.o: teststack.cpp stack.h
	g++ -c -g -Wall -pedantic $< -o $@

.PHONY: clean

clean:
	-rm stack.o teststack.o teststack
