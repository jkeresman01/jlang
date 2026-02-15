add_library(jlang-stl STATIC src/vector.c)
target_include_directories(jlang-stl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
