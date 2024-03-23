#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/set.h"

static void test_set_alloc(void)
{
    printf("%s", __func__);

    struct set *S = set_alloc(5);
    assert(S != NULL);
    assert(S->size == 5);
    for (unsigned int i = 0; i < S->size; i++)
    {
        assert(S->data[i] == 0);
    }
    set_free(S);

    printf("\tOK\n");
}

static void test_set_push(void)
{
    printf("%s", __func__);

    struct set *S = set_alloc(5);
    assert(S != NULL);
    assert(S->size == 5);
    set_push(S, 10);
    assert(S->size == 6);
    assert(S->data[5] == 10);
    set_free(S);

    printf("\tOK\n");
}

static void test_set_pop(void)
{
    printf("%s", __func__);

    struct set *S = set_alloc(5);
    assert(S != NULL);
    assert(S->size == 5);
    set_push(S, 10);
    unsigned int val = set_pop(S);
    assert(val == 10);
    assert(S->size == 5);
    set_free(S);

    printf("\tOK\n");
}

static void test_set_remove(void)
{
    printf("%s", __func__);

    struct set *S = set_alloc(5);
    assert(S != NULL);
    assert(S->size == 5);
    set_push(S, 20);
    set_push(S, 30);
    set_remove(20, S);
    assert(S->size == 6);
    assert(S->data[5] == 30);
    set_free(S);

    printf("\tOK\n");
}

int main()
{
    printf("%s\n", __FILE__);

    test_set_alloc();
    test_set_push();
    test_set_pop();
    test_set_remove();

    return 0;
}
