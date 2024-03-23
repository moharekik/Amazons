#include <assert.h>
#include "../src/interface.h"
#include "../src/player.h"
#include "test.h"
#include "../src/graph.h"

// void test_board_initialize()
// {
//     printf("%s", __func__);
//     unsigned int t[2][4] = {{2, 5, 16, 23},
//                             {40, 47, 58, 61}};

//     struct init_data_t data =
//         {
//             .num_vertices = 100,
//             .num_queens = 4,
//         };
//     data.queens = malloc(sizeof(unsigned int *) * 2);
//     data.queens[0] = malloc(sizeof(unsigned int) * 4);
//     data.queens[1] = malloc(sizeof(unsigned int) * 4);
//     for (int i = 0; i < 2; i++)
//     {
//         for (int j = 0; j < 4; j++)
//         {
//             data.queens[i][j] = t[i][j];
//         }
//     }
//     struct set *board = board_initialize(data);
//     assert(board->data[2] == 2);
//     assert(board->data[5] == 2);
//     assert(board->data[16] == 2);
//     assert(board->data[23] == 2);
//     assert(board->data[40] == 3);
//     assert(board->data[47] == 3);
//     assert(board->data[58] == 3);
//     assert(board->data[61] == 3);
//     free(data.queens[0]);
//     free(data.queens[1]);
//     free(data.queens);
//     free(board->data);
//     free(board);
//     printf("\tOK\n");
// }
// struct graph_t graph_initialize_square(unsigned int num_cases_width)
// {
//     struct graph_t graph;
//     graph.num_vertices = num_cases_width * num_cases_width;
//     graph.t = gsl_spmatrix_uint_alloc(num_cases_width, num_cases_width);
//     gsl_spmatrix_uint_set_zero(graph.t);
//     for (unsigned int i = 0; i < num_cases_width; i++)
//     {
//         int row_i = i / num_cases_width; // ligne de la case i
//         int col_i = i % num_cases_width; // colonne de la case i
//         for (unsigned int j = 0; j < num_cases_width; j++)
//         {
//             int row_j = j / num_cases_width; // ligne de la case j
//             int col_j = j % num_cases_width; // colonne de la case j

//             // Si les cases sont sur la même ligne, colonne ou diagonale
//             if ((row_i == row_j || col_i == col_j || (row_i - row_j) == (col_i - col_j)) ||
//                 (((row_i - row_j) == (col_j - col_i)) && (i != j))) // et si elles ne sont pas identiques
//             {
//                 // Calculer la direction pour aller de i à j
//                 enum dir_t dir;
//                 if (row_i == row_j && col_i - col_j == 1)
//                     dir = DIR_EAST;
//                 else if (row_i == row_j && col_i - col_j == -1)
//                     dir = DIR_WEST;
//                 else if (col_i == col_j && row_i - row_j == 1)
//                     dir = DIR_NORTH;
//                 else if (col_i == col_j && row_i - row_j == -1)
//                     dir = DIR_SOUTH;
//                 else if (row_i - row_j == 1 && col_i - col_j == 1)
//                     dir = DIR_NE;
//                 else if (row_i - row_j == 1 && col_i - col_j == -1)
//                     dir = DIR_NW;
//                 else if (row_i - row_j == -1 && col_i - col_j == 1)
//                     dir = DIR_SE;
//                 else if (row_i - row_j == -1 && col_i - col_j == -1)
//                     dir = DIR_SW;
//                 else
//                     dir = NO_DIR;
//                 gsl_spmatrix_uint_set(graph.t, i, j, dir); // Mettre à jour la matrice d'adjacence
//             }
//         }
//     }
//     return graph;
// }
/*
void test_get_neighboors()
{
    printf("%s", __func__);
    struct init_data_t data =
        {
            .num_vertices = 100,
            .num_queens = 4,
            .queens = {{2, 5, 16, 23},
                       {40, 47, 58, 61}},
        };
    unsigned int *board = board_initialize(data);
    struct graph_t graph = graph_initialize_square(100);
    struct set set = get_neighboors(18, &graph, board);
    assert(set.size == 8);
    assert(set.data[0] == 17);
    assert(set.data[1] == 19);
    assert(set.data[2] == 28);
    assert(set.data[3] == 38);
    assert(set.data[4] == 48);
    assert(set.data[5] == 58);
    assert(set.data[6] == 68);
    assert(set.data[7] == 78);
    free(board);
    printf("\tOK\n");
}
*/

int main()
{
    //     test_board_initialize();
    //     // test_get_neighboors();
    //     struct graph_t matrix = graph_initialize_square(3);
    //     print(matrix);
    //     gsl_spmatrix_uint_free(matrix.t);
    puts("OK");
    return 0;
}