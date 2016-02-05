#include "figurebracetoken.h"
#include "squarebracetoken.h"
#include "ortoken.h"
#include "texttoken.h"
#include "concattoken.h"
#include "tokens.h"

#include <stack>
#include <iostream>

//int BaseToken::Count_ = 0;

using namespace std;

bool MainToken::globalParse(const string &expr)
{
    parsed_ = true;

    //int customTokenNum = 1;
    string customTokenName, customTokenDef;
    auto it = begin(expr),end=std::end(expr);
    do{
        auto mem = it;
        while(*it!=L'='&& it!=end)++it;
        if(it == end) break;
        customTokenName.append(mem,it);
        customTokenName.erase(remove_if(begin(customTokenName),std::end(customTokenName), iswspace),std::end(customTokenName));
        ++it;

        mem = it;
        while(*it!=L';'&& it!=end)++it;
        if(it == end) break;
        customTokenDef.append(mem,it);
         ++it;

        customTokens_.insert(make_pair(customTokenName,customTokenDef));
        customTokenName.clear();
        customTokenDef.clear();
    }while(it!=end);
    //replace(begin(expr),end(expr),L"main","$0");

    return parsed_;
}

bool MainToken::buildTree(string &expr)
{
    if(!parsed_) return false;
    if(top_){
        delete top_;
        top_ = nullptr;
        result_.clear();
    }
    treeValid_ = true;
    typedef string::iterator it_type;
    stack<BaseToken*> tokenStack;
    it_type it = expr.begin(),end = expr.end();
    BaseToken* current = new BraceToken;
    top_ = current;
    try{
        while(it!=end){
            switch(*it){
            case '\"':{
                string temp;
                while(*(++it)!='\"') temp.push_back(*it);
                TextToken *tt = new TextToken(temp);
                current->setChild(tt);
                break;
            }
            case ',':{
                ConcatToken *ct = new ConcatToken;
                current->resetChild(ct);
                current = ct;
                break;
            }
            case '|':{
                OrToken *ct = new OrToken;
                current->resetChild(ct);
                current = ct;
                break;
            }
            case '(':{
                BraceToken *bt = new BraceToken;
                tokenStack.push(current);
                current->setChild(bt);
                current = bt;
                break;
            }
            case '[':{
                SquareBraceToken *sbt = new SquareBraceToken;
                tokenStack.push(current);
                current->setChild(sbt);
                current = sbt;
                break;
            }
            case '{':{
                FigureBraceToken *fbt = new FigureBraceToken;
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
                throw logic_error("wrong literal\n");
            }
            ++it;
        }
    }
    catch(logic_error &le){
        cout<<le.what();
        treeValid_  =false;
    }
    catch(int i){
        cout<<i;
        treeValid_  =false;
    }

    return treeValid_;
}

bool MainToken::generate()
{
    if(!treeValid_) return false;
    if(top_) top_->doProc(result_);
    else return false;

    if(result_.empty()) return false;
    return true;
}

MainToken::~MainToken(){if(top_)delete top_;}
