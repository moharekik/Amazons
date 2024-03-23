#ifndef _AMAZON_PROTOTYPES_H_
#define _AMAZON_PROTOTYPES_H_

#include "graph.h"

struct graph_t *graph_initialize_square(unsigned int num_cases_width);

struct graph_t *create_donut_graph(int size);

void graph_free(struct graph_t *g);

#endif // _AMAZON_PROTOTYPES_H_