#include "tokens.h"
#include "tree.h"

size_t BaseToken::FigureBraceRepeatCount=   BaseToken::DefaultFigureBraceRepeatCount;
size_t BaseToken::MaxConcatenationDepth =   BaseToken::DefaultMaxConcatenationDepth;
size_t BaseToken::MaxRecursionDepth     =   BaseToken::DefaultMaxRecursionDepth;
size_t BaseToken::FigureBraceStep       =   BaseToken::DefaultFigureBraceStep;
size_t BaseToken::FigureBraceDepth      =   BaseToken::DefaultFigureBraceDepth;
size_t BaseToken::TreeDepth = 0;

void ConcatToken::proc(StringList &rt)
{
    using namespace std;


    if(!left_)  throw DGException("ConcatToken::proc error. Left not set.");
    if(!right_) throw DGException("ConcatToken::proc error. Right not set.");
    StringList rr,lr;
    DepthAcceptor verticalControl(BaseToken::TreeDepth);

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
        if(l<=verticalControl) rt.push_back(move(*lrIt+*rrIt));
        if(++lrIt == lrEnd) lrIt = lrBegin;
        if(++rrIt == rrEnd) rrIt = rrBegin;

    }
}



void FigureBraceToken::proc(StringList &rt)
{
    using namespace std;
    if (!child_) throw DGException("FigureBraceToken::proc error. Arg not set.");
    StringList temp;
    DepthAcceptor verticalControl(BaseToken::TreeDepth);
    child_->proc(temp);
    if(temp.empty()) return;
    //child_->proc(rt);
    size_t	addingCount = FigureBraceStep, iterationCount = max(FigureBraceDepth,verticalControl/FigureBraceDepth);				 ///count of stacked element
    
    
    string tempStr;							 ///stacked elements
    rt.push_back("");						 ///{} represens 0(!) or more elements
    rt.insert(end(rt),begin(temp),end(temp));///1element
    while(addingCount<iterationCount)
    {
        for(auto &elem:temp)
        {
            if(elem.size() > iterationCount) continue;
            tempStr.append(elem);
            routines::ConcatNRandUnit(addingCount,temp,tempStr);//insert addingCount rand elements from temp in tempStr
            rt.emplace_back(move(tempStr));
        }
        addingCount+=FigureBraceStep;
    }
}


void CustomToken::proc(StringList &rt)
{
    DepthAcceptor da(recurseDepth_);
    if(da<=BaseToken::MaxRecursionDepth)
    {
        if(tree_)
        {
            //DepthAcceptor verticalControl(BaseToken::TreeDepth);
            tree_->generate(/*ifChanged()*/true);
            auto &r = tree_->getResults();
            rt.insert(end(rt),begin(r),end(r));

        }
        else throw DGException("CustomToken::proc error. Token: "+name_+" have no Tree.");
    }
}

size_t CustomToken::preCount()
{
    DepthAcceptor da(recurseDepth_);
    size_t result = 0;
    if(da<=BaseToken::MaxRecursionDepth)
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





void OrToken::proc(StringList &rt)
{
    if (children_.size()>=2)
    {
        DepthAcceptor verticalControl(BaseToken::TreeDepth);
        auto begin = std::begin(children_);
        std::for_each(std::begin(probabilities_),std::end(probabilities_), //probabilities_.size() <= children_.size()
                      [&](int prob)
                                  {
                                    if(rand()%100 < prob) (*begin)->proc(rt);
                                    ++begin;
                                  }
        );
        std::for_each(begin,std::end(children_),[&](BasePtr &child){child->proc(rt);});

    }
    else throw DGException("args not set in OrToken");
}

size_t OrToken::preCount()
{
    if (children_.size()>=2)
    {
        size_t result = 0;
        for(auto &c:children_) result+=c->preCount();
        return result;
    }
    else throw DGException("args not set in OrToken");
}

void SquareBraceToken::proc(StringList &rt)
{
    if(child_)
    {
        DepthAcceptor verticalControl(BaseToken::TreeDepth);
        if(rand()%100 < probability_) child_->proc(rt);
        if(rand()%100 < 100-probability_) rt.push_back("");
    }
    else throw DGException("Arg child_ not set in SquareBraceToken");
}
