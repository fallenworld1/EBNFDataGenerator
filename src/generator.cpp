#include "generator.h"
#include <time.h>
Generator::Generator()
{
    srand(clock());
    setMainTokenName("grammar");
}

Generator::Generator(const Generator &other)
{
    result_           = other.result_;
    customTokenTrees_ = other.customTokenTrees_;
    mainTokenName_    = other.mainTokenName_;
}

Generator& Generator::operator=(const Generator &other)
{
    Generator that(other);
    swap(that);
    return *this;
}
Generator::Generator(Generator &&other)
{
    swap(other);
}

Generator& Generator::operator=(Generator &&other)
{
    swap(other);
    return *this;
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

void Generator::setOrTokensProbabilities(const std::string &name, size_t tokenNumber, const std::list<int> &probabilities)
{
    try
    {
        auto &tree = customTokenTrees_.at(name);
        tree->setProbabilities(tokenNumber,probabilities);
    }
    catch(DGException &){throw;}
    catch(...)
    {
        throw DGException("Generator::setOrTokensProbabilities error. Error while setting dictionary. Custom token <"+name+"> not found");
    }
}
void Generator::setSquareBraceProbability(const std::string &name, size_t tokenNumber, int probability)
{
    try
    {
        auto &tree = customTokenTrees_.at(name);
        tree->setProbability(tokenNumber,probability);
    }
    catch(DGException &){throw;}
    catch(...)
    {
        throw DGException("Generator::setSquareBraceProbability error. Error while setting dictionary. Custom token <"+name+"> not found");
    }
}

bool Generator::checkSize(size_t checked,size_t passed)
{
    bool correct = false;
    if(checked<passed)       BaseToken::increaseRanges();
    //else if(checked>passed*11/10) BaseToken::decreaseRanges();
    else correct = true;
    return correct;
}

void Generator::swap(Generator &other)
{
    result_.swap(other.result_);
    customTokenTrees_.swap(other.customTokenTrees_);
    mainTokenName_.swap(other.mainTokenName_);
}



void Generator::generate(const std::string &customTokenName, size_t count, int attemptCout)
{
    TreePtr p_Tree;
    try
    {
        p_Tree = customTokenTrees_.at(customTokenName);
    }
    catch(...)
    {
        throw DGException("Generator::generate error. Not find token: <"+mainTokenName_+">");
    }

    if(!p_Tree)  throw routines::DGException("Generator::generate error. Token "+mainTokenName_+" suddenly have no tree assosiated with");
    do
    {
        if(checkSize(p_Tree->preCount(),count)) break;
    }
    while(--attemptCout>0);
    p_Tree->generate(true);
    result_= p_Tree->getResults();

}
