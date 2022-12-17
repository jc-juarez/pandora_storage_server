# *************************************
# Pandora Storage Server
# Source
# 'Makefile'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

pandora: main.o server_options.o server_endpoints.o server_utilities.o storage.o element_container_cache.o element_container.o element.o
	-rm -r build
	mkdir build
	g++ -std=c++17 main.o server_options.o server_endpoints.o server_utilities.o storage.o element_container_cache.o element_container.o element.o -o ./build/pandora -lhttpserver -pthread
	rm *.o

main.o: ./server/main.cc
	g++ -std=c++17 -c ./server/main.cc -lhttpserver -pthread

server_options.o: ./server/server_options.cc ./server/server_options.h
	g++ -std=c++17 -c ./server/server_options.cc -lhttpserver -pthread

server_endpoints.o: ./server/server_endpoints.cc ./server/server_endpoints.h
	g++ -std=c++17 -c ./server/server_endpoints.cc -lhttpserver -pthread

server_utilities.o: ./server/server_utilities.cc ./server/server_utilities.h
	g++ -std=c++17 -c ./server/server_utilities.cc -lhttpserver -pthread

storage.o: ./storage/storage.cc ./storage/storage.h
	g++ -std=c++17 -c ./storage/storage.cc -lhttpserver -pthread

element_container_cache.o: ./storage/core/live-memory/element_container_cache.cc ./storage/core/live-memory/element_container_cache.h
	g++ -std=c++17 -c ./storage/core/live-memory/element_container_cache.cc -lhttpserver -pthread	

element_container.o: ./storage/core/element_container.cc ./storage/core/element_container.h
	g++ -std=c++17 -c ./storage/core/element_container.cc -lhttpserver -pthread

element.o: ./storage/core/element.cc ./storage/core/element.h
	g++ -std=c++17 -c ./storage/core/element.cc -lhttpserver -pthread

clean:
	-rm *.o
	-rm -r build