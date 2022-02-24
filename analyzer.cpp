#include "analyzer.hpp"
using dataStructure::clas;

analyzer::analyzer()
{
    insideClass = false;
    this->add_class("global"); //global class_of vip
}

analyzer::~analyzer()
{
}
void analyzer::add_element(std::string class_name, std::string members_name, type mem_type)
{
    if (class_table.find(class_name) != class_table.end())
    {
        clas d;
        d;
        class_table[class_name] = d;
    }
    else
    {
        throw SemanticError("class '" + class_name + "' not found .");
    }
}

/*
bool analyzer::find_member(std::string class_name, std::string func_name)
{

    if (class_table.find(class_name) != class_table.end())
    {
        if (func_name == class_table[class_name].first)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        throw SemanticError("class '" + class_name + "' not found .");
    }
    return false;
}

type analyzer::get_member(std::string class_name, std::string func_name)
{
    if (class_table.find(class_name) != class_table.end())
    {
        if (func_name == class_table[class_name].first)
        {
            return class_table[class_name].second;
        }
        else
        {
            throw SemanticError("memeber '" + func_name + "' is undefined");
        }
    }
    else
    {
        throw SemanticError("class '" + class_name + "' not found .");
    }
}
*/
void analyzer::add_class(std::string class_name)
{
    for (const auto l : class_table)
    {
        if (l.first == class_name)
        {
            throw SemanticError("class already declared");
        }
    }

    clas typ;
    class_table.insert({class_name, typ});
}

bool analyzer::evaluvateToken(Token t)
{
    return false;
}
bool analyzer::evaluvateIdentifier(std::string id)
{
    istringstream iss(id);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, '.'))
    {
        if (!token.empty())
            tokens.push_back(token);
    }
    for (size_t i = 0; i < tokens.size() - 1; i++)
    {
        for (const auto l : class_table)
        {
            if (tokens[0] == l.first)
            {
                return true;
            }
        }
    }

    return false;
}