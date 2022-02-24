/** 
  * Description: Implementation file for class Token
  */

#include "Token.h"
//#include "parser.h"
#include <iostream>
using namespace std;
//default constructor
Token::Token() {}

Token::Token(type Type, int line, int column, std::string Lexeme) : lexeme(Lexeme)
{
	//bool localshift = false;
	token_type = Type;
	current_line = line;
	current_coloumn = column;
}
