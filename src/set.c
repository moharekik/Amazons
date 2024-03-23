#include <stdlib.h>
#include <stdio.h>

#include "set.h"

struct set *set_alloc(unsigned int n)
{
    struct set *S = malloc(sizeof(struct set));
    S->data = calloc(n, sizeof(unsigned int));
    S->size = n;
    return S;
}

void set_free(struct set *S)
{
    free(S->data);
    free(S);
}

void set_print(struct set *S)
{
    printf("\n[");
    for (unsigned int i = 0; i < S->size; i++)
    {
        printf(" %3u", S->data[i]);
    }
    printf("] (%u)\n", S->size);
}

void set_remove(unsigned int elem, struct set *S)
{
    if (S->size > 0 && is_in_array(elem, S->data, S->size))
    {
        unsigned int i = 0;
        while (S->data[i] != elem)
        {
            i++;
        }
        S->data[i] = S->data[S->size - 1];
        S->data[S->size - 1] = elem;
        set_pop(S);
    }
}

unsigned int set_pop(struct set *s)
{
    unsigned int val = s->data[s->size - 1];
    s->size--;
    s->data = realloc(s->data, sizeof(unsigned int) * s->size);
    return val;
}

void set_push(struct set *s, unsigned int val)
{
    s->size++;
    s->data = realloc(s->data, sizeof(unsigned int) * s->size);
    s->data[s->size - 1] = val;
}

// ===================== Tools on Arrays =====================

int is_in_array(unsigned int elem, unsigned int *tab, unsigned int len_tab)
{
    for (unsigned int i = 0; i < len_tab; i++)
    {
        if (tab[i] == elem)
            return 1;
    }
    return 0;
}