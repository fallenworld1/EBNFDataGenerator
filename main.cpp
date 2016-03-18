#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>

#include "src/generator.h"
#include <chrono>

class DictionarySizePolicy:public AddingPolicy
{
    size_t addingAmount_ = 0;
    size_t lastAddedCount_ = 0;
    size_t percentage_ = 0;
public:
     DictionarySizePolicy(size_t percentage):percentage_(percentage){}

     void update(const StringList &dictionary) override
     {
         addingAmount_ = dictionary.size()*percentage_/100;
     }
     virtual bool check(const std::string &elem)
     {
         if(lastAddedCount_<addingAmount_)
         {
             ++lastAddedCount_;
             return true;
         }
     }
     virtual void refresh()
     {
       lastAddedCount_ = 0;
     }
};


void readEBNF(const std::string &filename, std::string &grammar)
{
    using namespace std;
    ifstream ifs(filename);

    string str;
    grammar.clear();


    while(ifs>>str)
    {
        grammar.append(str);
        grammar.append(" ");
    }
}
void readAndSetSettings(const std::string &filename, Generator &g)
{
    using namespace std;
    ifstream ifs(filename);
    char data[1024];
    while(ifs.getline(data,1024))
    {
        if(data[0]==0)continue;
        string str(data);
        istringstream command(str);
        auto it = istream_iterator<string>(command);
        auto end = istream_iterator<string>();

        if(*it=="probabilities")
        {

            if(++it==end) throw runtime_error("Wrong command: "+str);
            string tokenName = *it;
            if(++it==end) throw runtime_error("Wrong command: "+str);
            size_t operatorNum = atoi(it->c_str());
            list<int> probabilities;
            while(++it!=end) probabilities.push_back(atoi(it->c_str()));

            g.setOrTokensProbabilities(tokenName,operatorNum,probabilities);

        }
        else if(*it=="probability")
        {
            if(++it==end) throw runtime_error("Wrong command: "+str);
            string tokenName = *it;
            if(++it==end) throw runtime_error("Wrong command: "+str);
            size_t operatorNum = atoi(it->c_str());
            int probability;
            if(++it==end) throw runtime_error("Wrong command: "+str);
            probability = atoi(it->c_str());
            g.setSquareBraceProbability(tokenName,operatorNum,probability);

        }
        else if(*it=="main")
        {
            if(++it==end) throw runtime_error("Wrong command: "+str);
            g.setMainTokenName(*it);
        }
        else if(*it=="dictionary")
        {
            if(++it==end) throw runtime_error("Wrong command: "+str);
            string tokenName = *it;
            if(++it==end) throw runtime_error("Wrong command: "+str);
            StringList dictionary;
            if(*it=="load")
            {
                if(++it==end) throw runtime_error("Wrong command: "+str);
                routines::loadDictionaryFromFile(*it,dictionary);
            }
            else if(*it=="set")
            {
                while(++it!=end) dictionary.push_back(*it);
            }
            else throw runtime_error("Wrong command: "+str);
            if(tokenName == "all") g.setDictionary(dictionary);
            else g.setDictionary(tokenName, dictionary);

        }
        else if(*it=="policy")
        {
            if(++it==end) throw runtime_error("Wrong command: "+str);
            string tokenName = *it;
            if(++it==end) throw runtime_error("Wrong command: "+str);
            PolicyPtr policy;
            if(*it=="DefaultPolicy")
            {
                policy = std::make_shared<DefaultPolicy>();
            }
            else if(*it=="MinMaxDPolicy")
            {
                policy = std::make_shared<MinMaxDPolicy>();
            }
            else if(*it=="MinMaxPolicy")
            {
                if(++it==end) throw runtime_error("Wrong command: "+str);
                size_t min = atoi(it->c_str());
                if(++it==end) throw runtime_error("Wrong command: "+str);
                size_t max = atoi(it->c_str());
                policy = std::make_shared<MinMaxPolicy>(min,max);
            }
            else if(*it=="NearAveragePolicy")
            {
                if(++it==end) throw runtime_error("Wrong command: "+str);
                size_t min = atof(it->c_str());
                if(++it==end) throw runtime_error("Wrong command: "+str);
                size_t max = atof(it->c_str());
                policy = std::make_shared<MinMaxPolicy>(min,max);
            }
            else if(*it=="DictionarySizePolicy")
            {
                if(++it==end) throw runtime_error("Wrong command: "+str);
                policy = std::make_shared<DictionarySizePolicy>(atoi(it->c_str()));
            }
            else throw runtime_error("Wrong command: "+str);
            if(tokenName == "all") g.setAddingPolicy(policy);
            else g.setAddingPolicy(tokenName, policy);
        }
        else throw runtime_error("Wrong command: "+str);

    }
}
void storeResults(const std::string &filename, const StringList &results, size_t size=0)
{
    std::ofstream ofs(filename);
    if(size==0) size = results.size();
    routines::showResults(results,ofs,size);
}

int main(int argc, char **argv)
{
    using namespace std;

    try
    {
        Generator generator;
        Parser parser;
        std::string grammar;

        readEBNF("input",grammar);
        generator.getTokens(grammar,parser);
        readAndSetSettings("settings",generator);
        size_t count=0;
        int attemts=-1;
        if(argc == 2)
        {
            count = static_cast<size_t>(atoi(argv[1]));
            attemts = 100;
        }
        generator.generate(count,attemts);
        storeResults("output",generator.getResults(),count);

    }
    catch(exception &e)
    {
        cerr<<"Error occured: "<<endl;
        cerr<<e.what()<<endl;
    }
    return 0;
}

