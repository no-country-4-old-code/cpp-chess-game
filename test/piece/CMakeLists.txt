add_subdirectory(pieces)

add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-update.cpp" Pieces)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-api--init-army.cpp" Pieces)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-api--move-piece.cpp" Pieces)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-api--calc-moves.cpp" Pieces)
add_gtest("${CMAKE_CURRENT_SOURCE_DIR}/test-piece-embraced-sqrs-mask.cpp" Pieces)
