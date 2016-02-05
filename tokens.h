#ifndef TOKENS
#define TOKENS

#include "basetoken.h"
#include <map>
class MainToken{
    BaseToken *top_;
    bool treeValid_;
    bool parsed_;
    ResultType result_;
    map<string,string> customTokens_;




public:


    MainToken():top_(nullptr),treeValid_(false),parsed_(true){}

    bool               buildTree(std::string &expr);
    bool               generate();

     bool globalParse(const std::string &expr);

    const ResultType&  getResults(){return result_;}
    ~MainToken();

};

#endif // TOKENS



