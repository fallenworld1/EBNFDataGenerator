#include "tree.h"
#include <gtest/gtest.h>


TEST(TreeTests, EmptyTreeTest)
{
	Tree tree;
    ASSERT_THROW(tree.generate(true),routines::DGException);
    ASSERT_THROW(tree.generate(false),routines::DGException);
    ASSERT_FALSE(tree.canChange());
	auto r = tree.getResults();
    ASSERT_TRUE(r.empty());
    auto t = tree.getCustomTokensList();
    ASSERT_TRUE(t.empty());
    auto n = tree.name();
    ASSERT_TRUE(n.empty());

}
TEST(TreeTests, SimpleSuccessParsingTest)
{
    Tree tree;
    std::string expr;
    ASSERT_THROW(tree.buildTree(expr,expr.begin()),routines::DGException);
    expr = "  #comment#  main    =    \"1\"\t\t;";
    auto it = expr.begin();
    tree.buildTree(expr,it);
    ASSERT_FALSE(tree.canChange());
    ASSERT_EQ(it,expr.end());
    tree.generate(true);
    auto r = tree.getResults();
    ASSERT_EQ(r.size(),1);
    ASSERT_EQ(r[0],"1");
    auto t = tree.getCustomTokensList();
    ASSERT_EQ(t.size(),1);
    auto n = tree.name();
    ASSERT_EQ(n,"main");

}
TEST(TreeTests, SimpleFailParsingTest)
{
    using namespace std;

    string error("main");
    Tree tree;
    try
    {

          tree.buildTree(error,begin(error));
    }
    catch(exception &e)
    {
       ASSERT_EQ(string(e.what()),"Tree::buildTree error. Unexpected end of string.");
       ASSERT_FALSE(tree.isValid())<<"Tree::buildTree error. Tree valid after exception";
    }
    error = "main main=";
    try
    {
        tree.buildTree(error,begin(error));
    }
    catch(exception &e)
    {
        ASSERT_EQ(string(e.what()),"Tree::buildTree error. Wrong syntax near main");
        ASSERT_FALSE(tree.isValid())<<"Tree::buildTree error. Tree valid after exception";
    }
    error = "main=main=";
    try
    {
        tree.buildTree(error,begin(error));
    }
    catch(exception &e)
    {
        ASSERT_EQ(string(e.what()),"Tree::buildTree error. Unexpected occurence of: <=> at 9");
        ASSERT_FALSE(tree.isValid())<<"Tree::buildTree error. Tree valid after exception";
    }
    error = "(}";
    try
    {
        tree.buildTree(error,begin(error));
    }
    catch(exception &e)
    {
        ASSERT_EQ(string(e.what()),"Tree::buildTree error. Wrong closing brace: <}> at 1");
        ASSERT_FALSE(tree.isValid())<<"Tree::buildTree error. Tree valid after exception";
    }
    error = "*";
    try
    {
          tree.buildTree(error,begin(error));
    }
    catch(exception &e)
    {
        ASSERT_EQ(string(e.what()),"Tree::buildTree error. Wrong literal: <*> at 0");
        ASSERT_FALSE(tree.isValid())<<"Tree::buildTree error. Tree valid after exception";
    }

}
TEST(TreeTests, GeneratingTest)
{
    Tree tree;
    std::string expr = "a={[\"1\"]|[\"2\"]};";
    tree.buildTree(expr,std::begin(expr));
    ASSERT_TRUE(tree.isValid());
    tree.generate(true);
    auto results = tree.getResults();
    for(auto &r:results){
       ASSERT_TRUE((r.empty()
                    || std::find(std::begin(r),std::end(r),'1')!=std::end(r)
                    || std::find(std::begin(r),std::end(r),'2')!=std::end(r)));
       ASSERT_LE(r.size(),4);
    }
    StringList dictionary;
    dictionary.emplace_back("3");
    dictionary.emplace_back("4");
    tree.setDictionary(dictionary);
    ASSERT_TRUE(tree.isValid());
    tree.generate(true);
    results = tree.getResults();
    for(auto &d:dictionary){
       ASSERT_NE(std::find(std::begin(results),std::end(results),d),std::end(results));
    }
    tree.setPolicy(std::make_shared<MinMaxPolicy>(0,2));
    ASSERT_TRUE(tree.isValid());
    tree.generate(true);
    results = tree.getResults();
    for(auto &d:dictionary){
       ASSERT_NE(std::find(std::begin(results),std::end(results),d),std::end(results));
    }
    for(auto &r:results){
       ASSERT_LE(r.size(),2);
    }
}
