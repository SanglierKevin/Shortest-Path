#ifndef LEPL1503__GRAPH__
#define LEPL1503__GRAPH__

#include <stdint.h>
#include <stdio.h>
#include "../include/portable_endian.h"

typedef struct graph {
    int64_t **links;
    uint32_t nb_links;
    uint32_t nb_nodes;
} graph_t;

typedef struct {
    int64_t *buf;
    int length;
    int id;
    int ops;
    int links_index;
    int first;
} thread_t;

extern int64_t **links;
extern int64_t *buf;

void *read_graph(void *arg);
graph_t read_infos(FILE *fd);

#endif  // LEPL1503__GRAPH__