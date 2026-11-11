//
// Created by dylanbrass on 2026-11-06.
//
#include <stdio.h>
#include <string.h>

#include <ds/dynamic_array.h>

static int failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            fprintf(stderr, "  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            failures++;                                                    \
        }                                                                  \
    } while (0)

#define RUN(test)                                                          \
    do {                                                                   \
        int before = failures;                                             \
        test();                                                            \
        printf("%s %s\n", failures == before ? "PASS" : "FAIL", #test);    \
    } while (0)

static void test_init_sets_up_empty_array(void)
{
    ds_dynamic_array a;
    CHECK(ds_dynamic_array_init(&a, sizeof(int)) == 0);
    CHECK(a.length == 0);
    CHECK(a.capacity >= 1);
    CHECK(a.type_size == sizeof(int));
    CHECK(a.data != nullptr);
    ds_dynamic_array_destroy(&a);
}

static void test_init_rejects_bad_arguments(void)
{
    ds_dynamic_array a;
    CHECK(ds_dynamic_array_init(nullptr, sizeof(int)) == -1);
    CHECK(ds_dynamic_array_init(&a, 0) == -1);
}

static void test_destroy_resets_and_is_safe_to_repeat(void)
{
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(int));
    int x = 1;
    ds_dynamic_array_add(&a, &x);

    ds_dynamic_array_destroy(&a);
    CHECK(a.data == nullptr);
    CHECK(a.length == 0);
    CHECK(a.capacity == 0);

    ds_dynamic_array_destroy(&a);
    ds_dynamic_array_destroy(nullptr);
}

static void test_add_and_get_single_element(void)
{
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(int));

    int x = 42;
    CHECK(ds_dynamic_array_add(&a, &x) == 0);
    CHECK(a.length == 1);

    int* got = ds_dynamic_array_get(&a, 0);
    CHECK(got != nullptr);
    if (got != nullptr) CHECK(*got == 42);

    ds_dynamic_array_destroy(&a);
}

static void test_add_many_ints_forces_growth(void)
{
    enum { N = 10000 };
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(int));

    for (int i = 0; i < N; i++)
    {
        CHECK(ds_dynamic_array_add(&a, &i) == 0);
    }

    CHECK(a.length == N);
    CHECK(a.capacity >= a.length);

    for (int i = 0; i < N; i++)
    {
        const int* got = ds_dynamic_array_get(&a, (size_t)i);
        CHECK(got != nullptr);
        if (got != nullptr) CHECK(*got == i);
    }

    ds_dynamic_array_destroy(&a);
}

static void test_add_copies_the_value(void)
{
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(int));

    int x = 5;
    ds_dynamic_array_add(&a, &x);
    x = 999;

    const int* got = ds_dynamic_array_get(&a, 0);
    CHECK(got != nullptr);
    if (got != nullptr) CHECK(*got == 5);

    ds_dynamic_array_destroy(&a);
}

static void test_add_rejects_null_arguments(void)
{
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(int));
    int x = 1;

    CHECK(ds_dynamic_array_add(nullptr, &x) == -1);
    CHECK(ds_dynamic_array_add(&a, nullptr) == -1);
    CHECK(a.length == 0);

    ds_dynamic_array_destroy(&a);
}

static void test_get_out_of_range_returns_null(void)
{
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(int));

    CHECK(ds_dynamic_array_get(&a, 0) == nullptr);

    int x = 1;
    ds_dynamic_array_add(&a, &x);
    CHECK(ds_dynamic_array_get(&a, 0) != nullptr);
    CHECK(ds_dynamic_array_get(&a, 1) == nullptr);
    CHECK(ds_dynamic_array_get(&a, 1000000) == nullptr);
    CHECK(ds_dynamic_array_get(nullptr, 0) == nullptr);

    ds_dynamic_array_destroy(&a);
}

static void test_get_does_not_see_unused_capacity(void)
{
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(int));

    for (int i = 0; i < 5; i++) ds_dynamic_array_add(&a, &i);

    CHECK(a.capacity > a.length);
    CHECK(ds_dynamic_array_get(&a, a.length) == nullptr);

    ds_dynamic_array_destroy(&a);
}

static void test_chars(void)
{
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(char));

    const char* text = "hello, world";
    size_t len = strlen(text);
    for (size_t i = 0; i < len; i++)
    {
        CHECK(ds_dynamic_array_add(&a, &text[i]) == 0);
    }

    CHECK(a.length == len);
    for (size_t i = 0; i < len; i++)
    {
        const char* got = ds_dynamic_array_get(&a, i);
        CHECK(got != nullptr);
        if (got != nullptr) CHECK(*got == text[i]);
    }

    ds_dynamic_array_destroy(&a);
}

static void test_doubles(void)
{
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(double));

    for (int i = 0; i < 100; i++)
    {
        double d = i * 0.5;
        ds_dynamic_array_add(&a, &d);
    }

    for (int i = 0; i < 100; i++)
    {
        const double* got = ds_dynamic_array_get(&a, (size_t)i);
        CHECK(got != nullptr);
        if (got != nullptr) CHECK(*got == i * 0.5);
    }

    ds_dynamic_array_destroy(&a);
}

typedef struct
{
    int id;
    double score;
    char tag;
} Record;

static void test_structs(void)
{
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(Record));

    for (int i = 0; i < 500; i++)
    {
        Record r = { .id = i, .score = i * 1.5, .tag = (char)('a' + i % 26) };
        CHECK(ds_dynamic_array_add(&a, &r) == 0);
    }

    for (int i = 0; i < 500; i++)
    {
        const Record* got = ds_dynamic_array_get(&a, (size_t)i);
        CHECK(got != nullptr);
        if (got != nullptr)
        {
            CHECK(got->id == i);
            CHECK(got->score == i * 1.5);
            CHECK(got->tag == (char)('a' + i % 26));
        }
    }

    ds_dynamic_array_destroy(&a);
}

static void test_pointers_to_strings(void)
{
    ds_dynamic_array a;
    ds_dynamic_array_init(&a, sizeof(char*));

    const char* words[] = { "alpha", "beta", "gamma", "delta", "epsilon" };
    for (size_t i = 0; i < 5; i++)
    {
        ds_dynamic_array_add(&a, &words[i]);
    }

    for (size_t i = 0; i < 5; i++)
    {
        const char** got = ds_dynamic_array_get(&a, i);
        CHECK(got != nullptr);
        if (got != nullptr) CHECK(strcmp(*got, words[i]) == 0);
    }

    ds_dynamic_array_destroy(&a);
}

static void test_two_arrays_are_independent(void)
{
    ds_dynamic_array ints, chars;
    ds_dynamic_array_init(&ints, sizeof(int));
    ds_dynamic_array_init(&chars, sizeof(char));

    for (int i = 0; i < 50; i++)
    {
        char c = (char)('A' + i % 26);
        ds_dynamic_array_add(&ints, &i);
        ds_dynamic_array_add(&chars, &c);
    }

    CHECK(ints.length == 50);
    CHECK(chars.length == 50);
    const int* i7 = ds_dynamic_array_get(&ints, 7);
    const char* c7 = ds_dynamic_array_get(&chars, 7);
    CHECK(i7 != nullptr && *i7 == 7);
    CHECK(c7 != nullptr && *c7 == 'H');

    ds_dynamic_array_destroy(&ints);
    ds_dynamic_array_destroy(&chars);
}

int main(void)
{
    RUN(test_init_sets_up_empty_array);
    RUN(test_init_rejects_bad_arguments);
    RUN(test_destroy_resets_and_is_safe_to_repeat);

    RUN(test_add_and_get_single_element);
    RUN(test_add_many_ints_forces_growth);
    RUN(test_add_copies_the_value);
    RUN(test_add_rejects_null_arguments);

    RUN(test_get_out_of_range_returns_null);
    RUN(test_get_does_not_see_unused_capacity);

    RUN(test_chars);
    RUN(test_doubles);
    RUN(test_structs);
    RUN(test_pointers_to_strings);

    RUN(test_two_arrays_are_independent);

    if (failures != 0)
    {
        fprintf(stderr, "\n%d check(s) failed\n", failures);
        return 1;
    }

    printf("\nAll tests passed\n");
    return 0;
}