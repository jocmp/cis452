#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bump in the size to 32 to
// stay within bounds
#define SIZE 32

int main () {
    char *data1;
    
    data1 = malloc (SIZE);
    printf ("Please input username: ");
    scanf ("%s", data1);
    printf ("you entered: %s\n", data1);
    free (data1);

    return 0;
}
