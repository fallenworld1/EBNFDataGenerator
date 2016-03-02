#include "tokens.h"
#include "tree.h"
#include <stack>
#include <iostream>

using namespace std;

void Tree::buildTree(const string &expr, constStrIt &begin)
{
    if(expr.empty()) throw myException("empty token string");
    if(top_)
    {
        top_.reset();
        result_.clear();
    }
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
                readLiteralName(begin,end,temp);
                BasePtr tt = std::make_shared<TextToken>(temp);
                current->setChild(tt);
                break;
            }
            case ',':
            {
                BasePtr ct = std::make_shared<ConcatToken>();
                current->resetChild(ct);
                current = ct;
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
                break;
            }
            case '=':
                name_ = customTokens_.back()->name();
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
                else throw myException("wrong closing brace: ",*begin,begin-expr.begin());

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
                        if(it == end) throw myException("unexpected end of string");
                        ++it;
                    }
                    string name(begin,it);
                    std::shared_ptr<CustomToken> ct = std::make_shared<CustomToken>(name,nullptr);
                    current->setChild(ct);
                    customTokens_.push_back(ct);
                    begin=it-1;
                }
                else throw myException("wrong literal: ",*begin,begin-expr.begin());
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
    if(reGenerate)
    {
        result_.clear();
        if(top_) top_->proc(result_);
        else return false;
    }
    return true;
}

