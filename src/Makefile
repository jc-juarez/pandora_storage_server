# *************************************
# Pandora Storage Server
# Source
# 'Makefile'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

pandora: main.o server_options.o server_endpoints.o server_utilities.o storage_operations.o containers_cache.o orion_engine.o elements_container.o
	-rm -r build
	mkdir build
	g++ -std=c++17 main.o server_options.o server_endpoints.o server_utilities.o storage_operations.o containers_cache.o orion_engine.o elements_container.o -o ./build/pandora -lhttpserver -pthread
	rm *.o

main.o: ./server/main.cc
	g++ -std=c++17 -c ./server/main.cc -lhttpserver -pthread

server_options.o: ./server/server_options.cc ./server/server_options.h
	g++ -std=c++17 -c ./server/server_options.cc -lhttpserver -pthread

server_endpoints.o: ./server/server_endpoints.cc ./server/server_endpoints.h
	g++ -std=c++17 -c ./server/server_endpoints.cc -lhttpserver -pthread

server_utilities.o: ./server/server_utilities.cc ./server/server_utilities.h
	g++ -std=c++17 -c ./server/server_utilities.cc -lhttpserver -pthread

storage_operations.o: ./storage/storage_operations.cc ./storage/storage_operations.h
	g++ -std=c++17 -c ./storage/storage_operations.cc -lhttpserver -pthread

containers_cache.o: ./storage/core/containers_cache.cc ./storage/core/containers_cache.h
	g++ -std=c++17 -c ./storage/core/containers_cache.cc -lhttpserver -pthread	

orion_engine.o: ./storage/core/orion_engine.cc ./storage/core/orion_engine.h
	g++ -std=c++17 -c ./storage/core/orion_engine.cc -lhttpserver -pthread

elements_container.o: ./storage/core/elements_container.cc ./storage/core/elements_container.h
	g++ -std=c++17 -c ./storage/core/elements_container.cc -lhttpserver -pthread

clean:
	-rm *.o
	-rm -r build