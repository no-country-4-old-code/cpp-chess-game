#include <gtest/gtest.h>
#include <notation.h>
#include <squares.h>
#include <sstream> 
#include <tuple>

namespace {
    template <typename DUT, typename TEXT>
    void expect_osstream(DUT obj, TEXT expected) {
        std::ostringstream oss;
        oss << obj;
        auto result = oss.str();
        EXPECT_EQ(result, expected) << "Expected output of '" << expected << "' but got '" << result << "'" ;
    };

}

using TestDataCSqr = std::tuple<int, const char*>;
class TestSuiteConstructorSqrt : public ::testing::TestWithParam<TestDataCSqr> {};
class TestSuiteAsSquares : public ::testing::TestWithParam<int> {};
std::string generate_name_csqr_test(const ::testing::TestParamInfo<TestDataCSqr>& info);
constexpr int number_of_bits_in_sqrs = sizeof(board::bitmap::Squares) * 8;


TEST(ChessNotation, ConstructorChar) {
    board::notation::ChessNotation obj{"a1"};
    expect_osstream(obj, "a1");
}

INSTANTIATE_TEST_SUITE_P(ChessNotation,
    TestSuiteConstructorSqrt,
    ::testing::Values(
        std::make_tuple(0, "a1"),
        std::make_tuple(1, "b1"),
        std::make_tuple(7, "h1"),
        std::make_tuple(8, "a2"),
        std::make_tuple(28, "e4"),
        std::make_tuple(55, "h7"),
        std::make_tuple(56, "a8"),
        std::make_tuple(62, "g8"),
        std::make_tuple(63, "h8")),
    generate_name_csqr_test
    );

TEST_P(TestSuiteConstructorSqrt, ConstructorSqr) {
    auto [bit_index, expected] = GetParam();
    // set up board and create square for testing
    board::Board board{8, 8};
    board::bitmap::Squares sqr = 1ULL << bit_index;
    // create notation for created square
    board::notation::ChessNotation obj{sqr, board};
    // check if notation-string is correct for created square
    expect_osstream(obj, expected);
}


INSTANTIATE_TEST_SUITE_P(ChessNotation,
    TestSuiteAsSquares,
    ::testing::Range(0, number_of_bits_in_sqrs, 7)
);

TEST_P(TestSuiteAsSquares, AsSquares) {
    auto bit_index = GetParam();
    // set up board, square, notation for testing
    board::Board board{8, 8};
    board::bitmap::Squares sqr = 1ULL << bit_index;
    board::notation::ChessNotation obj{sqr, board};
    // map notation back to squares
    auto result = obj.as_squares(board);
    // check if value (and only value) of squares are idenetical
    EXPECT_EQ(result, sqr);
    EXPECT_NE(&result, &sqr);
}

TEST(ChessNotation, ConstructorSqrLongBoard1) {
    board::Board board{4, 9};
    board::bitmap::Squares sqr = 1ULL << 18;
    board::notation::ChessNotation obj{sqr, board};
    expect_osstream(obj, "c5");
}

TEST(ChessNotation, ConstructorSqrLongBoard2) {
    board::Board board{4, 9};
    board::bitmap::Squares sqr = 1ULL << 35;
    board::notation::ChessNotation obj{sqr, board};
    expect_osstream(obj, "d9");
}

TEST(ChessNotation, ConstructorSqrShortBoard1) {
    board::Board board{20, 3};
    board::bitmap::Squares sqr = 1ULL << 18;
    board::notation::ChessNotation obj{sqr, board};
    expect_osstream(obj, "s1");
}

TEST(ChessNotation, ConstructorSqrShortBoard2) {
    board::Board board{20, 3};
    board::bitmap::Squares sqr = 1ULL << 63;
    board::notation::ChessNotation obj{sqr, board};
    expect_osstream(obj, "d4");
}

// helper

std::string generate_name_csqr_test(const ::testing::TestParamInfo<TestDataCSqr>& info) {
    int bit_index = std::get<0>(info.param);
    const char* expected = std::get<1>(info.param);
    return "_" + std::to_string(bit_index) + "_should_result_in_" + expected;
}
