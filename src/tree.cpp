#include "tokens.h"
#include "tree.h"
#include "treebuilder.h"
//#include <stack>
#include <iostream>
using namespace std;
const bool TOKENS_MAP[7][7] =
//CBRACE,NAME,EQUAL,T orC,OBRACE,OPER,END
{{true ,false,false,false,false,true ,true }, //CBRACE
 {false,false,true ,false,false,false,false}, //NAME
 {false,false,false,true ,true ,false,false}, //EQUAL
 {true ,false,true ,false,false,true ,true }, //T orC
 {false,false,false,true ,true ,false,false}, //OBRACE
 {false,false,false,true ,true ,false,false}, //OPER
 {false,true ,false,false,false,false,false}};//END


void Tree::adjustResults(StringList &rt)
{
    addingPolicy_->refresh();
    /* result_.remove_if(
                 [this](const std::string &elem)->bool{return !addingPolicy_->check(elem);}
     );*/
    auto it = std::remove_if(
                begin(rt),
                end(rt),
                [this](const std::string &elem)->bool{return !addingPolicy_->check(elem);}
    );
    if(it!=end(rt)) rt.erase(it,end(rt));

    rt.insert(std::end(rt),begin(dictionary_),end(dictionary_));

}

/*bool Tree::sequenceControl(Tree::TokenType &prev, const Tree::TokenType &current)
{
    bool result = TOKENS_MAP[(int)prev][(int)current];
    prev = current;
    return result;
}*/

/*void Tree::buildTree(const string &expr, ConstStrIt &begin)
{
    if(expr.empty()) throw DGException("Tree::buildTree error. Empty token string.");
    refresh();

    TokenType lastTokenType = TokenType::OBRACE;
    stack<BasePtr> tokenStack;
    auto end = std::end(expr);
    // auto first =  std::make_shared<RoundBraceToken>();
    BasePtr current =  std::make_shared<RoundBraceToken>();
    BasePtr lastToken;
    //TopTokenGuard tg(current,top_);
    //top_ = current;
    auto checkType = [&](const Tree::TokenType &current)
    {
        if(!sequenceControl(lastTokenType,current))
            throw DGException("Tree::buildTree error. Unexpected occurence of: ",*begin,begin-std::begin(expr));
    };

    try
    {
        while(1)
        {
            if(begin == end)
            {
                if(tokenStack.size() || name_.size() || (bool)lastToken || customTokens_.size())
                    throw DGException("Tree::buildTree error. Unexpected end of string.");
                else return;
            }
            switch(*begin)
            {
            case Literals::QUOTE:
            {
                string temp;
                ++begin;
                routines::readLiteralName(begin,end,temp);//thows or put begin at closing "
                checkType(TokenType::TEXT_CUSTOM);
                lastToken = std::make_shared<TextToken>(temp);
                current->setChild(lastToken);
                break;
            }
            case Literals::CONCAT:
                checkType(TokenType::OPERATION);
                lastToken = std::make_shared<ConcatToken>();
                if(current->checkType(',') || current->checkType('|')) lastToken->setChild(current);
                else current->resetChild(lastToken);
                current = lastToken;
                //canChange_=true;
                break;
            case Literals::OR:
                checkType(TokenType::OPERATION);
                if(!current->checkType('|'))
                {
                    orTokens_.push_back(std::make_shared<OrToken>());
                    lastToken = orTokens_.back();
                    if(current->checkType(','))lastToken->setChild(current);
                    else current->resetChild(lastToken);
                    current = lastToken;
                }
                break;
            case Literals::OPEN_ROUND_BRACE:
                checkType(TokenType::OBRACE);
                lastToken = std::make_shared<RoundBraceToken>();
                tokenStack.push(current);
                current->setChild(lastToken);
                current = lastToken;

                break;
            case Literals::OPEN_SQUARE_BRACE:
                checkType(TokenType::OBRACE);
                SBTokens_.push_back(std::make_shared<SquareBraceToken>());
                lastToken = SBTokens_.back();
                tokenStack.push(current);
                current->setChild(lastToken);
                current = lastToken;
                break;
            case Literals::OPEN_FIGURE_BRACE:
                checkType(TokenType::OBRACE);
                lastToken = std::make_shared<FigureBraceToken>();
                tokenStack.push(current);
                current->setChild(lastToken);
                current = lastToken;
                //canChange_=true;
                break;
            case Literals::NAME_BODY_DELIMETR:
                if(!name_.empty()) throw DGException("Tree::buildTree error. Unexpected occurence of: ",*begin,begin-std::begin(expr));
                if(customTokens_.size()>1) throw DGException(string("Tree::buildTree error. Wrong syntax near "+customTokens_.back()->name()),'-',begin-std::begin(expr));
                checkType(TokenType::EQUAL);
                name_ = customTokens_.back()->name();
                //canChange_=false;
                break;
            case Literals::TOKENS_DELIMETR1: case Literals::TOKENS_DELIMETR2:
                checkType(TokenType::END);
                if(!tokenStack.empty()){
                    lastToken = tokenStack.top();
                    char c='(';
                    if(!lastToken->checkType(c))
                    {
                        c='[';
                        if(!lastToken->checkType(c))c='{';
                    }

                    throw DGException("Tree::buildTree error. Unterminated brace in string",c,begin-std::begin(expr));
                }
                ++begin;
                treeValid_ = true;
                top_ = current;
                return;
                break;
            case Literals::CRBRACE:case Literals::CSBRACE:case Literals::CFBRACE:
                checkType(TokenType::CBRACE);
                if(tokenStack.empty()) throw DGException("Tree::buildTree error. Unexpected closing brace: ",*begin,begin-std::begin(expr));
                lastToken = tokenStack.top();
                if(lastToken->checkChildType(*begin))
                {
                    current = lastToken;
                    tokenStack.pop();
                }
                else throw DGException("Tree::buildTree error. Wrong closing brace: ",*begin,begin-std::begin(expr));
                break;
            case Literals::COMMENT:
                while(++begin != end && (Literals)*begin != Literals::COMMENT);
                break;
            case Literals::SPACE:case Literals::TAB:case Literals::CARRIAGE_RETURN_WIN:case Literals::CARRIAGE_RETURN:/*case 0:*/
               /* break;
            default:
                auto it = begin;
                checkType(TokenType::TEXT_CUSTOM);
                while(it != end && (isalnum(*it) || (Literals)*it == Literals::UNDERSCORE)) ++it;
                string name(begin,it);

                if(name.empty()) throw DGException("Tree::buildTree error. Wrong literal in token name: ",*begin,begin-std::begin(expr));

                auto ct = std::make_shared<CustomToken>(name,nullptr);
                current->setChild(ct);
                customTokens_.push_back(ct);
                begin=it-1;
            }
            ++begin;
        }
    }
    catch(...)
    {
        refresh();
        throw;
    }
}*/

/*void Tree::buildTree(const string &expr, ConstStrIt &begin)
{
    TreeBuilder builder(customTokens_,orTokens_,SBTokens_,name_,treeValid_,top_);
    builder.buildTree(expr,begin);
}*/

void Tree::generate(bool reGenerate)
{
    if(!treeValid_) throw DGException("Tree::generate error. Tree " + name_ + " are not valid.");
    //if(/*canChange_ ||*/ result_.empty())
    // {
    if(reGenerate)
    {
        result_.clear();
        if(top_)
        {
            //result_(top_->preCount());
            top_->proc(result_);
        }
        else throw DGException("Tree::generate error. Tree " + name_ + " pointer to top is NULL.");
        adjustResults(result_);
    }
    // }
}
void Tree::generateAndGet(StringList &rt)
{

    if (!treeValid_) throw DGException("Tree::generateAndGet error. Tree " + name_ + " not valid");
    //if (canChange_)
    // {
    if (top_) top_->proc(result_);
    else throw DGException("Tree::generateAndGet error. Tree " + name_ + " had empty top");
    adjustResults(rt);
    //}
    //else
    // {
    //    if (result_.empty()) generate(true);
    //     rt = result_;
    //}



}

void Tree::refresh()
{
    result_.clear();
    customTokens_.clear();
    treeValid_ = false;
    //canChange_ = false;
    name_.clear();
    dictionary_.clear();
    addingPolicy_ = std::make_shared<DefaultPolicy>();

}

void Tree::setProbabilities(size_t tokenNumber, const std::list<int> &probabilities)
{
    if(orTokens_.size()<=tokenNumber) throw DGException("Tree::setProbabilities error. Wrong OrTokenNumber");
    orTokens_.at(tokenNumber)->setProbabilities(probabilities);
}

void Tree::setProbability(size_t tokenNumber, int probability)
{
    if(SBTokens_.size()<=tokenNumber) throw DGException("Tree::setProbability error. Wrong SquareBraceTokenNumber");
    SBTokens_.at(tokenNumber)->setProbability(probability);
}



