#ifndef TREE_H
#define TREE_H

#include "tokens.h"

class Tree
{
    typedef std::list<std::shared_ptr<CustomToken>> CTContainer;//Custom Tokens Container

    BasePtr     top_;
    ResultType  result_;
    CTContainer customTokens_;
    string      name_;// current tree name, for linking
    bool        treeValid_;

public:
    Tree():treeValid_(false){}
    size_t preCount(){ return top_->preCount();}
    bool buildTree(const std::string &name, constStrIt &begin);
    bool generate(bool reGenerate);

    ResultType&         getResults()         {  return result_;         }
    CTContainer&        getCustomTokensList(){  return customTokens_;   }
    const std::string & name()               {  return name_;           }
};

#endif //TREE



