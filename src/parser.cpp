#include "parser.h"
#include "tree.h"
#include <time.h>

void Parser::link()
{
    for(const auto &tree:customTokenTrees_)
    {
        auto customTokensList = tree.second->getCustomTokensList();
        for(auto &token:customTokensList)
        {
            token->setMain(customTokenTrees_.at(token->name()));
        }
    }
}

Parser::Parser()
{
    srand(clock());
    setMainTokenName("main");
}
//singletone
Parser &Parser::getParser()
{
    static Parser p;
    return p;
}


void Parser::parse(const string &expr)
{
    if(expr.empty()) throw myException("empty input");
    customTokenTrees_.clear();
    auto begin = std::begin(expr),end = std::end(expr);
    while(begin!=end)
    {
        std::shared_ptr<Tree> mt = std::make_shared<Tree>();
        mt->buildTree(expr,begin);
        customTokenTrees_.insert(make_pair(mt->name(),mt));
    }
    link();
    
    
}

bool Parser::checkSize(size_t checked,size_t passed)
{
    bool correct = false;
    if(checked<passed*9/10)       BaseToken::increaseRanges();
    else if(checked>passed*11/10) BaseToken::decreaseRanges();
    else correct = true;
    return correct;
}

bool Parser::generate(size_t count,int attemptCout)
{
    TreePtr tree;
    try
    {
        tree = customTokenTrees_.at(mainTokenName_);
    }
    catch(...)
    {
        throw myException("Not find token: <"+mainTokenName_+"> May be grammar have custom main token name?");
    }

    do
    {
        if(checkSize(tree->preCount(),count))
        {
            if(!tree->generate(true)) throw myException("Generating error");
            result_= tree->getResults();
            return true;
        }

    }
    while(--attemptCout>0);
    return false;
}

