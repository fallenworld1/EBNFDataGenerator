#include "generator.h"
//#include <time.h>
Generator::Generator()
{
    //setMainTokenName("main");
}


void Generator::setDictionary(const std::string &customTokenName,const StringList &d)
{
    try
    {
        auto &tree = customTokenTrees_.at(customTokenName);
        tree->setDictionary(d);
    }
    catch(...)
    {
        throw DGException("Generator::setDictionary error. Error while setting dictionary. Custom token <"+customTokenName+"> not found");
    }
}

void Generator::setAddingPolicy(const std::string &customTokenName, PolicyPtr policy)
{
    try
    {
        auto &tree = customTokenTrees_.at(customTokenName);
        tree->setPolicy(policy);
    }
    catch(...)
    {
        throw DGException("Generator::setAddingPolicy error. Error while setting dictionary. Custom token <"+customTokenName+"> not found");
    }
}
void Generator::setAddingPolicy(PolicyPtr policy)
{
    for(auto &tree:customTokenTrees_) tree.second->setPolicy(policy);
}

void Generator::setDictionary(const StringList &d)
{
    for(auto &tree:customTokenTrees_) tree.second->setDictionary(d);
}

void Generator::getTokens(const std::string &expr, Parser &parser)
{
    parser.parse(expr);
    customTokenTrees_ = parser.resultsOfParsing();
}

bool Generator::checkSize(size_t checked,size_t passed)
{
    bool correct = false;
    if(checked<passed)       BaseToken::increaseRanges();
    //else if(checked>passed*11/10) BaseToken::decreaseRanges();
    else correct = true;
    return correct;
}

void Generator::generate(size_t count,int attemptCout)
{
    TreePtr tree;
    try
    {
        tree = customTokenTrees_.at(mainTokenName_);
    }
    catch(...)
    {
        throw DGException("Generator::generate error. Not find token: <"+mainTokenName_+">");
    }

    if(!tree)  throw routines::DGException("Generator::generate error. Token "+mainTokenName_+" suddenly have no tree assosiated with");

    if(attemptCout<0)
    {
        tree->generate(true);
        result_= tree->getResults();

    }
    else
    {
        do
        {
            if(checkSize(tree->preCount(),count))
            {
                tree->generate(true);
                result_= tree->getResults();
                return;
            }

        }
        while(--attemptCout>0);
    }
}

void Generator::generate(std::string customTokenName, size_t count, int attemptCout)
{
    routines::MainTokenNameGuard mtng(mainTokenName_);
    mainTokenName_ = customTokenName;
    return generate(count,attemptCout);

}
