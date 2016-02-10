#ifndef BASETOKEN_H
#define BASETOKEN_H
#include <list>
#include <memory>
#include "routines.h"

using namespace Routines;

using namespace std;
typedef list<string> ResultType;
class BaseToken;
typedef shared_ptr<BaseToken> BasePtr;
class Tree;
typedef shared_ptr<Tree> TreePtr;

class BaseToken
{
    //static int Count_;
protected:
    enum Setting{MinElemCountToMultiplying = 3,
                DefaultFigureBraceRepeatCount = 5,
                DefaultMaxConcatenationDepth = 3,
                DefaultMaxRecursionDepth = 3};
    static size_t FigureBraceRepeatCount;
    static size_t MaxConcatenationDepth;
    static size_t MaxRecursionDepth;

public:
    BaseToken(){
        //    cout<<"created: "<<++Count_<<endl;
    }

    virtual void setChild(BasePtr child)=0;
    virtual void resetChild(BasePtr other)=0;
    //do specified operation
    virtual void proc(ResultType &rt) = 0;

    virtual ~BaseToken(){
        //  cout<<"Left: "<<--Count_<<endl;
    }
    static void increaseRanges();
    static void decreaseRanges();
};
class CustomToken : public BaseToken
{
    //token for named tokens
    TreePtr tree_;
    string name_;
    size_t recurseDepth_;
    size_t memFBC;
    size_t memMCD;
    size_t memMRD;
public:
    CustomToken(const string &name, TreePtr mt):tree_(mt),name_(name),recurseDepth_(0){save();}
    const string& name(){return name_;}
    void setMain(TreePtr mt){tree_=mt;}
    void setChild(BasePtr){}
    void resetChild(BasePtr){}
    void proc(ResultType &rt);
    void save(){
        memFBC =  FigureBraceRepeatCount;
        memMCD = MaxConcatenationDepth;
        memMRD =  MaxRecursionDepth;
    }

    bool ifChanged(){
        if(memFBC !=  FigureBraceRepeatCount ||
           memMCD != MaxConcatenationDepth||
           memMRD !=  MaxRecursionDepth)
            return false;
        else return true;
    }
};
class OneArgToken:public BaseToken
{
protected:
    BasePtr child_;
public:
    void setChild(BasePtr child){
        child_ = child;
    }
    void resetChild(BasePtr other){
        other->setChild(child_);
        child_ = other;
    }
};
class TwoArgToken:public BaseToken
{
protected:
    BasePtr left_;
    BasePtr right_;
public:
    void setChild(BasePtr child){
        if(left_==nullptr) left_=child;
        else right_ = child;
    }
    void resetChild(BasePtr other){
        other->setChild(this->right_);
        this->right_ = other;
    }
};

class BraceToken:public OneArgToken
{
public:
    //token that need to set pryority of operation
    //simple call child proc
    void proc(ResultType &rt){
        if(child_) child_->proc(rt);
        else throw myException("Arg child_ not set in BraceToken");
    }
};
class ConcatToken : public TwoArgToken
{
public:
    void proc(ResultType &rt);
};

class CopyToken:public BaseToken
{
    ResultType result;
public:
    CopyToken(ResultType &rt):result(rt){}
    void setChild(BasePtr){}
    void resetChild(BasePtr){}
    //copy saved reasul. Its routine for figure braces
    void proc(ResultType &rt){
        rt = result;
    }
};

class TextToken : public BaseToken
{
    std::string text_;
public:
    TextToken(const  std::string &text):text_(text){}

    virtual void setChild(BasePtr){}
    virtual void resetChild(BasePtr){}
    //its a literal, just add text_ to result
    void proc(ResultType &rt){
        if(!text_.empty() && !contain(rt,text_)) rt.push_back(text_);
    }
};

class SquareBraceToken:public OneArgToken{

public:
    //adds 1 or nothing
    void proc(ResultType &rt){
        if(child_){
            child_->proc(rt);
            if(!contain(rt,"")) rt.push_back("");
        }
        else throw myException("Arg child_ not set in SquareBraceToken");
    }
};
class FigureBraceToken:public OneArgToken{
public:
    void proc(ResultType &rt);
};
class OrToken : public TwoArgToken
{
public:
    void proc(ResultType &rt){
        if(left_ && right_){
            left_->proc(rt);
            right_->proc(rt);
        }
        else throw myException("args not set in OrToken");
    }
};

#endif // BASETOKEN_H
