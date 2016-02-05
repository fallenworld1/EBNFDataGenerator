#include "parser.h"
#include "maintoken.h"
#include <boost/tokenizer.hpp>
#include <time.h>

bool Parser::link()
{

    for(const auto &p:customTokenTrees_){
        auto customTokensList = p.second->getCustomTokensList();
        for(auto &p:customTokensList){
            p->setMain(customTokenTrees_.at(p->name()));
        }

    }

    return true;
}

Parser::Parser()
{
    srand(clock());
    BaseToken::ConcatenationDepth = 5;
    BaseToken::FigureBraceRepeatCount = 5;
    BaseToken::RecursionDepth = 5;
}

Parser &Parser::getParser()
{
    static Parser p;
    return p;
}

Parser::~Parser()
{
    // delete customTokenTrees_["main"];
}

bool Parser::customParse(const string &expr)
{
    customTokenTrees_.clear();
    customTokens_.clear();
    if(!tokenize(expr)) throw logic_error("Syntax error");
    for(const auto &p:customTokens_){
        std::shared_ptr<MainToken> mt = std::make_shared<MainToken>();
        mt->buildTree(p.second);
        customTokenTrees_.insert(make_pair(p.first,mt));
    }
    return link();


}

bool Parser::tokenize(const string &expr)
{
    if(expr.empty()) throw logic_error("empty input");
    using namespace boost;
    typedef tokenizer<boost::char_separator<char> >
            tokenizer;
    try{
        tokenized_ = true;
        boost::char_separator<char> sep("=;");
        tokenizer t(expr,sep);
        for (tokenizer::iterator tok_iter = t.begin();
             tok_iter != t.end(); ++tok_iter){
            string temp = *tok_iter++;
            removeSpaces(temp);
            customTokens_.insert(make_pair(temp,*tok_iter));
        }
    }catch(...){
        tokenized_=false;
    }

    return tokenized_;
}
bool Parser::generate(size_t count,int attemptCout)
{

    auto mainToken = customTokenTrees_.at("main");
    do{
        if(mainToken->generate()){
            result_= mainToken->getResults();
            if(result_.size()<count*9/10) increaseRanges();
            else if(result_.size()>count*11/10) decreaseRanges();
            else return true;

        }else return false;
    }while(--attemptCout);
    return true;
}
void Parser::increaseRanges(){
    ++BaseToken::ConcatenationDepth;
    ++BaseToken::FigureBraceRepeatCount;
    ++BaseToken::RecursionDepth;
}
void Parser::decreaseRanges(){
    if(BaseToken::ConcatenationDepth>1)--BaseToken::ConcatenationDepth;
    if(BaseToken::FigureBraceRepeatCount)--BaseToken::FigureBraceRepeatCount;
    if(BaseToken::RecursionDepth>1)--BaseToken::RecursionDepth;
}
