#ifndef _AMAZON_GAME_H_
#define _AMAZON_GAME_H_

#include "server.h"
#include "interface.h"

/**
 * @brief Main game loop
 *
 * @param players
 * @param num_queens
 * @param queens
 * @param graph
 * @param arrows
 */
void game(struct player_t *players, int num_queens, unsigned int **queens, struct graph_t *graph, struct set *arrows);

#endif // _AMAZON_GAME_H_