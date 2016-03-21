#include "routines.h"
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include "generator.h"


namespace routines{

void readLiteralName(ConstStrIt &begin, const ConstStrIt &end, std::string &out)
{
    if(begin == end) throw DGException("routines::readLiteralName error. Unexpected end of string.");
    std::string result;
    while(*(begin)!='\"')
    {
        if(*begin=='\\')readEcqSequence(++begin,end,result);
        else result.push_back(*begin++);
        if(begin == end) throw DGException("routines::readLiteralName error. Unexpected end of string.");
    }
    out.insert(std::end(out),std::begin(result),std::end(result));
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

void loadDictionaryFromFile(const std::string &fileName, StringList &dictionary)
{
    using namespace std;

    ifstream ifs(fileName);
   // string word;
    char buf[1024];
    while(!ifs.eof())
    {
        ifs.getline(buf,1023);
     //   buf[ifs.gcount()-2]=0;//position of \n symbol
        dictionary.emplace_back(buf);
    }
}

void showResults(const StringList &rt, std::ostream &os, size_t count)
{
    using namespace std;


    size_t step = rt.size()/count;
    if(step==0) ++step;
     os<<"results("<<min(count,rt.size())<<")\n";
    auto b = begin(rt),e = end(rt)-step;

    for(;b<=e;b+=step)
    {
         os<<b->c_str()<<endl;
         if(--count == 0)
             break;

    }
    /*for(auto &s:rt)
    {
        const char *str = s.c_str();
        os<<str<<std::endl;
        if(--count==0) break;
    }*/
    os<<"end"<<endl;
}

void showCorrespondingResults(StringList &rt, std::ostream &os, size_t count)
{
    std::sort(
                rt.begin(),
                rt.end(),
                [](const std::string &l,const std::string &r){return l.size()<r.size();}
    );
    showResults(rt,os,count);
}

}
