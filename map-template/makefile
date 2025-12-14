
TestMap: TestMap.o map.o
	g++ -g $^ -o $@

map.o: map.cpp map.h
	g++ -c -g -Wall -pedantic $< -o $@

TestMap.o: TestMap.cpp map.h
	g++ -c -g -Wall -pedantic $< -o $@

.PHONY: clean

clean:
	-rm map.o test*.o
run:
	clear && $(MAKE) && valgrind --leak-check=full ./TestMap*
