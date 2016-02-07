#include "parser.h"
#include "tree.h"
#include <boost/tokenizer.hpp>
#include <time.h>

void Parser::link()
{//links all custom tokens whith their trees
    for(const auto &tree:customTokenTrees_){
        auto customTokensList = tree.second->getCustomTokensList();
        for(auto &token:customTokensList){
            token->setMain(customTokenTrees_.at(token->name()));
        }
    }
}
//set seed to rand according systime
//and set default main token name
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

//parse nput string to tokens, building trees and link tokens name with its tree
void Parser::parse(const string &expr)
{
    if(expr.empty()) throw myException("empty input");
    customTokenTrees_.clear();
    customTokenStrings_.clear();
    auto begin = std::begin(expr),end = std::end(expr);
    while(begin!=end){
        std::shared_ptr<Tree> mt = std::make_shared<Tree>();
        mt->buildTree(expr,begin);
        customTokenTrees_.insert(make_pair(mt->name(),mt));
    }
    link();
    
    
}
//tokenize input string whith = and ; delimetrs
//vulnerable to = and ; literals
//NOT USED
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
//check if count of generated result in [count*0.9;count*1.1] range
// and adjust generating ranges
bool Parser::checkSize(size_t count)
{
    bool correct = false;
    if(result_.size()<count*9/10)       BaseToken::increaseRanges();
    else if(result_.size()>count*11/10) BaseToken::decreaseRanges();
    else correct = true;
    return correct;
}
// trying to generate ~count(+-10%) results for attemptCout attempts
// if success return true
// else return false(include worong generated count
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

