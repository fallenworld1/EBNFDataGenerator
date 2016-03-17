#include <iostream>
#include <fstream>
#include <string>

#include "src/generator.h"
#include <chrono>



/*!
 * \brief This is an example of using EBNF Generator
 *
 * include generator.h and linc DataGenerator lib
 */


int main()
{
    using namespace std;

    setlocale(LC_ALL,"ru-RU.UTF-8");
    ifstream ifs("input");
    ofstream ofs("output");
    string expr,str;
    cout<<"Reading EBNF..."<<endl;
    //std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::duration<double> delta;
    auto start = std::chrono::steady_clock::now();
    while(ifs>>str) //read all data from input file to expr
    {
        expr.append(str);
        expr.append(" ");
    }

    try
    {

        Generator generator; //main class to use
        Parser parser;
        generator.getTokens(expr,parser);//parse expr for tokens with parser and store it to generator
        generator.setOrTokensProbabilities("character",0,{5,5,40,50});//for token character type like ...a|b|c|d...  - a and b have 5%chance to be added c - 40% and d - 50%
        generator.setSquareBraceProbability("not",0,40);//for token not type like ...[x]... x has 40%chance to be added
        generator.setSquareBraceProbability("word",0,20);//for token word type like ...[x]...x has 20%chance to be added
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
                cout<<"Waiting for input...(to exit enter \"q\")...\n";
                cin >>command;
                if(command == "q") break;


                cout<<"Generating data for token "<<command<<"... \n";
                start = std::chrono::steady_clock::now();
                generator.generate(command);//generates results for token named $command
                StringList results = generator.getResults();//gets results as vector<string>
                routines::showCorrespondingResults(results,ofs,results.size());//store result to ofs(output file)
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

