#ifndef PARSER_H
#define PARSER_H

#include "maintoken.h"
#include <map>
template<class String>
void removeSpaces(String &str){
    using namespace std;
    str.erase(remove_if(begin(str),end(str),[](typename String::value_type c){return std::isspace(c);}),end(str));
}

class Parser
{
    bool treeValid_;
    bool tokenized_;
    ResultType result_;
    map<string,string> customTokens_;
    map<string,MainPtr> customTokenTrees_;
    bool link();
     Parser();
     void decreaseRanges();
     void increaseRanges();
public:
   static Parser& getParser();
    ~Parser();
    bool  generate(size_t count, int attemptCout = 500);

    bool  customParse(const std::string &expr);

    const ResultType&  getResults(){return result_;}

    bool tokenize(const string &expr);

};

#endif // PARSER_H
