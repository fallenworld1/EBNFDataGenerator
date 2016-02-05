#include "tokens.h"
#include "tree.h"

#include <stack>
#include <iostream>



using namespace std;


bool Tree::buildTree(const string &expr)
{
    if(expr.empty()) throw myException("empty token string");
    if(top_){
        top_.reset();
        result_.clear();
    }
    treeValid_ = true;
    //typedef string::iterator it_type;
    stack<BasePtr> tokenStack;
    auto it = expr.begin(),end = expr.end();
    BasePtr current = std::make_shared<BraceToken>();
    top_ = current;
    try{
        while(it!=end){
            switch(*it){
            case '\"':{
                string temp;
                while(*(++it)!='\"') temp.push_back(*it);
                BasePtr tt = std::make_shared<TextToken>(temp);
                current->setChild(tt);
                break;
            }
            case ',':{
                BasePtr ct = std::make_shared<ConcatToken>();
                current->resetChild(ct);
                current = ct;
                break;
            }
            case '|':{
                BasePtr ct = std::make_shared<OrToken>();
                current->resetChild(ct);
                current = ct;
                break;
            }
            case '(':{
                BasePtr bt = std::make_shared<BraceToken>();
                tokenStack.push(current);
                current->setChild(bt);
                current = bt;
                break;
            }
            case '[':{
                BasePtr sbt = std::make_shared<SquareBraceToken>();
                tokenStack.push(current);
                current->setChild(sbt);
                current = sbt;
                break;
            }
            case '{':{
                BasePtr fbt = std::make_shared<FigureBraceToken>();
                tokenStack.push(current);
                current->setChild(fbt);
                current = fbt;
                break;
            }
            case ')':case']':case'}':
                current = tokenStack.top();
                tokenStack.pop();
                break;
            case ' ':case 0:break;
            default:
                if(std::isalnum(*it)||*it=='_'){//probably custom token name
                   auto begin = it;
                   while(std::isalnum(*it)||*it=='_')++it;
                   string name(begin,it);
                   std::shared_ptr<CustomToken> ct = std::make_shared<CustomToken>(name,nullptr);
                   current->setChild(ct);
                   customTokens_.push_back(ct);
                   --it;
                }
                else throw myException("wrong literal: ",*it);
            }
            ++it;
        }
    }
    catch(myException &le){
        //cout<<le.what()<<endl;
        treeValid_  =false;
        throw le;
    }
    return treeValid_;
}

bool Tree::generate()
{
    if(!treeValid_) return false;
    result_.clear();
    if(top_) top_->proc(result_);
    return true;
}

