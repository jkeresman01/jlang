#include "AST/AstPrinter.h"
#include "CodeGen/CodeGen.h"
#include "Parser/Parser.h"
#include "Preprocessor/PreprocessorFacade.h"
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

int Compile(const std::string &filePath, bool dumpAst, bool emitIR, const std::string &outputPath)
{
    std::string sourceCode = Load(filePath);
    if (sourceCode.empty())
    {
        return 1;
    }

    PreprocessorFacade preprocessor(sourceCode, filePath);
    std::string processed = preprocessor.Process();

    Scanner scanner(processed);
    const std::vector<Token> &tokens = scanner.Tokenize();

    Parser parser(tokens);
    std::vector<std::shared_ptr<AstNode>> program = parser.Parse();

    if (dumpAst)
    {
        AstPrinter printer;
        std::cout << printer.Print(program) << "\n";
        return 0;
    }

    SemanticAnalyzer analyzer;
    analyzer.Analyze(program);

    CodeGenerator codegen;
    codegen.Generate(program);

    if (emitIR)
    {
        codegen.DumpIR();
        return 0;
    }

    if (!codegen.EmitExecutable(outputPath))
    {
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: Jlang [options] <source_file.j>\n"
                  << "Options:\n"
                  << "  --dump-ast    Print the AST and exit\n"
                  << "  --emit-ir     Print LLVM IR to stdout\n"
                  << "  -o <file>     Output executable path (default: a.out)\n";
        return 1;
    }

    bool dumpAst = false;
    bool emitIR = false;
    std::string outputPath = "a.out";
    const char *filePath = nullptr;

    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "--dump-ast") == 0)
        {
            dumpAst = true;
        }
        else if (std::strcmp(argv[i], "--emit-ir") == 0)
        {
            emitIR = true;
        }
        else if (std::strcmp(argv[i], "-o") == 0)
        {
            if (i + 1 < argc)
            {
                outputPath = argv[++i];
            }
            else
            {
                std::cerr << "Error: -o requires an argument\n";
                return 1;
            }
        }
        else
        {
            filePath = argv[i];
        }
    }

    if (!filePath)
    {
        std::cerr << "Error: No source file specified.\n";
        return 1;
    }

    return Compile(filePath, dumpAst, emitIR, outputPath);
}
