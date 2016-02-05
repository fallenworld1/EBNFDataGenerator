#include "customtoken.h"
#include "maintoken.h"

CustomToken::~CustomToken(){}//{if(mt_) delete mt_;}

void CustomToken::proc(ResultType &rt){
    if(mt_){
        if(mt_->generate()){
            ResultType &r = mt_->getResults();
            copy(begin(r),end(r),back_inserter(rt));
        }
        else logic_error("Error generating: "+name_);
    }else throw logic_error("Arg child_ not set in CustomToken: "+name_);



}
