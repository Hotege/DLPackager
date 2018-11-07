cmake_minimum_required(VERSION 3.12)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(PROJECT_POSTFIX "-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(INSTALL_DIR "${PROJECT_SOURCE_DIR}/lib${PROJECT_NAME}${PROJECT_POSTFIX}")

file(
        GLOB srcs
        DLPackager.cpp
)

include_directories(
        ${ZLIB_INCLUDE_DIR}
)

link_directories(
        ${ZLIB_LINK_DIR}
)

link_libraries(
        z
)

add_library(DLPackager ${srcs})

if (${CMAKE_BUILD_TYPE} MATCHES "Release")
    install(FILES DLPackager.h DESTINATION "${INSTALL_DIR}/include")
    install(
            TARGETS DLPackager
            ARCHIVE DESTINATION "${INSTALL_DIR}/lib"
            LIBRARY DESTINATION "${INSTALL_DIR}/lib"
    )
endif ()