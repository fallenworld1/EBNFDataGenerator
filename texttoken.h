#ifndef TEXTTOKEN_H
#define TEXTTOKEN_H


#include "basetoken.h"

class TextToken : public BaseToken
{
    std::string text_;
public:
    TextToken(const  std::string &text):text_(text){}

    virtual void setChild(BaseToken *){}
    virtual void resetChild(BaseToken *){}
    void proc(ResultType &rt){
       // if(rt.isEmpty())
        if(!text_.empty() && !contain(rt,text_)) rt.push_back(text_);
        //else for(auto &s:rt) s.append(text_);
    }
};

#endif // TEXTTOKEN_H
