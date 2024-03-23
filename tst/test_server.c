#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/server.h"

static void test_shuffle()
{
    printf("%s", __func__);

    // Test shuffle with an array of integers
    int array1[] = {1, 2, 3, 4, 5};
    int array2[] = {1, 2, 3, 4, 5};
    int n = sizeof(array1) / sizeof(array1[0]);

    // Shuffle array1
    shuffle((char **)array1, n);

    // Verify that array1 is shuffled
    int same_elements = 1;
    for (int i = 0; i < n; i++)
    {
        if (array1[i] != array2[i])
        {
            same_elements = 0;
            break;
        }
    }

    assert(same_elements == 0);

    printf("\tOK\n");
}

int main()
{
    printf("%s\n", __FILE__);

    test_shuffle();

    return 0;
}
