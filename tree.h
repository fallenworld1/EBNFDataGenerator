#ifndef TREE_H
#define TREE_H

#include "tokens.h"

class Tree{
    typedef std::list<std::shared_ptr<CustomToken>> CTContainer;

    BasePtr top_;
    ResultType  result_;
    CTContainer customTokens_;
    string name_;
    bool treeValid_;
    size_t recurseDepth_;

public:
    Tree():treeValid_(false),recurseDepth_(0){}

    bool buildTree(const std::string &name, constStrIt &begin);
    bool generate(bool reGenerate);
    ResultType&  getResults(){return result_;}
    CTContainer& getCustomTokensList(){return customTokens_;}
    const std::string & name(){return name_;}
};

#endif //TREE



