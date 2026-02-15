add_library(jlang-preprocessor STATIC
    src/PreprocessorFacade.cpp
    src/DirectiveParser.cpp
    src/MacroTable.cpp
    src/ConditionalStack.cpp
    src/IncludeResolver.cpp
)

target_include_directories(jlang-preprocessor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(jlang-preprocessor PUBLIC jlang-shared)
