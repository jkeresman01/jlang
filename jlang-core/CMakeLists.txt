add_library(jlang-core-lib STATIC
    src/AST/AstPrinter.cpp
    src/Scanner/Scanner.cpp
    src/Parser/Parser.cpp
    src/CodeGen/CodeGen.cpp
    src/CodeGen/CodeGenTypes.cpp
    src/CodeGen/Expr/CodeGenBinaryExpr.cpp
    src/CodeGen/Expr/CodeGenUnaryExpr.cpp
    src/CodeGen/Expr/CodeGenResultExpr.cpp
    src/CodeGen/Expr/CodeGenArrayExpr.cpp
    src/CodeGen/Expr/CodeGenAccessExpr.cpp
    src/CodeGen/Expr/CodeGenLiteralExpr.cpp
    src/CodeGen/Stmt/CodeGenStmt.cpp
    src/CodeGen/Decl/CodeGenDecl.cpp
    src/SemanticAnalyzer/SemanticAnalyzer.cpp
)

target_include_directories(jlang-core-lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LLVM_INCLUDE_DIRS}
)

target_link_directories(jlang-core-lib PUBLIC ${LLVM_LIBRARY_DIRS})
target_compile_definitions(jlang-core-lib PUBLIC ${LLVM_DEFINITIONS})

target_link_libraries(jlang-core-lib PUBLIC
    jlang-preprocessor
    ${LLVM_LIBS}
    ZLIB::ZLIB
    zstd::libzstd_shared
)

add_executable(Jlang src/Main.cpp)
target_link_libraries(Jlang PRIVATE jlang-core-lib)

find_package(GTest QUIET)
if(GTest_FOUND)
    enable_testing()

    add_executable(jlang-core-tests
        test/Scanner/ScannerTests.cpp
    )

    target_link_libraries(jlang-core-tests PRIVATE
        jlang-core-lib
        GTest::GTest
        GTest::Main
        pthread
    )

    add_test(NAME JlangCoreTests COMMAND jlang-core-tests)
endif()
