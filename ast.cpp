#include "ast.h"
#include <vector>
#define T_CALL "LOAD " //load
#define T_LINK "LINK " //move
#define T_REQUEST "REQUEST"
#define T_OBJECT "OBJECT"
std::string isEnd = "\n";
// Function to modify an identifier

std::string FunctionDecl::evaluvate()
{
    this->context = "METHOD_DECL";
    string ram = "(function  " + method_name + isEnd;
    ram += this->method_body->evaluvate();
    ram += ")" + isEnd;
    return ram;
}
std::string FunctionCall::evaluvate()
{
    //this->insert(method_name, "METHOD_CALL", this->gen_idr());
    std::string ram = "(params.exe 'savethis')";
    ram += this->params->evaluvate();

    ram += "(call " + function_name + ")" + isEnd;
    return ram;
}

std::string ALExpr::evaluvate()
{
#ifdef DEBUG
    cout << "evaluvating  arithematic and logical " << endl;
#endif
    switch (op)
    {
    case type::IDENTIFIER:
    {
        this->result += "(push.exe )" + isEnd;

        break; //not implemented.
    }
    case type::INT:
    {
        this->result+= "(push.exe )" + isEnd;
        break;
    }
    case type::STRING:
    {
        cout << "string dled detect" << endl;
        this->result+= "(static )" + isEnd;
        break;
    }
    case type::FLOAT:
    {
        this->result+= "(push.exe )" + isEnd;
        break;
    }
    case type::TRUE:
    {
        this->result+= "(push.exe )" + isEnd;
        break;
    }
    case type::FALSE:
    {
        this->result+= "(push.exe )" + isEnd;
        break;
    }
    default:
        cout <<  " has a type mismatch " + to_string((int)op) << endl;
        break;
    }

    if(lhs!=nullptr)
{
    this->result+=lhs->evaluvate();
}
if(rhs!=nullptr)
{
    this->result+=rhs->evaluvate();
}

    return "";
}

std::string staticExpr::evaluvate()
{
    std::string out ="\0";
    out+="static "+mToken.lexeme+" ;"+isEnd;
    return out;
}
std::string whileExpr::evaluvate()
{
    this->context = "WHILE";
    std::string ram = "\0";
    if (while_body != nullptr)
    {
        ram += "(whileblock  " + isEnd;
        ram += this->while_body->evaluvate();
        ram + isEnd + " )" + isEnd;
    }

    if (condition != nullptr)
    {
        ram += this->condition->evaluvate();
    }
    else
    {
        throw BuiltINMethodInvalidCallException("while method requres an argument");
    }

    ram += "(jnz.exe)" + isEnd; //200 is an expr indexed driver

    return ram;
}

std::string IfStatement::evaluvate()
{
    this->context = ("IF");
    std::string ram = "\0";
    if (if_block != nullptr)
    {
        ram += "(blockif " + isEnd;
        ram += this->if_block->evaluvate();
        ram += ")" + isEnd; //jnz disable temp.
        ram += "(blockelse " + this->else_block->evaluvate();
        ram += ")" + isEnd;
        if (!elseifBlock.empty())
        {
            ram += "(blockelif " + isEnd;
            for (const auto l : elseifBlock)
            {
                ram += l->evaluvate();
            }
        }
        ram += ")" + isEnd;
    }
    else
    {
        throw BuiltINMethodInvalidCallException("if cannot be null");
    }
    if (condition != nullptr)
    {
        ram += "(expr " + this->condition->evaluvate();
        ram += ")" + isEnd; //jnz driver to go label
    }
    else
    {
        throw BuiltINMethodInvalidCallException("if method requires an argument");
    }

    return ram;
}

std::string SelfExpr::evaluvate()
{
    cout << "self called " << endl;
    this->context = "SELF";
    std::string ram = "(local ";
    if (this->argument != nullptr)
    {
        cout << "self called " << endl;
        ram += this->argument->evaluvate();
        ram += ")" + isEnd;
    }
    else
    {
        throw BuiltINMethodInvalidCallException("self method requires an arguement");
    }

    return ram;
}

std::string BlockExpr::evaluvate()
{
#ifdef DEBUG
    std::cout << "evaluvating {block} expr" << std::endl;
#endif
    this->context = "BLOCK";
    std::string ram = "\0";
    if(block.size()>0)
    for (const auto l : block)
    {
        ram += l->evaluvate();
    }
    return ram;
}

std::string ReturnStatement::evaluvate()
{
#ifdef DEBUG
    std::cout << "evaluvating return statement" << std::endl;
#endif
    std::string rom = "(returnNode.exe expr(";
    if(ret_expr!=nullptr)
    rom += this->ret_expr->evaluvate();
    rom += "))" + isEnd;
    return rom;
}
std::string VariableDecl::evaluvate()
{
#ifdef DEBUG
    std::cout << "evaluvating variable decl " << std::endl;
#endif
    std::string rom = "\0";
    if (!var_list.empty())
    {
        for (const auto l : var_list)
        {
            rom += "(decl " + l;
        }
    }
    if (expr != nullptr)
    {
        rom += " expr(" + this->expr->evaluvate();
        rom += "))" + isEnd;
    }

    return rom;
}

std::string STDlibExpr::evaluvate()
{
#ifdef DEBUG
    std::cout << "evaluvating  std library " << this->lib_name << std::endl;
#endif
    std::string rom = "\0";
    rom += expr_->evaluvate();
    rom += "(call-library " + this->lib_name;
    rom += ")" + isEnd;
    return rom;
}

std::string AssignmentExpr::evaluvate()
{
#ifdef DEBUG
    std::cout << "evaluvating  assignment Expr" << std::endl;
#endif
    std::string rom = "\0";

    if (field != nullptr)
    {
        rom += "(search " + field->evaluvate();
        if (offset != nullptr)
        {
            rom += " assign-expr(" + this->offset->evaluvate();
            rom += ")" + isEnd;
        }
    }
    rom += ")" + isEnd;
    return rom;
}

std::string BunddleExpr::evaluvate()
{
#ifdef DEBUG
    std::cout << "evaluvating (expr) " << std::endl;
#endif
    std::string rom = "\0";
    std::string dorge = "\0";
    if (!expr_.empty())
    {
        for (const auto l : this->expr_)
        {
            dorge += l->evaluvate();
        }
    }

    switch (mToken.token_type)
    {
    case type::PLUS:
    {

        rom += "(add.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }
    case type::MINUS:
    {

        rom += "(sub.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::STAR:
    {

        rom += "(mul.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::SLASH:
    {

        rom += "(div.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::POWER:
    {

        rom += "(power.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::LESS_THAN:
    {

        rom += "(lt.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::LESS_THAN_EQ:
    {

        rom += "(lte.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::GREATER_THAN:
    {

        rom += "(gt.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::GREATER_THAN_EQ:
    {

        rom += "(gte.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::EQUALS:
    {

        rom += "(eq.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::NOT_EQUAL:
    {

        rom += "(noteq.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::AND:
    {

        rom += "(and.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }

    case type::OR:
    {
        rom += "(or.exe exp(" + isEnd + dorge + "))" + isEnd;
        break;
    }
    case type::NOT:
    {

        rom += "(not exp (" + dorge + "))" + isEnd;
        break;
    }
    case type::NEGATIVE_NUMBER:
    {
        rom += "something amezing" + isEnd;
        break;
    }
    default:
    {
        break;
    }
    }
    return rom;
}

std::string ClassExpr::evaluvate()
{
#ifdef DEBUG
    std::cout << "evaluvating  class expr" << std::endl;
#endif
    g.add_class(this->class_name);
    std::string ram = "(decl-class.exe  ";
    ram += this->class_name + "(" + isEnd;
    ram += this->class_body->evaluvate();
    ram += "))" + isEnd;
    return ram;
}

std::string ClassInstanceExpr::evaluvate()
{
#ifdef DEBUG
    std::cout << "evaluvating  class instance " << std::endl;
#endif
    std::string ram = "\0";
    ram += "(inst.exe  " + class_name + " " + cls_assign->evaluvate() + " )\n";
    return ram;
}

std::string RequireExpr::evaluvate()
{
#ifdef DEBUG
    std::cout << "evaluvating  using expr " << std::endl;
#endif
    std::string ram = "\0";
    ram += "(load " + this->exp->evaluvate() + " )";
    return ram;
};