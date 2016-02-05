#ifndef FIGUREBRACETOKEN
#define FIGUREBRACETOKEN
#include "basetoken.h"
#include "concattoken.h"



class CopyToken:public BaseToken{
    ResultType result;
public:
    CopyToken(ResultType &rt):result(rt){}
    void setChild(BaseToken*){}
     void resetChild(BaseToken*){}
     void proc(ResultType &rt){
         rt = result;
     }
};

class FigureBraceToken:public BaseToken{
    BaseToken *child_;
protected:
     ~FigureBraceToken(){if(child_) delete child_;}
public:
    FigureBraceToken():child_(nullptr){}
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

            ConcatToken *top = new ConcatToken;
            top->setChild(new CopyToken(rt));

            int i=1;
            ConcatToken *current, *prev=top;
            do{
                prev->setChild(new CopyToken(rt));
                top->doProc(rt);
                current = new ConcatToken;
                prev->resetChild(current);
                prev = current;
            }while(++i<FigureBraceRepeatCount-1);
            prev->setChild(new CopyToken(rt));
            top->doProc(rt);
            delete static_cast<BaseToken*>(top);
            //ResultType temp(rt);
            //rt.reserve(rt.size()*FigureBraceRepeatCount);
            /*for(int i=0,count = temp.count();i<FigureBraceRepeatCount-1;++i){
                for(int j=0;j<count;++j)
                    temp[j].append(rt[j]);
                std::copy(temp.begin(),temp.end(),std::back_inserter(rt));
           }*/

        }
        else throw logic_error("Arg child_ not set in FigureBraceToken");
    }
};
#endif // FIGUREBRACETOKEN

