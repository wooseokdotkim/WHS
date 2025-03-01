#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#define ALLOC_SIZE (100 * 1024 * 1024) //100MB
#define ALLOC_TIME 30

int main() {
    char **memBlocks = NULL;
    size_t blockCount = 0;
    struct rusage usage;

    printf("Starting memory hog...\n");

    time_t startTime = time(NULL);
    while (time(NULL) - startTime < ALLOC_TIME) {
        char *block = (char *)malloc(ALLOC_SIZE);
        if (block == NULL) {
            printf("Memory allocation failed! (Out of memory)\n");
            break;
        }

        memset(block, 0, ALLOC_SIZE);

        memBlocks = (char **)realloc(memBlocks, (blockCount + 1) * sizeof(char *));
        memBlocks[blockCount++] = block;

        getrusage(RUSAGE_SELF, &usage);

        sleep(0.5);
    }

    printf("Memory hog finished. Freeing memory...\n");

    for (size_t i = 0; i < blockCount; i++) {
        free(memBlocks[i]);
    }
    free(memBlocks);

    return 0;
}


