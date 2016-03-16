#include <iostream>
#include <fstream>
#include <string>

#include "src/generator.h"
#include <chrono>






int main()
{
    using namespace std;

    setlocale(LC_ALL,"ru-RU.UTF-8");
    ifstream ifs("input");
    ofstream ofs("output");
    string expr,str;
    cout<<"Reading EBNF..."<<endl;
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::duration<double> delta;
    start = std::chrono::steady_clock::now();
    while(ifs>>str)
    {
        expr.append(str);
        expr.append(" ");
    }

    try
    {

        Generator generator;
        generator.getTokens(expr,Parser());
        generator.setOrTokensProbabilities("character",0,{5,5,40,50});
        generator.setSquareBraceProbability("not",0,40);
        generator.setSquareBraceProbability("word",0,20);
        delta = std::chrono::steady_clock::now() - start;
        cout<<"Time spent: "<<delta.count()<<endl;
        //generator.setAddingPolicy("word",std::make_shared<NearAveragePolicy>(6.0,2.0));
        //generator.setAddingPolicy("rule",std::make_shared<MaxPolicy>(50));
        //generator.setMainTokenName("grammar");

        // loadDictionaryFromFile("word","words",generator);
        //generator.setAddingPolicy("word",std::make_shared<MinMaxPolicy>());

        std::string command;
        while(1)
        {
            try
            {
                cout<<"...\n";
                cin >>command;
                if(command == "q") break;


                cout<<"Generating...\n";
                start = std::chrono::steady_clock::now();
                generator.generate(command);
                StringList results = generator.getResults();
                routines::showCorrespondingResults(results,ofs,results.size());
                delta = std::chrono::steady_clock::now() - start;
                cout<<"Time spent: "<<delta.count()<<endl;
            }
            catch(exception &e)
            {
                cerr<<"Error occured: "<<endl;
                cerr<<e.what()<<endl;
            }
        }
    }
    catch(exception &e)
    {
        cerr<<"Error occured: "<<endl;
        cerr<<e.what()<<endl;
    }
    //system("pause");
    return 0;




}

