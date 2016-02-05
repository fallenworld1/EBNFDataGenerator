#include "tokens.h"
#include "tree.h"
size_t BaseToken::FigureBraceRepeatCount=5;
size_t BaseToken::MaxConcatenationDepth=5;
size_t BaseToken::MaxRecursionDepth=5;
/*concates the rr and lr results each to each
 * size of results reduced according ConcatenationDepth
 */
void ConcatToken::proc(ResultType &rt){
    using namespace std;
    if(left_ && right_){
        ResultType rr,lr;
        left_->proc(lr);
        right_->proc(rr);
        size_t lrStep, rrStep;

        if(lr.size()>MaxConcatenationDepth) lrStep = lr.size()/MaxConcatenationDepth;
        else lrStep=1;

        if(rr.size()>MaxConcatenationDepth) rrStep = rr.size()/MaxConcatenationDepth;
        else rrStep=1;


        auto lrIt = begin(lr),lrEnd = end(lr), rrEnd = end(rr);
        //add 0-lrStep-1 to the begiining pos, to randomize results
        do{
            auto rrIt = begin(rr);
            do{
                string temp(*lrIt+*rrIt);
                if(!contain(rt,temp)) rt.push_back(std::move(temp));
            }while(advance_(rrIt,rrEnd,rand()%rrStep+1));

        }while(advance_(lrIt,lrEnd,rand()%lrStep+1));

    }else throw myException("args not set ConcatToken");
}

/*multiply results rt FigureBraceRepeatCount times
 *
 */
void FigureBraceToken::proc(ResultType &rt){
    typedef std::shared_ptr<ConcatToken> ConcatPtr;
    if(child_){
        child_->proc(rt);

        ConcatPtr top = std::make_shared<ConcatToken>();
        top->setChild(std::make_shared<CopyToken>(rt));

        size_t i=1;
        ConcatPtr  current, prev=top;
        do{
            prev->setChild(std::make_shared<CopyToken>(rt));
            top->proc(rt);
            current = std::make_shared<ConcatToken>();
            prev->resetChild(current);
            prev = current;
        }while(++i<FigureBraceRepeatCount);
        prev->setChild(std::make_shared<CopyToken>(rt));
        top->proc(rt);
    }
    else throw myException("Arg child_ not set in FigureBraceToken");
}
//add results of generating custom brunch to rt
void CustomToken::proc(ResultType &rt){
    ++recurseDepth_;
    if(recurseDepth_<=MaxRecursionDepth){
        if(tree_){
            if(tree_->generate()){
                ResultType &r = tree_->getResults();
                copy(begin(r),end(r),back_inserter(rt));
            }
            else myException("Error generating: "+name_);
        }else throw myException("Arg child_ not set in CustomToken: "+name_);
    }
    --recurseDepth_;
}
void BaseToken::increaseRanges(){
    MaxConcatenationDepth+=1;
  ++FigureBraceRepeatCount;
    ++MaxRecursionDepth;
}
void BaseToken::decreaseRanges(){
    if(MaxConcatenationDepth>1)--MaxConcatenationDepth;
    if(FigureBraceRepeatCount)--FigureBraceRepeatCount;
    //if(MaxRecursionDepth>1)--MaxRecursionDepth;
}
