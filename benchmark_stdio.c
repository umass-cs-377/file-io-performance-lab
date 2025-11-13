#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

double elapsed(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 +
           (end.tv_usec - start.tv_usec) / 1000.0;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <filename> <block_size> <mode: sequential|random>\n", argv[0]);
        exit(1);
    }

    const char *path = argv[1];
    size_t block_size = atol(argv[2]);
    int random = strcmp(argv[3], "random") == 0;

    FILE *fp = fopen(path, "rb");
    if (!fp) { perror("fopen"); exit(1); }

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);
    char *buf = malloc(block_size);
    if (!buf) { perror("malloc"); exit(1); }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    size_t total = 0;
    while (total < filesize) {
        if (random) { // if random flag is True, we will randomly pick an offset
            // TODO: get a random offset within with block boundary
            // TODO: move the file offset to the offset above using lseek
        }
        size_t n = fread(buf, 1, block_size, fp);
        if (n == 0) break;
        total += n;
    }

    gettimeofday(&end, NULL);
    printf("Buffered (%s, block=%zu): %.2f ms\n",
           random ? "random" : "sequential", block_size, elapsed(start, end));

    free(buf);
    fclose(fp);
    return 0;
}
