#ifndef ROUTINES_H
#define ROUTINES_H

#include <algorithm>
#include <string>
#include <exception>



namespace routines{
using strIt= std::string::iterator;
using constStrIt= std::string::const_iterator;
//same as std::advance, but check for end
template<class IterType>
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
void readEcqSequence(constStrIt &begin, const constStrIt &end, std::string &out);
void readLiteralName(constStrIt &begin, const constStrIt &end, std::string &out);

}
#endif // ROUTINES_H

