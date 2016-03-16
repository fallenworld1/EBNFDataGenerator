#ifndef BASETOKEN_H
#define BASETOKEN_H

#include "routines.h"
#include "defines.h"




using routines::DGException;
using routines::DepthAcceptor;
class BaseToken
{
    //static int Count_;
protected:

    static size_t FigureBraceRepeatCount;
    static size_t MaxConcatenationDepth;
    static size_t FigureBraceDepth;
    static size_t MaxRecursionDepth;
    static size_t FigureBraceStep;
    static size_t TreeDepth;


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
        //cout<<"created: "<<++Count_<<endl;
    }
    /*!
     * \brief set one of token child(according token type) to \a child
     * \param child
     */
    virtual void setChild(BasePtr child)=0;
    /*!
     * \brief reset one of token child(according token type) to \a other and sets child of \a other
     * \param other
     */
    virtual void resetChild(BasePtr other)=0;
    /*!
     * \brief process token specific operations and store results to \a rt
     * \param rt
     */
    virtual void proc(StringList &rt) = 0;
    /*!
     * \brief preCount returns ~amount of results this token can produce
     * \return
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
    /*!
     * \brief increase generating ranges to produce more results
     */
    static void increaseRanges();
    /*!
     * \brief decrease generatirn ranges to produce less amount of results
     */
    static void decreaseRanges();
};
/*!
 * \brief The CustomToken class represents token with name
 *
 * Represents all nonliteral tokens \a name_ = string;
 * string parsed to \a tree_
 */
class CustomToken : public BaseToken
{
    size_t      recurseDepth_;    ///controls recursion call to avoid a=b;b=a;



    TreePtr     tree_;            ///corresponding token tree
    std::string name_;            ///token name

    size_t      memFBC_;          ///to control need of regeneration
    size_t      memMCD_;          ///to control need of regeneration
    size_t      memMRD_;          ///to control need of regeneration

public:
    /*!
     * \brief creates CustomToken
     * \param name created token name
     * \param mt created token tree
     */
    CustomToken(const std::string &name, TreePtr mt):tree_(mt),name_(name),recurseDepth_(0){/*save();*/}
    const std::string& name(){return name_;}
    void setMain(TreePtr mt){tree_=mt;}
    void setChild(BasePtr) override {}
    void resetChild(BasePtr) override {}
    void proc(StringList &rt) override ;
    /*!
     * \brief save current generating state
     */
    /* void save()
    {
        memFBC_ =  FigureBraceRepeatCount;
        memMCD_ = MaxConcatenationDepth;
        memMRD_ =  MaxRecursionDepth;
    }*/
    /*!
     * \brief ifChanged checks if generating state changed
     * \return true if generating state changed
     */
    /*bool ifChanged()
    {
        if(memFBC_ !=  FigureBraceRepeatCount ||
                memMCD_ != MaxConcatenationDepth||
                memMRD_ !=  MaxRecursionDepth)
            return false;
        else return true;
    }*/

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
        if(!left_) left_=child;
        else right_ = child;
    }
    void resetChild(BasePtr other) override
    {
        other->setChild(this->right_);
        this->right_ = other;
    }
    bool checkChildType(char c)const override
    {
        if(right_)return right_->checkType(c);
        else return false;
    }
};


class RoundBraceToken:public OneArgToken
{
public:

    //token that need to set pryority of operation
    //simple call child proc
    void proc(StringList &rt) override
    {
        if(child_)
        {
            DepthAcceptor verticalControl(BaseToken::TreeDepth);
            child_->proc(rt);
        }
        else throw DGException("Arg child_ not set in BraceToken");
    }
    size_t preCount() override
    {
        if(child_) return child_->preCount();
        else throw DGException("Arg child_ not set in RoundBraceToken");
    }
    bool checkType(char c) const override {return c==')'||c=='(';}
};
class ConcatToken : public TwoArgToken
{
public:
    void proc(StringList &rt) override ;
    size_t preCount() override
    {
        if (left_ && right_) return std::max(right_->preCount(), left_->preCount());
        else throw DGException("args not set in ConcatToken");
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
    void proc(StringList &rt) override
    {
        //if(!text_.empty())
        rt.push_back(text_);
    }
    size_t preCount() override {return 1;}
    bool checkType(char) const override {return false;}
    bool checkChildType(char)const override {return false;}
};

class SquareBraceToken:public OneArgToken{

public:

    //adds 1 or nothing
    void proc(StringList &rt) override
    {
        if(child_)
        {
            DepthAcceptor verticalControl(BaseToken::TreeDepth);
            child_->proc(rt);
            rt.push_back("");
        }
        else throw DGException("Arg child_ not set in SquareBraceToken");
    }
    size_t preCount() override
    {
        if (child_) return child_->preCount() + 1;
        else throw DGException("Arg child_ not set in SquareBraceToken");
    }
    bool checkType(char c) const override {return c==']'||c=='[';}
};
class FigureBraceToken:public OneArgToken{
public:

    void proc(StringList &rt) override ;
    size_t preCount() override
    {
        //auto s = child_->preCount();
        //return min(s,FigureBraceDepth)*FigureBraceRepeatCount+s;
        if (child_) return child_->preCount()*(FigureBraceDepth/FigureBraceStep) + 1;
        else throw DGException("Arg child_ not set in FigureBraceToken");
    }
    bool checkType(char c) const override {return c=='}'||c=='{';}
};
class OrToken : public TwoArgToken
{
public:
    void proc(StringList &rt) override
    {
        if(left_ && right_){
            DepthAcceptor verticalControl(BaseToken::TreeDepth);
            right_->proc(rt);
            left_->proc(rt);
        }
        else throw DGException("args not set in OrToken");
    }
    size_t preCount() override
    {
        if (left_ && right_) return right_->preCount()+left_->preCount();
        else throw DGException("args not set in OrToken");
    }
    bool checkType(char c) const override {return c=='|';}

};

#endif // BASETOKEN_H
