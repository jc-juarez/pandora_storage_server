# *************************************
# Pandora Storage Server
# Source
# 'Makefile'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

pandora: main.o server_options.o server_endpoints.o server_utilities.o storage.o element_container_cache.o element_container.o element_container_operations.o element_operations.o main_data.o shard.o
	-rm -r build
	mkdir build
	g++ -std=c++17 main.o server_options.o server_endpoints.o server_utilities.o storage.o element_container_cache.o element_container.o element_container_operations.o element_operations.o main_data.o shard.o -o ./build/pandora -O3 -l:libhttpserver.a -lmicrohttpd -pthread
	rm *.o

main.o: ./server/main.cc
	g++ -std=c++17 -c ./server/main.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

server_options.o: ./server/server_options.cc ./server/server_options.h
	g++ -std=c++17 -c ./server/server_options.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

server_endpoints.o: ./server/server_endpoints.cc ./server/server_endpoints.h
	g++ -std=c++17 -c ./server/server_endpoints.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

server_utilities.o: ./server/server_utilities.cc ./server/server_utilities.h
	g++ -std=c++17 -c ./server/server_utilities.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

storage.o: ./storage/storage.cc ./storage/storage.h
	g++ -std=c++17 -c ./storage/storage.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

element_container_cache.o: ./storage/core/live-memory/element_container_cache.cc ./storage/core/live-memory/element_container_cache.h
	g++ -std=c++17 -c ./storage/core/live-memory/element_container_cache.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

element_container.o: ./storage/core/element_container.cc ./storage/core/element_container.h
	g++ -std=c++17 -c ./storage/core/element_container.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

element_container_operations.o: ./storage/core/element_container_operations.cc ./storage/core/element_container_operations.h
	g++ -std=c++17 -c ./storage/core/element_container_operations.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

element_operations.o: ./storage/core/element_operations.cc ./storage/core/element_operations.h
	g++ -std=c++17 -c ./storage/core/element_operations.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

main_data.o: ./storage/core/main_data.cc ./storage/core/main_data.h
	g++ -std=c++17 -c ./storage/core/main_data.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

shard.o: ./storage/core/shard.cc ./storage/core/shard.h
	g++ -std=c++17 -c ./storage/core/shard.cc -O3 -l:libhttpserver.a -lmicrohttpd -pthread

clean:
	-rm *.o
	-rm *.deb
	-rm -r build
	-rm -r pandora-storage-server-pkg