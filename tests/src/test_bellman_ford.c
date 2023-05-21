#include "../include/test_bellman_ford.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

#include "../../include/helper.h"

void test_bellman_ford_1() {
    int64_t links[18][3] = {
        {1, 0, 5},
        {1, 3, 4},
        {1, 6, 2},
        {0, 7, 9},
        {3, 2, 2},
        {2, 6, 6},
        {8, 1, 10},
        {8, 9, 1},
        {8, 3, 9},
        {8, 4, 10},
        {6, 0, 1},
        {6, 3, 8},
        {6, 1, 6},
        {9, 4, 1},
        {4, 2, 4},
        {5, 1, 2},
        {5, 9, 5},
        {5, 9, 4}};

    int64_t **linkss = (int64_t **)malloc(18 * sizeof(int64_t *));
    for (int i = 0; i < 18; i++) {
        linkss[i] = (int64_t *)malloc(3 * sizeof(int64_t));
        for (int j = 0; j < 3; j++) {
            linkss[i][j] = links[i][j];
        }
    }
    graph_t graph = {linkss, 18, 10};
    int64_t dist[10] = {13, 10, 6, 9, 2, INT64_MAX, 12, 22, 0, 1};
    uint32_t path[10] = {6, 8, 4, 8, 9, -1, 1, 0, -1, 8};
    dp bford = bellman_ford(graph, 8, false);
    for (int i = 0; i < 10; i++) {
        CU_ASSERT_EQUAL(bford.dist[i], dist[i]);
        CU_ASSERT_EQUAL(bford.path[i], path[i]);
    }
    for (int i = 0; i < 18; i++) {
        free(linkss[i]);
    }
    free(linkss);
}