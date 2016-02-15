#include "tokens.h"
#include "tree.h"

size_t BaseToken::FigureBraceRepeatCount=   BaseToken::DefaultFigureBraceRepeatCount;
size_t BaseToken::MaxConcatenationDepth =   BaseToken::DefaultMaxConcatenationDepth;
size_t BaseToken::MaxRecursionDepth     =   BaseToken::DefaultMaxRecursionDepth;
size_t BaseToken::FigureBraceStep       =   BaseToken::DefaultFigureBraceStep;
size_t BaseToken::FigureBraceDepth       =  BaseToken::DefaultFigureBraceDepth;


void ConcatToken::proc(ResultType &rt){
    using namespace std;
    if(left_ && right_)
    {
        ResultType rr,lr;
        right_->proc(rr);
        left_->proc(lr);
        if(lr.empty())
        {
            copy(begin(rr),end(rr),back_inserter(rt));
            return;
        }
        if(rr.empty())
        {
            copy(begin(lr),end(lr),back_inserter(rt));
            return;
        }
        size_t lrStep, rrStep;

        if(lr.size()>MaxConcatenationDepth) lrStep = lr.size()/MaxConcatenationDepth;
        else                                lrStep=1;

        if(rr.size()>MaxConcatenationDepth) rrStep = rr.size()/MaxConcatenationDepth;
        else                                rrStep=1;

        auto lrIt = begin(lr),lrEnd = end(lr), rrEnd = end(rr);

        advance_(lrIt,lrEnd,rand()%lrStep/2);//add 0-lrStep-1 to the begining pos, to randomize results
        do
        {
           auto rrIt = std::begin(rr);
           advance_(rrIt,rrEnd,rand()%rrStep/2);
           do
           {
               rt.push_back(std::move(*lrIt+*rrIt));
           }
           while(advance_(rrIt,rrEnd,rand()%rrStep*2+1));
        }
        while(advance_(lrIt,lrEnd,rand()%lrStep*2+1));
    }
    else throw myException("args not set ConcatToken");
}



void FigureBraceToken::proc(ResultType &rt)
{
    ResultType temp;
    child_->proc(temp);
    child_->proc(rt);
    size_t  repeatCount = 0,
            defaultCountOfReproduced = min(temp.size(),FigureBraceDepth),
            addingCount = FigureBraceStep,
            countOfReproduced;
    countOfReproduced = defaultCountOfReproduced;
    string tempStr;
    do
    {
        do
        {
            addXTimes(addingCount,temp,tempStr);
            rt.push_back(tempStr);
            tempStr.clear();
        }
        while(--countOfReproduced);
        addingCount+=FigureBraceStep;
        countOfReproduced = defaultCountOfReproduced;
    }
    while(++repeatCount<FigureBraceRepeatCount);//6

}


void CustomToken::proc(ResultType &rt)
{
    ++recurseDepth_;
    if(recurseDepth_<=MaxRecursionDepth)
    {
        if(tree_)
        {

            if(tree_->generate(/*ifChanged()*/true))
            {
                //save();
                ResultType &r = tree_->getResults();
                copy(begin(r),end(r),back_inserter(rt));
            }
            else myException("Error generating: "+name_);
        }
        else throw myException("token: "+name_+"not defined");
    }
    --recurseDepth_;
}

size_t CustomToken::preCount()
{
    if(tree_)return tree_->preCount();
    else return 0;
}


void BaseToken::increaseRanges()
{
    ++MaxConcatenationDepth;
    ++FigureBraceDepth;
    ++FigureBraceRepeatCount;
    ++MaxRecursionDepth;
}


void BaseToken::decreaseRanges()
{
    if(MaxConcatenationDepth>DefaultMaxConcatenationDepth)--MaxConcatenationDepth;
    if(FigureBraceRepeatCount>DefaultFigureBraceRepeatCount)--FigureBraceRepeatCount;
    if(MaxRecursionDepth>DefaultMaxRecursionDepth)--MaxRecursionDepth;
    if(FigureBraceDepth>DefaultFigureBraceDepth)--FigureBraceDepth;
}


/*void FigureBraceToken::proc(ResultType &rt)
{
    typedef std::shared_ptr<ConcatToken> ConcatPtr;
    if(child_)
    {

        child_->proc(rt);
        size_t repeatCount = 0,MultiplyCount;
        ConcatPtr top = std::make_shared<ConcatToken>();
        std::shared_ptr<CopyToken> ct = std::make_shared<CopyToken>(rt);
        top->setChild(ct);


        ConcatPtr  current, prev=top;
        do
        {
            ResultType temp;
            child_->proc(temp);
            prev->setChild(std::make_shared<CopyToken>(temp));
            if(repeatCount%FigureBraceStep==0)
            {
                top->proc(rt);
            }
            current = std::make_shared<ConcatToken>();
            prev->resetChild(current);
            prev = current;
        }
        while(--repeatCount);
        prev->setChild(child_);
        top->proc(rt);
    }
    else throw myException("Arg child_ not set in FigureBraceToken");
}*/

