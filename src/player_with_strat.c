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

/*Give the player name*/
char const *get_player_name()
{
    return "Samourai";
}

static struct player_data_t data;

/* Grab data from the server and initialize the graph*/
void initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{

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

/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize()
{
    set_free(data.arrows);
    queens_free(data.queens); // si lefree est fait par le serveur sinon on free ici
    gsl_spmatrix_uint_free(data.graph->t);
    free(data.graph);
}

unsigned int choose_random(unsigned int *tab, unsigned int longueur)
{
    return tab[rand() % longueur];
}

/*RETURNS :
 * - the indexe of the first elem of s1 presents in s2 too
 * - if not found return size of s1
 * - if one set is empty return UINT_MAX
 * */
unsigned int first_elem_in_common(struct set *s1, struct set *s2)
{
    if (s1->size == 0 || s2->size == 0)
    {
        return UINT_MAX;
    }
    unsigned int i = 0;
    while (!is_in_array(s1->data[i], s2->data, s2->size) || i < s1->size)
    {
        i++;
    }
    return i;
}

/*PRECOND : le tableau des queens doit être à jour avec le deplacement de la queen du joueur*/
unsigned int choose_arrow(unsigned int queen_src)
{
    unsigned int i = 0;
    unsigned int arrow;
    struct set *neighboors_of_queen_src = get_neighboors(queen_src, data.graph, data.num_queens, data.queens, data.arrows);
    struct set *neighboors_of_adv_queens = get_closest_neighboors(data.queens[(data.player_id + 1) % NUM_PLAYERS][i], data.graph, data.num_queens, data.queens, data.arrows);
    unsigned int idx_common = first_elem_in_common(neighboors_of_queen_src, neighboors_of_adv_queens);
    while (neighboors_of_adv_queens->size == 0 && idx_common >= neighboors_of_queen_src->size && ++i < data.num_queens)
    {
        // cette boucle vérifie que les voisins proches de la queen adverse sont accessible par notre queen_src
        set_free(neighboors_of_adv_queens);
        neighboors_of_adv_queens = get_closest_neighboors(data.queens[(data.player_id + 1) % NUM_PLAYERS][i++], data.graph, data.num_queens, data.queens, data.arrows);
        idx_common = first_elem_in_common(neighboors_of_queen_src, neighboors_of_adv_queens);
    }
    //  Si aucune des queens adverse ne possede de voisin et ou ne possède aucuns voisins accessible par queen_src
    if (neighboors_of_adv_queens->size == 0 || idx_common >= neighboors_of_queen_src->size)
    {
        //  On passe aux voisin plus éloignés
        i = 0;
        while (neighboors_of_adv_queens->size == 0 && idx_common >= neighboors_of_queen_src->size && ++i < data.num_queens)
        {
            // cette boucle vérifie que les voisins de la queen adverse sont accessible par notre queen_src
            set_free(neighboors_of_adv_queens);
            neighboors_of_adv_queens = get_neighboors_closer_first(data.queens[(data.player_id + 1) % NUM_PLAYERS][i++], data.graph, data.num_queens, data.queens, data.arrows);
            idx_common = first_elem_in_common(neighboors_of_queen_src, neighboors_of_adv_queens);
        }
        if (neighboors_of_adv_queens->size == 0 && idx_common >= neighboors_of_queen_src->size)
        {
            // choix de la flèche le plus éloignée
            idx_common = neighboors_of_queen_src->size - 1;
        }
    }
    arrow = idx_common;
    set_free(neighboors_of_queen_src);
    set_free(neighboors_of_adv_queens);
    return arrow;
}

/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player. If this is the
 *                  first move of the game, `previous_move` is equal to
 *                  { UINT_MAX, UINT_MAX, UINT_MAX }
 * RETURNS: - the next move for the player.
 */
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
    move.arrow_dst = choose_arrow(queen_dst);
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
