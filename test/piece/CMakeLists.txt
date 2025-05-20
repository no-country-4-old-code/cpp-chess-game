add_subdirectory(pieces)

add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-api--init-army.cpp" PieceActions TestHelper)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-api--move-piece.cpp" PieceActions TestHelper)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-api--calc-moves.cpp" PieceActions TestHelper)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-embraced-sqrs-mask.cpp" PieceActions TestHelper)

