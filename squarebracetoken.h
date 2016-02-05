#ifndef SQUAREBRACECLASS
#define SQUAREBRACECLASS
#include "basetoken.h"

class SquareBraceToken:public BaseToken{
    BasePtr child_;
public:
     ~SquareBraceToken(){}//if(child_) delete child_;}

    SquareBraceToken():child_(nullptr){}
    void setChild(BasePtr child){
        child_ = child;
    }
    void resetChild(BasePtr other){
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

