front: front.cpp lib/cliser.o
	g++ -std=c++11 front.cpp cliser.o -o front -lpthread

slave: slave.cpp lib/cliser.o
	g++ -std=c++11 slave.cpp cliser.o -o slave -lpthread

lib/cliser.o: lib/cliser.cpp
	g++ -c -std=c++11 lib/cliser.cpp -o cliser.o -lpthread

