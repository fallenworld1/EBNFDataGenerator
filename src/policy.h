#ifndef POLICY
#define POLICY
#include "defines.h"



/*!
 * \brief The Policy class base class for all adding policies in Tree class
 */
class AddingPolicy
{
public:
    /*!
     * \brief update function updates parametrs of current policy(called in setDictionary)
     * \param dictionary new dictionary with new parametrs
     */
    virtual void update(const StringList &dictionary)=0;
    /*!
     * \brief check function checks if \a elem fits to this policy
     * \param [in] elem checked
     * \return true if \a elem fits policy
     */
    virtual bool check(const std::string &elem)=0;
    /*!
     * \brief refresh refresh internal state
     *
     * called before forming output
     */
    virtual void refresh()=0;
    virtual ~AddingPolicy(){}
};
/*!
 * \brief The DefaultPolicy class is default set in all Tree classes. Allows all elements
 */
class DefaultPolicy:public AddingPolicy
{
public:
    void update(const StringList &) override {}

    bool check(const std::string &) override
    {
        return true;
    }
    void refresh() override{}
};

/*!
 * \brief The MinMaxPolicy class allows only elements with lengthes more than min dictionary element length and
 * less than max
 */
class MinMaxDPolicy:public AddingPolicy
{
    size_t min_;
    size_t max_;
public:

    void update(const StringList &dictionary) override;
    void refresh() override{}
    bool check(const std::string &elem) override;

};
class MinMaxPolicy:public AddingPolicy
{
    size_t min_;
    size_t max_;
public:
    MinMaxPolicy(size_t min=0,size_t max=-1):min_(min),max_(max){}
    void update(const StringList &) override{}
    void refresh() override{}
    bool check(const std::string &elem) override{return elem.size()>=min_ && elem.size()<=max_;}

};
/*!
 * \brief The NoNamePolicy class allow elements whith lengthes near average
 */
class NearAveragePolicy: public AddingPolicy
{
    double defaultAverageSize_ ;
    double averageSize_;
    double range_;
    size_t elemCount   = 1;

    void add(double size);///adds size to average

public:
    NearAveragePolicy(double defaultASize = 0.0, double range = 2.0);

    virtual void update(const StringList &d)override;
    virtual bool check(const std::string &elem)override;
    void refresh() override;
};

/*!
 * \brief The DictionarySizePolicy class adds dictionary.size()*percentage_ elements
 */
class DictionarySizePolicy:public AddingPolicy
{
    size_t addingAmount_ = 0;
    size_t lastAddedCount_ = 0;
    size_t percentage_ = 0;
public:
    DictionarySizePolicy(size_t percentage):percentage_(percentage){}

    void update(const StringList &dictionary) override
    {
        addingAmount_ = dictionary.size()*percentage_/100;
    }
    virtual bool check(const std::string &)
    {
        if(lastAddedCount_<addingAmount_)
        {
            ++lastAddedCount_;
            return true;
        }
        return false;
    }
    virtual void refresh()
    {
        lastAddedCount_ = 0;
    }
};
#endif // POLICY

