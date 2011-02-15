//g++ test.cpp -o rope_perf -I /usr/local/include/boost-1_44 -g -Wall -lboost_unit_test_framework -ISTLport-5.2.1/stlport && ./rope_perf
//g++ test.cpp -o rope_perf -I /usr/local/include/boost-1_44 -g -Wall -lboost_unit_test_framework -ISTLport-5.2.1/stlport && valgrind --log-file=valgrind_run --num-callers=50 --error-limit=no --leak-check=full ./rope_perf

//g++ perf.cpp -o rope_perf -I /usr/local/include/boost -DNDEBUG -O2 -g -Wall -lboost_unit_test_framework -I../STLport-5.2.1/stlport -L../STLport-5.2.1/build/lib/obj/gcc/so -lstlport

#include "../rope.h"

#include <boost/timer.hpp>

#include <iostream>

#define HAVE_STLPORT 1
#if HAVE_STLPORT
#include <rope>
#endif


// HACK! This must be kept in sync with the value in rope.cpp
const std::size_t c_short_string_max = 512;

const std::size_t c_repeat_iterations = 10;

#define TEST_COMMON(test_code)                          \
    boost::timer timer;                                 \
    std::size_t iterations = 1;                         \
    double result = 0.0;                                \
    do {                                                \
        timer.restart();                                \
        for (std::size_t i = 0; i < iterations; ++i) {  \
            test_code                                   \
        }                                               \
        result = timer.elapsed();                       \
        iterations *= 2;                                \
    } while (result < 0.5);                             \
    iterations /= 2;                                    \
                                                        \
    double run = 0.0;                                   \
    for (std::size_t repetition = 0;                    \
         repetition < c_repeat_iterations;              \
         ++repetition) {                                \
        timer.restart();                                \
        for (std::size_t i = 0; i < iterations; ++i) {  \
            test_code                                   \
        }                                               \
        run = timer.elapsed();                          \
        result = (std::min)(run, result);               \
    }                                                   \
    return result / iterations

#define TEST_ROPE(test_name, value_init_code, test_code)        \
    double test_##test_name##_rope()                            \
    {                                                           \
        rope value;                                             \
        rope other_value;                                       \
        rope other_value_2;                                     \
        typedef rope::const_iterator iter;                      \
        value_init_code                                         \
        TEST_COMMON(test_code);                                 \
    }

#if HAVE_STLPORT
#define TEST_STD_CROPE(test_name, value_init_code, test_code)   \
    double test_##test_name##_std_crope()                       \
    {                                                           \
        std::crope value;                                       \
        std::crope other_value;                                 \
        std::crope other_value_2;                               \
        typedef std::crope::const_iterator iter;                \
        value_init_code                                         \
        TEST_COMMON(test_code);                                 \
    }
#else
#define TEST_STD_CROPE(test_name, value_init_code, test_code)
#endif

#define TEST_STRING(test_name, value_init_code, test_code)      \
    double test_##test_name##_string()                          \
    {                                                           \
        std::string value;                                      \
        std::string other_value;                                \
        std::string other_value_2;                              \
        typedef std::string::const_iterator iter;               \
        value_init_code                                         \
        TEST_COMMON(test_code);                                 \
    }

// 16k characters
const char* c_long_string =
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"

    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";

////////////////////////////////////////////////////////////////////////////////
// iteration over a rope containing a single string node
////////////////////////////////////////////////////////////////////////////////
#define FLAT_ITERATION_INIT()                   \
    value = c_long_string;

#define ITERATION()                                          \
    char c;                                                  \
    const iter end_it = value.end();                         \
    for (iter it = value.begin(); it != end_it; ++it) {      \
        c = *it;                                             \
    }

TEST_ROPE(flat_iteration, FLAT_ITERATION_INIT(), ITERATION());
TEST_STD_CROPE(flat_iteration, FLAT_ITERATION_INIT(), ITERATION());
TEST_STRING(flat_iteration, FLAT_ITERATION_INIT(), ITERATION());


////////////////////////////////////////////////////////////////////////////////
// iteration over a rope consisting of a tree of shared string nodes
////////////////////////////////////////////////////////////////////////////////
#define TREE_ITERATION_INIT()                   \
    value =                                     \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"; \
    for (std::size_t i = 0; i < 10; ++i) {      \
        value.append(value);                    \
    }

TEST_ROPE(tree_iteration, TREE_ITERATION_INIT(), ITERATION());
TEST_STD_CROPE(tree_iteration, TREE_ITERATION_INIT(), ITERATION());
TEST_STRING(tree_iteration, TREE_ITERATION_INIT(), ITERATION());


////////////////////////////////////////////////////////////////////////////////
// iteration over a substring of a substring of a substring ...
////////////////////////////////////////////////////////////////////////////////
#define DEEP_SUBSTRING_INIT()                   \
    value = c_long_string;                      \
    for (std::size_t i = 0; i < 50; ++i) {      \
        value = value.substr(0, value.size());  \
    }

TEST_ROPE(deep_substring_iteration, DEEP_SUBSTRING_INIT(), ITERATION());
TEST_STD_CROPE(deep_substring_iteration, DEEP_SUBSTRING_INIT(), ITERATION());
TEST_STRING(deep_substring_iteration, DEEP_SUBSTRING_INIT(), ITERATION());


////////////////////////////////////////////////////////////////////////////////
// appending c-strings of various sizes
////////////////////////////////////////////////////////////////////////////////
#define _1_CHAR_C_STR_APPEND()                  \
    value.clear();                              \
    for (std::size_t j = 0; j < 1 << 20; ++j) { \
        value += 'c';                           \
    }

TEST_ROPE(1_char_c_str_append, ;, _1_CHAR_C_STR_APPEND());
TEST_STD_CROPE(1_char_c_str_append, ;, _1_CHAR_C_STR_APPEND());
TEST_STRING(1_char_c_str_append, ;, _1_CHAR_C_STR_APPEND());

#define _2_CHAR_C_STR_APPEND()                   \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 19; ++j) {  \
        value += "cc";                           \
    }

TEST_ROPE(2_char_c_str_append, ;, _2_CHAR_C_STR_APPEND());
TEST_STD_CROPE(2_char_c_str_append, ;, _2_CHAR_C_STR_APPEND());
TEST_STRING(2_char_c_str_append, ;, _2_CHAR_C_STR_APPEND());

#define _4_CHAR_C_STR_APPEND()                   \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 18; ++j) {  \
        value += "cccc";                         \
    }

TEST_ROPE(4_char_c_str_append, ;, _4_CHAR_C_STR_APPEND());
TEST_STD_CROPE(4_char_c_str_append, ;, _4_CHAR_C_STR_APPEND());
TEST_STRING(4_char_c_str_append, ;, _4_CHAR_C_STR_APPEND());

#define _8_CHAR_C_STR_APPEND()                   \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 17; ++j) {  \
        value += "cccccccc";                     \
    }

TEST_ROPE(8_char_c_str_append, ;, _8_CHAR_C_STR_APPEND());
TEST_STD_CROPE(8_char_c_str_append, ;, _8_CHAR_C_STR_APPEND());
TEST_STRING(8_char_c_str_append, ;, _8_CHAR_C_STR_APPEND());

#define _16_CHAR_C_STR_APPEND()                  \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 16; ++j) {  \
        value += "cccccccccccccccc";             \
    }

TEST_ROPE(16_char_c_str_append, ;, _16_CHAR_C_STR_APPEND());
TEST_STD_CROPE(16_char_c_str_append, ;, _16_CHAR_C_STR_APPEND());
TEST_STRING(16_char_c_str_append, ;, _16_CHAR_C_STR_APPEND());

#define _32_CHAR_C_STR_APPEND()                  \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 15; ++j) {  \
        value += "cccccccccccccccccccccccccccccccc"; \
    }

TEST_ROPE(32_char_c_str_append, ;, _32_CHAR_C_STR_APPEND());
TEST_STD_CROPE(32_char_c_str_append, ;, _32_CHAR_C_STR_APPEND());
TEST_STRING(32_char_c_str_append, ;, _32_CHAR_C_STR_APPEND());

#define _64_CHAR_C_STR_APPEND()                  \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 14; ++j) {  \
        value +=                                 \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"; \
    }

TEST_ROPE(64_char_c_str_append, ;, _64_CHAR_C_STR_APPEND());
TEST_STD_CROPE(64_char_c_str_append, ;, _64_CHAR_C_STR_APPEND());
TEST_STRING(64_char_c_str_append, ;, _64_CHAR_C_STR_APPEND());

#define _128_CHAR_C_STR_APPEND()                 \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 13; ++j) {  \
        value +=                                 \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"; \
    }

TEST_ROPE(128_char_c_str_append, ;, _128_CHAR_C_STR_APPEND());
TEST_STD_CROPE(128_char_c_str_append, ;, _128_CHAR_C_STR_APPEND());
TEST_STRING(128_char_c_str_append, ;, _128_CHAR_C_STR_APPEND());

#define _256_CHAR_C_STR_APPEND()                 \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 12; ++j) {  \
        value +=                                 \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"; \
    }

TEST_ROPE(256_char_c_str_append, ;, _256_CHAR_C_STR_APPEND());
TEST_STD_CROPE(256_char_c_str_append, ;, _256_CHAR_C_STR_APPEND());
TEST_STRING(256_char_c_str_append, ;, _256_CHAR_C_STR_APPEND());

#define _512_CHAR_C_STR_APPEND()                 \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 11; ++j) {  \
        value +=                                 \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"; \
    }

TEST_ROPE(512_char_c_str_append, ;, _512_CHAR_C_STR_APPEND());
TEST_STD_CROPE(512_char_c_str_append, ;, _512_CHAR_C_STR_APPEND());
TEST_STRING(512_char_c_str_append, ;, _512_CHAR_C_STR_APPEND());

#define _1024_CHAR_C_STR_APPEND()                \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 10; ++j) {  \
        value +=                                 \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"; \
    }

TEST_ROPE(1024_char_c_str_append, ;, _1024_CHAR_C_STR_APPEND());
TEST_STD_CROPE(1024_char_c_str_append, ;, _1024_CHAR_C_STR_APPEND());
TEST_STRING(1024_char_c_str_append, ;, _1024_CHAR_C_STR_APPEND());


////////////////////////////////////////////////////////////////////////////////
// appending ropes/std::strings of various sizes
////////////////////////////////////////////////////////////////////////////////
#define _1_CHAR_APPEND_INIT()                   \
    other_value = "c";

#define _1_CHAR_APPEND()                         \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 20; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(1_char_append, _1_CHAR_APPEND_INIT(), _1_CHAR_APPEND());
TEST_STD_CROPE(1_char_append, _1_CHAR_APPEND_INIT(), _1_CHAR_APPEND());
TEST_STRING(1_char_append, _1_CHAR_APPEND_INIT(), _1_CHAR_APPEND());

#define _2_CHAR_APPEND_INIT()                   \
    other_value = "cc";

#define _2_CHAR_APPEND()                         \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 19; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(2_char_append, _2_CHAR_APPEND_INIT(), _2_CHAR_APPEND());
TEST_STD_CROPE(2_char_append, _2_CHAR_APPEND_INIT(), _2_CHAR_APPEND());
TEST_STRING(2_char_append, _2_CHAR_APPEND_INIT(), _2_CHAR_APPEND());

#define _4_CHAR_APPEND_INIT()                   \
    other_value = "cccc";

#define _4_CHAR_APPEND()                         \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 18; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(4_char_append, _4_CHAR_APPEND_INIT(), _4_CHAR_APPEND());
TEST_STD_CROPE(4_char_append, _4_CHAR_APPEND_INIT(), _4_CHAR_APPEND());
TEST_STRING(4_char_append, _4_CHAR_APPEND_INIT(), _4_CHAR_APPEND());

#define _8_CHAR_APPEND_INIT()                   \
    other_value = "cccccccc";

#define _8_CHAR_APPEND()                         \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 17; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(8_char_append, _8_CHAR_APPEND_INIT(), _8_CHAR_APPEND());
TEST_STD_CROPE(8_char_append, _8_CHAR_APPEND_INIT(), _8_CHAR_APPEND());
TEST_STRING(8_char_append, _8_CHAR_APPEND_INIT(), _8_CHAR_APPEND());

#define _16_CHAR_APPEND_INIT()                  \
    other_value = "cccccccccccccccc";

#define _16_CHAR_APPEND()                        \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 16; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(16_char_append, _16_CHAR_APPEND_INIT(), _16_CHAR_APPEND());
TEST_STD_CROPE(16_char_append, _16_CHAR_APPEND_INIT(), _16_CHAR_APPEND());
TEST_STRING(16_char_append, _16_CHAR_APPEND_INIT(), _16_CHAR_APPEND());

#define _32_CHAR_APPEND_INIT()                  \
    other_value = "cccccccccccccccccccccccccccccccc";

#define _32_CHAR_APPEND()                        \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 15; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(32_char_append, _32_CHAR_APPEND_INIT(), _32_CHAR_APPEND());
TEST_STD_CROPE(32_char_append, _32_CHAR_APPEND_INIT(), _32_CHAR_APPEND());
TEST_STRING(32_char_append, _32_CHAR_APPEND_INIT(), _32_CHAR_APPEND());

#define _64_CHAR_APPEND_INIT()                  \
    other_value =                               \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";

#define _64_CHAR_APPEND()                        \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 14; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(64_char_append, _64_CHAR_APPEND_INIT(), _64_CHAR_APPEND());
TEST_STD_CROPE(64_char_append, _64_CHAR_APPEND_INIT(), _64_CHAR_APPEND());
TEST_STRING(64_char_append, _64_CHAR_APPEND_INIT(), _64_CHAR_APPEND());

#define _128_CHAR_APPEND_INIT()                 \
    other_value =                               \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";

#define _128_CHAR_APPEND()                       \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 13; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(128_char_append, _128_CHAR_APPEND_INIT(), _128_CHAR_APPEND());
TEST_STD_CROPE(128_char_append, _128_CHAR_APPEND_INIT(), _128_CHAR_APPEND());
TEST_STRING(128_char_append, _128_CHAR_APPEND_INIT(), _128_CHAR_APPEND());

#define _256_CHAR_APPEND_INIT()                 \
    other_value =                               \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";

#define _256_CHAR_APPEND()                       \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 12; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(256_char_append, _256_CHAR_APPEND_INIT(), _256_CHAR_APPEND());
TEST_STD_CROPE(256_char_append, _256_CHAR_APPEND_INIT(), _256_CHAR_APPEND());
TEST_STRING(256_char_append, _256_CHAR_APPEND_INIT(), _256_CHAR_APPEND());

#define _512_CHAR_APPEND_INIT()                 \
    other_value =                               \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";

#define _512_CHAR_APPEND()                       \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 11; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(512_char_append, _512_CHAR_APPEND_INIT(), _512_CHAR_APPEND());
TEST_STD_CROPE(512_char_append, _512_CHAR_APPEND_INIT(), _512_CHAR_APPEND());
TEST_STRING(512_char_append, _512_CHAR_APPEND_INIT(), _512_CHAR_APPEND());

#define _1024_CHAR_APPEND_INIT()                \
    other_value =                               \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc" \
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";

#define _1024_CHAR_APPEND()                      \
    value.clear();                               \
    for (std::size_t j = 0; j < 1 << 10; ++j) {  \
        value += other_value;                    \
    }

TEST_ROPE(1024_char_append, _1024_CHAR_APPEND_INIT(), _1024_CHAR_APPEND());
TEST_STD_CROPE(1024_char_append, _1024_CHAR_APPEND_INIT(), _1024_CHAR_APPEND());
TEST_STRING(1024_char_append, _1024_CHAR_APPEND_INIT(), _1024_CHAR_APPEND());


////////////////////////////////////////////////////////////////////////////////
// substring creation
////////////////////////////////////////////////////////////////////////////////
#define CREATE_SUBSTRING_INIT()                 \
    value = c_long_string;

#define CREATE_SUBSTRINGS()                                     \
    for (std::size_t j = 0; j < value.size(); ++j) {            \
        other_value = value.substr(j, value.size() - j);        \
    }

TEST_ROPE(substring_creation, CREATE_SUBSTRING_INIT(), CREATE_SUBSTRINGS());
TEST_STD_CROPE(substring_creation, CREATE_SUBSTRING_INIT(), CREATE_SUBSTRINGS());
TEST_STRING(substring_creation, CREATE_SUBSTRING_INIT(), CREATE_SUBSTRINGS());


////////////////////////////////////////////////////////////////////////////////
// c-string insertion into a 128MB rope/std::string
////////////////////////////////////////////////////////////////////////////////
#define C_STR_INSERT_INIT()                             \
    for (std::size_t i = 0; i < 8 * 1024; ++i) {        \
        value += c_long_string;                         \
    }

#define C_STR_ROPE_INSERT()                                     \
    const std::size_t step_size = value.size() / 10;            \
    for (std::size_t j = 0; j < 10; j += step_size) {           \
        other_value = insert(value, j, "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"); \
    }

#define C_STR_STRING_INSERT()                                   \
    const std::size_t step_size = value.size() / 10;            \
    for (std::size_t j = 0; j < 10; j += step_size) {           \
        value.insert(j, "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"); \
    }

TEST_ROPE(c_str_insertion, C_STR_INSERT_INIT(), C_STR_ROPE_INSERT());
TEST_STD_CROPE(c_str_insertion, C_STR_INSERT_INIT(), C_STR_STRING_INSERT());
TEST_STRING(c_str_insertion, C_STR_INSERT_INIT(), C_STR_STRING_INSERT());


////////////////////////////////////////////////////////////////////////////////
// rope/std::string insertion into a 128MB rope/std::string
////////////////////////////////////////////////////////////////////////////////
#define INSERT_INIT()                                   \
    for (std::size_t i = 0; i < 8 * 1024; ++i) {        \
        value += c_long_string;                         \
    }                                                   \
    other_value = "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";

#define ROPE_INSERT()                                           \
    const std::size_t step_size = value.size() / 10;            \
    for (std::size_t j = 0; j < 10; j += step_size) {           \
        other_value_2 = insert(value, j, other_value);          \
    }

#define STRING_INSERT()                                         \
    const std::size_t step_size = value.size() / 10;            \
    for (std::size_t j = 0; j < 10; j += step_size) {           \
        value.insert(j, other_value);                           \
    }

TEST_ROPE(insertion, INSERT_INIT(), ROPE_INSERT());
TEST_STD_CROPE(insertion, INSERT_INIT(), STRING_INSERT());
TEST_STRING(insertion, INSERT_INIT(), STRING_INSERT());


////////////////////////////////////////////////////////////////////////////////
// erasure from a 128MB rope/std::string
////////////////////////////////////////////////////////////////////////////////
#define ROPE_ERASE()                            \
    erase(value, value.size() / 2, 16 * 1024);

#define STRING_ERASE()                          \
    value.erase(value.size() / 2, 16 * 1024);

TEST_ROPE(erasure, ;, ROPE_ERASE());
TEST_STD_CROPE(erasure, ;, STRING_ERASE());
TEST_STRING(erasure, ;, STRING_ERASE());


////////////////////////////////////////////////////////////////////////////////
// replacing part of a 128MB rope/std::string with a c-string
////////////////////////////////////////////////////////////////////////////////
#define C_STR_REPLACE_INIT()                            \
    for (std::size_t i = 0; i < 8 * 1024; ++i) {        \
        value += c_long_string;                         \
    }

#define C_STR_ROPE_REPLACE()                                         \
    const std::size_t step_size = value.size() / 10;                 \
    for (std::size_t j = 0; j < 10; j += step_size) {                \
        other_value_2 = replace(value, j, 16 * 1024, "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"); \
    }

#define C_STR_STRING_REPLACE()                                  \
    const std::size_t step_size = value.size() / 10;            \
    for (std::size_t j = 0; j < 10; j += step_size) {           \
        value.replace(j, 16 * 1024, "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"); \
    }

TEST_ROPE(c_str_replacement, C_STR_REPLACE_INIT(), C_STR_ROPE_REPLACE());
TEST_STD_CROPE(c_str_replacement, C_STR_REPLACE_INIT(), C_STR_STRING_REPLACE());
TEST_STRING(c_str_replacement, C_STR_REPLACE_INIT(), C_STR_STRING_REPLACE());


////////////////////////////////////////////////////////////////////////////////
// replacing part of a 128MB rope/std::string with a rope/std::string
////////////////////////////////////////////////////////////////////////////////
#define REPLACE_INIT()                                  \
    for (std::size_t i = 0; i < 8 * 1024; ++i) {        \
        value += c_long_string;                         \
    }                                                   \
    other_value = "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";

#define ROPE_REPLACE()                                             \
    const std::size_t step_size = value.size() / 10;               \
    for (std::size_t j = 0; j < 10; j += step_size) {              \
        other_value_2 = replace(value, j, 16 * 1024, other_value); \
    }

#define STRING_REPLACE()                                        \
    const std::size_t step_size = value.size() / 10;            \
    for (std::size_t j = 0; j < 10; j += step_size) {           \
        value.replace(j, 16 * 1024, other_value);               \
    }

TEST_ROPE(replacement, REPLACE_INIT(), ROPE_REPLACE());
TEST_STD_CROPE(replacement, REPLACE_INIT(), STRING_REPLACE());
TEST_STRING(replacement, REPLACE_INIT(), STRING_REPLACE());


////////////////////////////////////////////////////////////////////////////////
// test reoprting macros
////////////////////////////////////////////////////////////////////////////////
#if HAVE_STLPORT
#  define TEST_AND_REPORT_STD_CROPE(x)                                  \
    double crope_time = test_##x##_std_crope();                         \
    ratio = rope_time / crope_time;                                     \
    speedup = ratio < 1.0;                                              \
    if (speedup)                                                        \
        ratio = 1.0 / ratio;                                            \
    std::cout                                                           \
        << #x", STLPort rope: " << crope_time << "s "                   \
        << "(rope is " << ratio << "x " << (speedup ? "faster" : "slower") << ")" \
        << std::endl
#  define TEST_AND_REPORT_STD_STRING(x)
#else
#  define TEST_AND_REPORT_STD_CROPE(x)
#  define TEST_AND_REPORT_STD_STRING(x)                                 \
    double string_time = test_##x##_string();                           \
    ratio = rope_time / string_time;                                    \
    speedup = ratio < 1.0;                                              \
    if (speedup)                                                        \
        ratio = 1.0 / ratio;                                            \
    std::cout                                                           \
        << #x", std::string:  " << string_time << "s "                  \
        << "(rope is " << ratio << "x " << (speedup ? "faster" : "slower") << ")\n" \
        << std::endl;
#endif

#define TEST_AND_REPORT(x)                                              \
    {                                                                   \
        double ratio;                                                   \
        bool speedup;                                                   \
        std::cout << "Starting performance test \""#x"\" ..." << std::endl; \
        double rope_time = test_##x##_rope();                           \
        std::cout                                                       \
            << #x", rope:         " << rope_time << "s" << std::endl;   \
        TEST_AND_REPORT_STD_CROPE(x);                                   \
        TEST_AND_REPORT_STD_STRING(x);                                  \
    }


int main()
{
    TEST_AND_REPORT(flat_iteration);
    TEST_AND_REPORT(tree_iteration);
    TEST_AND_REPORT(deep_substring_iteration);
    TEST_AND_REPORT(1_char_c_str_append);
    TEST_AND_REPORT(2_char_c_str_append);
    TEST_AND_REPORT(4_char_c_str_append);
    TEST_AND_REPORT(8_char_c_str_append);
    TEST_AND_REPORT(16_char_c_str_append);
    TEST_AND_REPORT(32_char_c_str_append);
    TEST_AND_REPORT(64_char_c_str_append);
    TEST_AND_REPORT(128_char_c_str_append);
    TEST_AND_REPORT(256_char_c_str_append);
    TEST_AND_REPORT(512_char_c_str_append);
    TEST_AND_REPORT(1024_char_c_str_append);
    TEST_AND_REPORT(1_char_append);
    TEST_AND_REPORT(2_char_append);
    TEST_AND_REPORT(4_char_append);
    TEST_AND_REPORT(8_char_append);
    TEST_AND_REPORT(16_char_append);
    TEST_AND_REPORT(32_char_append);
    TEST_AND_REPORT(64_char_append);
    TEST_AND_REPORT(128_char_append);
    TEST_AND_REPORT(256_char_append);
    TEST_AND_REPORT(512_char_append);
    TEST_AND_REPORT(1024_char_append);
    TEST_AND_REPORT(substring_creation);
    TEST_AND_REPORT(c_str_insertion);
    TEST_AND_REPORT(insertion);
    TEST_AND_REPORT(erasure);
    TEST_AND_REPORT(c_str_replacement);
    TEST_AND_REPORT(replacement);

    return 0;
}
