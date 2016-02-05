#ifndef CONCATTOKEN_H
#define CONCATTOKEN_H
#include "basetoken.h"


class ConcatToken : public BaseToken
{
    BasePtr left_;
    BasePtr right_;
public:
    virtual ~ConcatToken(){
        //if(left_) delete left_;
        //if(right_) delete right_;

    }

    ConcatToken():left_(nullptr),right_(nullptr){}
    void setChild(BasePtr child){
        if(left_==nullptr) left_=child;
        else right_ = child;
    }
    void resetChild(BasePtr other){
        other->setChild(this->right_);
        this->right_ = other;
    }
    void proc(ResultType &rt){
        using namespace std;
        if(left_ && right_){
            ResultType rr,lr;
            left_->doProc(lr);
            right_->doProc(rr);
            size_t lrStep,
                    rrStep;
            if(lr.size()>ConcatenationDepth) lrStep = lr.size()/ConcatenationDepth;
            else lrStep=1;

            if(rr.size()>ConcatenationDepth) rrStep = rr.size()/ConcatenationDepth;
            else rrStep=1;

            //for(auto &ls:lr){
            //    for(auto &rs:rr){
            auto lrIt = begin(lr),lrEnd = end(lr), rrEnd = end(rr);
            advance_(lrIt,lrEnd,rand()%lrStep);
            do{
                auto rrIt = begin(rr);
                advance_(rrIt,rrEnd,rand()%rrStep);
                do{
                    string temp(*lrIt+*rrIt);
                    if(!contain(rt,temp)) rt.push_back(std::move(temp));
                }while(advance_(rrIt,rrEnd,rrStep));

            }while(advance_(lrIt,lrEnd,lrStep));

        }else throw logic_error("args not set ConcatToken");
    }

};

#endif // CONCATTOKEN_H
