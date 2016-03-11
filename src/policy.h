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
class MinMaxPolicy:public AddingPolicy
{
    size_t min;
    size_t max;
public:

    void update(const StringList &dictionary) override;
    void refresh() override{}
    bool check(const std::string &elem) override;

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
class MaxPolicy: public AddingPolicy
{
    size_t maxSize_;
public:
    MaxPolicy(size_t maxSize):maxSize_(maxSize){}
    virtual void update(const StringList &)override{}
    virtual bool check(const std::string &elem)override
    {
        return elem.size()<=maxSize_;
    }
    void refresh() override{}
};

#endif // POLICY

