#ifndef _AMAZON_INTERFACE_H_
#define _AMAZON_INTERFACE_H_
#include "move.h"
#include "graph.h"
#include "set.h"

#define NUM_PLAYERS 2
struct init_data_t
{
    unsigned int num_vertices;
    unsigned int num_queens;
    unsigned int **queens;
};

/**
 * @brief Initialize the queens array with the right number of queens for each player
 *
 * @param num_vertices
 * @param num_queens
 * @return unsigned int**
 */
unsigned int **queens_init(unsigned int num_vertices, unsigned int num_queens);

/**
 * @brief Free the queens array
 *
 * @param queens
 */
void queens_free(unsigned int **queens);

/**
 * @brief Print the queens array
 *
 * @param queens
 * @param num_queens
 */
void print_queens(unsigned int **queens, unsigned int num_queens);

/**
 * @brief Check if a case is free
 *
 * @param case_id
 * @param num_queens
 * @param queens
 * @param arrows
 * @return boolean
 */
unsigned int case_is_free(unsigned int case_id, unsigned int num_queens, unsigned int **queens, struct set *arrows);

/**
 * @brief Update the queens array and the arrows set
 * if the move is valid
 *
 * @param move
 * @param id_player
 * @param arrows
 * @param queens
 */
void update(struct move_t move, unsigned int id_player, struct set *arrows, unsigned int **queens);

/**
 * @brief Get the neighboors of a case
 *
 * @param case_id
 * @param graph
 * @param num_queens
 * @param queens
 * @param arrows
 * @return struct set*
 */
struct set *get_neighboors(unsigned int case_id, struct graph_t *graph, unsigned int num_queens, unsigned int **queens, struct set *arrows);

// utile qu'au player
struct set *get_closest_neighboors(unsigned int case_id, struct graph_t *graph, unsigned int num_queens, unsigned int **queens, struct set *arrows);

struct set *get_neighboors_closer_first(unsigned int case_id, struct graph_t *graph, unsigned int num_queens, unsigned int **queens, struct set *arrows);

/**
 * @brief Check if a player has lost (i.e. if none of his queens can move)
 *
 * @param player_id
 * @param num_queens
 * @param queens
 * @param graph
 * @param arrows
 * @return int
 */
int has_lost(unsigned int player_id, int num_queens, unsigned int **queens, struct graph_t *graph, struct set *arrows);

#endif // _AMAZON_INTERFACE_H_
