#ifndef ROUTINES
#define ROUTINES
#include <algorithm>
#include <string>
#include <exception>

template<class IterType>
//same as std::advance, but check for end
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
template<class String>
void removeSpaces(String &str){
    using namespace std;
    str.erase(remove_if(begin(str),end(str),[](typename String::value_type c){return std::isspace(c);}),end(str));
}

class myException: public std::exception
{
   std::string what_;

  virtual const char* what() const throw()
  {
    return what_.c_str();
  }
public:

   myException(const std::string &&word):what_(word){}
   myException(const std::string &word):what_(word){}
   myException(const char *word, const char lit):what_(word){
       char str[4] = "< >";
       str[1] = lit;
       what_.append(str);
   }
};

#endif // ROUTINES

