#ifndef _AMAZON_SERVER_H_
#define _AMAZON_SERVER_H_

#include "graph.h"
#include "move.h"

/* Structure representing a player */
struct player_t
{
    char *(*get_player_name)();
    void (*initialize)(unsigned int, struct graph_t *,
                       unsigned int, unsigned int **);
    void (*finalize)();
    struct move_t (*play)();
    char *error;
    void *handle;
    int looser;
};

void shuffle(char *array[], int n);

#endif // _AMAZON_SERVER_H_
