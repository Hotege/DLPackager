cmake_minimum_required(VERSION 3.12)

file(
        GLOB tests
        test.cpp
)

include_directories(
        ${PROJECT_SOURCE_DIR}/src

        ${FILEKIT_INCLUDE_DIR}
)

link_directories(
        ${FILEKIT_LINK_DIR}
        ${ZLIB_LINK_DIR}
)

link_libraries(
        DLPackager
        DLFileKit
        z
)

add_executable(test ${tests})