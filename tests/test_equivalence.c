#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include <equivalence.h>

static struct files {
    const char *file1;
    const char *file2;
} files [] = {
    {"../data/h2_1",  "../data/h2_2"},
    {"../data/h4_1",  "../data/h4_2"},
    {"../data/h8_1",  "../data/h8_2"},
    {"../data/h16_1", "../data/h16_2"},
};

static const size_t files_size = sizeof(files)/sizeof(files[0]);


static void read_matrix(FILE *fp, TInt **matrix, TInt order)
{
    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            fscanf(fp, "%d", &matrix[i][j]);
        }
    }
}

START_TEST (test_n_order)
{
    int32_t order = 0;
    FILE *fp = fopen(files[_i].file1, "r");
    fscanf(fp, "%d", &order);
    TInt **mat1 = matrix_create(order, order);
    read_matrix(fp, mat1, order);
    min_matrix(mat1, order);
    TInt **res1 = get_result(order);
    reset(order);
    fclose(fp);

    FILE *fp2 = fopen(files[_i].file2, "r");
    fscanf(fp2, "%d", &order);
    TInt **mat2 = matrix_create(order, order);
    read_matrix(fp2, mat2, order);

    min_matrix(mat2, order);
    TInt **res2 = get_result(order);
    reset(order);
    fclose(fp2);

    if (!matrisequal(res1, res2, order)) {
        ck_abort_msg("Min matrices are not equal\n");
    }

    matrix_destroy(mat1, order);
    matrix_destroy(res1, order);
    matrix_destroy(mat2, order);
    matrix_destroy(res2, order);
}
END_TEST

Suite *matrix_equal_suite()
{
    Suite *s = suite_create("Equal Test Suite");
    TCase *tcase = tcase_create("Test Cases with equal matrix");

    tcase_add_loop_test(tcase, test_n_order, 0, files_size);

    suite_add_tcase(s, tcase);
    return s;
}

int main()
{
    int number_failed;
    SRunner *sr;

    sr = srunner_create(matrix_equal_suite());
    srunner_run_all(sr, CK_VERBOSE);
    srunner_set_log (sr, "test.log");
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
