#include "routines.h"
#include <cctype>
#undef _DEB
namespace routines{

void readLiteralName(ConstStrIt &begin, const ConstStrIt &end, std::string &out)
{
    if(begin == end) throw DGException("routines::readLiteralName error. Unexpected end of string.");
    while(*(begin)!='\"')
    {
        if(*begin=='\\')readEcqSequence(++begin,end,out);
        else out.push_back(*begin++);
        if(begin == end) throw DGException("routines::readLiteralName error. Unexpected end of string.");
    }
}


void readEcqSequence(ConstStrIt &begin, const ConstStrIt &end, std::string &out)
{
    unsigned char symbol;
    if(begin == end) throw DGException("routines::readEcqSequence error. Unexpected end of string.");
    if(*begin=='x' && begin<end-2)
    {
        char symbolString[3] = {0};
        char* e = std::copy(begin+1,begin+3,symbolString);
        symbol = (unsigned char)strtoul(symbolString, &e, 16);//number must fit in char e.g. 0-FF
        begin+=3;
    }
    else if(isdigit(*begin) && begin<end-2)//writing
    {
        char symbolString[4] = {0};
        char * e = std::copy(begin,begin+3,symbolString);
        symbol = (unsigned char)strtoul(symbolString, &e, 10);//number must fit in char e.g. 0-255
        begin+=3;
    }
    else symbol = *begin++;
    out.push_back(symbol);
}


void removeSpaces(std::string &str)
{
    using namespace std;
    str.erase(remove_if(begin(str),end(str),[](char c){return isspace(c);}),end(str));
}

}
