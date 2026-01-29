#include "AST/AstPrinter.h"
#include "CodeGen/CodeGen.h"
#include "Parser/Parser.h"
#include "Preprocessor/Preprocessor.h"
#include "Scanner/Scanner.h"
#include "SemanticAnalyzer/SemanticAnalyzer.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace jlang;

std::string Load(const std::string &path)
{
    std::ifstream in(path);

    if (!in.is_open())
    {
        std::cerr << "Error: Cannot open file: " << path << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << in.rdbuf();

    in.close();

    return buffer.str();
}

void Compile(const std::string &filePath, bool dumpAst)
{
    std::string sourceCode = Load(filePath);
    if (sourceCode.empty())
    {
        return;
    }

    Preprocessor preprocessor(sourceCode, filePath);
    std::string processed = preprocessor.Process();

    Scanner scanner(processed);
    const std::vector<Token> &tokens = scanner.Tokenize();

    Parser parser(tokens);
    std::vector<std::shared_ptr<AstNode>> program = parser.Parse();

    if (dumpAst)
    {
        AstPrinter printer;
        std::cout << printer.Print(program) << "\n";
        return;
    }

    SemanticAnalyzer analyzer;
    analyzer.Analyze(program);

    CodeGenerator codegen;
    codegen.Generate(program);
    codegen.DumpIR();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: Jlang [--dump-ast] <source_file.j>\n";
        return 1;
    }

    bool dumpAst = false;
    const char *filePath = nullptr;

    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "--dump-ast") == 0)
            dumpAst = true;
        else
            filePath = argv[i];
    }

    if (!filePath)
    {
        std::cerr << "Error: No source file specified.\n";
        return 1;
    }

    Compile(filePath, dumpAst);
    return 0;
}
