#ifndef ROUTINES_H
#define ROUTINES_H

#include <algorithm>
#include <string>
#include <exception>
#include <assert.h>

namespace routines{

using strIt= std::string::iterator;
using constStrIt= std::string::const_iterator;


/**
 * @brief The myException class simple wrap of std::exception to siplify making error messages
 */
class myException: public std::exception
{
    std::string what_;

    virtual const char* what() const throw()
    {
        return what_.c_str();
    }
public:

    myException(const std::string &word):what_(word){}
    myException(const char *word, const char lit):what_(word)
    {
        char str[4] = "< >";
        str[1] = lit;
        what_.append(str);
    }
     myException(const char *word, const char lit, size_t offset):what_(word)
     {
         char str[10] = "< >";
         str[1] = lit;
         what_.append(str);
         what_.append(" at ");
         sprintf(str,"%d",(int)offset);
         what_.append(str);
     }
};


/**
 * @brief advance @a it to @a count step forward
 * @param IterType iterator type, at least forward
 * @param it begin of range
 * @param end end of range
 * @param count count of steps
 * @return true if @a it!= @a end after @a count of steps
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

/**
 * @brief advance @a it to @a count step forward over @a begin if @a end is reached
 * @param IterType iterator type, at least forward
 * @param it start position
 * @param begin beginning of container
 * @param end end of container
 * @param count count of steps
 *
 * move @a it to @a count steps forward, over @a end(resets to @a begin if reached)
 */
template<class IterType>
void advanceOver(IterType &it,const IterType &begin,const IterType &end,size_t count)
{
    assert(begin!=end);
    if(it==end)it=begin;
    while(count--)
        if(++it==end)it=begin;

}

template<class Container, class Element>
/**
 * @brief contain
 * @param c
 * @param e
 * @return return true if @a c contain @a e, false otherwise
 */
bool contain(const Container &c, const Element &e)
{
    using namespace std;
    return find(begin(c),end(c),e)!=end(c);
}


/**
 * @brief addXTimes
 * @param Container container type, supports size(),const_iterator
 * @param count count of addings
 * @param from
 * @param result
 *
 * adds @a count of @a from elements to @a result
 */
template<class Container>
void addXTimes(size_t count,const Container &from, std::string &result){
    assert(from.size());
    typename Container::const_iterator b = std::begin(from),e = std::end(from),it = b;
    size_t size = from.size()+1;
    while(count--){
        advanceOver(it,b,e,rand()%size);
        result+=*it;
    }
}


/**
 * @brief remove spaces from @a str
 * @param str
 */
void removeSpaces(std::string &str);
/**
 * @brief readEcqSequence
 * @param begin
 * @param end
 * @param out
 *
 * reads ESQ-sequens (\<x>,\xNN,\NNN) from @a begin to @a end and store to @a out
 */
void readEcqSequence(constStrIt &begin, const constStrIt &end, std::string &out);
/**
 * @brief readLiteralName
 * @param begin
 * @param end
 * @param out
 *
 * reads sequence of character from @a begin to '"' or @a end and store to @a out
 * parse ESQ-sequence if found '\'
 */
void readLiteralName(constStrIt &begin, const constStrIt &end, std::string &out);

}
#endif // ROUTINES_H

