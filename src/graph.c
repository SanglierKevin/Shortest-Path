#include "../include/graph.h"

#include <stdlib.h>

graph_t read_infos(FILE *fd) {
    /* By Alba Dekens ; Kevin Sanglier ; Pedro Miguel Da Silva Matos
    Reads the number of nodes and links situated at the stat of the binary file in argument.

    @args :
        FILE *fd : a pointer towards a binary file that follows the format ; number of nodes, number of links and number of links times links (from, to, cost)
    @returns : 
        a structure graph_t with nb_nodes = the number of nodes, nb_links = the number of links, links = NULL
    */
    fseek(fd, 0, SEEK_END);
    int length = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    int64_t *buf = malloc(length * (length / sizeof(int64_t)));
    size_t ret = fread(buf, length, length / sizeof(int64_t), fd);
    if (ret == -1) {
        exit(EXIT_FAILURE);
    }
    uint32_t nb_nodes = be32toh(*((int *)buf));
    uint32_t nb_links = be32toh(*((int *)buf + 1));
    graph_t graph;
    graph.nb_links = nb_links;
    graph.nb_nodes = nb_nodes;
    graph.links = NULL;
    free(buf);
    return graph;
}

void *read_graph(void *arg) {
    /* By Alba Dekens ; Kevin Sanglier ; Pedro Miguel Da Silva Matos
    Reads the informations related to links in the variable buf, which is a buffer containing the informations of a file. 

    @args :
        void *arg : a pointer towards a structure thread_t that has been casted to void *. The structure contains the bufffer (buf), the id of the thread
                    ops, the number of operations the thread must perform, links_index, the starting index in which the thread must write the informations,
                    first, the node at which the thread is supposed to start reading
    @returns : NULL
               the global variable links has been modified to have the informations read by the threads
    */
    thread_t lb = *((thread_t *)arg);
    int i = 2;
    int k = 0;
    while (i < lb.ops * 3) {
        uint32_t from_node = be32toh(*((int *)lb.buf + lb.first + i));
        uint32_t to_node = be32toh(*((int *)lb.buf + lb.first + i + 1));
        int32_t cost = be32toh(*((int *)lb.buf + lb.first + i + 2));
        int64_t *l = malloc(3 * sizeof(int64_t));
        l[0] = from_node;
        l[1] = to_node;
        l[2] = cost;
        links[lb.links_index + k] = l;
        i += 3;
        k++;
    }
    return NULL;
}