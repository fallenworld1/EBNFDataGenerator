#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>

#include "src/generator.h"
#include <chrono>
/*!
 * \brief The DictionarySizePolicy class adds dictionary.size()*percentage_ elements
 */
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
        return false;
    }
    virtual void refresh()
    {
        lastAddedCount_ = 0;
    }
};

/*!
 * \brief readEBNF read file \a filename and store all to \a grammar
 * \param [in] filename file with data
 * \param [out] grammar readed data
 */
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
/*!
 * \brief The SettingsReader class reads file and process settings
 */
class SettingsReader
{
    //using namespace std;
    using str = std::istream_iterator<std::string>;
    std::string str_;
    std::istream_iterator<std::string> it,end;

    str getNext ()
    {
        if(++it==end) throw std::runtime_error("Wrong command: "+str_);
        else return it;
    }
    void getNext (size_t &next)
    {
        if(++it==end) throw std::runtime_error("Wrong command: "+str_);
        else next = static_cast<size_t>(atoi(it->c_str()));
    }
    void getNext (double &next)
    {
        if(++it==end) throw std::runtime_error("Wrong command: "+str_);
        else next = atof(it->c_str());;
    }
public:
    /*!
     * \brief Reads Settings from \a filename  And Sets it to \a g
     * \param [in] filename name of file to read
     * \param [in] g generator to tune
     */
    void readAndSetSettings(const std::string &filename, Generator &g)
    {
        using namespace std;
        ifstream ifs(filename);
        char data[1024];
        while(ifs.getline(data,1024))
        {
            if(data[0]==0)continue;
            str_ = data;
            istringstream parser(str_);
            it = str(parser);
            end = str();
            str command = it;
            if(command->front() == '#') continue;
            str tokenName = getNext();

            if(*command=="probabilities")
            {
                size_t operatorNum;
                getNext(operatorNum);
                list<int> probabilities;
                while(++it!=end) probabilities.push_back(atoi(it->c_str()));
                g.setOrTokensProbabilities(*tokenName,operatorNum,probabilities);

            }
            else if(*command=="probability")
            {
                size_t operatorNum;
                getNext(operatorNum);
                size_t probability;
                getNext(probability);
                g.setSquareBraceProbability(*tokenName,operatorNum,(int)probability);

            }
            else if(*command=="main")
            {
                g.setMainTokenName(*tokenName);
            }
            else if(*command=="dictionary")
            {
                str type = getNext();
                StringList dictionary;
                if(*type=="load")
                {
                    str filename = getNext();
                    routines::loadDictionaryFromFile(*filename,dictionary);
                }
                else if(*type=="set")
                {
                    while(++it!=end) dictionary.push_back(*it);
                }
                else if(*type=="pregenerate")
                {
                    size_t count;
                    getNext(count);
                    g.generate(*tokenName,count);
                    dictionary = g.getResults();
                }
                else throw runtime_error("Wrong command: "+str_);
                if(*tokenName == "all") g.setDictionary(dictionary);
                else g.setDictionary(*tokenName, dictionary);

            }
            else if(*command=="policy")
            {
                str type = getNext();
                PolicyPtr policy;
                if(*type=="DefaultPolicy")
                {
                    policy = std::make_shared<DefaultPolicy>();
                }
                else if(*type=="MinMaxDPolicy")
                {
                    policy = std::make_shared<MinMaxDPolicy>();
                }
                else if(*type=="MinMaxPolicy")
                {

                    size_t min,max;
                    getNext(min);
                    getNext(max);
                    policy = std::make_shared<MinMaxPolicy>(min,max);
                }
                else if(*type=="NearAveragePolicy")
                {
                    double avrg;
                    getNext(avrg);
                    policy = std::make_shared<NearAveragePolicy>(avrg);
                }
                else if(*type=="DictionarySizePolicy")
                {
                    size_t mul;
                    getNext(mul);
                    policy = std::make_shared<DictionarySizePolicy>((mul));
                }
                else throw runtime_error("Wrong command: "+str_);
                if(*tokenName == "all") g.setAddingPolicy(policy);
                else g.setAddingPolicy(*tokenName, policy);
            }
            else throw runtime_error("Wrong command: "+str_);

        }
    }
};
/*!
 * \brief store Results to file \a filename
 * \param [in] filename name of file to store
 * \param [in] results container with results to store
 * \param [in] size amoun of results to store
 */
void storeResults(const std::string &filename, StringList &results, size_t size=0)
{
    std::ofstream ofs(filename);
    if(size==0) size = results.size();
    routines::showCorrespondingResults(results,ofs,size);
}

int main(int argc, char **argv)
{
    using namespace std;

    try
    {
        Generator generator;
        Parser parser;
        std::string grammar;
        SettingsReader r;
        readEBNF("input",grammar);                  ///reads all data from file named input
        generator.getTokens(grammar,parser);        /// generates tokens by parser and store it to generator
        r.readAndSetSettings("settings",generator); ///apply settings from file settings
        size_t count=0;
        int attemts=-1;
        if(argc == 2)
        {
            count = static_cast<size_t>(atoi(argv[1]));
            attemts = 100;
        }
        generator.generate(count,attemts);         ///generates at least(if possible) count of tokens
        auto results = generator.getResults();
        storeResults("output",results,count);      ///store results to file output

    }
    catch(exception &e)
    {
        cerr<<"Error occured: "<<endl;
        cerr<<e.what()<<endl;
    }
    return 0;
}

