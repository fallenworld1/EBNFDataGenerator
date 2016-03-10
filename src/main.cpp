#include <iostream>
#include <fstream>
#include <string>

#include "parser.h"
#include "generator.h"
#include <chrono>

void loadDictionaryFromFile(const std::string &tokenName,const std::string & fileName,Generator &g)
{
    using namespace std;

    ifstream ifs(fileName);
    StringList dictionary;
    string word;
    while(!ifs.eof())
    {
        ifs>>word;
        dictionary.emplace_back(move(word));
    }
    g.setDictionary(tokenName,dictionary);
}
void showResults(const StringList &rt,std::ostream &os,size_t count)
{

    os<<"results("<<rt.size()<<")\n";
    for(auto &s:rt)
    {
        const char *str = s.c_str();
        os<<str<<std::endl;
        if(--count==0) break;
    }
    os<<"end"<<std::endl;
}
void showCorrespondingResults(StringList &rt,std::ostream &os,size_t count)
{
    std::sort(
                rt.begin(),
                rt.end(),
                [](const std::string &l,const std::string &r){return l.size()<r.size();}
    );
    showResults(rt,os,count);
}



//tests for tree
// | had priority for ,
//многопоточно делать конкатенацию и фигурные скобки

int main()
{
    using namespace std;

    setlocale(LC_ALL,"ru-RU.UTF-8");
    ifstream ifs("input");
    ofstream ofs("output");
    string expr, temp;
    size_t resultsCount=150;
    cout<<"Hello from EBNF data generator, reading EBNF..."<<endl;
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::duration<double> delta;
    while(ifs>>temp)
    {
        expr.append(" ");
        expr.append(temp);
    }
    //std::chrono::duration<double> sec = std::chrono::steady_clock::now() - start;
    // cout<<"Time spent: "<<sec.count()<<std::endl;
    try
    {
        cout<<"Parsing..."<<endl;
        //
        Generator generator;
        generator.getTokens(expr,Parser());
        generator.setAddingPolicy(std::make_shared<NoNamePolicy>(3.0));
        //generator.setMainTokenName("grammar");

       // loadDictionaryFromFile("word","words",generator);
        //generator.setAddingPolicy("word",std::make_shared<MinMaxPolicy>());
        //
        std::string command;
        int attempts;
        while(1)
        {
            try
            {
                cout<<"...\n";
                cin >>command;
                if(command == "q") break;

                cin >>attempts;

                cout<<"Generating..."<<endl;
                 start = std::chrono::steady_clock::now();
                if(!generator.generate(command,resultsCount,attempts))
                    generator.generate(command,resultsCount);
                StringList results = generator.getResults();
                showCorrespondingResults(results,ofs,results.size());
                delta = std::chrono::steady_clock::now() - start;
                cout<<"Time spent: "<<delta.count()<<endl;
            }
            catch(exception &e)
            {
                cerr<<"Error occured: "<<endl;
                cerr<<e.what()<<endl;
            }
        }
        /*   start = std::chrono::steady_clock::now();
       if(!parser.generate(resultsCount,500))
           parser.generate(resultsCount);
       sec = std::chrono::steady_clock::now() - start;
       cout<<"Time spent: "<<sec.count()<<std::endl;

       cout<<"Writing out..."<<endl;
       start = std::chrono::steady_clock::now();
       showResults(parser.getResults(),ofs,resultsCount);
       sec = std::chrono::steady_clock::now() - start;
       cout<<"Time spent: "<<sec.count()<<std::endl;*/
    }
    catch(exception &e)
    {
        cerr<<"Error occured: "<<endl;
        cerr<<e.what()<<endl;
    }
    //system("pause");
    return 0;




}

