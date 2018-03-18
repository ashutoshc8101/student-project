LIBS=$(wildcard libs/*)
INCLUDE_PATH=libs/

main: libraries
	g++ src/main.cpp $(wildcard libraries/*) -I$(INCLUDE_PATH) -lsqlite3 -o main

libraries:
	mkdir libraries/
	cd libs; \
	   gcc -c database/sqlite3.c -lpthread

clean:
	rm -rf libraries/ main
