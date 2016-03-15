#include "tokens.h"
#include "tree.h"

size_t BaseToken::FigureBraceRepeatCount=   BaseToken::DefaultFigureBraceRepeatCount;
size_t BaseToken::MaxConcatenationDepth =   BaseToken::DefaultMaxConcatenationDepth;
size_t BaseToken::MaxRecursionDepth     =   BaseToken::DefaultMaxRecursionDepth;
size_t BaseToken::FigureBraceStep       =   BaseToken::DefaultFigureBraceStep;
size_t BaseToken::FigureBraceDepth      =   BaseToken::DefaultFigureBraceDepth;


void ConcatToken::proc(StringList &rt)
{
    using namespace std;

    if(!left_ || !right_) throw DGException("ConcatToken::proc error. Args not set.");

    StringList rr,lr;
    right_->proc(rr);
    left_->proc(lr);
    if(lr.empty())
    {
        rt.insert(end(rt),begin(rr),end(rr));
        return;
    }
    if(rr.empty())
    {
        rt.insert(end(rt),begin(lr),end(lr));
        return;
    }
    map<size_t,size_t> wordsLength;
    size_t iterCount = max(rr.size(),lr.size());


    auto lrIt = begin(lr),lrBegin = begin(lr),lrEnd = end(lr),rrIt = begin(rr),rrBegin = begin(rr),rrEnd = end(rr);
    while(iterCount-->0)
    {
        auto &l = wordsLength[lrIt->size()+rrIt->size()];
        ++l;
        if(l<=5) rt.push_back(move(*lrIt+*rrIt));
        if(++lrIt == lrEnd) lrIt = lrBegin;
        if(++rrIt == rrEnd) rrIt = rrBegin;

    }
}



void FigureBraceToken::proc(StringList &rt)
{
    using namespace std;
    if (!child_) throw DGException("FigureBraceToken::proc error. Arg not set.");
    StringList temp;

    child_->proc(temp);
    if(temp.empty()) return;
    //child_->proc(rt);
    size_t	addingCount = FigureBraceStep;				 ///count of stacked element
    
    
    string tempStr;							 ///stacked elements
    rt.push_back("");						 ///{} represens 0(!) or more elements
    rt.insert(end(rt),begin(temp),end(temp));///1element
    while(addingCount<FigureBraceDepth)
    {
        for(auto &elem:temp)
        {
            tempStr.append(elem);
            routines::ConcatNRandUnit(addingCount,temp,tempStr);//insert addingCount rand elements from temp in tempStr
            rt.emplace_back(move(tempStr));
        }
        addingCount+=FigureBraceStep;
    }
}


void CustomToken::proc(StringList &rt)
{
    DepthAcceptor da(*this);
    if(da)
    {
        if(tree_)
        {

            tree_->generate(/*ifChanged()*/true);
            auto &r = tree_->getResults();
            rt.insert(end(rt),begin(r),end(r));

        }
        else throw DGException("CustomToken::proc error. Token: "+name_+" have no Tree.");
    }
}

size_t CustomToken::preCount()
{
    DepthAcceptor da(*this);
    size_t result = 0;
    if(da)
    {
        if(tree_)result =  tree_->preCount();
        else throw DGException("CustomToken::preCount error. Tree not set in CustomToken "+ name_);
    }
    return result;
}



void BaseToken::increaseRanges()
{
    MaxConcatenationDepth+=2;
    ++FigureBraceDepth;
    ++FigureBraceRepeatCount;
    //++MaxRecursionDepth;
}


void BaseToken::decreaseRanges()
{
    if(MaxConcatenationDepth>DefaultMaxConcatenationDepth+2)MaxConcatenationDepth-=2;
    if(FigureBraceRepeatCount>DefaultFigureBraceRepeatCount)--FigureBraceRepeatCount;
    //if(MaxRecursionDepth>DefaultMaxRecursionDepth)--MaxRecursionDepth;
    if(FigureBraceDepth>DefaultFigureBraceDepth)--FigureBraceDepth;
}




