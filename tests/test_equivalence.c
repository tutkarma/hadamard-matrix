#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include <equivalence.h>

static struct files_eq {
    const char *file1;
    const char *file2;
} files_eq [] = {
    {"../data/h2_1",  "../data/h2_2"},
    {"../data/h4_1",  "../data/h4_2"},
    {"../data/h8_1",  "../data/h8_2"},
    {"../data/h16_1", "../data/h16_2"},
    {"../data/h20_1", "../data/h20_2"},
    {"../data/h24_1", "../data/h24_2"},
    {"../data/h28_1", "../data/h28_2"},
    {"../data/h32_1", "../data/h32_2"},
};

static struct files_noneq {
    const char *file1;
    const char *file2;
} files_noneq [] = {
    {"../data/h16_1", "../data/h16_3"},
    {"../data/h20_1", "../data/h20_3"},
    {"../data/h24_1", "../data/h24_3"},
    {"../data/h28_1", "../data/h28_3"},
    {"../data/h32_1", "../data/h32_3"},
};

static const size_t files_eq_size = sizeof(files_eq)/sizeof(files_eq[0]);
static const size_t files_noneq_size = sizeof(files_noneq)/sizeof(files_noneq[0]);


START_TEST (test_equal_n_order)
{
    Matrix mat1 = matrix_from_file(files_eq[_i].file1);
    find_min_matrix(mat1);
    Matrix res1 = get_result();
    reset();

    Matrix mat2 = matrix_from_file(files_eq[_i].file2);
    find_min_matrix(mat2);
    Matrix res2 = get_result();
    reset();

    if (!matrisequal(res1, res2)) {
        ck_abort_msg("Min matrices are not equal\n");
    }

    matrix_destroy(mat1);
    matrix_destroy(res1);
    matrix_destroy(mat2);
    matrix_destroy(res2);
}
END_TEST

START_TEST (test_nonequal_n_order)
{
    Matrix mat1 = matrix_from_file(files_noneq[_i].file1);
    find_min_matrix(mat1);
    Matrix res1 = get_result();
    reset();

    Matrix mat2 = matrix_from_file(files_noneq[_i].file2);
    find_min_matrix(mat2);
    Matrix res2 = get_result();
    reset();

    if (matrisequal(res1, res2)) {
        ck_abort_msg("Min matrices are equal\n");
    }

    matrix_destroy(mat1);
    matrix_destroy(res1);
    matrix_destroy(mat2);
    matrix_destroy(res2);
}
END_TEST


Suite *matrix_equal_suite()
{
    Suite *s = suite_create("Equal Test Suite");
    TCase *tcase = tcase_create("Test Cases with equal matrix");

    tcase_set_timeout(tcase, 300000);
    tcase_add_loop_test(tcase, test_equal_n_order, 0, files_eq_size);

    suite_add_tcase(s, tcase);
    return s;
}

Suite *matrix_nonequal_suite()
{
    Suite *s = suite_create("Nonequal Test Suite");
    TCase *tcase = tcase_create("Test Cases with nonequal matrix");

    tcase_set_timeout(tcase, 300000);
    tcase_add_loop_test(tcase, test_nonequal_n_order, 0, files_noneq_size);

    suite_add_tcase(s, tcase);
    return s;
}

int main()
{
    int number_failed;
    SRunner *sr;

    sr = srunner_create(matrix_equal_suite());
    srunner_add_suite (sr, matrix_nonequal_suite());
    srunner_set_log (sr, "test.log");
    srunner_set_xml (sr, "test.xml");
    srunner_run_all(sr, CK_VERBOSE);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
