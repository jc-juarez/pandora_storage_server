# *************************************
# Pandora Storage Server
# Source
# 'Makefile'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

pandora: main.o server_utilities.o storage_operations.o
	-rm -r build
	mkdir build
	g++ -std=c++17 main.o server_utilities.o storage_operations.o -o ./build/pandora -lhttpserver -pthread
	rm *.o

main.o: ./server/main.cc
	g++ -std=c++17 -c ./server/main.cc -lhttpserver -pthread

server_utilities.o: ./server/server_utilities.cc ./server/server_utilities.h
	g++ -std=c++17 -c ./server/server_utilities.cc -lhttpserver -pthread

storage_operations.o: ./storage/storage_operations.cc ./storage/storage_operations.h
	g++ -std=c++17 -c ./storage/storage_operations.cc -lhttpserver -pthread

clean:
	-rm *.o
	-rm -r build