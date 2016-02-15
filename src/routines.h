#ifndef ROUTINES_H
#define ROUTINES_H

#include <algorithm>
#include <string>
#include <exception>



namespace routines{
using strIt= std::string::iterator;
using constStrIt= std::string::const_iterator;

template<class IterType>
/**
 * @brief advance_
 * @param it begin of range
 * @param end end of range
 * @param count count of steps
 * @return true if @a it!= @a end after @a count of steps
 */
bool advance_(IterType &it, IterType &end,size_t count)
{
    if(it==end)return false;
    while(count)
    {
        ++it;
        --count;
        if(it==end)return false;
    }
    return true;
}

template<class IterType>
/**
 * @brief advanceOver
 * @param it start position
 * @param begin beginning of container
 * @param end end of container
 * @param count count of steps
 *
 * move @a it to @a count steps forward, over @a end(resets to @a begin if reached)
 */
void advanceOver(IterType &it,const IterType &begin,const IterType &end,size_t count)
{
    if(it==end)it=begin;
    while(count--)
        if(++it==end)it=begin;

}

template<class Container, class Element>
bool contain(const Container &c, const Element &e)
{
    using namespace std;
    return find(begin(c),end(c),e)!=end(c);
}
template<class Container>
/**
 * @brief addXTimes
 * @param count count of addings
 * @param from
 * @param result
 *
 * adds @a count of @a from elements to @a result
 */
void addXTimes(size_t count,const Container &from, std::string &result){
    typename Container::const_iterator b = std::begin(from),e = std::end(from),it = b;
    size_t size = from.size()+1;
    while(count--){
        advanceOver(it,b,e,rand()%size);
        result+=*it;
    }
}


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
};
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
 */
void readLiteralName(constStrIt &begin, const constStrIt &end, std::string &out);

}
#endif // ROUTINES_H

