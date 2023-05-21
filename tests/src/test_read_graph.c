#include "../include/test_read_graph.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

#include "../../include/graph.h"

int64_t **links;

void test_read_graph_1() {
    int64_t lin[30][3] = {
        {1, 14, 9},
        {1, 7, 1},
        {2, 6, 4},
        {2, 1, 4},
        {2, 19, 3},
        {2, 6, 10},
        {3, 0, 5},
        {3, 12, 2},
        {3, 12, 5},
        {5, 13, 6},
        {7, 4, 2},
        {8, 4, 4},
        {9, 2, 4},
        {10, 3, 2},
        {10, 12, 5},
        {11, 19, 5},
        {11, 18, 4},
        {11, 6, 5},
        {12, 3, 6},
        {13, 1, 1},
        {13, 7, 8},
        {14, 11, 3},
        {14, 12, 5},
        {16, 19, 1},
        {17, 2, 10},
        {17, 6, 9},
        {17, 9, 10},
        {17, 7, 3},
        {17, 7, 6},
        {18, 8, 1}};
    uint32_t nb_links = 30;
    links = (int64_t **)malloc(nb_links * sizeof(int64_t));
    FILE *fd = fopen("./tests/in/g20n30l.bin", "r");

    fseek(fd, 0, SEEK_END);
    int length = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    int64_t *buf = malloc(length * (length / sizeof(int64_t)));
    size_t ret = fread(buf, length, length / sizeof(int64_t), fd);
    if (ret == -1) {
        exit(EXIT_FAILURE);
    }
    thread_t arg = {buf, length, 0, nb_links, 0, 0};
    thread_t arg_arr[1];
    arg_arr[0] = arg;
    read_graph((void *)&arg_arr[0]);
    int testval = 1;
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 3; j++) {
            if (links[i][j] != lin[i][j]) {
                testval = 0;
            }
        }
    }
    CU_ASSERT_EQUAL(testval, 1);
    fclose(fd);
    free(buf);
    for (int i = 0; i < nb_links; i++) {
        free(links[i]);
    }
}