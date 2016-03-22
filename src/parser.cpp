#include "parser.h"
#include "treebuilder.h"
void Parser::link()
{
    for(const auto &tree:customTokenTrees_)
    {
        auto customTokensList = tree.second->getCustomTokensList();
        for(auto &token:customTokensList)
        {
            try
            {
                token->setMain(customTokenTrees_.at(token->name()));
            }
            catch(...)
            {
                throw DGException("Parser::link error. Token <"+token->name()+"> not defined.");
            }
        }
    }
}
void Parser::parse(const std::string &expr)
{
    if(expr.empty()) throw DGException("Input string is empty!");
    customTokenTrees_.clear();
    auto begin = std::begin(expr),end = std::end(expr);
    TreeBuilder builder;
    while(begin!=end)
    {
        TreePtr mt = builder.buildTree(expr,begin);
        if(mt)
        {
            auto name = mt->name();
            if(name.size())
            {
                auto result = customTokenTrees_.insert(make_pair(name,mt));
                if(result.second == false) throw DGException("Parser::parse error. Token <"+name+"> is implemented more than once.");
            }
        }
        else break;
    }
    link();
    
    
}
/*void Parser::parse2(const std::string &expr)
{
    if(expr.empty()) throw DGException("Input string is empty!");
    customTokenTrees_.clear();
    auto begin = std::begin(expr),end = std::end(expr);
    while(begin!=end)
    {
        std::shared_ptr<Tree> mt = std::make_shared<Tree>();
        mt->buildTreeWithBuilder(expr,begin);
        auto result = customTokenTrees_.insert(make_pair(mt->name(),mt));
        if(result.second == false) throw DGException("Token <"+mt->name()+"> is implemented more than once!");
    }
    link();


}*/

