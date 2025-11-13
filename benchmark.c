#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

double elapsed(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 +
           (end.tv_usec - start.tv_usec) / 1000.0; // ms
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <filename> <block_size> <mode: sequential|random>\n", argv[0]);
        exit(1);
    }

    const char *path = argv[1];
    size_t block_size = atol(argv[2]);
    int random = strcmp(argv[3], "random") == 0;

    int fd = open(path, O_RDONLY);
    if (fd < 0) { perror("open"); exit(1); }

    off_t filesize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    char *buf = malloc(block_size);
    if (!buf) { perror("malloc"); exit(1); }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    size_t total_read = 0;
    while (total_read < filesize) {
        if (random) {// if random flag is True, we will randomly pick an offset
            // TODO: get a random offset within with block boundary
            // TODO: move the file offset to the offset above using lseek
        }
        ssize_t bytes = read(fd, buf, block_size);
        if (bytes <= 0) break;
        total_read += bytes;
    }

    gettimeofday(&end, NULL);
    printf("%s access (block=%zu bytes): %.2f ms\n",
           random ? "Random" : "Sequential", block_size, elapsed(start, end));

    free(buf);
    close(fd);
    return 0;
}
