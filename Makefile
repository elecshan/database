all: db

.PHONY: all

db: main.o command.o db.o sds.o
	g++ -o bin/db main.o command.o db.o sds.o

main.o: main.cpp sds.hpp types.hpp command.hpp db.hpp
	g++ -c main.cpp

command.o: command.cpp command.hpp types.hpp
	g++ -c command.cpp

db.o: db.cpp db.hpp
	g++ -c db.cpp

sds.o: sds.cpp sds.hpp
	g++ -c sds.cpp

.PHONY:clean
clean:
	rm -f main.o command.o db.o sds.o db