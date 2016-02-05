#ifndef TREE_H
#define TREE_H

#include "tokens.h"

class Tree{
    typedef std::list<std::shared_ptr<CustomToken>> CTContainer;

    BasePtr top_;
    ResultType  result_;
    CTContainer customTokens_;

    bool treeValid_;
    size_t recurseDepth_;
public:
    Tree():treeValid_(false),recurseDepth_(0){}

    bool buildTree(const std::string &name);
    bool generate();
    ResultType&  getResults(){return result_;}
    CTContainer& getCustomTokensList(){return customTokens_;}
};

#endif //TREE



