LIBS=$(wildcard libs/*)
INCLUDE_PATH=libs/

main: libraries
	g++ src/main.cpp $(wildcard libraries/*) -I$(INCLUDE_PATH) -lsqlite3 -lpthread -ldl -o main

libraries:
	mkdir libraries/
	cd libs; \
	gcc -c database/sqlite3.c -lpthread -o ../libraries/sqlite3.o
	

clean:
	rm -rf libraries/ main
