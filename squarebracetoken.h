#ifndef SQUAREBRACECLASS
#define SQUAREBRACECLASS
#include "basetoken.h"

class SquareBraceToken:public BaseToken{
    BaseToken *child_;
protected:
     ~SquareBraceToken(){if(child_) delete child_;}
public:
    SquareBraceToken():child_(nullptr){}
    void setChild(BaseToken *child){
        child_ = child;
    }
    void resetChild(BaseToken *other){
        other->setChild(child_);
        child_ = other;
    }
    void proc(ResultType &rt){
        if(child_){
            child_->doProc(rt);
            if(!contain(rt,"")) rt.push_back("");

        }
        else throw logic_error("Arg child_ not set in SquareBraceToken");
    }



};

#endif // SQUAREBRACECLASS

