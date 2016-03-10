#include "tokens.h"
#include "tree.h"
#include <stack>
#include <iostream>

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
     if(it!=end(result_)) result_.erase(it);
                  //[this](const std::string &elem)->bool{return elem.size()>maxResultLength_;});
     result_.insert(std::end(result_),begin(dictionary_),end(dictionary_));

}

void Tree::buildTree(const string &expr, constStrIt &begin)
{
    if(expr.empty()) throw DGException("empty token string");

    treeValid_ = true;
    stack<BasePtr> tokenStack;
    auto end = std::end(expr);
    BasePtr current = std::make_shared<RoundBraceToken>();
    top_ = current;

    try
    {
        while(begin!=end)
        {
            switch(*begin)
            {
            case '\"':
            {
                string temp;
                ++begin;
                routines::readLiteralName(begin,end,temp);
                BasePtr tt = std::make_shared<TextToken>(temp);
                current->setChild(tt);
                break;
            }
            case ',':
            {
                BasePtr ct = std::make_shared<ConcatToken>();
                current->resetChild(ct);
                current = ct;
                canChange_=true;
                break;
            }
            case '|':
            {
                BasePtr ct = std::make_shared<OrToken>();
                current->resetChild(ct);
                current = ct;
                break;
            }
            case '(':
            {
                BasePtr bt = std::make_shared<RoundBraceToken>();
                tokenStack.push(current);
                current->setChild(bt);
                current = bt;
                break;
            }
            case '[':
            {
                BasePtr sbt = std::make_shared<SquareBraceToken>();
                tokenStack.push(current);
                current->setChild(sbt);
                current = sbt;
                break;
            }
            case '{':
            {
                BasePtr fbt = std::make_shared<FigureBraceToken>();
                tokenStack.push(current);
                current->setChild(fbt);
                current = fbt;
                canChange_=true;
                break;
            }
            case '=':
                name_ = customTokens_.back()->name();
                canChange_=false;
                break;
            case ';':
                ++begin;
                return;
                break;
            case ')':case']':case'}':
            {
                BasePtr last = tokenStack.top();
                if(last->checkChildType(*begin))
                {
                    current = last;
                    tokenStack.pop();

                }
                else throw DGException("wrong closing brace: ",*begin,begin-expr.begin());

               // current = tokenStack.top();
               // tokenStack.pop();

                break;
            }
            case ' ':case 0:break;
            default:
                if(isalnum(*begin)||*begin=='_')
                {//probably custom token name
                    auto it = begin;
                    while(isalnum(*it)||*it=='_')
                    {
                        if(it == end) throw DGException("Unexpected end of string.");
                        ++it;
                    }
                    string name(begin,it);
                    auto ct = std::make_shared<CustomToken>(name,nullptr);
                    current->setChild(ct);
                    customTokens_.push_back(ct);
                    begin=it-1;
                    canChange_=true;//TODO add smart changing flag, checks for changing ability of all custom tokens after full parsing
                }
                else throw DGException("Wrong literal: ",*begin,begin-expr.begin());
            }
            ++begin;
        }
    }
    catch(...)
    {
        treeValid_  =false;
        throw;
    }
}

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
            if(!canChange_) adjustResults();
        }
    }
    return true;
}



