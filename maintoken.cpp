#include "figurebracetoken.h"
#include "squarebracetoken.h"
#include "ortoken.h"
#include "texttoken.h"
#include "concattoken.h"
#include "customtoken.h"
#include "maintoken.h"

#include <stack>
#include <iostream>


//int BaseToken::Count_ = 0;

int BaseToken::FigureBraceRepeatCount=5;
int BaseToken::ConcatenationDepth=5;
 int BaseToken::RecursionDepth=5;
using namespace std;


bool MainToken::buildTree(const string &expr)
{
    if(expr.empty()) throw logic_error("empty token string");
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
                   customTokens.push_back(ct);
                   --it;
                }
                else throw logic_error("wrong literal");
            }
            ++it;
        }
    }
    catch(exception &le){
        //cout<<le.what()<<endl;
        treeValid_  =false;
        throw le;
    }
    catch(int i){//?????
        cout<<i;
        treeValid_  =false;
    }
    return treeValid_;
}

bool MainToken::generate()
{
    if(!treeValid_) return false;
    ++recurseDepth_;
    result_.clear();
    if(recurseDepth_>BaseToken::RecursionDepth){
        --recurseDepth_;
            return true;
    }

    if(top_) top_->doProc(result_);
    --recurseDepth_;
    if(result_.empty()) return false;
    return true;
}


MainToken::~MainToken(){}//if(top_)delete top_;}
