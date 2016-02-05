#ifndef TOKENS
#define TOKENS

#include "basetoken.h"

#include <map>

#include "customtoken.h"

class MainToken{
    BasePtr top_;
    bool treeValid_;
    ResultType result_;
    std::list<std::shared_ptr<CustomToken> > customTokens;
    int recurseDepth_;
public:
    MainToken():top_(nullptr),treeValid_(false),recurseDepth_(0){}

    bool buildTree(const std::string &name);
    bool generate();
    ResultType& getResults(){return result_;}
    std::list<std::shared_ptr<CustomToken> > &getCustomTokensList(){return customTokens;}
    ~MainToken();


};

#endif // TOKENS



