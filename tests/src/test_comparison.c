#include "../include/test_comparison.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

#include "../../include/helper.h"

void test_comparison_200n800l() {
    int sys = system("./sp tests/in/g200n800l.bin -n 1 -f tests/out/o200n800l.bin");
    if (sys == -1) {
        CU_FAIL("Error while executing ./sp.");
        return;
    }
    CU_ASSERT_EQUAL(0, comp_files("tests/out/o200n800l.bin", "tests/correct_out/co200n800l.bin"));
}

void test_comparison_800n200l() {
    int sys = system("./sp tests/in/g800n200l.bin -n 1 -f tests/out/o800n200l.bin");
    if (sys == -1) {
        CU_FAIL("Error while executing ./sp.");
        return;
    }
    CU_ASSERT_EQUAL(0, comp_files("tests/out/o800n200l.bin", "tests/correct_out/co800n200l.bin"));
}

void test_comparison_200n200l() {
    int sys = system("./sp tests/in/g200n200l.bin -n 1 -f tests/out/o200n200l.bin");
    if (sys == -1) {
        CU_FAIL("Error while executing ./sp.");
        return;
    }
    CU_ASSERT_EQUAL(0, comp_files("tests/out/o200n200l.bin", "tests/correct_out/co200n200l.bin"));
}

void test_comparison_1n0l() {
    int sys = system("./sp tests/in/g1n0l.bin -n 1 -f tests/out/o1n0l.bin");
    if (sys == -1) {
        CU_FAIL("Error while executing ./sp.");
        return;
    }
    CU_ASSERT_EQUAL(0, comp_files("tests/out/o1n0l.bin", "tests/correct_out/co1n0l.bin"));
}

void test_comparison_neg() {
    int sys = system("./sp tests/in/gneg.bin -n 1 -f tests/out/oneg.bin");
    if (sys == -1) {
        CU_FAIL("Error while executing ./sp.");
        return;
    }
    CU_ASSERT_EQUAL(0, comp_files("tests/out/oneg.bin", "tests/correct_out/coneg.bin"));
}

void test_comparison_alone() {
    int sys = system("./sp tests/in/galone.bin -n 1 -f tests/out/oalone.bin");
    if (sys == -1) {
        CU_FAIL("Error while executing ./sp.");
        return;
    }
    CU_ASSERT_EQUAL(0, comp_files("tests/out/oalone.bin", "tests/correct_out/coalone.bin"));
}

int comp_files(char* o_filename, char* co_filename) {
    FILE* o_file = fopen(o_filename, "r");
    if (o_file == NULL) {
        CU_FAIL("File not open.");
        return 1;
    }
    FILE* co_file = fopen(co_filename, "r");
    if (co_file == NULL) {
        fclose(o_file);
        CU_FAIL("File not open.");
        return 1;
    }
    int index_o = 0;
    int index_co = 0;
    while (index_o != EOF && index_co != EOF) {
        index_o = getc(o_file);
        index_co = getc(co_file);
        if (index_o != index_co) {
            fclose(o_file);
            fclose(co_file);
            return 1;
        }
    }
    fclose(o_file);
    fclose(co_file);
    return 0;
}