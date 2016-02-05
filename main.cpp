#include <iostream>
#include "tokens.h"
#include <time.h>
#include <fstream>
using namespace std;

//inline QDebug operator<<(QDebug debug, const std::string &str){
//        return debug<<str.data();
//}

void myAssert(bool condition,const string &message){
   if(!condition)cerr<<message<<endl;
}


void simpleTest(){
    MainToken mt;
    string expr = "  \"hellow generator\"  ";
    mt.buildTree(expr);
    mt.generate();
    myAssert(mt.getResults().front() == "hellow generator","Simple TestFailed");
}
void concatTest(){
    MainToken mt;
    string expr = " \"hellow \", \"generator\" ";
    mt.buildTree(expr);
    mt.generate();
    myAssert(mt.getResults().front() == "hellow generator","Concat test failed");
}
void orTest(){
    MainToken mt;
    string expr = " \"1\"| \"2\"|\"3\" ";
    mt.buildTree(expr);
    mt.generate();
    ResultType test;
    test.push_back("1");
    test.push_back("2");
    test.push_back("3");
    myAssert(mt.getResults() == test,"or test failed");
}
void braceTest(){
    MainToken mt;
    string expr = "\"1\",(\"2\"|\"3\"),\"4\"";
    mt.buildTree(expr);
    mt.generate();
    ResultType test;
    test.push_back("124");
    test.push_back("134");
    myAssert(mt.getResults() == test,"brace test failed");
}
void SquareBraceTest(){
    MainToken mt;
    string expr = " [\"1\"] , [\"2\"], [\"3\"]";
    mt.buildTree(expr);
    mt.generate();
    ResultType test;
    test.push_back("123");
    test.push_back("12");
    test.push_back("13");
    test.push_back("1");
    test.push_back("23");
    test.push_back("2");
    test.push_back("3");
    test.push_back("");

    myAssert(mt.getResults() == test,"square brace test failed(part1)");
    test.clear();
    expr = " [\"1\"]| [\"2\"]| [\"3\"]";
    mt.buildTree(expr);
    mt.generate();
    test.push_back("1");
    test.push_back("");
    test.push_back("2");
    test.push_back("3");
      myAssert(mt.getResults() == test,"square brace test failed(part2)");
}
void figurebraceTest(){
    MainToken mt;
    string expr = "{\"1\"}";
    mt.buildTree(expr);
    mt.generate();
    ResultType test;
    test.push_back("1");
    test.push_back("11");
    test.push_back("111");
    test.push_back("1111");
    myAssert(mt.getResults() == test,"figure brace test failed");
}
void RunAllTests(){
    cout<<"Starting test... \n";
    simpleTest();
    concatTest();
    orTest();
    braceTest();
    SquareBraceTest();
    figurebraceTest();
    cout<<"All tests done!\n";

}

void showResults(const ResultType &rt,ostream &os){
    os<<"results("<<rt.size()<<")\n";
    for(auto &s:rt){
        const char *str = s.c_str();
        os<<"{"<<str<<"} ";
    }
    os<<"end"<<endl;


}
/*what left
 * optimising or
 * custom tokens
 *
 * пробелы в именах токенов отсутствуют
 * тоесть "token name = expr;" нельзя
 * русские буквы и спец символы только в качестве литералов.
 */
int main()
{
   setlocale(LC_ALL,"ru-RU.UTF-8");
   srand(clock());
   RunAllTests();
   MainToken mt;
   ifstream wfs("input");

   string expr;// = "{(\"1\"|\"2\"|\"3\"|\"4\"|\"5\"|\"6\"|\"7\"|\"8\"|\"9\"|\"10\")}";
   wfs>>expr;
   std::cout<<"Building tree: "<<(mt.buildTree(expr)? "success":"fai")<<endl;
   std::cout<<"Generating out: "<<(mt.generate()? "success":"fai")<<endl;
   showResults(mt.getResults(),cout);
  // mt.globalParse(" main = [1] ; \n a = [1] ; \n b =[1] ; \n c =[1] ; \n d = [1];");

   return 0;




}

