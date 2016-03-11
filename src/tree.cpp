#include "tokens.h"
#include "tree.h"
#include <stack>
#include <iostream>

#include "treebuilder.h"
using namespace std;



void Tree::adjustResults()
{
    addingPolicy_->refresh();
    /* result_.remove_if(
                 [this](const std::string &elem)->bool{return !addingPolicy_->check(elem);}
     );*/
    auto it = std::remove_if(
                begin(result_),
                end(result_),
                [this](const std::string &elem)->bool{return !addingPolicy_->check(elem);}
    );
    if(it!=end(result_)) result_.erase(it,end(result_));
    //[this](const std::string &elem)->bool{return elem.size()>maxResultLength_;});
    result_.insert(std::end(result_),begin(dictionary_),end(dictionary_));

}

void Tree::buildTree(const string &expr, ConstStrIt &begin)
{
    if(expr.empty()) throw DGException("empty token string");
    customTokens_.clear();
    treeValid_ = true;

    stack<BasePtr> tokenStack;
    auto end = std::end(expr);

    BasePtr current = std::make_shared<RoundBraceToken>();
    BasePtr lastToken;
    top_ = current;
    try
    {
        while(1)
        {
            if(begin == end) throw DGException("Unexpected end of string.");

            switch(*begin)
            {
            case QUOTES:
            {
                //(this->*(operations[*begin]))(begin);
                string temp;
                ++begin;
                routines::readLiteralName(begin,end,temp);//thows or put begin at closing "
                lastToken = std::make_shared<TextToken>(temp);
                current->setChild(lastToken);
                break;
            }
            case CONCAT:
                lastToken = std::make_shared<ConcatToken>();
                current->resetChild(lastToken);
                current = lastToken;
                canChange_=true;
                break;
            case OR:
                lastToken = std::make_shared<OrToken>();
                current->resetChild(lastToken);
                current = lastToken;
                break;
            case OPEN_ROUND_BRACE:
                lastToken = std::make_shared<RoundBraceToken>();
                tokenStack.push(current);
                current->setChild(lastToken);
                current = lastToken;
                break;
            case OPEN_SQUARE_BRACE:
                lastToken = std::make_shared<SquareBraceToken>();
                tokenStack.push(current);
                current->setChild(lastToken);
                current = lastToken;
                break;
            case OPEN_FIGURE_BRACE:
                lastToken = std::make_shared<FigureBraceToken>();
                tokenStack.push(current);
                current->setChild(lastToken);
                current = lastToken;
                canChange_=true;
                break;
            case NAME_BODY_DELIMETR:
                if(!name_.empty()) throw DGException("Unexpected occurence of: ",*begin,begin-std::begin(expr));
                if(customTokens_.size()>1) throw DGException("Wrong syntax near "+customTokens_.back()->name());
                name_ = customTokens_.back()->name();
                canChange_=false;
                break;
            case TOKENS_DELIMETR:
                ++begin;
                return;
                break;
            case CRBRACE:case CSBRACE:case CFBRACE:
                lastToken = tokenStack.top();
                if(lastToken->checkChildType(*begin))
                {
                    current = lastToken;
                    tokenStack.pop();
                }
                else throw DGException("Wrong closing brace: ",*begin,begin-std::begin(expr));
                break;
            case COMMENT:
                while(++begin != end && *begin != COMMENT);
                break;
            case SPACE:case TAB:case CARRIAGE_RETURN_WIN:case CARRIAGE_RETURN:/*case 0:*/
                break;
            default:
                if(isalnum(*begin)||*begin==UNDERSCORE)
                {//probably custom token name
                    auto it = begin;
                    do
                    {
                        if(++it == end)break;// throw DGException("Unexpected end of string.",*it,it-std::begin(expr));
                    }
                    while(isalnum(*it)||*it==UNDERSCORE);
                    string name(begin,it);
                    auto ct = std::make_shared<CustomToken>(name,nullptr);
                    current->setChild(ct);
                    customTokens_.push_back(ct);
                    begin=it-1;
                    canChange_=true;//TODO add smart changing flag, checks for changing ability of all custom tokens after full parsing
                }
                else throw DGException("Wrong literal: ",*begin,begin-std::begin(expr));
            }
            ++begin;
        }
    }
    catch(...)
    {
        treeValid_  = false;
        throw;
    }
}

/*void Tree::buildTreeWithBuilder(const string &expr, ConstStrIt &begin)
{
    const TreeBuilder& builder = TreeBuilder::getBuilder();
    builder.buildTree(*this,expr,begin);
}*/

bool Tree::generate(bool reGenerate)
{
    if(!treeValid_) return false;
    if(canChange_ || result_.empty())
    {
        if(reGenerate)
        {
            result_.clear();
            if(top_) top_->proc(result_);
            else return false;
            if(canChange_) adjustResults();
        }
    }
    return true;
}



