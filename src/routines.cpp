#include "routines.h"

//read literals name from begin and copy it to out
namespace Routines{

void readLiteralName(constStrIt &begin, const constStrIt &end, std::string &out)
{
    if(begin == end) throw myException("unexpected end of string");
    while(*(begin)!='\"'){
        if(*begin=='\\')readEcqSequence(++begin,end,out);
        else out.push_back(*begin++);
        if(begin == end) throw myException("unexpected end of string");
    }
}
//read esq-sequence from begin to end  and appends it to out
//types \(symbol) = symbol, \x(digit)(digit) = hex number,\(digit)(digit)(digit) = decimal number

void readEcqSequence(constStrIt &begin, const constStrIt &end, std::string &out)
{
    unsigned char symbol;
    if(begin == end) throw myException("unexpected end of string");
    if(*begin=='x' && begin+2<end){
        char *b = const_cast<char*>(begin.base()+1),*e = b+2;
        symbol = strtoul(b, &(e), 16);
        begin+=3;
    }
    else if(isdigit(*begin) && begin+3<end){
        char *b =const_cast<char*>(begin.base()),*e = b+3;
        symbol = strtoul(b, &(e), 10);
        begin+=3;
    }
    else symbol = *begin++;
    out.push_back(symbol);
    //
}


void removeSpaces(std::string &str){
    using namespace std;
    str.erase(remove_if(begin(str),end(str),[](char c){return isspace(c);}),end(str));
}
}
