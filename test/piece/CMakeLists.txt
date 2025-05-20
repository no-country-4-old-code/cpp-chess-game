add_subdirectory(pieces)

add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-update.cpp" PieceActions)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-api--init-army.cpp" PieceActions)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-api--move-piece.cpp" PieceActions)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-api--calc-moves.cpp" PieceActions)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-embraced-sqrs-mask.cpp" PieceActions)

