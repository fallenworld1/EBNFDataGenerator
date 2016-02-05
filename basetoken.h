#ifndef BASETOKEN_H
#define BASETOKEN_H
#include <string>
#include <stdexcept>
#include <list>
#include <algorithm>

enum{FigureBraceRepeatCount = 3, ConcatenationDepth = 5};
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

typedef std::list<std::string> ResultType;

template<class Container, class Element>
bool contain(const Container &c, const Element &e){
    using namespace std;
    return find(begin(c),end(c),e)!=end(c);
}


using namespace std;




class BaseToken
{
   // static int Count_;
protected:

public:
    BaseToken(){
       // qDebug()<<"created: "<<++Count_;
    }

    virtual void setChild(BaseToken *child)=0;
    virtual void resetChild(BaseToken *other)=0;
    virtual void proc(ResultType &rt) = 0;
    void doProc(ResultType &rt){
      //  qDebug()<<"recDepth"<<++recDepth;
        proc(rt);
      //  --recDepth;
    }

    virtual ~BaseToken(){
        // qDebug()<<"Left: "<<--Count_;
    }
};

class BraceToken:public BaseToken
{
    BaseToken *child_;
protected:
     ~BraceToken(){if(child_) delete child_;}
public:
    BraceToken():child_(nullptr){}
    void setChild(BaseToken *child){
        child_ = child;
    }
    void resetChild(BaseToken *other){
        other->setChild(child_);
        child_ = other;
    }
    void proc(ResultType &rt){
        if(child_) child_->doProc(rt);
        else throw logic_error("Arg child_ not set in BraceToken");
    }

};




#endif // BASETOKEN_H
