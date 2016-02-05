#ifndef BASETOKEN_H
#define BASETOKEN_H
#include <string>
#include <stdexcept>
#include <list>
#include <algorithm>
#include <memory>
#include <iostream>




template<class IterType>
bool advance_(IterType &it, IterType &end,size_t count){
   if(it==end)return false;
    while(count){
        ++it;
        --count;
        if(it==end)return false;
    }
    return true;
}
template<class Container, class Element>
bool contain(const Container &c, const Element &e){
    using namespace std;
    return find(begin(c),end(c),e)!=end(c);
}
class MainToken;
typedef std::list<std::string> ResultType;
typedef std::shared_ptr<MainToken> MainPtr;

using namespace std;


class BaseToken;
typedef std::shared_ptr<BaseToken> BasePtr;
class BaseToken
{
public: //static int Count_;
static int FigureBraceRepeatCount;
static int ConcatenationDepth;
static int RecursionDepth;


    BaseToken(){
    //    cout<<"created: "<<++Count_<<endl;
    }

    virtual void setChild(BasePtr child)=0;
    virtual void resetChild(BasePtr other)=0;
    virtual void proc(ResultType &rt) = 0;
    void doProc(ResultType &rt){
      //  qDebug()<<"recDepth"<<++recDepth;
        proc(rt);
      //  --recDepth;
    }

    virtual ~BaseToken(){
     //  cout<<"Left: "<<--Count_<<endl;
    }
};

class BraceToken:public BaseToken
{
    BasePtr child_;
public:
     ~BraceToken(){}//if(child_) delete child_;}
    BraceToken():child_(nullptr){}
    void setChild(BasePtr child){
        child_ = child;
    }
    void resetChild(BasePtr other){
        other->setChild(child_);
        child_ = other;
    }
    void proc(ResultType &rt){
        if(child_) child_->doProc(rt);
        else throw logic_error("Arg child_ not set in BraceToken");
    }

};




#endif // BASETOKEN_H
