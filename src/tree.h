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
    /**
     * @brief preCount
     * @return rough count of results that tree can produced
     */
    size_t preCount(){ return top_->preCount();}
    /**
     * @brief buildTree
     * @param expr
     * @param begin
     *
     * builds tree from pos @a begin  of @a expr to end(@a expr) or ';'
     */
    void buildTree(const std::string &expr, constStrIt &begin);
    /**
     * @brief generate
     * @param reGenerate
     * @return
     * generates results if @a reGenerate true
     */
    bool generate(bool reGenerate);

    ResultType&         getResults()         {  return result_;         }
    CTContainer&        getCustomTokensList(){  return customTokens_;   }
    const std::string & name()               {  return name_;           }
};

#endif //TREE



