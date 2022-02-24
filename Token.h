/** 
  * Description: Header file for class Token.
  * Token class is used as a container for the attributes of a token for the 'little language'
  */

#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <vector>

enum class type
{
	COMMENT, // #
	ERROR,
	IDENTIFIER,	   //$variable
	FUNCTION_CALL, // method_call()
	STD_FUNC_CALL,

	INT,
	STRING,
	FLOAT,
	LINK,  // @"https://www.xnovity.com/redirect"
	TRUE,  //1
	FALSE, //0
	VOID,

	IF,
	ELSE,
	WHEN,

	FOR, // for
	FOR_EACH,
	WHILE,

	DOT,		  //.
	DOUBLE_POINT, // ..

	ASSIGN,		  // :
	EQUALS,		  // == 
	DOUBLE_ADD,	  // ++
	DOUBLE_MINUS, //--
	NEGATIVE_NUMBER,
	MEMBEROF, //::

	PLUS,  // +
	MINUS, // -
	STAR,  // *
	SLASH, // /
	POWER, // ^
	MOD,   // %
	CHECK, // ?
	IF_AND_ONLY_IF,// <=>
	IMPLICATION, // ->
	EQUALANCE,

	LPAR,		// (
	RPAR,		// )
	LBRACE,		// {
	RBRACE,		// }
	L_SQ_BRACE, // [
	R_SQ_BRACE, // ]

	COMMA,	   // ,
	SEMICOLON, // ;

	GREATER_THAN,	 // >
	GREATER_THAN_EQ, // >=
	LESS_THAN,		 // <
	LESS_THAN_EQ,	 // <=
	ELEMENT_OF,      //=>
	OR,				 // |
	AND,			 // &
	NOT,			 // !
	NOT_EQUAL,		 //!=
	HASH,			 //#

	THIS,	// this
	RETURN, // return
	THEN, // then
	OVERRIDE,
	OVERRIDE_R,
	DEF, // function decl
	NAMESPACE,
	REQUIRE,
	PROVIDE,
	CLASS,
	PARENT,
	EXTENDS, // extends
	DEPENDS,
	ENUM,	 //enum
	CONST,

	INT_T,	 //int
	STRING_T, //string
	FLOAT_T,	 //float
	BOOL_T,	 //bool
	CONST_T,	 //const
	VOID_T,	 //void
	NULL_T, //null

	EOS,
	CREATE_INSTANCE, //create-instance
	CONSTRUCTOR,	 //constructor()
	DESTRUCTOR,		 // destructor
	VIP_STD_FUNC,
	END,
	ENDIF,
	ENDWHILE,
	ENDFOR,
	END_FOR_EACH,
	ENDF,
	ENDWHEN,
};


class Token
{
public:
	//default constructor
	Token();

	//constructor with line, column, type, and lexeme params
	Token(type Type, int line, int column, std::string Lexeme);
	//member var for type
	type token_type;
	//member var for line
	size_t current_line;

	size_t current_coloumn;
	std::string lexeme;
	std::string file;
};

#endif
