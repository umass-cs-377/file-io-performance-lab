# CS377: File I/O Performance Lab
# This Makefile builds and runs three experiments:
#   1. benchmark         - read performance (sequential/random, block sizes)
#   2. benchmark_stdio   - user-space buffering experiment
#   3. write_sync        - write vs write+fsync

CC = gcc
CFLAGS = -O2 -Wall

# === Build targets ===

all: benchmark write_sync

benchmark: benchmark.c
	$(CC) $(CFLAGS) benchmark.c -o benchmark

benchmark_stdio: benchmark_stdio.c
	$(CC) $(CFLAGS) benchmark_stdio.c -o benchmark_stdio

write_sync: write_sync.c
	$(CC) $(CFLAGS) write_sync.c -o write_sync

# === Example runs ===

# Read performance (sequential)
run-seq:
	./benchmark testfile.bin 4096 sequential

# Read performance (random)
run-rand:
	./benchmark testfile.bin 4096 random

# Write performance (no sync vs with fsync)
run-write:
	./write_sync 4096 normal
	./write_sync 4096 sync

# === Utility ===

clean:
	rm -f benchmark benchmark_stdio write_sync output.bin
