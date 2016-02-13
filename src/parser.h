#ifndef PARSER_H
#define PARSER_H

#include "tree.h"
#include <map>



class Parser
{
    bool tokenized_;
    ResultType result_;
    map<string,string>  customTokenStrings_;
    map<string,TreePtr> customTokenTrees_;
    string mainTokenName_;

    void link();
    bool checkSize(size_t checked, size_t passed);

    Parser();
    Parser(Parser &other);
    Parser& operator=(Parser& other);
public:
   static Parser& getParser();

    bool generate(size_t count, int attemptCout = 100);
    void setMainTokenName(const string &s){if(s.size()) mainTokenName_ = s;}
    void parse(const std::string &expr);

    const ResultType&  getResults(){return result_;}
};

#endif // PARSER_H
