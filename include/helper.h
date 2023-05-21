#ifndef HELPER
#define HELPER

#include <stdbool.h>

#include "graph.h"
#include "../include/portable_endian.h"

typedef struct dist_path {
    int64_t *dist;
    uint32_t *path;
} dp;

typedef struct cost_node {
    int64_t max_cost;
    uint32_t max_node;
} cn;

typedef struct path {
    uint32_t *p;
    uint32_t p_size;
} path_t;

dp bellman_ford(graph_t graph, uint32_t s, bool verbose);
cn get_max(const int64_t *dist, size_t distSize, uint32_t s);
path_t get_path(uint32_t source, uint32_t dest, const uint32_t *path, uint32_t nb_nodes);

#endif