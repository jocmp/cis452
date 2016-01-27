
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main () {
    char *data1, *data2, *tmp;
    int k;
    data1 = malloc (SIZE);
    data2 = malloc (SIZE);
    tmp = data1;
    
    printf ("Please input your username: ");
    scanf ("%s", data1); 
    for (k = 0; k < SIZE; k++)
	data2[k] = *(tmp++);
    /* Inside the for loop above, we increment
     * the position of tmp for SIZE = 16. This
     * means that tmp is now 16 bytes ahead of
     * data1, so we must reset tmp to a valid
     * address. */
    tmp = data1; 
    printf ("data2 :%s:\n", data2);  
    printf ("tmp :%s:\n", tmp);
    // Free data1 after printing tmp
    free (data1);
    free (data2);
    return 0;
}
