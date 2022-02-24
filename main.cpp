#include <iostream>
#include <fstream>
#include <vector>
#include "Exception.h"
#include <ostream>
#include "parser.h"
#include "Lexer.h"
#include <stdio.h>
#include <cstdlib>
#include <algorithm>
using namespace std;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char **argv)
{
ifstream fin;
    vector<string> filenames;
    for (int i = 1; i < argc; i++)
    {
        fin.open(argv[i]);
        if (fin.fail())
        {
            cerr << "file '" << argv[i] << "' not exist!";
            exit(-1);
        }
        filenames.push_back(argv[i]);
        fin.close();
    }
        Parser the_parser = Parser(filenames);
           std::ofstream outfile;
  outfile.open("test.vpx", std::ios_base::app); // append instead of overwrite
        for(const auto it : the_parser.ast_node)
        {

          outfile << it->evaluvate();
        }  
         outfile.close();
        cout <<"compiled succesfully"<<endl;
}
    