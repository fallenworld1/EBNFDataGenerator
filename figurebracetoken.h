#ifndef FIGUREBRACETOKEN
#define FIGUREBRACETOKEN
#include "basetoken.h"
#include "concattoken.h"



class CopyToken:public BaseToken{
    ResultType result;
public:
    CopyToken(ResultType &rt):result(rt){}
    void setChild(BasePtr){}
     void resetChild(BasePtr){}
     void proc(ResultType &rt){
         rt = result;
     }
};

class FigureBraceToken:public BaseToken{
    BasePtr child_;


public:
    FigureBraceToken():child_(nullptr){}
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

            std::shared_ptr<ConcatToken> top = std::make_shared<ConcatToken>();
            top->setChild(std::make_shared<CopyToken>(rt));

            int i=1;
            std::shared_ptr<ConcatToken>  current, prev=top;
            do{
                prev->setChild(std::make_shared<CopyToken>(rt));
                top->doProc(rt);
                current = std::make_shared<ConcatToken>();
                prev->resetChild(current);
                prev = current;
            }while(++i<FigureBraceRepeatCount);
            prev->setChild(std::make_shared<CopyToken>(rt));
            top->doProc(rt);
            //delete static_cast<BaseToken*>(top);
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
    ~FigureBraceToken(){}//if(child_) delete child_;}
};
#endif // FIGUREBRACETOKEN

