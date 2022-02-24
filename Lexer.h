/** 
  * Description: Header file for class Lexer. 
  * Class Lexer is used to convert written programs into its token stream.
  */

#ifndef _LEXER_H_
#define _LEXER_H_

#include <deque>
#include <fstream>
#include <set>
#include "Token.h"
#include <map>
#include <string>

using namespace std;

class Lexer
{

public:
     /* Returns the next token
     * 
		 * Return: The next token in the stream 
		                                 
                                     
     */
  Token nextToken();
  Token t;
  void resetRow();

  char read();
  void openFile(const string &filename);
  void closeFile();

private:
  bool is_stdfunc(string fname);
  std::vector<string> std_funcs;
  //member variable for the current column in the file stream
  size_t current_coloumn;
  //current reading string
  std::string current_string;
  //The ifstream that is to be tokenized
  std::ifstream input_stream;
  //member variable for the current line in the file stream
  size_t current_line;
  std::string current_file;
};

#endif
