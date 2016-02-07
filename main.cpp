#include <iostream>
#include <fstream>
#include "parser.h"
using namespace std;

//inline QDebug operator<<(QDebug debug, const std::string &str){
//        return debug<<str.data();
//}

void myAssert(bool condition,const string &message){
   if(!condition)cerr<<message<<endl;
}


/*void simpleTest(){
    Tree mt;
    string expr = "  \"hellow generator\"  ";
    mt.buildTree(expr);
    mt.generate();
    myAssert(mt.getResults().front() == "hellow generator","Simple TestFailed");
}
void concatTest(){
    Tree mt;
    string expr = " \"hellow \", \"generator\" ";
    mt.buildTree(expr);
    mt.generate();
    myAssert(mt.getResults().front() == "hellow generator","Concat test failed");
}
void orTest(){
    Tree mt;
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
    Tree mt;
    string expr = "\"1\",(\"2\"|\"3\"),\"4\"";
    mt.buildTree(expr);
    mt.generate();
    ResultType test;
    test.push_back("124");
    test.push_back("134");
    myAssert(mt.getResults() == test,"brace test failed");
}
void SquareBraceTest(){
    Tree mt;
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
    Tree mt;
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

}*/

void showResults(const ResultType &rt,ostream &os){
    os<<"results("<<rt.size()<<")\n";
    for(auto &s:rt){
        const char *str = s.c_str();
        os<<str<<endl;
    }
    os<<"end"<<endl;


}
/*what left
 *
 * спец символы ",;,=,\...c xnn и nnn
 */
int main()
{
   setlocale(LC_ALL,"ru-RU.UTF-8");

  // RunAllTests();
   ifstream wfs("input");
   ofstream ofs("output");
   string expr, temp;
   while(wfs>>temp){
       expr.append(" ");
       expr.append(temp);
   }
   try{
       Parser &parser = Parser::getParser();
       parser.setMainTokenName("grammar");
       parser.parse(expr);
       parser.generate(150,20);
       showResults(parser.getResults(),ofs);
    }catch(exception &e){
       cerr<<"Error occured: "<<endl;
       cerr<<e.what()<<endl;
   }

   return 0;




}

