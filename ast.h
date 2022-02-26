#ifndef AST_H
#define AST_H
#include "Token.h"
#include "Exception.h"
#include <string>
#include <map>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#define DEBUG 1
using namespace std;

class Expression;
class AST
{
public:
    AST() = default;
    int context_type;
    std::string result = "<VIP>";
    virtual std::string evaluvate() = 0;
    virtual ~AST(){};
};

class Expression : public AST
{
public:
    Expression() = default;
    ~Expression() = default;
    Token t;
    std::string context = "\0";
    std::string evaluvate()
    {
#ifdef DEBUG
        cout << "<AST> " << endl;
#endif
        return "(VIP-overrider-not-found)";
    }
};

class RequireExpr : public Expression
{
public:
    RequireExpr() {}
    Expression *exp;
    std::string evaluvate();
};

class BlockExpr : public Expression
{
public:
    vector<Expression *> block;
    BlockExpr(){};
    std::string evaluvate();
};

class staticExpr : public Expression //static
{
public:
    Token mToken;
    staticExpr() {}
    std::string evaluvate();
};

class TypeExpr : public Expression
{
public:
    TypeExpr() {}
    Token token;
    Expression *exp; // explicit declaration expr
    std::string evaluvate()
    {
        if (token.token_type == type::CLASS)
            return "(set  class.member " + token.lexeme + ")";
        else
            return "(set decl.local " + token.lexeme + ")";
    };
};
class STDlibExpr : public Expression
{
public:
    std::string lib_name;
    Expression *expr_;
    STDlibExpr() {}
    std::string evaluvate();
};

class ArrayExpr : public Expression
{
public:
    ArrayExpr() {}
    std::string array_name;
    Expression *expr;
    vector<Expression *> value;
    std::string evaluvate() { return "<doc VIP 409 NOT-IMPL>\0"; };
};

class ParamsExpr : public Expression
{
public:
    ParamsExpr() {}
    Expression *type;
    std::string param_name;
    std::string evaluvate() { return "(assign (" + type->evaluvate() + param_name + "))"; };
};
class BunddleExpr : public Expression
{
public:
    vector<Expression *> expr_;
    Token mToken;
    std::string evaluvate();
};

class ALExpr : public Expression
{
public:
    //implementation required
    ALExpr(){};
    type op;
    Expression *lhs;
    Expression *rhs;
    std::string evaluvate();
};
class ConstructorExpr : public Expression
{
public:
    Expression *params_;
    BlockExpr *block;
    std::string evaluvate() { return "<doc VIP 403 NOT-IMPL>\0"; };
};
class DestructorExpr : public Expression
{
public:
    Expression *params_;
    BlockExpr *block;
    std::string evaluvate() { return "<doc VIP 404 NOT-IMPL>\0"; };
};

class EnumClassExpr : public Expression
{
public:
    vector<string> expr_;
    std::string evaluvate() { return "<doc VIP 405 NOT-IMPL>\0"; };
};

class FunctionDecl : public Expression
{
public:
    FunctionDecl() {}
    std::string method_name;
    Expression *params;
    BlockExpr *method_body;
    std::string evaluvate();
};
class FunctionCall : public Expression
{
public:
    std::string function_name;
    Expression *params;
    FunctionCall(){};
    std::string evaluvate();
};

class ClassExpr : public Expression
{
public:
    std::string class_name;
    BlockExpr *class_body;
    type class_type; //default public
    ClassExpr() {}
    std::string evaluvate();
};
class ClassInstanceExpr : public Expression
{
public:
    ClassInstanceExpr() {}
    std::string class_name;
    std::string instance_block_name; //vector<string> instace_block;
    Expression *cls_assign;
    vector<std::string> instance_name;
    std::string evaluvate();
};

class IfStatement : public Expression
{
public:
    //brackExpr *if_;
    vector<Expression *> elseifBlock;
    BlockExpr *if_block;
    BlockExpr *else_block;
    Expression *condition;
    std::string evaluvate();
};

class whileExpr : public Expression
{
public:
    whileExpr() {}
    BlockExpr *while_body;
    Expression *condition;
    std::string evaluvate();
};

class ReturnStatement : public Expression
{
public:
    BlockExpr *ret_expr;
    int context;

public:
    std::string evaluvate();
};
class TernaryExpr : public Expression
{
public:
    Expression *l, *m, *r;
    int type;

public:
    TernaryExpr(){};
    std::string evaluvate()
    {
        throw BuiltINMethodInvalidCallException("this service is not implemented  till this update.");
    };
};

class AssignmentExpr : public Expression
{
public:
    AssignmentExpr() {}
    std::string evaluvate();
    std::string lhs;
    Expression *offset;
    Expression *field;
};

class SelfExpr : public Expression
{
    //expr1().expr; is also self because expr1
    //result will be local so refering local is self
public:
    Expression *argument;
    SelfExpr() {}
    std::string evaluvate();
};

class VariableDecl : public Expression
{
public:
    Expression *expr;
    type data_type;
    vector<std::string> var_list;
    VariableDecl() {}
    std::string evaluvate();
};
class VariableCall : public Expression
{
public:
    Token var_name; //public private protected etc.
    VariableCall() {}
    std::string evaluvate() { return "abbay manasilu"; };
};
class OverriderExpr : public Expression
{
public:
    OverriderExpr() {}
    Token overrider;
};
class finishExpr : public Expression
{
public:
      std::string evaluvate() {return this->result;}
};
#endif