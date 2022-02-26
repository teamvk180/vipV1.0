/**
  * File: Parser.cpp
  * Description: Implementation file for Parser.
  */

#include "Parser.h"
#include "ast.h"
#include "Lexer.h"
#include "Token.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <sstream>
#include "Lexer.h"
#include "Parser.h"
using std::cout;
using std::endl;
//#define DEBUG 1
const std::string isSpace = "\t";

//tools for parsing
Parser::Parser(vector<string> file_s)
{
    HasError = false;
    for (const auto d : file_s)
    {
        lex.openFile(d);

        next();
        try
        {
            while (!match(type::EOS))
            {

                Expression *stmt = this->statements();
                if (stmt != nullptr)
                {
                    ast_node.push_back(stmt);
                }
            }
            finishExpr *exp = new finishExpr;
               ast_node.push_back(exp);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

bool Parser::eval(type tokenType, const std::string errorMssg)
{
    if (match(tokenType))
    {
        next();
        return true;
    }
    else
    {
        Error(errorMssg);
        return false;
    }
}

bool Parser::eval(std::string token_type, const std::string errorMessage)
{
    if (match(token_type))
    {
        next();
        return true;
    }
    else
    {
        Error(errorMessage);
        return false;
    }
}
bool Parser::match(type p1)
{
    if (currentToken.token_type == p1)
    {
        return true;
    }

    return false;
}
bool Parser::match(std::string p1)
{

    if (currentToken.lexeme == p1)
    {
        return true;
    }
    return false;
}
void Parser::Error(const std::string &errorMssg)
{
    std::cout << errorMssg << std::endl;
    std::cout << "Error line: " << currentToken.current_line << " column: " << std::endl;
    throw ParseException("Parse Exception");
}

Expression *Parser::parsevarDecl()
{
#ifdef DEBUG
    std::cout << currentToken.lexeme << " >> type : " << (int)currentToken.token_type << "\tparseVarDecl" << std::endl;
#endif
    //eat let
    VariableDecl *var = new VariableDecl();
    type t = currentToken.token_type;
    next();
    std::string d = currentToken.lexeme;
    eval(type::IDENTIFIER, "expected variable name");

    if (match(type ::COMMA))
    {
        while (match(type ::COMMA))
        {
            next();
            if (match(type::SEMICOLON) || match(type::ASSIGN))
            {
                break;
            }
            else if (match(type::IDENTIFIER))
            {
                var->var_list.push_back(currentToken.lexeme);
                eval(type::IDENTIFIER, "expected variable name ");
            }
            else
            {
                Error("only identifier is allowed but got " + currentToken.lexeme);
            }
        }

        if (match(type::ASSIGN))
        {
            eval(type::ASSIGN, "expected : ");
            var->context = "VAR_DECL";
            var->expr = this->expr();
            return var;
        }
        return var;
    }
    else if (match(type::ASSIGN))
    {
        next(); //eat assign :
        var->context = "VAR_DECL";
        var->expr = this->expr();
    }
    else if (match(type::SEMICOLON))
    {
        next();
        return var;
    }
    else
    {
        Error("unknown token at var-decl '" + currentToken.lexeme + "'");
    }

    return var;
}

BlockExpr *Parser::parseParenExpr()
{
    if (match(type::LPAR))
    {
        next();
    }
    BlockExpr *my_t = new BlockExpr();
    vector<Expression *> m_expr;
    while (!match(type::LBRACE) || !match(type::SEMICOLON))
    {
        Expression *t = this->expr();
        if (t != nullptr)
        {
            m_expr.push_back(t);
        }
        else
        {
            next();
        }
    }

    eval(type::RPAR, "expected ')'");
    my_t->block = (m_expr);
    return my_t;
}
BlockExpr *Parser::parseBlock(type saddle,std::string error)
{
#ifdef DEBUG
    cout << "block detected " << (int)currentToken.token_type << endl;
#endif
    BlockExpr *b = new BlockExpr();
    vector<Expression *> block;
    while (!match(saddle))
    {
        if (match(type::EOS))
        {
            break;
        }
        Expression *k = this->statements();
        if (k != nullptr)
        {
            block.push_back(k);
        }
    }
    eval(saddle,error);
    b->block = (block);
    return b;
}

BlockExpr *Parser::parseIfBlock(type saddle,type saddle2,std::string error)
{
#ifdef DEBUG
    cout << "block detected " << (int)currentToken.token_type << endl;
#endif
    BlockExpr *b = new BlockExpr();
    vector<Expression *> block;
    while (!match(saddle)||!match(saddle2))
    {
        if (match(type::EOS))
        {
            break;
        }
        Expression *k = this->statements();
        if (k != nullptr)
        {
            block.push_back(k);
        }
    }
    eval(saddle,error);
    b->block = (block);
    return b;
}

Expression *Parser::parseWhileExpr()
{
    //eat while
    eval(type::WHILE, "while expected ");
    Expression *condition = this->expr();

    BlockExpr *body = this->parseBlock(type::ENDWHILE," end while ");
    whileExpr *while_node = new whileExpr();
    while_node->condition = (condition);
    while_node->while_body = (body);
    return while_node;
}

Expression *Parser::ClassDecl()
{
#ifdef DEBUG
    cout << "class detected " << endl;
#endif
    ClassExpr *p = new ClassExpr();
    eval(type::CLASS, "expected 'class'");
    p->class_name = currentToken.lexeme;
    eval(type::IDENTIFIER, "expected class name");
    p->class_body = this->parseBlock(type::END,"end expected");
    return p;
}
Expression *Parser::parseClassInstance(std::string cls_name)
{
#ifdef DEBUG
    std::cout << currentToken.lexeme << " >> type : " << (int)currentToken.token_type << "\tparseClassInstance" << std::endl;
#endif
    //eat let
    ClassInstanceExpr *var = new ClassInstanceExpr();
    var->class_name = cls_name;
    if (match(type::LESS_THAN))
    {
        next();
        while (!match(type ::GREATER_THAN))
        {
            if (match(type::SEMICOLON) || match(type::ASSIGN))
            {
                Error("invalid token " + currentToken.lexeme);
                break;
            }
            else if (match(type::COMMA))
            {
                next();
            }
            else if (match(type::IDENTIFIER))
            {
                var->instance_name.push_back(currentToken.lexeme);
                eval(type::IDENTIFIER, "expected variable name ");
            }
            else
            {
                Error("only identifier is allowed but got l " + currentToken.lexeme);
            }
        }
        eval(type::GREATER_THAN, "expected > symbol ");
        var->instance_block_name = currentToken.lexeme;
        eval(type::IDENTIFIER, "expected an instance name got " + currentToken.lexeme);
        if (match(type::ASSIGN))
        {
            next();
            var->context = "CLS_BLOCK_INST";
            var->cls_assign = this->expr();
            return var;
        }

        return var;
    }
    else if (match(type::ASSIGN))
    {
        Error("unexpected assignment at var-decl '" + currentToken.lexeme + "'");
    }
    else if (match(type::IDENTIFIER))
    {
        var->cls_assign = this->parseInstanceAssign();
        return var;
    }
    else
    {
        Error("unknown token at var-decl '" + currentToken.lexeme + "'");
    }

    return var;
}
Expression *Parser::parseInstanceAssign()
{
#ifdef DEBUG
    cout << "assign instace detected " << endl;
#endif
    ClassInstanceExpr *var = new ClassInstanceExpr();
    var->instance_name.push_back(currentToken.lexeme);
    next();
    while (match(type ::COMMA))
    {
        next();
        if (match(type::SEMICOLON) || match(type::ASSIGN))
        {
            break;
        }
        else if (match(type::IDENTIFIER))
        {
            var->instance_name.push_back(currentToken.lexeme);
            eval(type::IDENTIFIER, "expected variable name ");
        }
        else
        {
            Error("only identifier is allowed but got " + currentToken.lexeme);
        }
    }

    if (match(type::ASSIGN))
    {
        next();
        var->context = "CLS_INST";
        var->cls_assign = this->expr();
        return var;
    }
    return var;
}

Expression *Parser::parseAssign(std::string var_name)
{
#ifdef DEBUG
    cout << "assignment detected " << endl;
#endif
    AssignmentExpr *t = new AssignmentExpr();
    Expression *d = parseLeft();
    if (d == nullptr)
        t->lhs = var_name;
    else
        t->field = d;
    eval(type::ASSIGN, "expected ':' ");
    t->offset = this->expr();
    return t;
}
Expression *Parser::parseLeft()
{
    Expression *t = nullptr;
    if (match(type::L_SQ_BRACE))
    {
        t = parseArrayExpr();
    }
    return t;
}

Expression *Parser::parseSelf()
{

    SelfExpr *self = new SelfExpr();
    eval(type::THIS, "expected self");
    self->argument = (this->parseParenExpr());
    cout << "done " << endl;
    if (self != nullptr)
        return self;
    else
        return nullptr;
}
bool Parser::isValid(type t)
{
    return (t == type::PLUS ||
            t == type::MINUS ||
            t == type::STAR ||
            t == type::SLASH ||
            t == type::LESS_THAN ||
            t == type::LESS_THAN_EQ ||
            t == type::GREATER_THAN ||
            t == type::GREATER_THAN_EQ ||
            t == type::OR ||
            t == type::AND ||
            t == type::NOT ||
            t == type::EQUALS ||
            t == type::NOT_EQUAL);
}

bool Parser::isInvalid(type t)
{
    return (t == type::RPAR ||
             t== type::ENDIF||
             t== type::ENDF||
             t == type::END||
             t == type::THEN ||
            t == type::RBRACE ||
            t == type ::R_SQ_BRACE ||
            t == type::COMMA || //, is optional
            t == type::SEMICOLON);
}

// vector<VarDecl*>
Expression *Parser::parseArrayExpr(std::string array_name)
{
    ArrayExpr *a = new ArrayExpr();
    a->array_name = array_name;
    eval(type::L_SQ_BRACE, "expected '['");
    a->expr = this->expr();
    eval(type::R_SQ_BRACE, "expecting a ] / rbrace");
    return a;
}

Expression *Parser::expr()
{
    Expression *t = nullptr;
    if (isInvalid(currentToken.token_type))
    {
        cout << "expr " << endl;
        return t;
    }
    else
    {
        BunddleExpr *l = new BunddleExpr();
        l->expr_.push_back(this->parseExpr());
        while (match(type::COMMA) || match(type::DOT))
        {
            if (match(type::COMMA))
            {
                next();
            }

            l->expr_.push_back(this->parseExpr());
        }
        return l;
    }
    return t;
}

Expression *Parser::parseExpr()
{
#ifdef DEBUG
    std::cout << currentToken.lexeme << " >> type : " << (int)currentToken.token_type << "\tparseBooleanExpr" << std::endl;
#endif
    Expression *l = this->parseTermExpr();
    l=nullptr;
    while (isValid(currentToken.token_type))
    {
        ALExpr *j = new ALExpr();
        j->op = currentToken.token_type;
        next();
        j->lhs = l;
        j->rhs = this->parseTermExpr();
    }
    return l;
}

// static
Expression *Parser::parseTermExpr()
{
#ifdef DEBUG
    std::cout << currentToken.lexeme << " >> type : " << (int)currentToken.token_type << "\tparseExpr static" << std::endl;
#endif
    BunddleExpr *l = new BunddleExpr();
      l= nullptr;
    if (match(type::LPAR))
    {
        next();
        l->expr_.push_back(this->parseExpr());
        eval(type::RPAR, "expected ')'");
    }
    else if (match(type::L_SQ_BRACE))
    {
        l->expr_.push_back(this->parseArrayExpr());
    }
    else if (match(type::INT))
    {
        staticExpr *s = new staticExpr;
        s->mToken = currentToken;
        next();
        l->expr_.push_back(s);
    }
    else if (match(type::FLOAT))
    {
        staticExpr *s = new staticExpr;
        s->mToken = currentToken;
        next();
        l->expr_.push_back(s);
    }
    else if (match(type::STRING))
    {
        cout << "string detected" << endl;
        staticExpr *s = new staticExpr;
        s->mToken = currentToken;
        next();
        l->expr_.push_back(s);
    }
    else if (match(type::LINK))
    {
        cout << "LINK detected" << endl;
        staticExpr *s = new staticExpr;
        s->mToken = currentToken;
        next();
        l->expr_.push_back(s);
    }
    else if (match(type::TRUE))
    {
        staticExpr *s = new staticExpr;
        s->mToken = currentToken;
        next();
        l->expr_.push_back(s);
    }
    else if (match(type::DOT))
    {
        //.expr
        //eat .
        next();
        SelfExpr *s = new SelfExpr();
        s->argument = this->expr();
        l->expr_.push_back(s);
    }
    else if (match(type::DOUBLE_POINT))
    {
        staticExpr *s = new staticExpr;
        s->mToken = currentToken;
        next();
        l->expr_.push_back(s);
    }
    else if (match(type::FALSE))
    {
        staticExpr *s = new staticExpr;
        s->mToken = currentToken;
        next();

        l->expr_.push_back(s);
    }
    else if (match(type::THIS))
    {
        staticExpr *s = new staticExpr;
        s->mToken = currentToken;
        next();
        l->expr_.push_back(s);
    }
    else if (match(type::VOID))
    {
        staticExpr *s = new staticExpr;
        s->mToken = currentToken;
        next();

        l->expr_.push_back(s);
    }
    else if (match(type::IDENTIFIER))
    {
        std::string id = currentToken.lexeme;
        next();
        if (match(type::IDENTIFIER))
        {
            l->expr_.push_back(parseClassInstance(id));
        }
        else if (match(type::ASSIGN) || match(type::L_SQ_BRACE))
        {
            l->expr_.push_back(parseAssign(id));
        }
        else if (match(type::LPAR) || match(type::DOT))
        {
            l->expr_.push_back(parseCallStatement(id));
        }
        else if (match(type::RPAR) || match(type::R_SQ_BRACE) || match(type::SEMICOLON) || match(type::LBRACE) || match(type::COMMA))
        {
            //function(id);
            l->expr_.push_back(varCall(id));
        }
        else if (isValid(currentToken.token_type))
        {
            //p+q or p-q or p*q or p/q
            l->expr_.push_back(varCall(id));
        }
        else
        {
            Error("syntax error : or [ or ( or . expected");
        }
    }
    else
    {
        Error("invalid symbol " + currentToken.lexeme);
    }
    return l;
}

Expression *Parser::varCall(std::string var)
{
    //eval(type::IDENTIFIER, "expected variable name");
    VariableCall *call = new VariableCall();
    call->var_name = currentToken;
    return call;
}
// vector<VarDecl*>
Expression *Parser::parseArrayExpr()
{
    ArrayExpr *a = new ArrayExpr();
    eval(type::L_SQ_BRACE, "expected '['");
    a->value.push_back(this->expr());
    eval(type::R_SQ_BRACE, "expecting a ] / rbrace");
    return a;
}
Expression *Parser::statements()
{
#ifdef DEBUG
    std::cout << currentToken.lexeme << " >> type : " << (int)currentToken.token_type << "\tparseStatement" << std::endl;
#endif
    Expression *p = nullptr;
    switch (currentToken.token_type)
    {
    case type::DEF:
    {

        p = parseFunctionDecl();
        break;
    }
    case type::ENUM:
    {
        p = this->parseEnumClass();
        break;
    }
    case type::COMMA:
    {
        next();
        break;
    }
    case type::SEMICOLON:
    {
        next();
        break;
    }
    case type::REQUIRE:
    {
        p = parseRequireExpr();
        break;
    }
    case type::CLASS:
    {
        p = ClassDecl();
        break;
    }
    case type::ERROR:
    {
        cerr << "error : " << currentToken.lexeme << endl;
        p = nullptr;
        throw ParseException("lexing error");
        break;
    }
    case type::IDENTIFIER:
    {
        std::string id = currentToken.lexeme;
        next();
        if (match(type::IDENTIFIER) || match(type::LESS_THAN))
        {
            p = parseClassInstance(id);
            break;
        }
        else if (match(type::ASSIGN) || match(type::L_SQ_BRACE))
        {
            p = parseAssign(id);
            break;
        }
        else if (match(type::LPAR) || match(type::DOT))
        {
            p = parseCallStatement(id);
            break;
        }
        else
        {
            Error("syntax error : or [ or ( or . expected");
        }
        break;
    }
    case type::INT_T:
    case type::STRING_T:
    case type::FLOAT_T:
    case type::VOID_T:
    {
        p = parsevarDecl();
        break;
    }
    case type::IF:
    {
        p = parseIf();
        break;
    }
    case type::WHILE:
    {
        p = parseWhileExpr();
        break;
    }
    case type::RETURN:
    {
        p = parseReturnExpr();
        break;
    }
    default:
    {
        throw "something wrong in parsing";
        break;
    }
    }

    return p;
}

Expression *Parser::parseType()
{
    TypeExpr *t = new TypeExpr();
    if (match(type::IDENTIFIER))
    {
        Token tk;
        tk.lexeme = currentToken.lexeme;
        tk.token_type = type::CLASS;
        t->token = tk;
        next();
    }
    else if (match(type::STRING_T)||match(type::FLOAT_T)||match(type::INT_T)||match(type::VOID_T))
    {
        next();
        t->token.token_type = currentToken.token_type;
        std::string var_name;
        eval(type::IDENTIFIER, "expected variable name but got " + currentToken.lexeme);
        if (match(type::ASSIGN))
        {
            t->exp = parseAssign(var_name);
        }
        return t;
    }
    else
    {
        cerr << "invalid datatype found" << endl;
    }
    return t;
}
Expression *Parser::parseParamList()
{
    Expression *t = nullptr;
    if (!match(type ::RPAR))
    {
        BunddleExpr *b = new BunddleExpr();
        b->expr_.push_back(parseParam());
        while (match(type ::COMMA))
        {
            next();
            b->expr_.push_back(this->parseParam());
        }
        t = b;
    }

    return t;
}

//parse params
Expression *Parser::parseParam()
{
#ifdef DEBUG
    cout << "parsing Params : " << currentToken.lexeme << endl;
#endif // DEBUG
    bool class_dr = false;
    ParamsExpr *exp = new ParamsExpr();
    if (match(type::IDENTIFIER))
    {
        class_dr = true;
    }
    else
    {
        class_dr = false;
    }
    exp->type = this->parseType();

    if (class_dr)
    {
        exp->param_name = currentToken.lexeme;
        eval(type::IDENTIFIER, "expected var name");
    }
    return exp;
}
// FunctionCall*
Expression *Parser::parseCallStatement(std::string func_name)
{
#ifdef DEBUG
    std::cout << currentToken.lexeme << " >> type : " << (int)currentToken.token_type << "\tparseFunctionCall" << std::endl;
#endif
    FunctionCall *f = new FunctionCall();
    f->function_name = func_name;
    if (match(type::LPAR))
    {
        next();
        f->params = this->expr();
        eval(type::RPAR, "expected ')'L");
        return f;
    }
    else
    {
        cerr << "error on  function call of " << currentToken.lexeme << endl;
    }
    eval(type::SEMICOLON, "expected semicolon");
    return f;
}

// FunctionDecl*
Expression *Parser::parseFunctionDecl()
{
    FunctionDecl *p = new FunctionDecl;

    eval(type::DEF, "expected function keyword");
    p->method_name = currentToken.lexeme;
    eval(type::IDENTIFIER, "expected 'function name'");
    if (match(type::LPAR))
    {
        next();
        p->params = parseParamList();
        eval(type::RPAR, "expected ) but got " + currentToken.lexeme);
    }
    p->method_body = parseBlock(type::ENDF,"expected endf ");
    return p;
}

// IfOrElse*
Expression *Parser::parseIf()
{
    cout << "detect if ok" << endl;
    //check if is found or not before else at gen
    IfStatement *p = new IfStatement;
    if (match(type::IF))
    {
        next();
        p->condition = expr();
        p->if_block = parseIfBlock(type::ENDIF,type::ELSE,"expected endif or else");

        if (match(type::ELSE))
        {
            next();
            if (match(type::IF))
            {
                cout << "detect else if ok" << endl;
                p->elseifBlock.push_back(this->parseIf());
            }
            else
            {
                cout << "detect else ok " << currentToken.lexeme << endl;
                p->else_block = parseBlock(type::ENDIF,"expected endif");
            }
        }
    }

    return p;
}

Expression *Parser::parseReturnExpr()
{ //eat return
    eval(type::RETURN, "return expected ");
    Expression *t = this->expr();
    eval(type::SEMICOLON, "expected ';'");
    return t;
}

Expression *Parser::parseEnumClass()
{
    /*
    EnumClassExpr *l = new EnumClassExpr;
    eval(type::ENUM, "enum expected ");
    eval(type::CLASS, "class expected ");
    std::string enum_class_name = currentToken.lexeme;
    eval(type::IDENTIFIER, "expected enum class name");
    Token k = currentToken;
    eval(type::LBRACE, "an Enum class must have a'{' after class name");
    while (k.token_type != type::RBRACE)
    {
        eval(type::LET, "expected let");
        eval(type::IDENTIFIER, "expected enum variable name");
        std::string var_name = currentToken.lexeme;
        eval(type::COMMA, "Expected a comma");
        l->expr_.push_back(var_name);
    }
    if (match(type::RBRACE))
    {
        next();
    }
    return l;
    */
   return nullptr;
}

Expression *Parser::parseRequireExpr()
{
    //eat require
    eval(type::REQUIRE, "expected using");
    RequireExpr *p = new RequireExpr();
    p->exp = this->expr();
    eval(type::SEMICOLON, "expected semicolon");
    return p;
}
