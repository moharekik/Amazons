#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "dir.h"
#include "set.h"
void print_graph(struct graph_t *g)
{
    printf("\n");
    for (size_t i = 0; i < g->num_vertices; i++)
    {
        for (size_t j = 0; j < g->num_vertices; j++)
        {
            printf("%u ", gsl_spmatrix_uint_get(g->t, i, j)); // affichage de l'élément i,j de la matrice
        }
        printf("\n"); // saut de ligne à la fin de chaque ligne de la matrice
    }
}

struct graph_t *graph_initialize_square(unsigned int num_cases_width)
{
    struct graph_t *graph = malloc(sizeof(struct graph_t));
    graph->num_vertices = num_cases_width * num_cases_width;

    graph->t = gsl_spmatrix_uint_alloc(graph->num_vertices, graph->num_vertices);
    gsl_spmatrix_uint_set_zero(graph->t);
    for (unsigned int i = 0; i < graph->num_vertices; i++)
    {
        int row_i = i / num_cases_width; // ligne de la case i
        int col_i = i % num_cases_width; // colonne de la case i
        for (unsigned int j = 0; j < graph->num_vertices; j++)
        {

            int row_j = j / num_cases_width; // ligne de la case j
            int col_j = j % num_cases_width; // colonne de la case j

            // Si les cases sont sur la même ligne, colonne ou diagonale
            // if ((row_i == row_j || col_i == col_j || (row_i - row_j) == (col_i - col_j)) ||
            //     (((row_i - row_j) == (col_j - col_i)) && (i != j))) // et si elles ne sont pas identiques
            // {
            // Calculer la direction pour aller de i à j
            enum dir_t dir;
            if (row_i == row_j && col_i - col_j == 1)
                dir = DIR_EAST;
            else if (row_i == row_j && col_i - col_j == -1)
                dir = DIR_WEST;
            else if (col_i == col_j && row_i - row_j == 1)
                dir = DIR_NORTH;
            else if (col_i == col_j && row_i - row_j == -1)
                dir = DIR_SOUTH;
            else if (row_i - row_j == 1 && col_i - col_j == 1)
                dir = DIR_NE;
            else if (row_i - row_j == 1 && col_i - col_j == -1)
                dir = DIR_NW;
            else if (row_i - row_j == -1 && col_i - col_j == 1)
                dir = DIR_SE;
            else if (row_i - row_j == -1 && col_i - col_j == -1)
                dir = DIR_SW;
            else
                dir = NO_DIR;
            gsl_spmatrix_uint_set(graph->t, i, j, dir); // Mettre à jour la matrice d'adjacence
        }
        // }
    }
    // store the uncompressed matrix in a temporary variable
    gsl_spmatrix_uint *g = graph->t;
    // compress the matrix
    graph->t = gsl_spmatrix_uint_compress(graph->t, GSL_SPMATRIX_CSR);
    // free the uncompressed matrix
    gsl_spmatrix_uint_free(g);

    // print_graph(graph);
    return graph;
}

/* Make a cases free of neighboors*/
void set_coeff_to_zero(struct graph_t *g, int row_index, int col_index)
{
    // On récupère la matrice d'adjacence
    gsl_spmatrix_uint *matrix = g->t;
    // On se place sur la ligne de la case dans le tableau des éléments nz
    size_t start = matrix->p[row_index];
    size_t end = matrix->p[row_index + 1];
    // On parcourt la ligne
    for (size_t pos = start; pos < end; ++pos)
    {
        // Si on trouve la colonne de la case dans la ligne
        if (matrix->i[pos] == col_index)
        {
            // On décale les éléments suivants d'une position vers la gauche
            // Afin de suprimer l'élément data[k]
            for (size_t k = pos; k < matrix->nz - 1; ++k)
            {
                matrix->data[k] = matrix->data[k + 1];
                matrix->i[k] = matrix->i[k + 1];
            }
            // On met à jour le nombre d'éléments non nuls de la matrice
            matrix->nz--;
            // On met à jour les pointeurs de débuts de lignes.
            for (size_t k = row_index + 1; k <= matrix->size1; ++k)
                matrix->p[k]--;
        }
    }
}
void remove_node(struct graph_t *graph, int node_index)
{

    unsigned int node = node_index;
    if (node < graph->t->size1)
    {
        // On suprime tous les voisins de la case node dans la matrice d'adjacence
        for (unsigned int i = 0; i < graph->num_vertices; i++)
        {
            set_coeff_to_zero(graph, i, node);
        }
        // La matrice étant symétrique on fait la même chose pour les colonnes
        for (unsigned int j = 0; j < graph->num_vertices; j++)
        {
            set_coeff_to_zero(graph, node, j);
        }
    }
}
/* Initialize a donut graph
PRECOND m >= 5 @RETURN NULL if m isn't divisibale by 3*/
struct graph_t *create_donut_graph(int m)
{
    if (m % 3 != 0)
        exit(1);

    struct graph_t *graph = graph_initialize_square(m);

    for (int i = m / 3; i < 2 * (m / 3); i++)
    {
        for (int j = m / 3; j < 2 * (m / 3); j++)
        {
            remove_node(graph, i * m + j);
        }
    }
    return graph;
}

void graph_free(struct graph_t *g)
{
    gsl_spmatrix_uint_free(g->t);
    free(g);
}