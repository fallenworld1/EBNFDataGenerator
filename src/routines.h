#ifndef ROUTINES_H
#define ROUTINES_H
#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <exception>
#include <assert.h>
#include "defines.h"

namespace routines{


/*!
 * \brief The DepthAcceptor class automate depth check
 *
 * generating like this vulnerable to situation:
 * a = b;b = a;
 * cause ulimited call to b->proc(),a->proc()
 * this checks call amount on stack
 */
class DepthAcceptor
{
    size_t &var_;
public:
    DepthAcceptor(size_t &var):var_(var){++var_;}
    ~DepthAcceptor(){--var_;}
    operator size_t() const {return var_;}
};
/*!
 * \brief The MainTokenNameGuard class used to restore main token name.
 */
class MainTokenNameGuard{
    std::string &mainTokenName_;
    std::string saved_;
public:
    MainTokenNameGuard(std::string &mainTokenName):mainTokenName_(mainTokenName),saved_(std::move(mainTokenName)){}
	MainTokenNameGuard(const MainTokenNameGuard&) = delete;
	MainTokenNameGuard& operator=(const MainTokenNameGuard&) = delete;
    ~MainTokenNameGuard(){mainTokenName_ = std::move(saved_);}
};

/*!
 * \brief The myException class simple wrap of std::exception to siplify making error messages
 */
class DGException: public std::exception
{
    std::string what_;

    virtual const char* what() const throw()
    {
        return what_.c_str();
    }
public:
    /*!
      * \brief creates DGException with string \a word
      * \param [in] word error message
      */
    DGException(const std::string &word):what_(word){}
    /*!
      * \brief creates DGException with string \a word <\a lit >
      * \param [in] word error message
      * \param [in] lit wrong literal
      */
    DGException(const char *word, const char lit):what_(word)
    {
        char str[4] = "< >";
        str[1] = lit;
        what_.append(str);
    }
    /*!
      * \brief creates DGException with string \a word <\a lit > at \a offset
      * \param [in] word error message
      * \param [in] lit wrong literal
      * \param [in] offset position of wrong literal
      */
     DGException(const char *word, const char lit, size_t offset):what_(word)
     {
         char str[21] = "< >";//max digits amount in size_t - 19
         str[1] = lit;
         what_.append(str);
         what_.append(" at ");
         sprintf(str,"%d",(int)offset);
         what_.append(str);
     }
};


/*!
 * \brief advance \a it to \a count step forward
 * \param IterType iterator type, at least forward
 * \param it begin of range
 * \param end end of range
 * \param count count of steps
 * \return true if \a it!= \a end after \a count of steps
 */
template<class IterType>
bool advance_(IterType &it, IterType &end,size_t count)
{
    if(it==end) return false;
    while(count)
    {
        ++it;
        --count;
        if(it==end)return false;
    }
    return true;
}

/*!
 * \brief advance \a it to \a count step forward over \a begin if \a end is reached
 * \param IterType iterator type, at least forward
 * \param it start position
 * \param begin beginning of container
 * \param end end of container
 * \param count count of steps
 *
 * move \a it to \a count steps forward, over \a end(resets to \a begin if reached)
 */
template<class IterType>
void advanceOver(IterType &it,const IterType &begin,const IterType &end,size_t count)
{
	if (begin == end) throw DGException("Wrong arg in advanceOver(begin==end");
    if(it==end)it=begin;
    while(count--)
        if(++it==end)it=begin;

}


/*!
 * \brief contain
 * \param c
 * \param e
 * \return return true if \a c contain \a e, false otherwise
 */
template<class Container, class Element>
bool contain(const Container &c, const Element &e)
{
    using namespace std;
    return find(begin(c),end(c),e)!=end(c);
}


/*!
 * \brief addXTimes
 * \param Container container type, supports size(),const_iterator
 * \param [in]  count  count of addings
 * \param [in]  from   input container
 * \param [out] result string
 *
 * adds \a count elements of \a from  to \a result
 */
template<class Container>
void ConcatNRandUnit(size_t count,const Container &from, std::string &result){
    if(from.empty())throw DGException("Trying to get from empty container(ConcatNRandUnit)");
    typename Container::const_iterator b = std::begin(from),e = std::end(from),it = b;
    size_t size = from.size()+1;
    while(count--){
        advanceOver(it,b,e,rand()%size);
        result+=*it;
    }
}


/*!
 * \brief remove spaces from \a str
 * \param [in] str string to process
 */
void removeSpaces(std::string &str);
/*!
 * \brief reads ESQ-sequence
 * \param [in]  begin begin of string to parse from
 * \param [in]  end end of string to parse from
 * \param [out] out string to store result
 *
 * reads ESQ-sequens (\<x>,\xNN,\NNN) from \a begin to \a end and store to \a out
 */
void readEcqSequence(ConstStrIt &begin, const ConstStrIt &end, std::string &out);
/*!
 * \brief Reads literal name
 * \param [in]  begin begin of string to parse from
 * \param [in]  end end of string to parse from
 * \param [out] out string to store result
 *
 * reads sequence of character from \a begin to '"' or \a end and store to \a out
 * parse ESQ-sequence if found '\'
 */
void readLiteralName(ConstStrIt &begin, const ConstStrIt &end, std::string &out);

class Generator;
/*!
 * \brief loads Dictionary From File
 * \param [in] fileName name of file to load from
 * \param [out] dictionary list of words
 */
void loadDictionaryFromFile(const std::string & fileName,StringList &dictionary);
/*!
 * \brief prints \a count elements of \a rt to \a os
 * \param [in] rt list to copy from
 * \param [in] os stream to copy to
 * \param [in] count count of elemts to copy
 */
void showResults(const StringList &rt,std::ostream &os,size_t count);
/*!
 * \brief Shows Corresponding Results
 * \param [in] rt list to copy from
 * \param [in] os os stream to copy to
 * \param [in] count count count of elemts to copy
 *
 * sort rt in ascending order and call showResults
 */
void showCorrespondingResults(StringList &rt,std::ostream &os,size_t count);

}
#endif // ROUTINES_H

