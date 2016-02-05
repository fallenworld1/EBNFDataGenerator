#ifndef CUSTOMTOKEN_H
#define CUSTOMTOKEN_H
#include "basetoken.h"
class MainToken;
class CustomToken : public BaseToken
{
    MainPtr mt_;
    string name_;
public:
    CustomToken(const string &name, MainPtr mt):mt_(mt),name_(name){}
    ~CustomToken();
    const string& name(){return name_;}
    void setMain(MainPtr mt){mt_=mt;}
    void setChild(BasePtr ){

    }
    void resetChild(BasePtr ){

    }
    void proc(ResultType &rt);
};
#endif // CUSTOMTOKEN_H
