#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int tests_passed = 0;
static int tests_failed = 0;

static void write_input(const char *content){
    FILE *f = fopen("inputbis.txt", "w");
    if(f == NULL){
        printf("FATAL: cannot open inputbis.txt for writing\n");
        exit(1);
    }
    fprintf(f, "%s", content);
    fclose(f);
}

static void run_test(const char *name, const char *input, int expect_valid){
    int ret;
    int success;
    write_input(input);
    ret = system("./mini_c > /dev/null 2>&1");
    success = (ret == 0);
    if(success == expect_valid){
        printf("PASS: %s\n", name);
        tests_passed++;
    } else {
        printf("FAIL: %s  (expected %s, got %s)\n",
               name,
               expect_valid ? "valid" : "invalid",
               success    ? "valid" : "invalid");
        tests_failed++;
    }
}

int main(){
    printf("Running parser tests...\n\n");

    printf("-- Valid inputs --\n");

    run_test("simple integer assignment",
             "x = 42;\n",
             1);

    run_test("multiple sequential assignments",
             "x = 3;\ny = 6;\nz = 9;\n",
             1);

    run_test("arithmetic with operator precedence",
             "x = 2 + 3 * 4;\n",
             1);

    run_test("parenthesized arithmetic expression",
             "x = (2 + 3) * 4;\n",
             1);

    run_test("identifier used after declaration",
             "a = 10;\nb = a + 5;\n",
             1);

    run_test("if statement without else",
             "x = 5;\nif (x > 3) { y = 1; }\n",
             1);

    run_test("if-else statement",
             "x = 2;\nif (x == 2) { y = 10; } else { y = 20; }\n",
             1);

    run_test("while loop",
             "x = 0;\nwhile (x < 1) { y = x + 1; }\n",
             1);

    run_test("boolean AND expression in assignment",
             "x = 3;\ny = 4;\nz = (x < y) && (y > 2);\n",
             1);

    run_test("boolean OR and NOT expression",
             "x = 1;\ny = 0;\nz = (x == 1) || !(y > 5);\n",
             1);

    printf("\n-- Invalid inputs --\n");

    run_test("missing semicolon after assignment",
             "x = 5\n",
             0);

    run_test("undeclared identifier on right-hand side",
             "y = z + 1;\n",
             0);

    run_test("missing expression after assignment operator",
             "x = ;\n",
             0);

    run_test("if condition missing parentheses",
             "x = 3;\nif x > 2 { y = 1; }\n",
             0);

    run_test("unclosed parenthesis in expression",
             "x = (3 + 5;\n",
             0);

    run_test("if body missing opening brace",
             "x = 3;\nif (x > 2) y = 1; }\n",
             0);

    run_test("if body missing closing brace",
             "x = 3;\nif (x > 2) { y = 1;\n",
             0);

    run_test("while condition missing opening parenthesis",
             "x = 1;\nwhile x < 5 { y = 2; }\n",
             0);

    run_test("assignment to a literal instead of identifier",
             "3 = x;\n",
             0);

    run_test("binary operator with missing right operand",
             "x = 3 + ;\n",
             0);

    printf("\n%d passed, %d failed\n", tests_passed, tests_failed);
    return tests_failed > 0 ? 1 : 0;
}
