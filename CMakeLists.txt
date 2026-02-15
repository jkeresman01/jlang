cmake_minimum_required(VERSION 3.15)
project(Jlang LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(LLVM REQUIRED CONFIG)
find_package(ZLIB REQUIRED)

find_package(zstd QUIET)
if(NOT TARGET zstd::libzstd_shared)
    add_library(zstd::libzstd_shared SHARED IMPORTED)
    find_file(ZSTD_SO NAMES libzstd.so libzstd.so.1 PATHS /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE} /usr/lib)
    if(ZSTD_SO)
        set_target_properties(zstd::libzstd_shared PROPERTIES IMPORTED_LOCATION "${ZSTD_SO}")
    endif()
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Support
    ExecutionEngine
    Passes
    Target
    MC
    native
)

add_subdirectory(jlang-shared)
add_subdirectory(jlang-preprocessor)
add_subdirectory(jlang-core)
add_subdirectory(jlang-stl)
