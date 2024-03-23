#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "interface.h"

struct player_data_t
{
    unsigned int player_id; // correspond à l'indice du tableau queen dans le champ queens
    struct graph_t *graph;
    unsigned int num_queens;
    unsigned int **queens; // tableau de tableau d'indice, indiquant où sont les queens de chaque joueur
    struct set *arrows;    // tableau de taille num_vertices indiquant si une flèche est présente sur la case
};

/* Player's data is stored in the following global variable */
static struct player_data_t data;

char const *get_player_name()
{
    return "Samourai";
}

void initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
    // Initialize the global variable data
    data.player_id = player_id;
    data.graph = graph;
    data.graph->num_vertices = graph->num_vertices;
    data.num_queens = num_queens;
    data.queens = queens;
    data.arrows = set_alloc(graph->num_vertices);
    for (unsigned int i = 0; i < graph->num_vertices; i++)
    {
        data.arrows->data[i] = 0;
    }
}

void finalize()
{
    set_free(data.arrows);
    queens_free(data.queens);
    gsl_spmatrix_uint_free(data.graph->t);
    free(data.graph);
}

/**
 * @brief Return the element at a random position in an array
 *
 * @param tab
 * @param longueur
 * @return unsigned int
 */
unsigned int choose_random(unsigned int *tab, unsigned int longueur)
{
    return tab[rand() % longueur];
}

struct move_t play(struct move_t previous_move)
{
    update(previous_move, (data.player_id - 1) % NUM_PLAYERS, data.arrows, data.queens);

    struct move_t move;
    struct set *idx_not_tested = set_alloc(data.num_queens);
    for (unsigned int i = 0; i < idx_not_tested->size; i++)
    {
        idx_not_tested->data[i] = i;
    }
    unsigned int queen_src = data.queens[data.player_id][choose_random(idx_not_tested->data, idx_not_tested->size)];
    struct set *neighboors_src = get_neighboors(queen_src, data.graph, data.num_queens, data.queens, data.arrows);
    while (neighboors_src->size == 0)
    {
        set_remove(queen_src, idx_not_tested);
        queen_src = data.queens[data.player_id][choose_random(idx_not_tested->data, idx_not_tested->size)];
        set_free(neighboors_src);
        neighboors_src = get_neighboors(queen_src, data.graph, data.num_queens, data.queens, data.arrows);
    }
    set_free(idx_not_tested);
    unsigned int queen_dst = choose_random(neighboors_src->data, neighboors_src->size);
    move.queen_src = queen_src;
    move.queen_dst = queen_dst;
    move.arrow_dst = UINT_MAX;
    update(move, data.player_id, data.arrows, data.queens); //  update pour le déplacement
    struct set *neighboors_dst = get_neighboors(move.queen_dst, data.graph, data.num_queens, data.queens, data.arrows);
    if (neighboors_dst->size == 0)
    {
        move.arrow_dst = queen_src;
    }
    else
    {
        move.arrow_dst = neighboors_dst->data[neighboors_dst->size - 1];
    }
    set_free(neighboors_src);
    set_free(neighboors_dst);
    queen_src = move.queen_src;
    queen_dst = move.queen_dst;
    move.queen_dst = UINT_MAX;
    move.queen_src = UINT_MAX;
    update(move, data.player_id, data.arrows, data.queens); //  update pour fleche
    move.queen_src = queen_src;
    move.queen_dst = queen_dst;
    return move;
}
