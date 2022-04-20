all: main

main: main.o file_descriptor.o low_read_stat.o ifstream_stat.o ifstream_stat.o mmap_stat.o fread_stat.o
	g++ -std=c++2a -o main main.o file_descriptor.o low_read_stat.o ifstream_stat.o mmap_stat.o fread_stat.o

main.o: main.cpp
	g++ -std=c++2a -c main.cpp -I util -I stat_counter -I low_read_stat -I ifstream_stat -I mmap_stat -I fread_stat

file_descriptor.o: util/file_descriptor.cpp
	g++ -std=c++2a -c util/file_descriptor.cpp -I util

low_read_stat.o: low_read_stat/low_read_stat.cpp
	g++ -std=c++2a -c low_read_stat/low_read_stat.cpp -I low_read_stat -I stat_counter -I util

ifstream_stat.o: ifstream_stat/ifstream_stat.cpp
	g++ -std=c++2a -c ifstream_stat/ifstream_stat.cpp -I ifstream_stat -I stat_counter -I util

mmap_stat.o: mmap_stat/mmap_stat.cpp
	g++ -std=c++2a -c mmap_stat/mmap_stat.cpp -I mmap_stat -I stat_counter -I util

fread_stat.o: fread_stat/fread_stat.cpp
	g++ -std=c++2a -c fread_stat/fread_stat.cpp -I fread_stat -I stat_counter -I util

clean:
	rm -rf *.o main