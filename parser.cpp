#include "parser.h"
#include "tree.h"
#include <boost/tokenizer.hpp>
#include <time.h>

void Parser::link()
{//links all custom tokens whith their trees
    for(const auto &p:customTokenTrees_){
        auto customTokensList = p.second->getCustomTokensList();
        for(auto &p:customTokensList){
            p->setMain(customTokenTrees_.at(p->name()));
        }
    }
}

Parser::Parser()
{
    srand(clock());
    setMainTokenName("main");
}

Parser &Parser::getParser()
{
    static Parser p;
    return p;
}


void Parser::parse(const string &expr)
{
    customTokenTrees_.clear();
    customTokenStrings_.clear();
    tokenize(expr);
    for(const auto &p:customTokenStrings_){
        std::shared_ptr<Tree> mt = std::make_shared<Tree>();
        mt->buildTree(p.second);
        customTokenTrees_.insert(make_pair(p.first,mt));
    }
    link();
    
    
}

void Parser::tokenize(const string &expr)
{
    using namespace boost;
    typedef tokenizer<char_separator<char> > tokenizer;
    
    if(expr.empty()) throw myException("empty input");
    tokenized_ = true;
    char_separator<char> sep("=;");
    tokenizer t(expr,sep);
    tokenizer::iterator it = t.begin(),end = t.end();
    try{
        for (;it !=end ; ++it){
            string temp = *it++;
            if(it == end) throw myException("error in input syntax");
            removeSpaces(temp);
            customTokenStrings_.insert(make_pair(temp,*it));
        }
    }catch(std::exception &e){

        throw myException(e.what()+*it);
    }
}

bool Parser::checkSize(size_t count)
{
    bool correct = false;
    if(result_.size()<count*9/10)       BaseToken::increaseRanges();
    else if(result_.size()>count*11/10) BaseToken::decreaseRanges();
    else correct = true;
    return correct;
}
bool Parser::generate(size_t count,int attemptCout)
{
    TreePtr tree;
    try{
        tree = customTokenTrees_.at(mainTokenName_);
    }
    catch(exception &e){
        throw myException("Not find token: <"+mainTokenName_+"> May be grammar have custom main token name?");
    }

    do{
        if(tree->generate(true)){
            result_= tree->getResults();
            if(checkSize(count))return true;
        }else return false;
    }while(--attemptCout>0);
    return false;
}

