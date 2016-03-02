#ifndef BASETOKEN_H
#define BASETOKEN_H
#include <list>
#include <memory>
#include "routines.h"
//#include <iostream>


using namespace routines;
using namespace std;

using ResultType = list<string> ;
class BaseToken;
using BasePtr = shared_ptr<BaseToken>;
class Tree;
using TreePtr = shared_ptr<Tree> ;

class BaseToken
{
    //static int Count_;
protected:

    static size_t FigureBraceRepeatCount;
    static size_t MaxConcatenationDepth;
    static size_t FigureBraceDepth;
    static size_t MaxRecursionDepth;
    static size_t FigureBraceStep;

public:
    enum DefaultRanges
    {
        DefaultFigureBraceRepeatCount = 6, // count of repeating {token}
        DefaultFigureBraceStep        = 2, // step of multiplying x1,x3,x5...
        DefaultFigureBraceDepth       = 5, // min count to multiply
        DefaultMaxConcatenationDepth  = 5, // amount of multiplying steps
        DefaultMaxRecursionDepth      = 3  // to protect from left recursion
    };


    BaseToken()
    {
        //    cout<<"created: "<<++Count_<<endl;
    }
    /**
     * @brief set one of token child(according token type) to @a child
     * @param child
     */
    virtual void setChild(BasePtr child)=0;
    /**
     * @brief reset one of token child(according token type) to @a other and sets child of @a other
     * @param other
     */
    virtual void resetChild(BasePtr other)=0;
    /**
     * @brief process token specific operations and store results to @a rt
     * @param rt
     */
    virtual void proc(ResultType &rt) = 0;
    /**
     * @brief preCount returns ~amount of results this token can produce
     * @return
     */
    virtual size_t preCount()=0;
    /*!
     * \brief checkType
     * \param c symbol representation of token type
     * \return true if \a c is type of current Token
     *
     * some of tokens can be checked (like '{' or '|')
     */
    virtual bool checkType(char c) const = 0;
    /*!
     * \brief checkChildType
     * \param c symbol representation of token type
     * \return  checkType(\a c) of child token
     *
     * checks child token type
     */
    virtual bool checkChildType(char c) const = 0;
    virtual ~BaseToken()
    {

        //std::cout<<"Destroed\n";
        //  cout<<"Left: "<<--Count_<<endl;
    }
    /**
     * @brief increase generating ranges to produce more results
     */
    static void increaseRanges();
    /**
     * @brief decrease generatirn ranges to produce less amount of results
     */
    static void decreaseRanges();
};
class CustomToken : public BaseToken
{
    //token for named tokens
    TreePtr tree_;
    string name_;
    size_t recurseDepth_;
    size_t memFBC;
    size_t memMCD;
    size_t memMRD;
public:
    CustomToken(const string &name, TreePtr mt):tree_(mt),name_(name),recurseDepth_(0){save();}
    const string& name(){return name_;}
    void setMain(TreePtr mt){tree_=mt;}
    void setChild(BasePtr) override {}
    void resetChild(BasePtr) override {}
    void proc(ResultType &rt) override ;
    void save()
    {
        memFBC =  FigureBraceRepeatCount;
        memMCD = MaxConcatenationDepth;
        memMRD =  MaxRecursionDepth;
    }

    bool ifChanged()
    {
        if(memFBC !=  FigureBraceRepeatCount ||
                memMCD != MaxConcatenationDepth||
                memMRD !=  MaxRecursionDepth)
            return false;
        else return true;
    }
    bool checkType(char) const override {return false;}
    bool checkChildType(char)const override {return false;}
    size_t preCount() override ;
};
class OneArgToken:public BaseToken
{
protected:
    BasePtr child_;
public:
    void setChild(BasePtr child) override
    {
        child_ = child;
    }
    void resetChild(BasePtr other) override
    {
        other->setChild(child_);
        child_ = other;
    }
    bool checkChildType(char c)const override {return child_->checkType(c);}
};
class TwoArgToken:public BaseToken
{
protected:
    BasePtr left_;
    BasePtr right_;
public:
    void setChild(BasePtr child) override
    {
        if(left_==nullptr) left_=child;
        else right_ = child;
    }
    void resetChild(BasePtr other) override
    {
        other->setChild(this->right_);
        this->right_ = other;
    }
     bool checkChildType(char c)const override {return right_->checkType(c);}
};


class RoundBraceToken:public OneArgToken
{
public:

    //token that need to set pryority of operation
    //simple call child proc
    void proc(ResultType &rt) override
    {
        if(child_) child_->proc(rt);
        else throw myException("Arg child_ not set in BraceToken");
    }
    size_t preCount() override
    {
        if(child_)return child_->preCount();
        else return 0;
    }
    bool checkType(char c) const override {return c==')'||c=='(';}
};
class ConcatToken : public TwoArgToken
{
public:
    void proc(ResultType &rt) override ;
    size_t preCount() override
    {
        size_t rp =right_->preCount(), lp=left_->preCount();

        size_t lrStep = lp/(2*MaxConcatenationDepth);
        if(lrStep==0) lrStep=1;

        size_t rrStep = rp*2/MaxConcatenationDepth;
        if(rrStep==0) rrStep=1;

        return (lp/lrStep)*(rp/rrStep);
    }
    bool checkType(char c) const override {return c==',';}
};

/*class CopyToken:public BaseToken
{
    ResultType result;
public:
    CopyToken(ResultType &rt):result(rt){}
    void setChild(BasePtr){}
    void resetChild(BasePtr){}
    size_t preCount(){return result.size();}
    void proc(ResultType &rt)
    {
        rt = result;
    }
};*/

class TextToken : public BaseToken
{
    std::string text_;
public:
    TextToken(const  std::string &text):text_(text){}

    virtual void setChild(BasePtr) override {}
    virtual void resetChild(BasePtr) override {}
    //its a literal, just add text_ to result
    void proc(ResultType &rt) override
    {
        if(!text_.empty()) rt.push_back(text_);
    }
    size_t preCount() override {return 1;}
    bool checkType(char) const override {return false;}
     bool checkChildType(char)const override {return false;}
};

class SquareBraceToken:public OneArgToken{

public:

    //adds 1 or nothing
    void proc(ResultType &rt) override
    {
        if(child_)
        {
            child_->proc(rt);
            rt.push_back("");
        }
        else throw myException("Arg child_ not set in SquareBraceToken");
    }
    size_t preCount() override {return child_->preCount()+1;}
    bool checkType(char c) const override {return c==']'||c=='[';}
};
class FigureBraceToken:public OneArgToken{
public:

    void proc(ResultType &rt) override ;
    size_t preCount() override
    {
        auto s = child_->preCount();
        return min(s,FigureBraceDepth)*FigureBraceRepeatCount+s;
    }
    bool checkType(char c) const override {return c=='}'||c=='{';}
};
class OrToken : public TwoArgToken
{
public:
    void proc(ResultType &rt) override
    {
        if(left_ && right_){
            right_->proc(rt);
            left_->proc(rt);
        }
        else throw myException("args not set in OrToken");
    }
    size_t preCount() override {return right_->preCount()+left_->preCount();}
    bool checkType(char c) const override {return c=='|';}
};

#endif // BASETOKEN_H
