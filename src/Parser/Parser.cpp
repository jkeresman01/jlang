#include "Parser.h"

#include "../Common/Logger.h"

#include <cctype>
#include <unordered_set>

namespace jlang
{

Parser::Parser(const std::vector<Token> &tokens) : m_Tokens(tokens), m_CurrentPosition(0) {}

std::vector<std::shared_ptr<AstNode>> Parser::Parse()
{
    std::vector<std::shared_ptr<AstNode>> program;

    while (!IsEndReached())
    {
        auto declaration = ParseDeclaration();

        if (declaration)
        {
            program.push_back(declaration);
        }
    }

    return program;
}

bool Parser::IsMatched(TokenType type)
{
    if (Check(type))
    {
        Advance();
        return true;
    }

    return false;
}

bool Parser::Check(TokenType type) const
{
    if (IsEndReached())
    {
        return false;
    }

    return Peek().m_type == type;
}

const Token &Parser::Advance()
{
    if (!IsEndReached())
    {
        m_CurrentPosition++;
    }

    return Previous();
}

const Token &Parser::Peek() const
{
    return m_Tokens[m_CurrentPosition];
}

const Token &Parser::Previous() const
{
    return m_Tokens[m_CurrentPosition - 1];
}

bool Parser::IsEndReached() const
{
    return Peek().m_type == TokenType::EndOfFile;
}

std::shared_ptr<AstNode> Parser::ParseDeclaration()
{
    if (Check(TokenType::Interface))
    {
        return ParseInterface();
    }

    if (Check(TokenType::Struct))
    {
        return ParseStruct();
    }

    if (Check(TokenType::Fn))
    {
        return ParseFunction();
    }

    Advance();

    return nullptr;
}

std::shared_ptr<AstNode> Parser::ParseInterface()
{
    Advance();

    if (!IsMatched(TokenType::Identifier))
    {
        JLANG_ERROR("Expected interface name");
    }

    const std::string &name = Previous().m_lexeme;

    if (!IsMatched(TokenType::LBrace))
    {
        JLANG_ERROR("Expected '{' after interface name!");
    }

    auto interfaceDeclNode = std::make_shared<InterfaceDecl>();
    interfaceDeclNode->name = name;

    while (!Check(TokenType::RBrace) && !IsEndReached())
    {
        if (!IsMatched(TokenType::Fn))
        {
            JLANG_ERROR("Expected 'fn' in interface method");
            Advance(); // error recovery
            continue;
        }

        if (!IsMatched(TokenType::Identifier))
        {
            JLANG_ERROR("Expected method name");
            Advance(); // error recovery
            continue;
        }

        std::string methodName = Previous().m_lexeme;

        if (!IsMatched(TokenType::LParen) || !IsMatched(TokenType::RParen) ||
            !IsMatched(TokenType::Semicolon))
        {
            JLANG_ERROR("Expected '()' and ';' after method name");
            // Skip to semicolon for error recovery
            while (!IsEndReached() && !Check(TokenType::Semicolon) && !Check(TokenType::RBrace))
            {
                Advance();
            }
            IsMatched(TokenType::Semicolon);
            continue;
        }

        interfaceDeclNode->methods.push_back(methodName);
    }

    if (!IsMatched(TokenType::RBrace))
    {
        JLANG_ERROR("Expected '}' at end of interface");
    }

    return interfaceDeclNode;
}

std::shared_ptr<AstNode> Parser::ParseStruct()
{
    Advance(); // consume 'struct'

    if (!IsMatched(TokenType::Identifier))
    {
        JLANG_ERROR("Expected struct name");
    }

    const std::string name = Previous().m_lexeme;

    std::string implementedInterface;

    // Use colon for interface implementation: struct Name : Interface
    if (IsMatched(TokenType::Colon))
    {
        if (!IsMatched(TokenType::Identifier))
        {
            JLANG_ERROR("Expected interface name after ':'");
        }

        implementedInterface = Previous().m_lexeme;
    }

    if (!IsMatched(TokenType::LBrace))
    {
        JLANG_ERROR("Expected '{' after struct declaration");
    }

    auto structDeclNode = std::make_shared<StructDecl>();
    structDeclNode->name = name;
    structDeclNode->interfaceImplemented = implementedInterface;

    // Parse fields: fieldName: Type;
    while (!Check(TokenType::RBrace) && !IsEndReached())
    {
        if (!IsMatched(TokenType::Identifier))
        {
            JLANG_ERROR("Expected field name");
            // Skip to next semicolon or brace
            while (!IsEndReached() && !Check(TokenType::Semicolon) && !Check(TokenType::RBrace))
            {
                Advance();
            }
            IsMatched(TokenType::Semicolon);
            continue;
        }

        std::string fieldName = Previous().m_lexeme;

        // Expect colon after field name
        if (!IsMatched(TokenType::Colon))
        {
            JLANG_ERROR("Expected ':' after field name");
            while (!IsEndReached() && !Check(TokenType::Semicolon) && !Check(TokenType::RBrace))
            {
                Advance();
            }
            IsMatched(TokenType::Semicolon);
            continue;
        }

        // Parse field type
        std::string typeName = ParseTypeName();
        if (typeName.empty())
        {
            JLANG_ERROR("Expected field type");
            while (!IsEndReached() && !Check(TokenType::Semicolon) && !Check(TokenType::RBrace))
            {
                Advance();
            }
            IsMatched(TokenType::Semicolon);
            continue;
        }

        bool isPointer = IsMatched(TokenType::Star);

        bool isNullable = false;
        if (isPointer && IsMatched(TokenType::Question))
        {
            isNullable = true;
        }
        else if (!isPointer && Check(TokenType::Question))
        {
            JLANG_ERROR("Only pointer types can be nullable. Use '" + typeName + "*?' instead of '" +
                        typeName + "?'");
            Advance();
        }

        if (!IsMatched(TokenType::Semicolon))
        {
            JLANG_ERROR("Expected ';' after struct field");
        }

        bool isPublic = !fieldName.empty() && std::isupper(static_cast<unsigned char>(fieldName[0]));
        StructField field{fieldName, TypeRef{typeName, isPointer, isNullable}, isPublic};
        structDeclNode->fields.push_back(field);
    }

    if (!IsMatched(TokenType::RBrace))
    {
        JLANG_ERROR("Expected '}' after struct body");
    }

    return structDeclNode;
}

std::shared_ptr<AstNode> Parser::ParseFunction()
{
    Advance(); // consume 'fn'

    if (!IsMatched(TokenType::Identifier))
    {
        JLANG_ERROR("Expected function name!");
    }

    const std::string &functionName = Previous().m_lexeme;

    // Parse parameter list: (name: Type, name: Type, ...)
    if (!IsMatched(TokenType::LParen))
    {
        JLANG_ERROR("Expected '(' after function name");
    }

    std::vector<Parameter> params;

    if (!Check(TokenType::RParen))
    {
        do
        {
            if (!IsMatched(TokenType::Identifier))
            {
                JLANG_ERROR("Expected parameter name");
                break;
            }
            std::string paramName = Previous().m_lexeme;

            if (!IsMatched(TokenType::Colon))
            {
                JLANG_ERROR("Expected ':' after parameter name");
                break;
            }

            TypeRef paramType = ParseTypeWithParameters();
            if (paramType.name.empty())
            {
                JLANG_ERROR("Expected parameter type");
                break;
            }

            bool isPointer = IsMatched(TokenType::Star);
            paramType.isPointer = isPointer;

            bool isNullable = false;
            if (isPointer && IsMatched(TokenType::Question))
            {
                isNullable = true;
                paramType.isNullable = true;
            }
            else if (!isPointer && Check(TokenType::Question))
            {
                JLANG_ERROR("Only pointer types can be nullable. Use '" + paramType.name +
                            "*?' instead of '" + paramType.name + "?'");
                Advance();
            }

            params.push_back(Parameter{paramName, paramType});

        } while (IsMatched(TokenType::Comma));
    }

    if (!IsMatched(TokenType::RParen))
    {
        JLANG_ERROR("Expected ')' after parameters");
    }

    // Parse return type (optional, after ->), default to void
    TypeRef returnType{"void", false, false};

    if (IsMatched(TokenType::Arrow))
    {
        returnType = ParseTypeWithParameters();
        if (returnType.name.empty())
        {
            JLANG_ERROR("Expected return type after '->'");
        }

        bool isPointer = IsMatched(TokenType::Star);
        returnType.isPointer = isPointer;

        bool isNullable = false;
        if (isPointer && IsMatched(TokenType::Question))
        {
            isNullable = true;
            returnType.isNullable = true;
        }
        else if (!isPointer && Check(TokenType::Question))
        {
            JLANG_ERROR("Only pointer types can be nullable. Use '" + returnType.name + "*?' instead of '" +
                        returnType.name + "?'");
            Advance();
        }
    }

    m_CurrentFunctionReturnType = returnType;
    auto body = ParseBlock();
    m_CurrentFunctionReturnType = TypeRef{};

    auto functionDeclNode = std::make_shared<FunctionDecl>();
    functionDeclNode->name = functionName;
    functionDeclNode->params = params;
    functionDeclNode->returnType = returnType;
    functionDeclNode->body = body;

    return functionDeclNode;
}

std::shared_ptr<AstNode> Parser::ParseBlock()
{
    if (!IsMatched(TokenType::LBrace))
    {
        JLANG_ERROR("Expected '{' at the beginning of the block");
    }

    auto blockStmt = std::make_shared<BlockStatement>();

    while (!Check(TokenType::RBrace) && !IsEndReached())
    {
        auto statement = ParseStatement();

        if (statement)
        {
            blockStmt->statements.push_back(statement);
        }
    }

    if (!IsMatched(TokenType::RBrace))
    {
        JLANG_ERROR("Expected '}' after block");
    }

    return blockStmt;
}

std::shared_ptr<AstNode> Parser::ParseStatement()
{
    if (Check(TokenType::If))
    {
        return ParseIfStatement();
    }

    if (Check(TokenType::While))
    {
        return ParseWhileStatement();
    }

    if (Check(TokenType::For))
    {
        return ParseForStatement();
    }

    if (Check(TokenType::Var) || Check(TokenType::Val))
    {
        return ParseVarDecl();
    }

    if (Check(TokenType::Return))
    {
        return ParseReturnStatement();
    }

    if (Check(TokenType::LBrace))
    {
        return ParseBlock();
    }

    return ParseExprStatement();
}

std::shared_ptr<AstNode> Parser::ParseReturnStatement()
{
    Advance(); // consume 'return'

    std::shared_ptr<AstNode> value = nullptr;

    // Check if there's a return value (not just "return;")
    if (!Check(TokenType::Semicolon))
    {
        value = ParseExpression();

        // Propagate Result type from function return type to Ok/Err expressions
        if (m_CurrentFunctionReturnType.isResult())
        {
            if (auto okExpr = std::dynamic_pointer_cast<OkExpr>(value))
            {
                okExpr->resultType = m_CurrentFunctionReturnType;
            }
            else if (auto errExpr = std::dynamic_pointer_cast<ErrExpr>(value))
            {
                errExpr->resultType = m_CurrentFunctionReturnType;
            }
        }
    }

    if (!IsMatched(TokenType::Semicolon))
    {
        JLANG_ERROR("Expected ';' after return statement");
    }

    auto returnStmt = std::make_shared<ReturnStatement>();
    returnStmt->value = value;

    return returnStmt;
}

std::shared_ptr<AstNode> Parser::ParseVarDecl()
{
    bool isMutable = Check(TokenType::Var);
    Advance(); // consume 'var' or 'val'

    if (!IsMatched(TokenType::Identifier))
    {
        JLANG_ERROR("Expected variable name");
        Advance(); // error recovery
        return nullptr;
    }

    std::string varName = Previous().m_lexeme;

    std::string typeName;
    bool isPointer = false;
    bool isNullable = false;
    std::shared_ptr<AstNode> initializer = nullptr;

    // Check for type inference syntax: var x := expr;
    if (IsMatched(TokenType::ColonEqual))
    {
        // Type inference - parse initializer, type will be inferred in CodeGen
        initializer = ParseExpression();
        if (!initializer)
        {
            JLANG_ERROR("Expected initializer expression after ':='");
            return nullptr;
        }
    }
    else
    {
        // Explicit type syntax: var x: Type = expr;
        if (!IsMatched(TokenType::Colon))
        {
            JLANG_ERROR("Expected ':' or ':=' after variable name");
            return nullptr;
        }

        // Parse type with potential type parameters (e.g., Result<i32, char*>)
        TypeRef parsedType = ParseTypeWithParameters();
        if (parsedType.name.empty())
        {
            JLANG_ERROR("Expected variable type");
            return nullptr;
        }

        typeName = parsedType.name;

        isPointer = IsMatched(TokenType::Star);
        parsedType.isPointer = isPointer;

        if (isPointer && IsMatched(TokenType::Question))
        {
            isNullable = true;
            parsedType.isNullable = true;
        }
        else if (!isPointer && Check(TokenType::Question))
        {
            JLANG_ERROR("Only pointer types can be nullable. Use '" + typeName + "*?' instead of '" +
                        typeName + "?'");
            Advance();
        }

        if (IsMatched(TokenType::Equal))
        {
            initializer = ParseExpression();

            // For Result types, propagate type info to Ok/Err expressions
            if (parsedType.isResult())
            {
                if (auto okExpr = std::dynamic_pointer_cast<OkExpr>(initializer))
                {
                    okExpr->resultType = parsedType;
                }
                else if (auto errExpr = std::dynamic_pointer_cast<ErrExpr>(initializer))
                {
                    errExpr->resultType = parsedType;
                }
            }
        }

        // Store the full type including parameters
        auto varDecl = std::make_shared<VariableDecl>();
        varDecl->name = varName;
        varDecl->varType = parsedType;
        varDecl->initializer = initializer;
        varDecl->isMutable = isMutable;

        if (!IsMatched(TokenType::Semicolon))
        {
            JLANG_ERROR("Expected ';' after variable declaration");
        }

        return varDecl;
    }

    if (!IsMatched(TokenType::Semicolon))
    {
        JLANG_ERROR("Expected ';' after variable declaration");
    }

    auto varDecl = std::make_shared<VariableDecl>();
    varDecl->name = varName;
    varDecl->varType = TypeRef{typeName, isPointer, isNullable};
    varDecl->initializer = initializer;
    varDecl->isMutable = isMutable;

    return varDecl;
}

std::shared_ptr<AstNode> Parser::ParseIfStatement()
{
    Advance();

    if (!IsMatched(TokenType::LParen))
    {
        JLANG_ERROR("Expected '(' after 'if'");
    }

    auto condition = ParseExpression();

    if (!IsMatched(TokenType::RParen))
    {
        JLANG_ERROR("Expected ')' after condition");
    }

    auto thenBranch = ParseStatement();

    std::shared_ptr<AstNode> elseBranch = nullptr;
    if (IsMatched(TokenType::Else))
    {
        elseBranch = ParseStatement();
    }

    auto node = std::make_shared<IfStatement>();

    node->condition = condition;
    node->thenBranch = thenBranch;
    node->elseBranch = elseBranch;

    return node;
}

std::shared_ptr<AstNode> Parser::ParseWhileStatement()
{
    Advance(); // consume 'while'

    if (!IsMatched(TokenType::LParen))
    {
        JLANG_ERROR("Expected '(' after 'while'");
    }

    auto condition = ParseExpression();

    if (!IsMatched(TokenType::RParen))
    {
        JLANG_ERROR("Expected ')' after condition");
    }

    auto body = ParseStatement();

    auto node = std::make_shared<WhileStatement>();
    node->condition = condition;
    node->body = body;

    return node;
}

std::shared_ptr<AstNode> Parser::ParseForStatement()
{
    Advance(); // consume 'for'

    if (!IsMatched(TokenType::LParen))
    {
        JLANG_ERROR("Expected '(' after 'for'");
    }

    // Parse initializer (var decl or expression statement, or empty)
    std::shared_ptr<AstNode> init = nullptr;
    if (Check(TokenType::Semicolon))
    {
        Advance(); // empty initializer
    }
    else if (Check(TokenType::Var) || Check(TokenType::Val))
    {
        init = ParseVarDecl(); // already consumes semicolon
    }
    else
    {
        init = ParseExprStatement(); // already consumes semicolon
    }

    // Parse condition (or empty for infinite loop)
    std::shared_ptr<AstNode> condition = nullptr;
    if (!Check(TokenType::Semicolon))
    {
        condition = ParseExpression();
    }

    if (!IsMatched(TokenType::Semicolon))
    {
        JLANG_ERROR("Expected ';' after for loop condition");
    }

    // Parse update expression (or empty)
    std::shared_ptr<AstNode> update = nullptr;
    if (!Check(TokenType::RParen))
    {
        update = ParseExpression();
    }

    if (!IsMatched(TokenType::RParen))
    {
        JLANG_ERROR("Expected ')' after for clauses");
    }

    auto body = ParseStatement();

    auto node = std::make_shared<ForStatement>();
    node->init = init;
    node->condition = condition;
    node->update = update;
    node->body = body;

    return node;
}

std::shared_ptr<AstNode> Parser::ParseExpression()
{
    auto expr = ParseElvis();

    // Handle assignment: identifier = expression
    if (IsMatched(TokenType::Equal))
    {
        auto value = ParseExpression();

        // Check if left side is a variable
        if (auto varExpr = std::dynamic_pointer_cast<VarExpr>(expr))
        {
            auto assign = std::make_shared<AssignExpr>();
            assign->name = varExpr->name;
            assign->value = value;
            return assign;
        }
        else
        {
            JLANG_ERROR("Invalid assignment target");
        }
    }

    // Handle compound assignment: x += expr  =>  x = x + expr
    std::string compoundOp;
    if (IsMatched(TokenType::PlusEqual))
    {
        compoundOp = "+";
    }
    else if (IsMatched(TokenType::MinusEqual))
    {
        compoundOp = "-";
    }
    else if (IsMatched(TokenType::StarEqual))
    {
        compoundOp = "*";
    }
    else if (IsMatched(TokenType::SlashEqual))
    {
        compoundOp = "/";
    }
    else if (IsMatched(TokenType::PercentEqual))
    {
        compoundOp = "%";
    }
    else if (IsMatched(TokenType::AmpersandEqual))
    {
        compoundOp = "&";
    }
    else if (IsMatched(TokenType::PipeEqual))
    {
        compoundOp = "|";
    }
    else if (IsMatched(TokenType::CaretEqual))
    {
        compoundOp = "^";
    }
    else if (IsMatched(TokenType::LeftShiftEqual))
    {
        compoundOp = "<<";
    }
    else if (IsMatched(TokenType::RightShiftEqual))
    {
        compoundOp = ">>";
    }

    if (!compoundOp.empty())
    {
        auto rhs = ParseExpression();

        if (auto varExpr = std::dynamic_pointer_cast<VarExpr>(expr))
        {
            auto binary = std::make_shared<BinaryExpr>();
            binary->op = compoundOp;
            binary->left = expr;
            binary->right = rhs;

            auto assign = std::make_shared<AssignExpr>();
            assign->name = varExpr->name;
            assign->value = binary;
            return assign;
        }
        else
        {
            JLANG_ERROR("Invalid compound assignment target");
        }
    }

    return expr;
}

std::shared_ptr<AstNode> Parser::ParseElvis()
{
    auto left = ParseLogicalOr();

    while (Check(TokenType::QuestionColon))
    {
        Advance();
        auto right = ParseLogicalOr();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = "?:";
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseLogicalOr()
{
    auto left = ParseLogicalAnd();

    while (Check(TokenType::Or) || Check(TokenType::OrKeyword))
    {
        std::string op = Peek().m_lexeme;
        Advance();
        auto right = ParseLogicalAnd();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = op;
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseLogicalAnd()
{
    auto left = ParseBitwiseOr();

    while (Check(TokenType::And) || Check(TokenType::AndKeyword))
    {
        std::string op = Peek().m_lexeme;
        Advance();
        auto right = ParseBitwiseOr();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = op;
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseBitwiseOr()
{
    auto left = ParseBitwiseXor();

    while (Check(TokenType::Pipe))
    {
        Advance();
        auto right = ParseBitwiseXor();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = "|";
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseBitwiseXor()
{
    auto left = ParseBitwiseAnd();

    while (Check(TokenType::Caret))
    {
        Advance();
        auto right = ParseBitwiseAnd();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = "^";
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseBitwiseAnd()
{
    auto left = ParseEquality();

    while (Check(TokenType::Ampersand))
    {
        Advance();
        auto right = ParseEquality();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = "&";
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseEquality()
{
    auto left = ParseComparison();

    while (Check(TokenType::EqualEqual) || Check(TokenType::NotEqual))
    {
        std::string op = Peek().m_lexeme;
        Advance();
        auto right = ParseComparison();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = op;
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseComparison()
{
    auto left = ParseShift();

    while (Check(TokenType::Less) || Check(TokenType::LessEqual) || Check(TokenType::Greater) ||
           Check(TokenType::GreaterEqual))
    {
        std::string op = Peek().m_lexeme;
        Advance();
        auto right = ParseShift();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = op;
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseShift()
{
    auto left = ParseAdditive();

    while (Check(TokenType::LeftShift) || Check(TokenType::RightShift))
    {
        std::string op = Peek().m_lexeme;
        Advance();
        auto right = ParseAdditive();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = op;
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseAdditive()
{
    auto left = ParseMultiplicative();

    while (Check(TokenType::Plus) || Check(TokenType::Minus))
    {
        std::string op = Peek().m_lexeme;
        Advance();
        auto right = ParseMultiplicative();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = op;
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseMultiplicative()
{
    auto left = ParseUnary();

    while (Check(TokenType::Star) || Check(TokenType::Slash) || Check(TokenType::Percent))
    {
        std::string op = Peek().m_lexeme;
        Advance();
        auto right = ParseUnary();

        auto binary = std::make_shared<BinaryExpr>();
        binary->op = op;
        binary->left = left;
        binary->right = right;
        left = binary;
    }

    return left;
}

std::shared_ptr<AstNode> Parser::ParseUnary()
{
    if (Check(TokenType::Not) || Check(TokenType::Tilde) || Check(TokenType::Minus))
    {
        std::string op = Peek().m_lexeme;
        Advance();
        auto operand = ParseUnary();

        auto unary = std::make_shared<UnaryExpr>();
        unary->op = op;
        unary->operand = operand;
        return unary;
    }

    // Handle prefix increment/decrement
    if (Check(TokenType::PlusPlus) || Check(TokenType::MinusMinus))
    {
        std::string op = Peek().m_lexeme;
        Advance();
        auto operand = ParseUnary();

        auto prefix = std::make_shared<PrefixExpr>();
        prefix->op = op;
        prefix->operand = operand;
        return prefix;
    }

    return ParsePostfix();
}

std::shared_ptr<AstNode> Parser::ParsePostfix()
{
    auto expr = ParsePrimary();

    // Handle postfix increment/decrement
    while (Check(TokenType::PlusPlus) || Check(TokenType::MinusMinus))
    {
        std::string op = Peek().m_lexeme;
        Advance();

        auto postfix = std::make_shared<PostfixExpr>();
        postfix->op = op;
        postfix->operand = expr;
        expr = postfix;
    }

    return expr;
}

std::shared_ptr<AstNode> Parser::ParseExprStatement()
{
    auto expression = ParseExpression();

    if (!expression)
    {
        // Skip until we find a semicolon or closing brace for error recovery
        while (!IsEndReached() && !Check(TokenType::Semicolon) && !Check(TokenType::RBrace))
        {
            Advance();
        }
        IsMatched(TokenType::Semicolon);
        return nullptr;
    }

    if (!IsMatched(TokenType::Semicolon))
    {
        JLANG_ERROR("Expected ';' after expression");
    }

    auto stmt = std::make_shared<ExprStatement>();
    stmt->expression = expression;

    return stmt;
}

std::shared_ptr<AstNode> Parser::ParsePrimary()
{
    // Handle null literal (keyword)
    if (IsMatched(TokenType::Null))
    {
        auto literal = std::make_shared<LiteralExpr>();
        literal->value = "null";
        return literal;
    }

    // Handle true literal
    if (IsMatched(TokenType::True))
    {
        auto literal = std::make_shared<LiteralExpr>();
        literal->value = "true";
        return literal;
    }

    // Handle false literal
    if (IsMatched(TokenType::False))
    {
        auto literal = std::make_shared<LiteralExpr>();
        literal->value = "false";
        return literal;
    }

    // Handle match expression
    if (IsMatched(TokenType::Match))
    {
        return ParseMatchExpr();
    }

    // Handle Ok() expression
    if (IsMatched(TokenType::Ok))
    {
        return ParseOkExpr();
    }

    // Handle Err() expression
    if (IsMatched(TokenType::Err))
    {
        return ParseErrExpr();
    }

    // Handle alloc<Type>() expression
    if (IsMatched(TokenType::Alloc))
    {
        if (!IsMatched(TokenType::Less))
        {
            JLANG_ERROR("Expected '<' after alloc");
            return nullptr;
        }

        if (!IsMatched(TokenType::Identifier))
        {
            JLANG_ERROR("Expected type name in alloc<Type>");
            return nullptr;
        }

        std::string typeName = Previous().m_lexeme;

        if (!IsMatched(TokenType::Greater))
        {
            JLANG_ERROR("Expected '>' after type in alloc<Type>");
            return nullptr;
        }

        if (!IsMatched(TokenType::LParen) || !IsMatched(TokenType::RParen))
        {
            JLANG_ERROR("Expected '()' after alloc<Type>");
            return nullptr;
        }

        auto allocExpr = std::make_shared<AllocExpr>();
        allocExpr->allocType = TypeRef{typeName, true}; // always returns a pointer
        return allocExpr;
    }

    // Handle cast expressions: (struct Type*) expr or (Type*) expr
    if (IsMatched(TokenType::LParen))
    {
        // Check if this is a cast or a grouped expression
        if (Check(TokenType::Struct) || Check(TokenType::Identifier))
        {
            // This is likely a cast expression
            IsMatched(TokenType::Struct);

            if (!IsMatched(TokenType::Identifier))
            {
                JLANG_ERROR("Expected type name in cast");
                return nullptr;
            }

            std::string typeName = Previous().m_lexeme;
            bool isPointer = IsMatched(TokenType::Star);

            bool isNullable = false;
            if (isPointer && IsMatched(TokenType::Question))
            {
                isNullable = true;
            }
            else if (!isPointer && Check(TokenType::Question))
            {
                JLANG_ERROR("Only pointer types can be nullable. Use '" + typeName + "*?' instead of '" +
                            typeName + "?'");
                Advance();
            }

            if (!IsMatched(TokenType::RParen))
            {
                JLANG_ERROR("Expected ')' after cast type");
                return nullptr;
            }

            auto expr = ParsePrimary();

            auto cast = std::make_shared<CastExpr>();
            cast->targetType = TypeRef{typeName, isPointer, isNullable};
            cast->expr = expr;
            return cast;
        }
        else
        {
            // Grouped expression
            auto expr = ParseExpression();
            if (!IsMatched(TokenType::RParen))
            {
                JLANG_ERROR("Expected ')' after grouped expression");
            }
            return expr;
        }
    }

    // Handle identifiers, function calls, and member access
    if (IsMatched(TokenType::Identifier))
    {
        std::string name = Previous().m_lexeme;

        // Check for function call first (before member access)
        if (IsMatched(TokenType::LParen))
        {
            auto call = std::make_shared<CallExpr>();
            call->callee = name;

            if (!Check(TokenType::RParen))
            {
                do
                {
                    auto arg = ParseExpression();
                    call->arguments.push_back(arg);
                } while (IsMatched(TokenType::Comma));
            }

            if (!IsMatched(TokenType::RParen))
            {
                JLANG_ERROR("Expected ')' after arguments");
            }

            return call;
        }

        // Start with a variable expression
        std::shared_ptr<AstNode> expr = std::make_shared<VarExpr>();
        std::static_pointer_cast<VarExpr>(expr)->name = name;

        // Handle member access chain: obj.field1.field2
        while (IsMatched(TokenType::Dot))
        {
            if (!IsMatched(TokenType::Identifier))
            {
                JLANG_ERROR("Expected member name after '.'");
                break;
            }
            std::string memberName = Previous().m_lexeme;

            auto memberAccess = std::make_shared<MemberAccessExpr>();
            memberAccess->object = expr;
            memberAccess->memberName = memberName;
            expr = memberAccess;
        }

        return expr;
    }

    // Handle string literals
    if (IsMatched(TokenType::StringLiteral))
    {
        auto expression = std::make_shared<LiteralExpr>();
        expression->value = "\"" + Previous().m_lexeme + "\"";
        return expression;
    }

    // Handle number literals
    if (IsMatched(TokenType::NumberLiteral))
    {
        auto expression = std::make_shared<LiteralExpr>();
        expression->value = Previous().m_lexeme;
        return expression;
    }

    // Handle float literals
    if (IsMatched(TokenType::FloatLiteral))
    {
        auto expression = std::make_shared<LiteralExpr>();
        expression->value = Previous().m_lexeme;
        return expression;
    }

    // Handle character literals
    if (IsMatched(TokenType::CharLiteral))
    {
        auto expression = std::make_shared<LiteralExpr>();
        // Wrap in single quotes to distinguish from other literals in codegen
        expression->value = "'" + Previous().m_lexeme + "'";
        return expression;
    }

    JLANG_ERROR("Expected expression");
    return nullptr;
}

bool Parser::IsTypeKeyword() const
{
    static const std::unordered_set<TokenType> typeKeywords = {
        TokenType::Void, TokenType::I8,   TokenType::I16, TokenType::I32, TokenType::I64,
        TokenType::U8,   TokenType::U16,  TokenType::U32, TokenType::U64, TokenType::F32,
        TokenType::F64,  TokenType::Bool, TokenType::Char};
    return typeKeywords.count(Peek().m_type) > 0;
}

std::string Parser::ParseTypeName()
{
    if (IsTypeKeyword())
    {
        std::string name = Peek().m_lexeme;
        Advance();
        return name;
    }
    else if (IsMatched(TokenType::Identifier))
    {
        return Previous().m_lexeme;
    }

    return "";
}

TypeRef Parser::ParseTypeWithParameters()
{
    std::string typeName = ParseTypeName();
    if (typeName.empty())
    {
        return TypeRef{};
    }

    TypeRef typeRef;
    typeRef.name = typeName;

    // Check for generic type parameters: Type<T, E>
    if (IsMatched(TokenType::Less))
    {
        do
        {
            TypeRef paramType = ParseTypeWithParameters();
            if (paramType.name.empty())
            {
                JLANG_ERROR("Expected type parameter");
                break;
            }

            // Check for pointer in type parameter
            if (IsMatched(TokenType::Star))
            {
                paramType.isPointer = true;
            }

            typeRef.typeParameters.push_back(paramType);
        } while (IsMatched(TokenType::Comma));

        if (!IsMatched(TokenType::Greater))
        {
            JLANG_ERROR("Expected '>' after type parameters");
        }
    }

    return typeRef;
}

std::shared_ptr<AstNode> Parser::ParseOkExpr()
{
    // Already consumed 'Ok'
    if (!IsMatched(TokenType::LParen))
    {
        JLANG_ERROR("Expected '(' after 'Ok'");
        return nullptr;
    }

    auto okExpr = std::make_shared<OkExpr>();
    okExpr->value = ParseExpression();

    if (!IsMatched(TokenType::RParen))
    {
        JLANG_ERROR("Expected ')' after Ok value");
        return nullptr;
    }

    return okExpr;
}

std::shared_ptr<AstNode> Parser::ParseErrExpr()
{
    // Already consumed 'Err'
    if (!IsMatched(TokenType::LParen))
    {
        JLANG_ERROR("Expected '(' after 'Err'");
        return nullptr;
    }

    auto errExpr = std::make_shared<ErrExpr>();
    errExpr->error = ParseExpression();

    if (!IsMatched(TokenType::RParen))
    {
        JLANG_ERROR("Expected ')' after Err value");
        return nullptr;
    }

    return errExpr;
}

MatchArm Parser::ParseMatchArm()
{
    MatchArm arm;

    // Expect Ok or Err
    if (IsMatched(TokenType::Ok))
    {
        arm.pattern = "Ok";
    }
    else if (IsMatched(TokenType::Err))
    {
        arm.pattern = "Err";
    }
    else
    {
        JLANG_ERROR("Expected 'Ok' or 'Err' in match arm");
        return arm;
    }

    // Expect (binding)
    if (!IsMatched(TokenType::LParen))
    {
        JLANG_ERROR("Expected '(' after pattern");
        return arm;
    }

    if (!IsMatched(TokenType::Identifier))
    {
        JLANG_ERROR("Expected binding name in match arm");
        return arm;
    }
    arm.bindingName = Previous().m_lexeme;

    if (!IsMatched(TokenType::RParen))
    {
        JLANG_ERROR("Expected ')' after binding name");
        return arm;
    }

    // Expect =>
    if (!IsMatched(TokenType::FatArrow))
    {
        JLANG_ERROR("Expected '=>' after match pattern");
        return arm;
    }

    // Parse body - either a block or a single expression
    if (Check(TokenType::LBrace))
    {
        arm.body = ParseBlock();
    }
    else
    {
        arm.body = ParseExpression();
    }

    return arm;
}

std::shared_ptr<AstNode> Parser::ParseMatchExpr()
{
    // Already consumed 'match'
    auto matchExpr = std::make_shared<MatchExpr>();

    matchExpr->scrutinee = ParseExpression();

    if (!IsMatched(TokenType::LBrace))
    {
        JLANG_ERROR("Expected '{' after match scrutinee");
        return nullptr;
    }

    // Parse two arms (Ok and Err, in any order)
    bool hasOk = false;
    bool hasErr = false;

    while (!Check(TokenType::RBrace) && !IsEndReached())
    {
        MatchArm arm = ParseMatchArm();

        if (arm.pattern == "Ok")
        {
            if (hasOk)
            {
                JLANG_ERROR("Duplicate 'Ok' arm in match expression");
            }
            matchExpr->okArm = arm;
            hasOk = true;
        }
        else if (arm.pattern == "Err")
        {
            if (hasErr)
            {
                JLANG_ERROR("Duplicate 'Err' arm in match expression");
            }
            matchExpr->errArm = arm;
            hasErr = true;
        }

        // Consume optional comma between arms
        IsMatched(TokenType::Comma);
    }

    if (!hasOk)
    {
        JLANG_ERROR("Match expression missing 'Ok' arm");
    }
    if (!hasErr)
    {
        JLANG_ERROR("Match expression missing 'Err' arm");
    }

    if (!IsMatched(TokenType::RBrace))
    {
        JLANG_ERROR("Expected '}' at end of match expression");
        return nullptr;
    }

    return matchExpr;
}

} // namespace jlang
