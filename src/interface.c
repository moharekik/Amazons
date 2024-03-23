#include <stdlib.h>
#include <stdio.h>

#include "interface.h"
#include "player.h"
#include "dir.h"

unsigned int **queens_init(unsigned int num_vertices, unsigned int num_queens)
{
    //  CREATION DU TABLEAU **QUEENS
    unsigned int **queens = malloc(NUM_PLAYERS * sizeof(unsigned int[num_queens]));
    for (int player_id = 0; player_id < NUM_PLAYERS; player_id++)
    {
        queens[player_id] = calloc(num_queens, sizeof(unsigned int));
    }

    //  INITIALISATION DU TABLEAU **QUEENS
    unsigned int width = sqrt(num_vertices);
    for (unsigned int player_id = 0; player_id < NUM_PLAYERS; player_id++)
    {
        //  ligne horizontale pour chaque joueur
        for (unsigned int queen_id = 0; queen_id < num_queens / 4; queen_id++)
        {
            queens[player_id][2 * queen_id] = ((num_vertices - 1) * player_id) + (2 * player_id - 1) * (-1) * ((width / (num_queens / 2)) * queen_id + 1);
            queens[player_id][2 * queen_id + 1] = ((num_vertices - 1) * player_id) + (2 * player_id - 1) * (-1) * ((width - 1) - ((width / (num_queens / 2)) * queen_id + 1));

            queens[player_id][num_queens / 2 + 2 * queen_id] = ((num_vertices - 1) * player_id) + (2 * player_id - 1) * (-1) * (width * ((width / (num_queens / 2)) * queen_id + 1));
            queens[player_id][num_queens / 2 + 2 * queen_id + 1] = ((num_vertices - 1) * player_id) + (2 * player_id - 1) * (-1) * (width * ((width / (num_queens / 2)) * queen_id + 1) + width - 1);
        }

        //  ligne verticales
    }
    return queens;
}

void queens_free(unsigned int **queens)
{
    for (int player_id = 0; player_id < NUM_PLAYERS; player_id++)
    {
        free(queens[player_id]);
    }
    // free(queens);
}

void print_queens(unsigned int **queens, unsigned int num_queens)
{
    for (unsigned int p = 0; p < NUM_PLAYERS; p++)
    {
        for (unsigned int k = 0; k < num_queens; k++)
        {
            printf("%3u ", queens[p][k]);
        }
        printf("\n");
    }
}

unsigned int case_is_free(unsigned int case_id, unsigned int num_queens, unsigned int **queens, struct set *arrows)
{
    if (case_id == UINT_MAX)
        return 0;
    // verifie qu'il n'y ait aucune queen sur la case case_id
    for (int p = 0; p < NUM_PLAYERS; p++)
    {
        for (unsigned int q = 0; q < num_queens; q++)
        {
            if (queens[p][q] == case_id)
                return 0; // la case est occupée donc on return faux
        }
    }

    // vérifie que la case est bien accessible depuis une autre case du plateau de jeu
    return (arrows->data[case_id] == 0);
}

void update(struct move_t move, unsigned int id_player, struct set *arrows, unsigned int **queens)
{
    if (!(move.queen_dst == UINT_MAX || move.queen_src == UINT_MAX))
    {
        //  PARTIE MOUVEMENT DE QUEEN
        int i = 0;
        while (queens[id_player][i] != move.queen_src)
        {
            i++;
        }
        queens[id_player][i] = move.queen_dst;
    }
    if (move.arrow_dst != UINT_MAX)
    {
        //  PARTIE FLECHE - supprime les arrivées depuis les autres cases
        arrows->data[move.arrow_dst] = 1;
    }
}

/* Renvoie le voisin selon la direction dir d'une case comme representé sur le board renvoie UINT_MAX sinon*/
unsigned int get_neighboor(unsigned int case_id, enum dir_t dir, struct graph_t *graph)
{
    if (case_id == UINT_MAX)
        return UINT_MAX;
    /* Itére dans les cases du graph*/
    for (unsigned int i = 0; i < graph->num_vertices; i++)
    {
        /* Quand on tombe sur l'indice dans la ligne tel que l'élément est égal à la direction*/
        if (gsl_spmatrix_uint_get(graph->t, case_id, i) == dir)
        {
            /* On renvoie l'indice de la ligne correspondant*/
            return i;
        }
    }
    return UINT_MAX;
}

/* Return the neighboors positions as a sole case_id unsigned int with CSR implementation it finish with UINT_MAX it allocates memory that should be freed for an arrow to be shoot or an amazon to go*/
struct set *get_neighboors(unsigned int case_id, struct graph_t *graph, unsigned int num_queens, unsigned int **queens, struct set *arrows)
{
    struct set *neighboors = set_alloc(0);
    if (case_id == UINT_MAX)
        return neighboors;
    for (int dir = FIRST_DIR; dir < LAST_DIR; dir++)
    {
        unsigned int neighbor = get_neighboor(case_id, dir, graph);
        while (case_is_free(neighbor, num_queens, queens, arrows))
        {
            set_push(neighboors, neighbor);
            neighbor = get_neighboor(neighbor, dir, graph);
        }
    }
    return neighboors;
}

int another_dir_in(unsigned int current_neighboors[NUM_DIRS])
{
    for (enum dir_t dir = 0; dir < NUM_DIRS; dir++)
    {
        if (is_in_array(dir, current_neighboors, NUM_DIRS))
        {
            return 1;
        }
    }
    return 0;
}

struct set *get_neighboors_closer_first(unsigned int case_id, struct graph_t *graph, unsigned int num_queens, unsigned int **queens, struct set *arrows)
{
    struct set *neighboors = set_alloc(0);
    if (case_id == UINT_MAX)
    {
        return neighboors;
    }
    unsigned int current_neighboors[NUM_DIRS];
    for (enum dir_t dir = FIRST_DIR; dir < LAST_DIR; dir++)
    {
        current_neighboors[dir] = get_neighboor(current_neighboors[dir], dir, graph);
        if (case_is_free(current_neighboors[dir], num_queens, queens, arrows))
        {
            set_push(neighboors, current_neighboors[dir]);
        }
    }
    while (another_dir_in(current_neighboors))
    {
        for (enum dir_t dir = FIRST_DIR; dir < LAST_DIR; dir++)
        {
            current_neighboors[dir] = get_neighboor(current_neighboors[dir], dir, graph);
            if (case_is_free(current_neighboors[dir], num_queens, queens, arrows))
            {
                set_push(neighboors, current_neighboors[dir]);
            }
        }
    }
    set_print(neighboors);
    return neighboors;
}

struct set *get_closest_neighboors(unsigned int case_id, struct graph_t *graph, unsigned int num_queens, unsigned int **queens, struct set *arrows)
{
    struct set *neighboors = set_alloc(0);
    if (case_id == UINT_MAX)
    {
        return neighboors;
    }
    for (enum dir_t dir = FIRST_DIR; dir < LAST_DIR; dir++)
    {
        unsigned int neighbor = get_neighboor(case_id, dir, graph);
        if (case_is_free(neighbor, num_queens, queens, arrows))
        {
            set_push(neighboors, neighbor);
        }
    }
    set_print(neighboors);
    return neighboors;
}

int has_lost(unsigned int player_id, int num_queens, unsigned int **queens, struct graph_t *graph, struct set *arrows)
{
    int i = 0;
    // tant que les dames ne bougent pas, on regarde si la suivante peut bouger
    struct set *neighboors = get_neighboors(queens[player_id][i], graph, num_queens, queens, arrows);
    while ((neighboors->size == 0))
    {
        if (i == num_queens - 1)
        {
            set_free(neighboors);
            return 1;
        }
        i++;
        set_free(neighboors);
        neighboors = get_neighboors(queens[player_id][i], graph, num_queens, queens, arrows);
    }
    set_free(neighboors);
    return 0;
}
