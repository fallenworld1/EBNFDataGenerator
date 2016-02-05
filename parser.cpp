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
bool Parser::generate()
{
    auto mainToken = customTokenTrees_.at("main");
    if(mainToken->generate()){
        result_= mainToken->getResults();
        return true;
    }else return false;
}
