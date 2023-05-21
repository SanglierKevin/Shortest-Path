#include "../include/helper.h"

#include <stdlib.h>

dp bellman_ford(graph_t graph, uint32_t s, bool verbose) {
    /* By Alba Dekens ; Kevin Sanglier ; Pedro Miguel Da Silva Matos ; Jhon Perez
    @args :
        graph_t graph : Le graph créé par read_graph().
        uin32_t s : Le nombre source d'un node.
        bool verbose : bool that allows detecting negative cycles.
    @returns :
        dp result : structure avec les distances, le chemin du noeud de source au final et la taille des distances
    */
    int64_t **links = graph.links;
    uint32_t nb_nodes = graph.nb_nodes;
    uint32_t nb_links = graph.nb_links;

    int64_t *dist = (int64_t *)malloc(sizeof(int64_t) * nb_nodes);
    uint32_t *path = (uint32_t *)malloc(sizeof(uint32_t) * nb_nodes);
    if (dist == NULL || path == NULL) {
        dp empty = {NULL, NULL};
        return empty;
    }

    for (uint32_t k = 0; k < nb_nodes; k++) {
        dist[k] = INT64_MAX;
        path[k] = -1;
    }
    dist[s] = 0;

    // Bellman-Ford iteration.
    for (uint32_t i = 0; i < nb_nodes - 1; i++) {
        for (uint32_t j = 0; j < nb_links; j++) {
            uint32_t nodeFrom = *(*(links + j) + 0);
            uint32_t nodeTo = *(*(links + j) + 1);
            int32_t cost = *(*(links + j) + 2);
            if (dist[nodeFrom] != INT64_MAX && dist[nodeTo] > dist[nodeFrom] + cost) {
                dist[nodeTo] = dist[nodeFrom] + cost;
                path[nodeTo] = nodeFrom;
            }
        }
    }

    // Negative cycles detection.
    for (uint32_t j = 0; j < nb_links; j++) {
        uint32_t nodeFrom = *(*(links + j) + 0);
        uint32_t nodeTo = *(*(links + j) + 1);
        int32_t cost = *(*(links + j) + 2);
        if (dist[nodeFrom] != INT64_MAX && dist[nodeTo] > dist[nodeFrom] + cost) {
            if (verbose) {
                printf("Negative cycle detected\n");
            }
            for (uint32_t k = 0; k < nb_nodes; k++) {
                dist[k] = INT64_MAX;
                path[k] = -1;
            }
            dist[s] = 0;
        }
    }

    dp result = {dist, path};
    return result;
}

cn get_max(const int64_t *dist, size_t distSize, uint32_t s) {
    /* By Alba Dekens ; Kevin Sanglier ; Pedro Miguel Da Silva Matos
    Returns the index of the node of which exists a path from 's' to that node and the highest cost of that path.

    @args :
        const int64_t *dist : a pointer towards an array of distance calculated by the bellman ford algorithm
        size_t distSize : the length of the dist array
        uint32_t s : the node source
    @returns :
        cn result : a structure in which ther is an int64_t max_cost which is the highest cost of the path and uint32_t max_node,
                    the highest node
    */ 
    int64_t inf = INT64_MAX;
    cn result;
    result.max_cost = -inf;
    result.max_node = s;
    for (uint32_t node_idx = 0; node_idx < distSize; node_idx++) {
        if (node_idx != s && dist[node_idx] != inf && dist[node_idx] >= result.max_cost) {
            // Allows to split the destination nodes with the same shortest path being the highest.
            if (dist[node_idx] == result.max_cost && result.max_node < node_idx) {
                continue;
            }
            result.max_cost = dist[node_idx];
            result.max_node = node_idx;
        }
    }
    if (result.max_cost == -inf && dist[s] != inf && dist[s] >= result.max_cost) {
        result.max_cost = dist[s];
    }
    return result;
}

path_t get_path(uint32_t source, uint32_t dest, const uint32_t *path, uint32_t nb_nodes) {
    /* By Alba Dekens ; Kevin Sanglier ; Pedro Miguel Da Silva Matos
    Returns the path from a destination node to a source node

    @args :
        source : The source node.
        dest : The destination node.
        path : The precedence array.
    @returns : 
        path_t p_r, a structure that contains an uint32 *path and an uint32_t p_size, the length of the path
    */
    uint32_t *r = malloc(nb_nodes * sizeof(uint32_t));
    uint32_t i = dest, index = 0;
    while (i != source) {
        r[index] = i;
        i = path[i];
        index++;
    }
    r[index] = source;
    index++;
    path_t p_r = {r, index};
    return p_r;
}