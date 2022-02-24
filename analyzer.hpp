#ifndef _ANALYZER_HPP_
#define _ANALYZER_HPP_
#include "Token.h"
#include <algorithm>
#include <utility>
#include <map>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include "Exception.h"
#include <istream>
using namespace std;
//used for syntax check and code generation.
namespace dataStructure
{
    class members
    {
        std::string mem_name;
        type element_type; //function class or var etc
        type access_type;  // public private or protected
    };
    class functions
    {
        std::string function_name;
        std::string parentClass;
        size_t params_no; //function(para1,para2,para3 ...)
        bool self_enabled;
        type access_type; // public private or protected
        vector<functions *> sub_functions;
    };
    class clas
    {
        std::string class_name;
        std::string parentClass;
        type access_type; // public private or protected
        vector<functions *> function_list;
        vector<members *> members_list;
        vector<clas *> sub_classes;
    };
};
class analyzer
{
private:
    //class name,memmbers_name,type
    map<std::string, dataStructure::clas> class_table;
    bool insideClass;

public:
    bool evaluvateToken(Token t);
    bool evaluvateIdentifier(std::string id);
    void add_class(std::string class_name);
    bool find_member(std::string class_name, std::string func_name);
    type get_member(std::string class_name, std::string func_name);
    void add_element(std::string class_name, std::string members_name, type mem_type);
    analyzer();
    ~analyzer();
};

#endif // !_analyzer_HPP_
