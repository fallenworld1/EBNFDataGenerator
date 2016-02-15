#include "routines.h"


namespace routines{

void readLiteralName(constStrIt &begin, const constStrIt &end, std::string &out)
{
    if(begin == end) throw myException("unexpected end of string");
    while(*(begin)!='\"')
    {
        if(*begin=='\\')readEcqSequence(++begin,end,out);
        else out.push_back(*begin++);
        if(begin == end) throw myException("unexpected end of string");
    }
}


void readEcqSequence(constStrIt &begin, const constStrIt &end, std::string &out)
{
    unsigned char symbol;
    if(begin == end) throw myException("unexpected end of string");
    if(*begin=='x' && begin+2<end)
    {
        char symbolString[3];
        char* e = std::copy(begin+1,begin+3,symbolString);
        symbol = strtoul(symbolString, &e, 16);
        begin+=3;
    }
    else if(isdigit(*begin) && begin+2<end)
    {
        char symbolString[4];
        char * e = std::copy(begin,begin+3,symbolString);
        symbol = strtoul(symbolString, &e, 10);
        begin+=3;
    }
    else symbol = *begin++;
    out.push_back(symbol);
    //
}


void removeSpaces(std::string &str)
{
    using namespace std;
    str.erase(remove_if(begin(str),end(str),[](char c){return isspace(c);}),end(str));
}

}
