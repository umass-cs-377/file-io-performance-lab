#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

double elapsed(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 +
           (end.tv_usec - start.tv_usec) / 1000.0;
}

void test_write(const char *path, size_t size, int use_fsync) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); exit(1); }

    char *buf = malloc(size);
    memset(buf, 'A', size);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < 1024; i++) {  // write 1024 blocks
        if (write(fd, buf, size) != size) {
            perror("write"); exit(1);
        }
        if (use_fsync)
            fsync(fd);
    }

    gettimeofday(&end, NULL);
    printf("Write%s (block=%zu): %.2f ms\n",
           use_fsync ? "+fsync" : "", size, elapsed(start, end));

    free(buf);
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <block_size> <mode: normal|sync>\n", argv[0]);
        exit(1);
    }

    size_t block_size = atol(argv[1]);
    int use_fsync = strcmp(argv[2], "sync") == 0;

    test_write("output.bin", block_size, use_fsync);
    return 0;
}
