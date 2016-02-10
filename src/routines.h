#ifndef ROUTINES_H
#define ROUTINES_H

#include <algorithm>
#include <string>
#include <exception>



namespace Routines{
using strIt= std::string::iterator;
using constStrIt= std::string::const_iterator;
//same as std::advance, but check for end
template<class IterType>
bool advance_(IterType &it, IterType &end,size_t count){
    if(it==end)return false;
    while(count){
        ++it;
        --count;
        if(it==end)return false;
    }
    return true;
}
template<class Container, class Element>
bool contain(const Container &c, const Element &e){
    using namespace std;
    return find(begin(c),end(c),e)!=end(c);
}

void removeSpaces(std::string &str);

class myException: public std::exception
{
    std::string what_;

    virtual const char* what() const throw()
    {
        return what_.c_str();
    }
public:

    myException(const std::string &word):what_(word){}
    myException(const char *word, const char lit):what_(word){
        char str[4] = "< >";
        str[1] = lit;
        what_.append(str);
    }
};
void readEcqSequence(constStrIt &begin, const constStrIt &end, std::string &out);

void readLiteralName(constStrIt &begin, const constStrIt &end, std::string &out);

}
#endif // ROUTINES_H

