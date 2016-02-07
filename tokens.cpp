#include "tokens.h"
#include "tree.h"
size_t BaseToken::FigureBraceRepeatCount=BaseToken::DefaultFigureBraceRepeatCount;
size_t BaseToken::MaxConcatenationDepth=BaseToken::DefaultMaxConcatenationDepth;
size_t BaseToken::MaxRecursionDepth=BaseToken::DefaultMaxRecursionDepth;
/*concates the rr and lr results each to each
 * size of results reduced according ConcatenationDepth
 */
void ConcatToken::proc(ResultType &rt){
    using namespace std;
    if(left_ && right_){
        ResultType rr,lr;
        left_->proc(lr);
        right_->proc(rr);
        if(lr.empty()){
            copy(begin(rr),end(rr),back_inserter(rt));
            return;
        }
        if(rr.empty()){
            copy(begin(lr),end(lr),back_inserter(rt));
            return;
        }
        size_t lrStep, rrStep;

        if(lr.size()>MaxConcatenationDepth) lrStep = lr.size()/MaxConcatenationDepth;
        else lrStep=1;

        if(rr.size()>MaxConcatenationDepth) rrStep = rr.size()/MaxConcatenationDepth;
        else rrStep=1;

        auto lrIt = begin(lr),lrEnd = end(lr), rrEnd = end(rr);

        advance_(lrIt,lrEnd,rand()%lrStep/2);//add 0-lrStep-1 to the begiining pos, to randomize results
        do{
            auto rrIt = begin(rr);
            advance_(rrIt,rrEnd,rand()%rrStep/2);
            do{
                string temp(*lrIt+*rrIt);
              //if(!contain(rt,temp))//take very long time
                    rt.push_back(std::move(temp));
            }while(advance_(rrIt,rrEnd,rand()%rrStep+1));

        }while(advance_(lrIt,lrEnd,rand()%lrStep+1));


       /* size_t iterationCount = MaxConcatenationDepth;
        //iterationCount = min(iterationCount,MaxConcatenationDepth);

        auto lrBegin= begin(lr),lrEnd = end(lr),rrBegin = begin(rr), rrEnd = end(rr);
        auto rrIt = rrBegin,lrIt = lrBegin;
        advance_(rrIt,rrEnd,rand()%rr.size());
        while(--iterationCount){
            if(lrIt == lrEnd) lrIt=lrBegin;
            if(rrIt == rrEnd) rrIt=rrBegin;
            string temp(*lrIt+*rrIt);
            if(!contain(rt,temp)) rt.push_back(std::move(temp));
            advance_(rrIt,rrEnd,rrStep);
            advance_(lrIt,lrEnd,lrStep);

        }*/
    }else throw myException("args not set ConcatToken");
}

/*multiply results rt FigureBraceRepeatCount times
 *
 */
void FigureBraceToken::proc(ResultType &rt){
    typedef std::shared_ptr<ConcatToken> ConcatPtr;
    if(child_){
        child_->proc(rt);
        size_t repeatCount = min(min((size_t)rt.size(),(size_t)MinElemCountToMultiplying),FigureBraceRepeatCount);
        ConcatPtr top = std::make_shared<ConcatToken>();
        top->setChild(std::make_shared<CopyToken>(rt));


        ConcatPtr  current, prev=top;
        do{
            prev->setChild(std::make_shared<CopyToken>(rt));
            top->proc(rt);
            current = std::make_shared<ConcatToken>();
            prev->resetChild(current);
            prev = current;
        }while(--repeatCount);
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

            if(tree_->generate(/*ifChanged()*/true)){
                //save();
                ResultType &r = tree_->getResults();
                copy(begin(r),end(r),back_inserter(rt));
            }
            else myException("Error generating: "+name_);
        }else throw myException("Arg child_ not set in CustomToken: "+name_);
    }
    --recurseDepth_;
}
void BaseToken::increaseRanges(){
    ++MaxConcatenationDepth;
    ++FigureBraceRepeatCount;
    ++MaxRecursionDepth;
}
void BaseToken::decreaseRanges(){
    if(MaxConcatenationDepth>DefaultMaxConcatenationDepth)--MaxConcatenationDepth;
    if(FigureBraceRepeatCount>DefaultFigureBraceRepeatCount)--FigureBraceRepeatCount;
    if(MaxRecursionDepth>DefaultMaxRecursionDepth)--MaxRecursionDepth;
}
