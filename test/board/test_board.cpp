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


TEST(ChessNotation, PrintAsString) {
    board::notation::ChessNotation obj{"a1"};
    expect_osstream(obj, "a1");
}

class RangeOfBitIndex : public ::testing::TestWithParam<std::tuple<int, const char*>> {};

std::string renameTestCasesBitIndex(const ::testing::TestParamInfo<std::tuple<int, const char*>>& info) {
    int bit_index = std::get<0>(info.param);
    const char* expected = std::get<1>(info.param);
    return "_" + std::to_string(bit_index) + "_should_result_in_" + expected;
}


TEST_P(RangeOfBitIndex, BitIndex) {
    board::Board board{8, 8};
    auto [bit_index, expected] = GetParam();
    board::bitmap::Squares sqr = 1 << bit_index;
    board::notation::ChessNotation obj{sqr, board};
    expect_osstream(obj, expected);
}

INSTANTIATE_TEST_SUITE_P(ChessNotation,
    RangeOfBitIndex,
    ::testing::Values(
        std::make_tuple(0, "a1"),
        std::make_tuple(1, "b1"),
        std::make_tuple(7, "h1"),
        std::make_tuple(8, "a2"),
        std::make_tuple(32, "e4"),
        std::make_tuple(55, "h7"),
        std::make_tuple(57, "a8"),
        std::make_tuple(62, "g8"),
        std::make_tuple(63, "h8")),
    renameTestCasesBitIndex
    );



TEST(ChessNotation, AsSquares) {
    board::Board board{8, 8};
    board::bitmap::Squares sqr = 1 << 12;
    board::notation::ChessNotation obj{sqr, board};
    
    auto result = obj.as_squares(board);

    EXPECT_EQ(result, sqr);
    EXPECT_NE(&result, &sqr);
}



