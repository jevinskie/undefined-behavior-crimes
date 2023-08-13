#include <Block.h>
#include <stdio.h>

// https://stackoverflow.com/questions/18063373/is-it-possible-to-un-const-typeof-in-gcc-pure-c/54016713#54016713
#define UNCONST_TYPE_HAX_(TYPE)                                                                    \
    ({                                                                                             \
        TYPE _tmp_macro_var_;                                                                      \
        _tmp_macro_var_;                                                                           \
    })
#define UNCONST_TYPE(v)                                                                            \
    __typeof__(_Generic((v),                                                                       \
        signed char: UNCONST_TYPE_HAX_(signed char),                                               \
        const signed char: UNCONST_TYPE_HAX_(signed char),                                         \
        unsigned char: UNCONST_TYPE_HAX_(unsigned char),                                           \
        const unsigned char: UNCONST_TYPE_HAX_(unsigned char),                                     \
        short: UNCONST_TYPE_HAX_(short),                                                           \
        const short: UNCONST_TYPE_HAX_(short),                                                     \
        unsigned short: UNCONST_TYPE_HAX_(unsigned short),                                         \
        const unsigned short: UNCONST_TYPE_HAX_(unsigned short),                                   \
        int: UNCONST_TYPE_HAX_(int),                                                               \
        const int: UNCONST_TYPE_HAX_(int),                                                         \
        unsigned: UNCONST_TYPE_HAX_(unsigned),                                                     \
        const unsigned: UNCONST_TYPE_HAX_(unsigned),                                               \
        long: UNCONST_TYPE_HAX_(long),                                                             \
        const long: UNCONST_TYPE_HAX_(long),                                                       \
        unsigned long: UNCONST_TYPE_HAX_(unsigned long),                                           \
        const unsigned long: UNCONST_TYPE_HAX_(unsigned long),                                     \
        long long: UNCONST_TYPE_HAX_(long long),                                                   \
        const long long: UNCONST_TYPE_HAX_(long long),                                             \
        unsigned long long: UNCONST_TYPE_HAX_(unsigned long long),                                 \
        const unsigned long long: UNCONST_TYPE_HAX_(unsigned long long),                           \
        float: UNCONST_TYPE_HAX_(float),                                                           \
        const float: UNCONST_TYPE_HAX_(float),                                                     \
        double: UNCONST_TYPE_HAX_(double),                                                         \
        const double: UNCONST_TYPE_HAX_(double),                                                   \
        long double: UNCONST_TYPE_HAX_(long double),                                               \
        const long double: UNCONST_TYPE_HAX_(long double),                                         \
        signed char *: UNCONST_TYPE_HAX_(signed char *),                                           \
        const signed char *: UNCONST_TYPE_HAX_(signed char *),                                     \
        unsigned char *: UNCONST_TYPE_HAX_(unsigned char *),                                       \
        const unsigned char *: UNCONST_TYPE_HAX_(unsigned char *),                                 \
        short *: UNCONST_TYPE_HAX_(short *),                                                       \
        const short *: UNCONST_TYPE_HAX_(short *),                                                 \
        unsigned short *: UNCONST_TYPE_HAX_(unsigned short *),                                     \
        const unsigned short *: UNCONST_TYPE_HAX_(unsigned short *),                               \
        int *: UNCONST_TYPE_HAX_(int *),                                                           \
        const int *: UNCONST_TYPE_HAX_(int *),                                                     \
        unsigned *: UNCONST_TYPE_HAX_(unsigned *),                                                 \
        const unsigned *: UNCONST_TYPE_HAX_(unsigned *),                                           \
        long *: UNCONST_TYPE_HAX_(long *),                                                         \
        const long *: UNCONST_TYPE_HAX_(long *),                                                   \
        unsigned long *: UNCONST_TYPE_HAX_(unsigned long *),                                       \
        const unsigned long *: UNCONST_TYPE_HAX_(unsigned long *),                                 \
        long long *: UNCONST_TYPE_HAX_(long long *),                                               \
        const long long *: UNCONST_TYPE_HAX_(long long *),                                         \
        unsigned long long *: UNCONST_TYPE_HAX_(unsigned long long *),                             \
        const unsigned long long *: UNCONST_TYPE_HAX_(unsigned long long *),                       \
        float *: UNCONST_TYPE_HAX_(float *),                                                       \
        const float *: UNCONST_TYPE_HAX_(float *),                                                 \
        double *: UNCONST_TYPE_HAX_(double *),                                                     \
        const double *: UNCONST_TYPE_HAX_(double *),                                               \
        long double *: UNCONST_TYPE_HAX_(long double *),                                           \
        const long double *: UNCONST_TYPE_HAX_(long double *)))

void no_nested_function_here_used_block_type(void) {
    __block int i = 42;
    printf("i: %d\n", i);
    void (^incr)(void) = ^{
      i += 1;
      printf("block i: %d\n", i);
    };
    incr();
    i *= 2;
    printf("i: %d\n", i);
    incr();
    printf("i: %d\n", i);
}

void no_nested_function_here_use_ptr(void) {
    int i            = 42;
    int *const i_ptr = &i;
    printf("i: %d\n", i);
    void (^incr)(void) = ^{
      *i_ptr += 1;
      printf("block i: %d\n", *i_ptr);
    };
    incr();
    i *= 2;
    printf("i: %d\n", i);
    incr();
    printf("i: %d\n", i);
}

#define UNCONST(x) (*((UNCONST_TYPE(&(x))) & (x)))
void no_nested_function_here_evil_broken(void) {
    int i = 42;
    printf("i: %d\n", i);
    void (^incr)(void) = ^{
      UNCONST(i) += 1;
      printf("block i: %d\n", i);
    };
    incr();
    i *= 2;
    printf("i: %d\n", i);
    incr();
    printf("i: %d\n", i);
}

#define BLK_VAR_DECL(t, v)                                                                         \
    t v;                                                                                           \
    UNCONST_TYPE(v) *const __ptr_##v = &v;                                                         \
    v
#define BLK_VAR_ACCESS(n) *(__ptr_##n)
void no_nested_function_here_evil(void) {
    BLK_VAR_DECL(int, i) = 42;
    printf("i: %d\n", i);
    void (^incr)(void) = ^{
      BLK_VAR_ACCESS(i) += 1;
      printf("block i: %d\n", BLK_VAR_ACCESS(i));
    };
    incr();
    i *= 2;
    printf("i: %d\n", i);
    incr();
    printf("i: %d\n", i);
}

int main(void) {
    printf("calling no_nested_function_here_used_block_type()\n\n\n");
    no_nested_function_here_used_block_type();
    printf("\n\n\ndone\n");
    printf("calling no_nested_function_here_use_ptr\n\n\n");
    no_nested_function_here_use_ptr();
    printf("\n\n\ndone\n");
    printf("calling no_nested_function_here_evil_broken\n\n\n");
    no_nested_function_here_evil_broken();
    printf("\n\n\ndone\n");
    printf("calling no_nested_function_here_evil\n\n\n");
    no_nested_function_here_evil();
    printf("\n\n\ndone\n");
    return 0;
}
