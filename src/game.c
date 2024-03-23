#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "player.h"
#include "graph_prototypes.h"
#include "dir.h"

int what(unsigned int case_id, unsigned int **queens, unsigned int num_queens, struct set *arrows)
{
    if (arrows->data[case_id] == 1)
        return -1;
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        for (unsigned int q = 0; q < num_queens; q++)
        {
            if (queens[i][q] == case_id)
                return i + 1;
        }
    }
    return 0;
}

/**
 * Function using to print the board
 * @brief Print thr board
 */
void print_game(unsigned int **queens, unsigned int num_queens, struct set *arrows)
{
    unsigned int width = sqrt(arrows->size);
    printf("┌───");
    for (unsigned int j = 1; j < width; j++)
    {
        printf("┬───");
    }
    printf("┐\n");
    for (unsigned int i = 0; i < (width * width); i = i + width)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            unsigned int case_id = i + j;
            printf("│");
            switch (what(case_id, queens, num_queens, arrows))
            {
            case -1: // flèche
                printf("\033[1;31m \u2197 \033[0m");
                break;
            case 1:
                printf("\033[1;33m \u265B \033[0m");
                break;
            case 2:
                printf("\033[1;34m \u265B \033[0m");
                break;
            default:
                printf("   ");
                break;
            }
            // if (board->data[i + j] == 0)

            // else if (board->data[i + j] == 1)

            // else if (board->data[i + j] == 2)
            //     printf("\033[1;33m \u265B \033[0m");
            // else
            //     printf("\033[1;34m \u265B \033[0m");
        }
        printf("│\n");
        if (i != width * width - width)
        {
            printf("├───");
            for (unsigned int j = 1; j < width; j++)
            {
                printf("┼───");
            }
            printf("┤\n");
        }
        else
        {
            printf("└───");
            for (unsigned int j = 1; j < width; j++)
            {
                printf("┴───");
            }
            printf("┘\n");
        }
    }
}

/**
 * Function containing the game loop
 * @brief Play the game
 */

int game_is_over(int num_players)
{
    return (num_players < 2);
}

int right_move(unsigned int player_id, struct move_t move, struct graph_t *graph, unsigned int num_queens, unsigned int **queens, struct set *arrows)
{
    //  verifie que la queen_src est bien une queen du joueur
    struct set *neighboor_queen_src = get_neighboors(move.queen_src, graph, num_queens, queens, arrows);
    int ret = (is_in_array(move.queen_src, queens[player_id], num_queens));
    if (!ret)
    {
        set_free(neighboor_queen_src);
        return ret;
    }
    // déplace la queen dans un sens sans tirer de fleche
    struct move_t fictiv_move = move;
    fictiv_move.arrow_dst = UINT_MAX;
    update(fictiv_move, player_id, arrows, queens);
    struct set *neighboor_queen_dst = get_neighboors(move.queen_dst, graph, num_queens, queens, arrows);
    ret = ret && is_in_array(move.queen_dst, neighboor_queen_src->data, neighboor_queen_src->size);
    // déplace la queen dans l'autre sens tje sans tirer de fleche
    unsigned int change_queen = move.queen_dst;
    fictiv_move.queen_dst = move.queen_src;
    fictiv_move.queen_src = change_queen;
    update(fictiv_move, player_id, arrows, queens);
    ret = ret && is_in_array(move.arrow_dst, neighboor_queen_dst->data, neighboor_queen_dst->size);
    set_free(neighboor_queen_src);
    set_free(neighboor_queen_dst);
    return ret;
}

void print_move(struct move_t m)
{
    printf("{queen_src : %3u, queen_dst : %3u, arrow : %3u}\n", m.queen_src, m.queen_dst, m.arrow_dst);
}

void game(struct player_t *players, int num_queens, unsigned int **queens, struct graph_t *graph, struct set *arrows)
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        printf("Player %d: %s\n", i + 1, players[i].get_player_name());
    }
    struct move_t move = {.queen_src = -1, .queen_dst = -1, .arrow_dst = -1};
    int player_in_game = NUM_PLAYERS;
    print_game(queens, num_queens, arrows);
    while (!game_is_over(player_in_game))
    {
        int i = 0;
        while ((i < NUM_PLAYERS) && (!players[i].looser))
        {
            move = players[i].play(move);
            if (!right_move(i, move, graph, num_queens, queens, arrows))
            {
                players[i].looser = 1;
                player_in_game--;
                printf("WRONG MOVE!, Player %s lost\n", players[i].get_player_name());
                print_move(move);
            }
            else
            {
                update(move, i, arrows, queens);
                for (int j = 0; j < NUM_PLAYERS; j++)
                {
                    if (!players[j].looser && has_lost(j, num_queens, queens, graph, arrows))
                    {
                        players[j].looser = 1;
                        player_in_game--;
                        printf("Player %s Blocked Player %s \n", players[i].get_player_name(), players[j].get_player_name());
                    }
                }
                print_game(queens, num_queens, arrows);
                i++;
            }
        }
    }
    struct player_t winner;
    int i = 0;
    while (i < NUM_PLAYERS && players[i].looser)
    {
        i++;
    }
    if (i >= NUM_PLAYERS)
        printf("Both players lost\n");
    else
    {
        winner = players[i];
        printf("The winner is %s\n", winner.get_player_name());
    }
    print_game(queens, num_queens, arrows);
}
