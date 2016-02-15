#include <iostream>
#include <fstream>
#include "parser.h"
#include <boost/chrono.hpp>

void showResults(const ResultType &rt,ostream &os)
{
    using namespace std;
    os<<"results("<<rt.size()<<")\n";
    for(auto &s:rt)
    {
        const char *str = s.c_str();
        os<<str<<endl;
    }
    os<<"end"<<endl;
}
// todo {] {) and smth must not work
//tests for tree
// | had priority for ,
//многопоточно делать конкатенацию и фигурные скобки

int main()
{
   using namespace std;
   setlocale(LC_ALL,"ru-RU.UTF-8");
   ifstream wfs("input");
   ofstream ofs("output");
   string expr, temp;
   cout<<"Hellow from EBNF data generator, reading EBNF..."<<endl;
   auto start = boost::chrono::steady_clock::now();
   while(wfs>>temp)
   {
       expr.append(" ");
       expr.append(temp);
   }
   boost::chrono::duration<double> sec = boost::chrono::steady_clock::now() - start;
   cout<<"Time spent: "<<sec.count()<<std::endl;
   try
   {
       cout<<"Parsing..."<<endl;
       start = boost::chrono::steady_clock::now();
       Parser &parser = Parser::getParser();
       parser.setMainTokenName("grammar");
       parser.parse(expr);
       boost::chrono::duration<double> sec = boost::chrono::steady_clock::now() - start;
       cout<<"Time spent: "<<sec.count()<<std::endl;

       cout<<"Generating..."<<endl;
       start = boost::chrono::steady_clock::now();
       parser.generate(150);
       sec = boost::chrono::steady_clock::now() - start;
       cout<<"Time spent: "<<sec.count()<<std::endl;

       cout<<"Writing out..."<<endl;
       start = boost::chrono::steady_clock::now();
       showResults(parser.getResults(),ofs);
       sec = boost::chrono::steady_clock::now() - start;
       cout<<"Time spent: "<<sec.count()<<std::endl;
   }
   catch(exception &e)
   {
       cerr<<"Error occured: "<<endl;
       cerr<<e.what()<<endl;
   }
   return 0;




}

