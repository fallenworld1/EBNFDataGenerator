#ifndef ORTOKEN_H
#define ORTOKEN_H
#include "basetoken.h"

class OrToken : public BaseToken
{
    BasePtr left_;
    BasePtr right_;
public:
    ~OrToken(){
            //if(left_) delete left_;
            //if(right_) delete right_;

        }

    OrToken():left_(nullptr),right_(nullptr){}
    void setChild(BasePtr child){
        if(left_==nullptr) left_=child;
        else right_ = child;
    }
    void resetChild(BasePtr other){
        other->setChild(this->right_);
        this->right_ = other;
    }
    void proc(ResultType &rt){
        if(left_ && right_){
            left_->doProc(rt);
            right_->doProc(rt);
        }
        else throw logic_error("args not set OrToken");
    }
};

#endif // ORTOKEN_H
