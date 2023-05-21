// This file is inspired by the example given on the Cunit website at the following address :
// https://cunit.sourceforge.net/example.html

#include <CUnit/Basic.h>
#include "include/test_comparison.h"
#include "include/test_bellman_ford.h"
#include "include/test_get_max.h"
#include "include/test_read_graph.h"

int main() {
    // By Alba Dekens ; Kevin Sanglier ; Chris Bakashika
    CU_initialize_registry();

    // All comparison tests.
    CU_pSuite Suite_comparison = CU_add_suite("Suite comparison", 0, 0);
    CU_add_test(Suite_comparison, "comparison 200n800l", test_comparison_200n800l);
    CU_add_test(Suite_comparison, "comparison 800n200l", test_comparison_800n200l);
    CU_add_test(Suite_comparison, "comparison 200n200l", test_comparison_200n200l);
    CU_add_test(Suite_comparison, "comparison 1n0l", test_comparison_1n0l);
    CU_add_test(Suite_comparison, "comparison neg", test_comparison_neg);
    CU_add_test(Suite_comparison, "comparison alone", test_comparison_alone);

    // All bellman_ford tests.
    CU_pSuite Suite_bellman_ford = CU_add_suite("Suite bellman_ford", 0, 0);
    CU_add_test(Suite_bellman_ford, "bellman_ford 1", test_bellman_ford_1);

    // All get_max tests.
    CU_pSuite Suite_get_max = CU_add_suite("Suite get_max", 0, 0);
    CU_add_test(Suite_get_max, "get_max small", test_get_max_small);
    CU_add_test(Suite_get_max, "get_max medium", test_get_max_medium);
    CU_add_test(Suite_get_max, "get_max infinity", test_get_max_infinity);

    // All read_graph tests.
    CU_pSuite Suite_read_graph = CU_add_suite("Suite read_graph", 0, 0);
    CU_add_test(Suite_read_graph, "read_graph 1", test_read_graph_1);

    // Run all tests.
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}