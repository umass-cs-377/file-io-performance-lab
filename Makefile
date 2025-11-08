# CS377 File I/O Performance Benchmark Lab
# Build both unbuffered and buffered versions

CC = gcc
CFLAGS = -O2 -Wall

all: benchmark benchmark_stdio

benchmark: benchmark.c
	$(CC) $(CFLAGS) benchmark.c -o benchmark

benchmark_stdio: benchmark_stdio.c
	$(CC) $(CFLAGS) benchmark_stdio.c -o benchmark_stdio

run-seq:
	./benchmark testfile.bin 4096 sequential

run-rand:
	./benchmark testfile.bin 4096 random

run-stdio:
	./benchmark_stdio testfile.bin 4096 sequential

clean:
	rm -f benchmark benchmark_stdio
