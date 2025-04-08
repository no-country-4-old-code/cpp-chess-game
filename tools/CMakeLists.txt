# Update CMAKE_MODULE_PATH
set(TMP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH "${TMP_DIR};${TMP_DIR}/build-targets;${TMP_DIR}/other;
    ${TMP_DIR}/external_libs" PARENT_SCOPE)
