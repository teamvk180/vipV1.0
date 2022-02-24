#ifndef PARSER_H
#define PARSER_H
//#define DEBUG 45543
#include <vector>
#include "Lexer.h"
#include <stdbool.h>
#include "Token.h"
#include <string>
#include <cstdlib>
#include "Exception.h"
#include <istream>
#include <vector>
#include <queue>
#include <fstream>
#include <map>
#include "ast.h"
#include <stack>
using namespace std;
/*
template <typename T>
std::string ToString(T Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}
*/
class Parser
{
public:
    bool HasError;
    vector<Expression *> ast_node;
    vector<Expression *> getSyntaxTree() { return ast_node; }
    inline bool is_exist_(const std::string &name)
    {
        if (FILE *file = fopen(name.c_str(), "r"))
        {
            fclose(file);
            return true;
        }
        else
        {
            return false;
        }
    }
    //constructor, takes ref to Lexer as argument
    Parser(vector<string> file_s);
    /* Function to parse stream of tokens
		 * Return: A StmtList object if successfull
		 * and NULL if otherwise
		 */
    std::string link = "\0";
    //std::string make_link();
private:
    /*
1 	()   []   ->   .   :: 	Function call, scope, array/member access
2 	!   ~   -   +   *   &   sizeof   type cast   ++   --   	(most) unary operators, sizeof and type casts (right to left)
3 	*   /   % MOD 	Multiplication, division, modulo
4 	+   - 	Addition and subtraction
5 	<<   >> 	Bitwise shift left and right
6 	<   <=   >   >= 	Comparisons: less-than and greater-than
7 	==   != 	Comparisons: equal and not equal
8 	& 	Bitwise AND
9 	^ 	Bitwise exclusive OR (XOR)
10 	| 	Bitwise inclusive (normal) OR
11 	&& 	Logical AND
12 	|| 	Logical OR
13 	? : 	Conditional expression (ternary)
14 	=   +=   -=   *=   /=   %=   &=   |=   ^=   <<=   >>= 	Assignment operators (right to left)
15 	, 	Comma operator
*/

    //data member of type lexer
    //stmt statment;
    Lexer lex;
    //maintab ASTList
    //data member for the current token that is being Parsed
    Token currentToken;
    Token Temp;
    bool isValid(type t);
    bool isInvalid(type t);
    /*  Determines if current token has correct token type
		 *  Param: an int for tokenType, and a string for an err message
		 *  Post: if currentToken is of wrong type, Error is called
		 */
    bool eval(type token, const std::string errorMessage);
    bool eval(std::string token_type, const std::string errorMessage);
    bool match(type p1);
    bool match(std::string p1);
    /* Throws exception and couts error message
		 * Pre: invalid token detected
		 * Throw: ParseException
		 */
    void Error(const std::string &errMessage);
    //returns a token with the correct type
    bool insideClass;
    bool finished;

    Expression *parseFunctionDecl();
    Expression *parseEnumClass();
    Expression *statements();
    Expression *varCall(std::string var);
    Expression *parsevarDecl();
    Expression *parseLetId();
    BlockExpr  *parseParenExpr();
    BlockExpr  *parseBlock(type saddle,std::string error);
    BlockExpr  *parseIfBlock(type saddle,type saddle2,std::string error);
    Expression *parseIf();
    Expression *parseType();
    Expression *parseParamList();
    Expression *parseParam();
    Expression *parseWhileExpr();
    Expression *parseAssign(std::string var_name);
    Expression *parseLeft();
    Expression *parseCallStatement(std::string func_name);
    Expression *parseSelf();
    Expression *parseVipStdLibExpr();
    Expression *parseVariable();
    Expression *parseReturnExpr();
    Expression *parseOverrider();
    Expression *parseRequireExpr();
    Expression *parseArrayExpr(std::string array_name);
    Expression *parseArrayExpr();
    Expression *expr();
    Expression *parseExpr();
    Expression *parseTerminal();
    Expression *parseAdditiveExpr();
    Expression *parseBooleanExpr();
    Expression *parseTermExpr();
    Expression *parseFactorExpr();
    Expression *parsePositiveFactor();
    Expression *parseNotFactor();
    Expression *ClassDecl();
    Expression *parseForeachExpr();
    Expression *parseClassInstance(std::string cls_name);
    Expression *parseInstanceAssign();
    queue<Token> buffer;
    Token next() { return currentToken = lex.nextToken(); }
    void holdToken()
    {
        cout << "holding token : " << (int)currentToken.token_type;
        cout << " lex : " << currentToken.lexeme << endl;

        buffer.push(currentToken);
    }
    /*
    void update()
    {
        if (!buffer.empty())
        {
            currentToken = buffer.front();
            cout << "reload  : " << (int)currentToken.token_type;
            cout << " " << currentToken.lexeme << "size " << buffer.size() << endl;
            buffer.pop();
            return;
        }
        else
            currentToken = lex.nextToken();
        cout << "token : " << (int)currentToken.token_type;
        cout << "lex : " << currentToken.lexeme << endl;
    };
*/
    void clear()
    {
        std::queue<Token> empty;
        std::swap(buffer, empty);
    };
};
#endif
