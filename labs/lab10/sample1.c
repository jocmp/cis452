#include <stdio.h>
#include <stdlib.h>

#define KB 1024
#define LOOP 1
int main()
{
    int count, *intPtr;

    long int i, j, dim = 64 * KB;
    
    if ((intPtr = malloc(dim * dim * sizeof(int))) == 0) {
	    perror("totally out of space");
	    exit(1);
    }

    for (count = 1; count <= LOOP; count++) {
	    for (i = 0; i < dim; i++) {
	        for (j = 0; j < dim; j++) {
                // intPtr[j * dim + i] = (i + j) % count;
                intPtr[i * dim + j] = (i + j) % count; // OG
                // printf("OG: %d and ", intPtr[i * dim + j]);
                // printf("11: %d\n", intPtr[j * dim + i]);
            }
        }
    }
    free(intPtr);
    return 0;
}
