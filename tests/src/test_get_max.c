#include "../include/test_get_max.h"

#include "../../include/helper.h"
#include <CUnit/Basic.h>

void test_get_max_small() {
    int64_t array0[1] = {1};
    size_t array0s = sizeof(array0) / sizeof(array0[0]);
    cn result0 = get_max(array0, array0s, 1);
    CU_ASSERT_EQUAL(result0.max_cost, 1);
    CU_ASSERT_EQUAL(result0.max_node, 0);
}

void test_get_max_medium() {
    int64_t array1[9] = {1, 9, 15, 13, 6, 5, 0, 23, 20};
    size_t array1s = sizeof(array1) / sizeof(array1[0]);
    cn result1 = get_max(array1, array1s, 6);
    CU_ASSERT_EQUAL(result1.max_cost, 23);
    CU_ASSERT_EQUAL(result1.max_node, 7);
}

void test_get_max_infinity() {
    int64_t array2[9] = {INT64_MAX, INT64_MAX, INT64_MAX, INT64_MAX, INT64_MAX, 5, INT64_MAX, INT64_MAX, INT64_MAX};
    size_t array2s = sizeof(array2) / sizeof(array2[0]);
    cn result2 = get_max(array2, array2s, 6);
    CU_ASSERT_EQUAL(result2.max_cost, 5);
    CU_ASSERT_EQUAL(result2.max_node, 5);
}