#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <getopt.h>

#include "game.h"
#include "player.h"
#include "graph_prototypes.h"

/**
 * @brief Shuffle an array
 *
 * @param array
 * @param n
 */
void shuffle(char *array[], int n)
{
    if (n > 1)
    {
        for (int i = 0; i < n - 1; i++)
        {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            char *t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

/**
 * @brief Load a player given its shared library path
 *
 * @param pl
 * @return struct player_t
 */
struct player_t load_player(char *pl)
{
    void *handle;
    struct player_t player;
    handle = dlopen(pl, RTLD_LAZY);
    if (!handle)
    {
        fputs(dlerror(), stderr);
        exit(1);
    }
    player.get_player_name = dlsym(handle, "get_player_name");
    if ((player.error = dlerror()) != NULL)
    {
        fputs(player.error, stderr);
        exit(1);
    }

    player.initialize = dlsym(handle, "initialize");
    if ((player.error = dlerror()) != NULL)
    {
        fputs(player.error, stderr);
        exit(1);
    }

    player.play = dlsym(handle, "play");
    if ((player.error = dlerror()) != NULL)
    {
        fputs(player.error, stderr);
        exit(1);
    }

    player.finalize = dlsym(handle, "finalize");
    if ((player.error = dlerror()) != NULL)
    {
        fputs(player.error, stderr);
        exit(1);
    }

    player.looser = 0;

    player.handle = handle;
    return player;
}

/**
 * @brief Unload a player
 */
void unload_player(struct player_t player)
{
    player.finalize();
    dlclose(player.handle);
}

/**
 * @brief Main function
 * @return 0
 */
int main(int argc, char **argv)
{
    int opt;
    int optcount = 0; // number of optional arguments
    int width = 10;
    char shape = 'c';
    srand(time(NULL));
    while ((opt = getopt(argc, argv, "hm:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printf("Usage: player1 player2 ...\n");
            optcount++;
            return 0;
        case 'm':
            width = atoi(optarg);
            optcount += 2;
            break;
        case 't':
            shape = optarg[0];
            optcount += 2;
            break;

        default:
            printf("Usage: player1 player2 ...\n");
            return 1;
        }
    }

    if (argc - optcount < 2)
    {
        printf("Usage: player1 player2 ...\n");
        return 1;
    }
    else if (argc - optcount > 3)
    {
        printf("Too many players\n");
        return 1;
    }

    if (width < 5)
    {
        printf("Width too small (min=5)");
        return 2;
    }

    if (shape != 'c' && shape != 'd')
    {
        printf("Shape must be 'c' or 'd'\n");
        return 2;
    }

    if (shape == 'd' && width % 3 != 0)
    {
        printf("witdth must be a multiple of 3 for donut shape\n");
        return 2;
    }
    int num_queens = 4 * (width / 10 + 1);

    struct player_t players[NUM_PLAYERS];

    // Get players paths

    char *players_paths[NUM_PLAYERS];

    for (int i = 0; i < NUM_PLAYERS + optcount; i++)
    {
        players_paths[i] = argv[i + optcount + 1];
    }

    shuffle(players_paths, NUM_PLAYERS);

    // Load the players
    printf("Width : %d\n", width);
    struct graph_t *graph = (shape != 'd') ? graph_initialize_square(width) : create_donut_graph(width);
    struct graph_t *graphs[NUM_PLAYERS];

    unsigned int **tab_queens_for_player[NUM_PLAYERS];
    for (unsigned int i = 0; i < NUM_PLAYERS; i++)
    {
        graphs[i] = (shape != 'd') ? graph_initialize_square(width) : create_donut_graph(width);
        tab_queens_for_player[i] = queens_init(graphs[i]->num_vertices, num_queens);
        players[i] = load_player(players_paths[i]);
        players[i].initialize(i, graphs[i], num_queens, tab_queens_for_player[i]);
    }

    // Play the game
    struct set *arrows = set_alloc(graph->num_vertices);
    unsigned int **queens = queens_init(graph->num_vertices, num_queens);
    game(players, num_queens, queens, graph, arrows);

    // End of the game

    // Unload the players

    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        unload_player(players[i]);
        free(tab_queens_for_player[i]);
        // graph_free(graphs[i]);
    }
    // print_queens(queens, num_queens);
    queens_free(queens);
    free(queens);
    graph_free(graph);
    set_free(arrows);

    return 0;
}
