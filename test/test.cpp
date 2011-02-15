//g++ test.cpp -o rope_test -I /usr/local/include/boost-1_44 -g -Wall -lboost_unit_test_framework && ./rope_test
//g++ test.cpp -o rope_test -I /usr/local/include/boost-1_44 -g -Wall -lboost_unit_test_framework && valgrind --log-file=valgrind_run --num-callers=50 --error-limit=no --leak-check=full ./rope_test

//g++ test.cpp -o rope_test -I /usr/local/include/boost -DNDEBUG -g -Wall /usr/local/lib/libboost_unit_test_framework.a

#include "../rope.h"


//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Rope

#include <boost/test/unit_test.hpp>


// HACK! This must be kept in sync with the value in rope.cpp
const std::size_t c_short_string_max = 256;

const rope reference_value_empty;
const rope reference_value_test("test");
const rope reference_value_repeat_8_cs("cccccccc");
const rope reference_value_9_indices_as_chars(":;<=>?@AB");
const rope reference_value_unequal_to_all("This rope is unequal to all the others.               ");

BOOST_AUTO_TEST_CASE( ctors_assignments_and_subscript_operator )
{
    rope default_rope;
    BOOST_CHECK_EQUAL(default_rope, reference_value_empty);
    BOOST_CHECK(default_rope.empty());
    rope null_c_str_rope(0);
    BOOST_CHECK_EQUAL(null_c_str_rope, reference_value_empty);
    BOOST_CHECK(null_c_str_rope.empty());
    rope null_c_str_rope_2;
    null_c_str_rope_2 = 0;
    BOOST_CHECK_EQUAL(null_c_str_rope_2, reference_value_empty);
    BOOST_CHECK(null_c_str_rope_2.empty());
    rope null_c_str_rope_3;
    null_c_str_rope_3 = "";
    BOOST_CHECK_EQUAL(null_c_str_rope_3, reference_value_empty);
    BOOST_CHECK(null_c_str_rope_3.empty());
    rope c_str_rope("test");
    BOOST_CHECK_EQUAL(c_str_rope, reference_value_test);
    BOOST_CHECK_EQUAL(c_str_rope[0], 't');
    BOOST_CHECK_EQUAL(c_str_rope[1], 'e');
    BOOST_CHECK_EQUAL(c_str_rope[2], 's');
    BOOST_CHECK_EQUAL(c_str_rope[3], 't');
    BOOST_CHECK(c_str_rope.size() == 4);
    rope c_str_rope_2;
    c_str_rope_2 = "test";
    BOOST_CHECK_EQUAL(c_str_rope_2, reference_value_test);
    BOOST_CHECK_EQUAL(c_str_rope_2[0], 't');
    BOOST_CHECK_EQUAL(c_str_rope_2[1], 'e');
    BOOST_CHECK_EQUAL(c_str_rope_2[2], 's');
    BOOST_CHECK_EQUAL(c_str_rope_2[3], 't');
    BOOST_CHECK(c_str_rope_2.size() == 4);
    rope zero_repeated_chars_rope(0, 'c');
    BOOST_CHECK_EQUAL(zero_repeated_chars_rope, reference_value_empty);
    BOOST_CHECK(zero_repeated_chars_rope.empty());
    rope repeated_chars_rope(8, 'c');
    BOOST_CHECK_EQUAL(repeated_chars_rope, reference_value_repeat_8_cs);
    BOOST_CHECK_EQUAL(repeated_chars_rope[0], 'c');
    BOOST_CHECK_EQUAL(repeated_chars_rope[1], 'c');
    BOOST_CHECK_EQUAL(repeated_chars_rope[2], 'c');
    BOOST_CHECK_EQUAL(repeated_chars_rope[3], 'c');
    BOOST_CHECK_EQUAL(repeated_chars_rope[4], 'c');
    BOOST_CHECK_EQUAL(repeated_chars_rope[5], 'c');
    BOOST_CHECK_EQUAL(repeated_chars_rope[6], 'c');
    BOOST_CHECK_EQUAL(repeated_chars_rope[7], 'c');
    BOOST_CHECK(repeated_chars_rope.size() == 8);

#define TEST_COPY(name, ref_value)                    \
    rope copied_from_##name(name);                    \
    BOOST_CHECK_EQUAL(copied_from_##name, name);      \
    BOOST_CHECK_EQUAL(name, copied_from_##name);      \
    BOOST_CHECK_EQUAL(copied_from_##name, ref_value); \
    BOOST_CHECK_EQUAL(ref_value, copied_from_##name)

    TEST_COPY(default_rope, reference_value_empty);
    BOOST_CHECK(copied_from_default_rope.empty());
    TEST_COPY(null_c_str_rope, reference_value_empty);
    BOOST_CHECK(null_c_str_rope.empty());
    TEST_COPY(c_str_rope, reference_value_test);
    TEST_COPY(zero_repeated_chars_rope, reference_value_empty);
    BOOST_CHECK(zero_repeated_chars_rope.empty());
    TEST_COPY(repeated_chars_rope, reference_value_repeat_8_cs);

#undef TEST_COPY

#define TEST_ASSIGN(name, ref_value)                    \
    rope assigned_from_##name;                          \
    assigned_from_##name = name;                        \
    BOOST_CHECK_EQUAL(assigned_from_##name, name);      \
    BOOST_CHECK_EQUAL(name, assigned_from_##name);      \
    BOOST_CHECK_EQUAL(assigned_from_##name, ref_value); \
    BOOST_CHECK_EQUAL(ref_value, assigned_from_##name)

    TEST_ASSIGN(default_rope, reference_value_empty);
    BOOST_CHECK(default_rope.empty());
    TEST_ASSIGN(null_c_str_rope, reference_value_empty);
    BOOST_CHECK(null_c_str_rope.empty());
    TEST_ASSIGN(c_str_rope, reference_value_test);
    BOOST_CHECK_EQUAL(assigned_from_c_str_rope[0], 't');
    BOOST_CHECK_EQUAL(assigned_from_c_str_rope[1], 'e');
    BOOST_CHECK_EQUAL(assigned_from_c_str_rope[2], 's');
    BOOST_CHECK_EQUAL(assigned_from_c_str_rope[3], 't');
    BOOST_CHECK(assigned_from_c_str_rope.size() == 4);
    TEST_ASSIGN(zero_repeated_chars_rope, reference_value_empty);
    BOOST_CHECK(zero_repeated_chars_rope.empty());
    TEST_ASSIGN(repeated_chars_rope, reference_value_repeat_8_cs);
    BOOST_CHECK_EQUAL(assigned_from_repeated_chars_rope[0], 'c');
    BOOST_CHECK_EQUAL(assigned_from_repeated_chars_rope[1], 'c');
    BOOST_CHECK_EQUAL(assigned_from_repeated_chars_rope[2], 'c');
    BOOST_CHECK_EQUAL(assigned_from_repeated_chars_rope[3], 'c');
    BOOST_CHECK_EQUAL(assigned_from_repeated_chars_rope[4], 'c');
    BOOST_CHECK_EQUAL(assigned_from_repeated_chars_rope[5], 'c');
    BOOST_CHECK_EQUAL(assigned_from_repeated_chars_rope[6], 'c');
    BOOST_CHECK_EQUAL(assigned_from_repeated_chars_rope[7], 'c');
    BOOST_CHECK(assigned_from_repeated_chars_rope.size() == 8);

#undef TEST_ASSIGN
}

BOOST_AUTO_TEST_CASE( append )
{
    const char* zero_str = 0;
    const char* append_str = "01234567";
    const rope append_rope(append_str);
    const rope append_rope_plus_paren("01234567)");
    const rope append_rope_plus_paren_plus_test("01234567)test");
    const rope test_plus_append_rope("test01234567");
    const rope test_plus_append_rope_plus_paren("test01234567)");
    const rope test_plus_append_rope_plus_paren_plus_test("test01234567)test");
    const rope reference_value_test_copy(reference_value_test);

    rope built_up_from_empty;
    rope built_up_from_nonempty("test");
    rope built_up_from_copy(reference_value_test);

    built_up_from_empty.append(zero_str);
    BOOST_CHECK(built_up_from_empty.empty());
    built_up_from_empty.append("");
    BOOST_CHECK(built_up_from_empty.empty());
    built_up_from_empty.append(append_str);
    BOOST_CHECK_EQUAL(built_up_from_empty, append_rope);
    built_up_from_empty.append(')');
    BOOST_CHECK_EQUAL(built_up_from_empty, append_rope_plus_paren);
    built_up_from_empty.append(reference_value_test);
    BOOST_CHECK_EQUAL(built_up_from_empty, append_rope_plus_paren_plus_test);

    {
        rope built_up_from_empty_2;
        built_up_from_empty_2.append(')');
        BOOST_CHECK_EQUAL(built_up_from_empty_2, rope(")"));
    }

    {
        rope built_up_from_empty_3;
        built_up_from_empty_3.append(reference_value_test);
        BOOST_CHECK_EQUAL(built_up_from_empty_3, reference_value_test);
    }

    built_up_from_nonempty.append(zero_str);
    BOOST_CHECK_EQUAL(built_up_from_nonempty, reference_value_test);
    built_up_from_nonempty.append("");
    BOOST_CHECK_EQUAL(built_up_from_nonempty, reference_value_test);
    built_up_from_nonempty.append(append_str);
    BOOST_CHECK_EQUAL(built_up_from_nonempty, test_plus_append_rope);
    built_up_from_nonempty.append(')');
    BOOST_CHECK_EQUAL(built_up_from_nonempty, test_plus_append_rope_plus_paren);
    built_up_from_nonempty.append(reference_value_test);
    BOOST_CHECK_EQUAL(built_up_from_nonempty, test_plus_append_rope_plus_paren_plus_test);

    {
        rope built_up_from_nonempty_2("test");
        built_up_from_nonempty_2.append(')');
        BOOST_CHECK_EQUAL(built_up_from_nonempty_2, rope("test)"));
    }

    {
        rope built_up_from_nonempty_3("test");
        built_up_from_nonempty_3.append(reference_value_test);
        BOOST_CHECK_EQUAL(built_up_from_nonempty_3, rope("testtest"));
    }

    built_up_from_copy.append(zero_str);
    BOOST_CHECK_EQUAL(built_up_from_copy, reference_value_test);
    built_up_from_copy.append("");
    BOOST_CHECK_EQUAL(built_up_from_copy, reference_value_test);
    built_up_from_copy.append(append_str);
    BOOST_CHECK_EQUAL(built_up_from_copy, test_plus_append_rope);
    built_up_from_copy.append(')');
    BOOST_CHECK_EQUAL(built_up_from_copy, test_plus_append_rope_plus_paren);
    built_up_from_copy.append(reference_value_test);
    BOOST_CHECK_EQUAL(built_up_from_copy, test_plus_append_rope_plus_paren_plus_test);

    {
        rope built_up_from_copy_2(reference_value_test);
        built_up_from_copy_2.append(')');
        BOOST_CHECK_EQUAL(built_up_from_copy_2, rope("test)"));
    }

    {
        rope built_up_from_copy_3(reference_value_test);
        built_up_from_copy_3.append(reference_value_test);
        BOOST_CHECK_EQUAL(built_up_from_copy_3, rope("testtest"));
    }

    BOOST_CHECK_EQUAL(reference_value_test, reference_value_test_copy);
}

BOOST_AUTO_TEST_CASE( empty_and_size )
{
    BOOST_CHECK(reference_value_empty.empty());
    BOOST_CHECK(!reference_value_test.empty());
    BOOST_CHECK(!reference_value_repeat_8_cs.empty());
    BOOST_CHECK(!reference_value_9_indices_as_chars.empty());
    BOOST_CHECK(!reference_value_unequal_to_all.empty());

    BOOST_CHECK_EQUAL(reference_value_empty.size(), 0u);
    BOOST_CHECK_EQUAL(reference_value_test.size(), 4u);
    BOOST_CHECK_EQUAL(reference_value_repeat_8_cs.size(), 8u);
    BOOST_CHECK_EQUAL(reference_value_9_indices_as_chars.size(), 9u);
    BOOST_CHECK_EQUAL(reference_value_unequal_to_all.size(), 54u);

    rope reference_value_unequal_to_all_copy(reference_value_unequal_to_all);
    reference_value_unequal_to_all_copy.append(reference_value_unequal_to_all);
    reference_value_unequal_to_all_copy.append(reference_value_unequal_to_all);
    reference_value_unequal_to_all_copy.append(reference_value_unequal_to_all);
    reference_value_unequal_to_all_copy.append(reference_value_unequal_to_all);
    reference_value_unequal_to_all_copy.append(reference_value_unequal_to_all);
    reference_value_unequal_to_all_copy.append(reference_value_unequal_to_all);
    reference_value_unequal_to_all_copy.append(reference_value_unequal_to_all);
    reference_value_unequal_to_all_copy.append(reference_value_unequal_to_all);
    reference_value_unequal_to_all_copy.append(reference_value_unequal_to_all);
    BOOST_CHECK_EQUAL(reference_value_unequal_to_all_copy.size(), 10u * 54u);

    rope reference_value_unequal_to_all_copy_2(reference_value_unequal_to_all);
    reference_value_unequal_to_all_copy_2.append(reference_value_unequal_to_all_copy_2);
    reference_value_unequal_to_all_copy_2.append(reference_value_unequal_to_all_copy_2);
    reference_value_unequal_to_all_copy_2.append(reference_value_unequal_to_all_copy_2);
    reference_value_unequal_to_all_copy_2.append(reference_value_unequal_to_all_copy_2);
    reference_value_unequal_to_all_copy_2.append(reference_value_unequal_to_all_copy_2);
    reference_value_unequal_to_all_copy_2.append(reference_value_unequal_to_all_copy_2);
    reference_value_unequal_to_all_copy_2.append(reference_value_unequal_to_all_copy_2);
    reference_value_unequal_to_all_copy_2.append(reference_value_unequal_to_all_copy_2);
    reference_value_unequal_to_all_copy_2.append(reference_value_unequal_to_all_copy_2);
    reference_value_unequal_to_all_copy_2.append(reference_value_unequal_to_all_copy_2);
    BOOST_CHECK_EQUAL(reference_value_unequal_to_all_copy_2.size(), 1024u * 54u);
}

BOOST_AUTO_TEST_CASE( rope_rope_comparison_operators )
{
    const rope test_plus_test("testtest");
    rope concatenated_test_plus_test(reference_value_test);
    concatenated_test_plus_test.append(reference_value_test);

    // comparison to equal values (including to self)
    BOOST_CHECK_EQUAL(reference_value_empty, reference_value_empty);
    BOOST_CHECK_EQUAL(test_plus_test, test_plus_test);
    BOOST_CHECK_EQUAL(concatenated_test_plus_test, concatenated_test_plus_test);

    BOOST_CHECK_EQUAL(reference_value_empty, rope());
    BOOST_CHECK_EQUAL(test_plus_test, rope("testtest"));
    BOOST_CHECK_EQUAL(concatenated_test_plus_test, rope("testtest"));

    BOOST_CHECK_EQUAL(rope(), reference_value_empty);
    BOOST_CHECK_EQUAL(rope("testtest"), test_plus_test);
    BOOST_CHECK_EQUAL(rope("testtest"), concatenated_test_plus_test);

    BOOST_CHECK_EQUAL(test_plus_test, concatenated_test_plus_test);
    BOOST_CHECK_EQUAL(concatenated_test_plus_test, test_plus_test);

    BOOST_CHECK_LE(reference_value_empty, reference_value_empty);
    BOOST_CHECK_LE(test_plus_test, test_plus_test);
    BOOST_CHECK_LE(concatenated_test_plus_test, concatenated_test_plus_test);

    BOOST_CHECK_LE(reference_value_empty, rope());
    BOOST_CHECK_LE(test_plus_test, rope("testtest"));
    BOOST_CHECK_LE(concatenated_test_plus_test, rope("testtest"));

    BOOST_CHECK_LE(rope(), reference_value_empty);
    BOOST_CHECK_LE(rope("testtest"), test_plus_test);
    BOOST_CHECK_LE(rope("testtest"), concatenated_test_plus_test);

    BOOST_CHECK_LE(test_plus_test, concatenated_test_plus_test);
    BOOST_CHECK_LE(concatenated_test_plus_test, test_plus_test);

    BOOST_CHECK_GE(reference_value_empty, reference_value_empty);
    BOOST_CHECK_GE(test_plus_test, test_plus_test);
    BOOST_CHECK_GE(concatenated_test_plus_test, concatenated_test_plus_test);

    BOOST_CHECK_GE(reference_value_empty, rope());
    BOOST_CHECK_GE(test_plus_test, rope("testtest"));
    BOOST_CHECK_GE(concatenated_test_plus_test, rope("testtest"));

    BOOST_CHECK_GE(rope(), reference_value_empty);
    BOOST_CHECK_GE(rope("testtest"), test_plus_test);
    BOOST_CHECK_GE(rope("testtest"), concatenated_test_plus_test);

    BOOST_CHECK_GE(test_plus_test, concatenated_test_plus_test);
    BOOST_CHECK_GE(concatenated_test_plus_test, test_plus_test);

    // comparison to unequal values
    BOOST_CHECK_NE(reference_value_empty, reference_value_test);
    BOOST_CHECK_NE(reference_value_test, reference_value_empty);
    BOOST_CHECK_NE(reference_value_test, reference_value_repeat_8_cs);
    BOOST_CHECK_NE(reference_value_repeat_8_cs, reference_value_test);
    BOOST_CHECK_NE(reference_value_repeat_8_cs, reference_value_9_indices_as_chars);
    BOOST_CHECK_NE(reference_value_9_indices_as_chars, reference_value_repeat_8_cs);
    BOOST_CHECK_NE(reference_value_9_indices_as_chars, reference_value_unequal_to_all);
    BOOST_CHECK_NE(reference_value_unequal_to_all, reference_value_9_indices_as_chars);

    // comparisons to empty
    BOOST_CHECK_LT(reference_value_empty, reference_value_test);
    BOOST_CHECK_LT(reference_value_empty, reference_value_repeat_8_cs);
    BOOST_CHECK_LT(reference_value_empty, reference_value_9_indices_as_chars);
    BOOST_CHECK_LT(reference_value_empty, reference_value_unequal_to_all);

    BOOST_CHECK_LE(reference_value_empty, reference_value_test);
    BOOST_CHECK_LE(reference_value_empty, reference_value_repeat_8_cs);
    BOOST_CHECK_LE(reference_value_empty, reference_value_9_indices_as_chars);
    BOOST_CHECK_LE(reference_value_empty, reference_value_unequal_to_all);

    BOOST_CHECK_GT(reference_value_test, reference_value_empty);
    BOOST_CHECK_GT(reference_value_repeat_8_cs, reference_value_empty);
    BOOST_CHECK_GT(reference_value_9_indices_as_chars, reference_value_empty);
    BOOST_CHECK_GT(reference_value_unequal_to_all, reference_value_empty);

    BOOST_CHECK_GE(reference_value_test, reference_value_empty);
    BOOST_CHECK_GE(reference_value_repeat_8_cs, reference_value_empty);
    BOOST_CHECK_GE(reference_value_9_indices_as_chars, reference_value_empty);
    BOOST_CHECK_GE(reference_value_unequal_to_all, reference_value_empty);

    // comparisons to substrings
    BOOST_CHECK_LT(reference_value_test, concatenated_test_plus_test);
    BOOST_CHECK_LE(reference_value_test, concatenated_test_plus_test);
    BOOST_CHECK_GT(concatenated_test_plus_test, reference_value_test);
    BOOST_CHECK_GE(concatenated_test_plus_test, reference_value_test);

    // general comparisons
    BOOST_CHECK_LT(reference_value_repeat_8_cs, reference_value_test);
    BOOST_CHECK_LE(reference_value_repeat_8_cs, reference_value_test);
    BOOST_CHECK_GT(reference_value_unequal_to_all, reference_value_9_indices_as_chars);
    BOOST_CHECK_GE(reference_value_unequal_to_all, reference_value_9_indices_as_chars);
}

BOOST_AUTO_TEST_CASE( rope_c_str_comparison_operators )
{
    const rope default_rope;
    const rope c_str_rope("test");
    const rope repeated_chars_rope(8, 'c');

    // comparison to equal values
    BOOST_CHECK_EQUAL(default_rope, "");
    BOOST_CHECK_EQUAL(c_str_rope, "test");
    BOOST_CHECK_EQUAL(repeated_chars_rope, "cccccccc");

    BOOST_CHECK_LE(default_rope, "");
    BOOST_CHECK_LE(c_str_rope, "test");
    BOOST_CHECK_LE(repeated_chars_rope, "cccccccc");

    BOOST_CHECK_GE(default_rope, "");
    BOOST_CHECK_GE(c_str_rope, "test");
    BOOST_CHECK_GE(repeated_chars_rope, "cccccccc");

    // comparison to unequal values
    BOOST_CHECK_NE(default_rope, "foo");
    BOOST_CHECK_NE(c_str_rope, "");
    BOOST_CHECK_NE(c_str_rope, "test_");
    BOOST_CHECK_NE(c_str_rope, "tes");
    BOOST_CHECK_NE(repeated_chars_rope, "");
    BOOST_CHECK_NE(repeated_chars_rope, "ccccccc");
    BOOST_CHECK_NE(repeated_chars_rope, "ccccccccc");

    // comparisons to empty
    BOOST_CHECK_GT(c_str_rope, "");
    BOOST_CHECK_GT(repeated_chars_rope, "");

    BOOST_CHECK_GE(default_rope, "");
    BOOST_CHECK_GE(c_str_rope, "");
    BOOST_CHECK_GE(repeated_chars_rope, "");

    // general comparisons
    BOOST_CHECK_LT(default_rope, "foo");
    BOOST_CHECK_LT(c_str_rope, "test_");
    BOOST_CHECK_LT(repeated_chars_rope, "ccccccccc");
}

BOOST_AUTO_TEST_CASE( rebalance )
{
    {
        // implicit rebalance
        rope short_rope(8, 'c');
        for (std::size_t i = 0; i < 150; ++i) {
            short_rope.append(rope(8, 'd'));
        }
    }
    {
        // explicit rebalance
        rope short_rope(8, 'c');
        for (std::size_t i = 0; i < rope::c_max_depth - 1; ++i) {
            short_rope.append(rope(8, 'd'));
        }
        rope short_rope_copy(short_rope);
        short_rope.rebalance();
        BOOST_CHECK_EQUAL(short_rope, short_rope_copy);
    }
}

BOOST_AUTO_TEST_CASE( substring )
{
    rope default_rope;
    rope null_c_str_rope(0);
    rope c_str_rope("test");
    rope zero_repeated_chars_rope(0, 'c');
    rope repeated_chars_rope(8, 'c');
    rope concatenated_test_plus_test(c_str_rope);
    concatenated_test_plus_test.append(c_str_rope);

    const rope default_rope_initial_value("");
    const rope null_c_str_rope_initial_value("");
    const rope c_str_rope_initial_value("test");
    const rope zero_repeated_chars_rope_initial_value("");
    const rope repeated_chars_rope_initial_value("cccccccc");
    const rope concatenated_test_plus_test_initial_value("testtest");
    const rope reference_value_unequal_to_all_initial_value("This rope is unequal to all the others.               ");

    BOOST_CHECK_EQUAL(default_rope, default_rope_initial_value);
    BOOST_CHECK_EQUAL(null_c_str_rope, null_c_str_rope_initial_value);
    BOOST_CHECK_EQUAL(c_str_rope, c_str_rope_initial_value);
    BOOST_CHECK_EQUAL(zero_repeated_chars_rope, zero_repeated_chars_rope_initial_value);
    BOOST_CHECK_EQUAL(repeated_chars_rope, repeated_chars_rope_initial_value);
    BOOST_CHECK_EQUAL(concatenated_test_plus_test, concatenated_test_plus_test_initial_value);

#define ZERO_SIZE_SUBSTR(x)                             \
    rope zero_size_substring_of_##x = x.substr(0, 0);   \
    BOOST_CHECK(zero_size_substring_of_##x.empty())

    ZERO_SIZE_SUBSTR(default_rope);
    ZERO_SIZE_SUBSTR(null_c_str_rope);
    ZERO_SIZE_SUBSTR(c_str_rope);
    ZERO_SIZE_SUBSTR(zero_repeated_chars_rope);
    ZERO_SIZE_SUBSTR(repeated_chars_rope);
    ZERO_SIZE_SUBSTR(concatenated_test_plus_test);
    ZERO_SIZE_SUBSTR(reference_value_unequal_to_all);

#undef ZERO_SIZE_SUBSTR

#define FULL_SIZE_SUBSTR(x)                                      \
    rope full_size_substring_of_##x = x.substr(0, rope::npos);   \
    BOOST_CHECK_EQUAL(full_size_substring_of_##x, x)

    FULL_SIZE_SUBSTR(default_rope);
    FULL_SIZE_SUBSTR(null_c_str_rope);
    FULL_SIZE_SUBSTR(c_str_rope);
    FULL_SIZE_SUBSTR(zero_repeated_chars_rope);
    FULL_SIZE_SUBSTR(repeated_chars_rope);
    FULL_SIZE_SUBSTR(concatenated_test_plus_test);
    FULL_SIZE_SUBSTR(reference_value_unequal_to_all);

#undef FULL_SIZE_SUBSTR

#define COMPREHENSIVE_CHAR_CHECK(x)                                     \
    for (std::size_t substr_size = 0;                                   \
         substr_size < x.size();                                        \
         ++substr_size) {                                               \
        for (std::size_t substr_index = 0;                              \
             substr_index < x.size() - substr_size;                     \
             ++substr_index) {                                          \
            rope substr = x.substr(substr_index, substr_size);          \
            for (std::size_t i = 0; i < substr.size(); ++i) {           \
                BOOST_CHECK_EQUAL(substr[i], x[substr_index + i]);      \
            }                                                           \
        }                                                               \
    }

    COMPREHENSIVE_CHAR_CHECK(default_rope);
    COMPREHENSIVE_CHAR_CHECK(null_c_str_rope);
    COMPREHENSIVE_CHAR_CHECK(c_str_rope);
    COMPREHENSIVE_CHAR_CHECK(zero_repeated_chars_rope);
    COMPREHENSIVE_CHAR_CHECK(repeated_chars_rope);
    COMPREHENSIVE_CHAR_CHECK(concatenated_test_plus_test);
    COMPREHENSIVE_CHAR_CHECK(reference_value_unequal_to_all);

#undef COMPREHENSIVE_CHAR_CHECK

#define COMPREHENSIVE_CONCAT_CHECK(x)                                   \
    for (std::size_t i = 0; i < x.size(); ++i) {                        \
        rope concatenated(x.substr(0, i));                              \
        concatenated.append(x.substr(i, x.size() - i));                 \
        BOOST_CHECK_EQUAL(concatenated, x);                             \
    }                                                                   \

    COMPREHENSIVE_CONCAT_CHECK(default_rope);
    COMPREHENSIVE_CONCAT_CHECK(null_c_str_rope);
    COMPREHENSIVE_CONCAT_CHECK(c_str_rope);
    COMPREHENSIVE_CONCAT_CHECK(zero_repeated_chars_rope);
    COMPREHENSIVE_CONCAT_CHECK(repeated_chars_rope);
    COMPREHENSIVE_CONCAT_CHECK(concatenated_test_plus_test);
    COMPREHENSIVE_CONCAT_CHECK(reference_value_unequal_to_all);

#undef COMPREHENSIVE_CONCAT_CHECK

#define COMPREHENSIVE_SUBSTR_MUTATION_CHECK(x)                          \
    for (std::size_t substr_size = 0;                                   \
         substr_size < x.size();                                        \
         ++substr_size) {                                               \
        for (std::size_t substr_index = 0;                              \
             substr_index < x.size() - substr_size;                     \
             ++substr_index) {                                          \
            rope substr = x.substr(substr_index, substr_size);          \
            substr.append('c');                                         \
            BOOST_CHECK_EQUAL(x, x##_initial_value);                    \
        }                                                               \
    }

    COMPREHENSIVE_SUBSTR_MUTATION_CHECK(default_rope);
    COMPREHENSIVE_SUBSTR_MUTATION_CHECK(null_c_str_rope);
    COMPREHENSIVE_SUBSTR_MUTATION_CHECK(c_str_rope);
    COMPREHENSIVE_SUBSTR_MUTATION_CHECK(zero_repeated_chars_rope);
    COMPREHENSIVE_SUBSTR_MUTATION_CHECK(repeated_chars_rope);
    COMPREHENSIVE_SUBSTR_MUTATION_CHECK(concatenated_test_plus_test);
    COMPREHENSIVE_SUBSTR_MUTATION_CHECK(reference_value_unequal_to_all);

#undef COMPREHENSIVE_SUBSTR_MUTATION_CHECK

#define COMPREHENSIVE_ORIGNAL_STRING_MUTATION_CHECK(x)                  \
    for (std::size_t substr_size = 0;                                   \
         substr_size < x.size();                                        \
         ++substr_size) {                                               \
        for (std::size_t substr_index = 0;                              \
             substr_index < x.size() - substr_size;                     \
             ++substr_index) {                                          \
            rope substr = x.substr(substr_index, substr_size);          \
            rope initial_value_substr =                                 \
                x##_initial_value.substr(substr_index, substr_size);    \
            x.append('c');                                              \
            BOOST_CHECK_EQUAL(substr, initial_value_substr);            \
            x = x##_initial_value;                                      \
        }                                                               \
    }

    COMPREHENSIVE_ORIGNAL_STRING_MUTATION_CHECK(default_rope);
    COMPREHENSIVE_ORIGNAL_STRING_MUTATION_CHECK(null_c_str_rope);
    COMPREHENSIVE_ORIGNAL_STRING_MUTATION_CHECK(c_str_rope);
    COMPREHENSIVE_ORIGNAL_STRING_MUTATION_CHECK(zero_repeated_chars_rope);
    COMPREHENSIVE_ORIGNAL_STRING_MUTATION_CHECK(repeated_chars_rope);
    COMPREHENSIVE_ORIGNAL_STRING_MUTATION_CHECK(concatenated_test_plus_test);

#undef COMPREHENSIVE_ORIGNAL_STRING_MUTATION_CHECK
}

std::ostream& operator<<(std::ostream& os, const rope::const_iterator&)
{ return os << "rope::const_iterator"; }

BOOST_AUTO_TEST_CASE( iterator )
{
    rope default_rope;
    rope null_c_str_rope(0);
    rope c_str_rope("test");
    rope zero_repeated_chars_rope(0, 'c');
    rope repeated_chars_rope(8, 'c');
    rope concatenated_test_plus_test(c_str_rope);
    concatenated_test_plus_test.append(c_str_rope);

    // begin-end tests
#define CHECK_EMPTY_BEGIN_END(x)                                        \
    BOOST_CHECK(x.begin() <= x.end());                                  \
    BOOST_CHECK_EQUAL(static_cast<std::size_t>(x.end() - x.begin()), x.size()); \
    BOOST_CHECK(x.begin() == x.end())

#define CHECK_NONEMPTY_BEGIN_END(x)                                     \
    BOOST_CHECK(x.begin() <= x.end());                                  \
    BOOST_CHECK_EQUAL(static_cast<std::size_t>(x.end() - x.begin()), x.size()); \
    BOOST_CHECK(x.begin() < x.end())

    CHECK_EMPTY_BEGIN_END(default_rope);
    CHECK_EMPTY_BEGIN_END(null_c_str_rope);
    CHECK_NONEMPTY_BEGIN_END(c_str_rope);
    CHECK_EMPTY_BEGIN_END(zero_repeated_chars_rope);
    CHECK_NONEMPTY_BEGIN_END(repeated_chars_rope);
    CHECK_NONEMPTY_BEGIN_END(concatenated_test_plus_test);

#undef CHECK_EMPTY_BEGIN_END
#undef CHECK_NONEMPTY_BEGIN_END

#define CHECK_ITERATOR_ADDITION(x)                                      \
    {                                                                   \
        rope::const_iterator it = x.begin();                            \
        for (std::size_t i = 0; i <= x.size(); ++i) {                   \
            rope::const_iterator it2 = x.begin();                       \
            for (std::size_t j = 0; j <= x.size(); ++j) {               \
                std::ptrdiff_t offset = j - i;                          \
                rope::const_iterator it_plus_offset = it + offset;      \
                BOOST_CHECK_EQUAL(it_plus_offset, it2);                 \
                if (it2 != x.end())                                     \
                    BOOST_CHECK_EQUAL(x[j], *it_plus_offset);           \
                if (j != x.size())                                      \
                    ++it2;                                              \
            }                                                           \
            if (i != x.size())                                          \
                ++it;                                                   \
        }                                                               \
    }

    CHECK_ITERATOR_ADDITION(default_rope);
    CHECK_ITERATOR_ADDITION(null_c_str_rope);
    CHECK_ITERATOR_ADDITION(c_str_rope);
    CHECK_ITERATOR_ADDITION(zero_repeated_chars_rope);
    CHECK_ITERATOR_ADDITION(repeated_chars_rope);
    CHECK_ITERATOR_ADDITION(concatenated_test_plus_test);

#undef CHECK_ITERATOR_ADDITION

#define CHECK_ITERATOR_SUBTRACTION(x)                                   \
    {                                                                   \
        rope::const_iterator it = x.begin();                            \
        for (std::size_t i = 0; i <= x.size(); ++i) {                   \
            rope::const_iterator it2 = x.begin();                       \
            for (std::size_t j = 0; j <= x.size(); ++j) {               \
                std::ptrdiff_t offset = i - j;                          \
                rope::const_iterator it_plus_offset = it - offset;      \
                BOOST_CHECK_EQUAL(it_plus_offset, it2);                 \
                if (it2 != x.end())                                     \
                    BOOST_CHECK_EQUAL(x[j], *it_plus_offset);           \
                if (j != x.size())                                      \
                    ++it2;                                              \
            }                                                           \
            if (i != x.size())                                          \
                ++it;                                                   \
        }                                                               \
    }

    CHECK_ITERATOR_SUBTRACTION(default_rope);
    CHECK_ITERATOR_SUBTRACTION(null_c_str_rope);
    CHECK_ITERATOR_SUBTRACTION(c_str_rope);
    CHECK_ITERATOR_SUBTRACTION(zero_repeated_chars_rope);
    CHECK_ITERATOR_SUBTRACTION(repeated_chars_rope);
    CHECK_ITERATOR_SUBTRACTION(concatenated_test_plus_test);

#undef CHECK_ITERATOR_SUBTRACTION
}

BOOST_AUTO_TEST_CASE( reverse_iterator )
{
    rope default_rope;
    rope null_c_str_rope(0);
    rope c_str_rope("test");
    rope zero_repeated_chars_rope(0, 'c');
    rope repeated_chars_rope(8, 'c');
    rope concatenated_test_plus_test(c_str_rope);
    concatenated_test_plus_test.append(c_str_rope);

    // rbegin-rend tests
#define CHECK_EMPTY_RBEGIN_REND(x)                                      \
    BOOST_CHECK(x.rbegin() <= x.rend());                                \
    BOOST_CHECK_EQUAL(static_cast<std::size_t>(x.rend() - x.rbegin()), x.size()); \
    BOOST_CHECK(x.rbegin() == x.rend())

#define CHECK_NONEMPTY_RBEGIN_REND(x)                                   \
    BOOST_CHECK(x.rbegin() <= x.rend());                                \
    BOOST_CHECK_EQUAL(static_cast<std::size_t>(x.rend() - x.rbegin()), x.size()); \
    BOOST_CHECK(x.rbegin() < x.rend())

    CHECK_EMPTY_RBEGIN_REND(default_rope);
    CHECK_EMPTY_RBEGIN_REND(null_c_str_rope);
    CHECK_NONEMPTY_RBEGIN_REND(c_str_rope);
    CHECK_EMPTY_RBEGIN_REND(zero_repeated_chars_rope);
    CHECK_NONEMPTY_RBEGIN_REND(repeated_chars_rope);
    CHECK_NONEMPTY_RBEGIN_REND(concatenated_test_plus_test);

#undef CHECK_EMPTY_RBEGIN_REND
#undef CHECK_NONEMPTY_RBEGIN_REND
}

BOOST_AUTO_TEST_CASE( insert_ )
{
    rope default_rope;
    rope null_c_str_rope(0);
    rope c_str_rope("test");
    rope zero_repeated_chars_rope(0, 'c');
    rope repeated_chars_rope(8, 'c');
    rope concatenated_test_plus_test(c_str_rope);
    concatenated_test_plus_test.append(c_str_rope);

    BOOST_CHECK_EQUAL(insert(default_rope, 0, rope("--")), rope("--"));
    BOOST_CHECK_EQUAL(insert(null_c_str_rope, 0, rope("--")), rope("--"));
    BOOST_CHECK_EQUAL(insert(c_str_rope, 0, rope("--")), rope("--test"));
    BOOST_CHECK_EQUAL(insert(c_str_rope, 1, rope("--")), rope("t--est"));
    BOOST_CHECK_EQUAL(insert(c_str_rope, 2, rope("--")), rope("te--st"));
    BOOST_CHECK_EQUAL(insert(c_str_rope, 3, rope("--")), rope("tes--t"));
    BOOST_CHECK_EQUAL(insert(c_str_rope, 4, rope("--")), rope("test--"));
    BOOST_CHECK_EQUAL(insert(zero_repeated_chars_rope, 0, rope("--")), rope("--"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 0, rope("--")), rope("--cccccccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 1, rope("--")), rope("c--ccccccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 2, rope("--")), rope("cc--cccccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 3, rope("--")), rope("ccc--ccccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 4, rope("--")), rope("cccc--cccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 5, rope("--")), rope("ccccc--ccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 6, rope("--")), rope("cccccc--cc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 7, rope("--")), rope("ccccccc--c"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 8, rope("--")), rope("cccccccc--"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 0, rope("--")), rope("--testtest"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 1, rope("--")), rope("t--esttest"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 2, rope("--")), rope("te--sttest"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 3, rope("--")), rope("tes--ttest"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 4, rope("--")), rope("test--test"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 5, rope("--")), rope("testt--est"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 6, rope("--")), rope("testte--st"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 7, rope("--")), rope("testtes--t"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 8, rope("--")), rope("testtest--"));

    BOOST_CHECK_EQUAL(insert(default_rope, 0, "--"), rope("--"));
    BOOST_CHECK_EQUAL(insert(null_c_str_rope, 0, "--"), rope("--"));
    BOOST_CHECK_EQUAL(insert(c_str_rope, 0, "--"), rope("--test"));
    BOOST_CHECK_EQUAL(insert(c_str_rope, 1, "--"), rope("t--est"));
    BOOST_CHECK_EQUAL(insert(c_str_rope, 2, "--"), rope("te--st"));
    BOOST_CHECK_EQUAL(insert(c_str_rope, 3, "--"), rope("tes--t"));
    BOOST_CHECK_EQUAL(insert(c_str_rope, 4, "--"), rope("test--"));
    BOOST_CHECK_EQUAL(insert(zero_repeated_chars_rope, 0, "--"), rope("--"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 0, "--"), rope("--cccccccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 1, "--"), rope("c--ccccccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 2, "--"), rope("cc--cccccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 3, "--"), rope("ccc--ccccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 4, "--"), rope("cccc--cccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 5, "--"), rope("ccccc--ccc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 6, "--"), rope("cccccc--cc"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 7, "--"), rope("ccccccc--c"));
    BOOST_CHECK_EQUAL(insert(repeated_chars_rope, 8, "--"), rope("cccccccc--"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 0, "--"), rope("--testtest"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 1, "--"), rope("t--esttest"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 2, "--"), rope("te--sttest"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 3, "--"), rope("tes--ttest"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 4, "--"), rope("test--test"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 5, "--"), rope("testt--est"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 6, "--"), rope("testte--st"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 7, "--"), rope("testtes--t"));
    BOOST_CHECK_EQUAL(insert(concatenated_test_plus_test, 8, "--"), rope("testtest--"));
}

BOOST_AUTO_TEST_CASE( erase_ )
{
    rope default_rope;
    rope c_str_rope("test");
    rope repeated_chars_rope(8, 'c');
    rope concatenated_test_plus_test(c_str_rope);
    concatenated_test_plus_test.append(c_str_rope);

    BOOST_CHECK_EQUAL(erase(default_rope, 0, 0), "");
    BOOST_CHECK_EQUAL(erase(default_rope, rope::npos, 0), "");
    BOOST_CHECK_EQUAL(erase(c_str_rope, 0, 2), "st");
    BOOST_CHECK_EQUAL(erase(c_str_rope, 1, 2), "tt");
    BOOST_CHECK_EQUAL(erase(c_str_rope, 2, 2), "te");
    BOOST_CHECK_EQUAL(erase(c_str_rope, 3, 1), "tes");
    BOOST_CHECK_EQUAL(erase(c_str_rope, 4, 0), "test");
    BOOST_CHECK_EQUAL(erase(c_str_rope, 0, rope::npos), "");
    BOOST_CHECK_EQUAL(erase(c_str_rope, 1, rope::npos), "t");
    BOOST_CHECK_EQUAL(erase(c_str_rope, 2, rope::npos), "te");
    BOOST_CHECK_EQUAL(erase(c_str_rope, 3, rope::npos), "tes");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 0, 2), "cccccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 1, 2), "cccccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 2, 2), "cccccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 3, 2), "cccccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 4, 2), "cccccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 5, 2), "cccccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 6, 2), "cccccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 7, 1), "ccccccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 0, rope::npos), "");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 1, rope::npos), "c");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 2, rope::npos), "cc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 3, rope::npos), "ccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 4, rope::npos), "cccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 5, rope::npos), "ccccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 6, rope::npos), "cccccc");
    BOOST_CHECK_EQUAL(erase(repeated_chars_rope, 7, rope::npos), "ccccccc");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 0, 2), "sttest");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 1, 2), "tttest");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 2, 2), "tetest");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 3, 2), "tesest");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 4, 2), "testst");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 5, 2), "testtt");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 6, 2), "testte");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 7, 1), "testtes");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 0, rope::npos), "");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 1, rope::npos), "t");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 2, rope::npos), "te");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 3, rope::npos), "tes");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 4, rope::npos), "test");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 5, rope::npos), "testt");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 6, rope::npos), "testte");
    BOOST_CHECK_EQUAL(erase(concatenated_test_plus_test, 7, rope::npos), "testtes");
}

BOOST_AUTO_TEST_CASE( replace_ )
{
    rope default_rope;
    rope c_str_rope("test");
    rope repeated_chars_rope(8, 'c');
    rope concatenated_test_plus_test(c_str_rope);
    concatenated_test_plus_test.append(c_str_rope);

    BOOST_CHECK_EQUAL(replace(default_rope, 0, 0, rope("--")), "--");
    BOOST_CHECK_EQUAL(replace(default_rope, rope::npos, 0, rope("--")), "--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 0, 2, rope("--")), "--st");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 1, 2, rope("--")), "t--t");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 2, 2, rope("--")), "te--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 3, 1, rope("--")), "tes--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 4, 0, rope("--")), "test--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 0, rope::npos, rope("--")), "--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 1, rope::npos, rope("--")), "t--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 2, rope::npos, rope("--")), "te--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 3, rope::npos, rope("--")), "tes--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 0, 2, rope("--")), "--cccccc");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 1, 2, rope("--")), "c--ccccc");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 2, 2, rope("--")), "cc--cccc");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 3, 2, rope("--")), "ccc--ccc");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 4, 2, rope("--")), "cccc--cc");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 5, 2, rope("--")), "ccccc--c");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 6, 2, rope("--")), "cccccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 7, 1, rope("--")), "ccccccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 0, rope::npos, rope("--")), "--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 1, rope::npos, rope("--")), "c--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 2, rope::npos, rope("--")), "cc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 3, rope::npos, rope("--")), "ccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 4, rope::npos, rope("--")), "cccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 5, rope::npos, rope("--")), "ccccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 6, rope::npos, rope("--")), "cccccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 7, rope::npos, rope("--")), "ccccccc--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 0, 2, rope("--")), "--sttest");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 1, 2, rope("--")), "t--ttest");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 2, 2, rope("--")), "te--test");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 3, 2, rope("--")), "tes--est");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 4, 2, rope("--")), "test--st");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 5, 2, rope("--")), "testt--t");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 6, 2, rope("--")), "testte--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 7, 1, rope("--")), "testtes--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 0, rope::npos, rope("--")), "--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 1, rope::npos, rope("--")), "t--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 2, rope::npos, rope("--")), "te--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 3, rope::npos, rope("--")), "tes--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 4, rope::npos, rope("--")), "test--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 5, rope::npos, rope("--")), "testt--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 6, rope::npos, rope("--")), "testte--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 7, rope::npos, rope("--")), "testtes--");

    BOOST_CHECK_EQUAL(replace(default_rope, 0, 0, "--", 2), "--");
    BOOST_CHECK_EQUAL(replace(default_rope, rope::npos, 0, "--", 2), "--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 0, 2, "--", 2), "--st");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 1, 2, "--", 2), "t--t");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 2, 2, "--", 2), "te--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 3, 1, "--", 2), "tes--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 4, 0, "--", 2), "test--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 0, rope::npos, "--", 2), "--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 1, rope::npos, "--", 2), "t--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 2, rope::npos, "--", 2), "te--");
    BOOST_CHECK_EQUAL(replace(c_str_rope, 3, rope::npos, "--", 2), "tes--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 0, 2, "--", 2), "--cccccc");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 1, 2, "--", 2), "c--ccccc");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 2, 2, "--", 2), "cc--cccc");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 3, 2, "--", 2), "ccc--ccc");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 4, 2, "--", 2), "cccc--cc");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 5, 2, "--", 2), "ccccc--c");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 6, 2, "--", 2), "cccccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 7, 1, "--", 2), "ccccccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 0, rope::npos, "--", 2), "--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 1, rope::npos, "--", 2), "c--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 2, rope::npos, "--", 2), "cc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 3, rope::npos, "--", 2), "ccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 4, rope::npos, "--", 2), "cccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 5, rope::npos, "--", 2), "ccccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 6, rope::npos, "--", 2), "cccccc--");
    BOOST_CHECK_EQUAL(replace(repeated_chars_rope, 7, rope::npos, "--", 2), "ccccccc--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 0, 2, "--", 2), "--sttest");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 1, 2, "--", 2), "t--ttest");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 2, 2, "--", 2), "te--test");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 3, 2, "--", 2), "tes--est");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 4, 2, "--", 2), "test--st");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 5, 2, "--", 2), "testt--t");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 6, 2, "--", 2), "testte--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 7, 1, "--", 2), "testtes--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 0, rope::npos, "--", 2), "--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 1, rope::npos, "--", 2), "t--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 2, rope::npos, "--", 2), "te--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 3, rope::npos, "--", 2), "tes--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 4, rope::npos, "--", 2), "test--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 5, rope::npos, "--", 2), "testt--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 6, rope::npos, "--", 2), "testte--");
    BOOST_CHECK_EQUAL(replace(concatenated_test_plus_test, 7, rope::npos, "--", 2), "testtes--");
}
