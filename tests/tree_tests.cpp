#include "tree.h"
#include <gtest/gtest.h>


TEST(TreeTests, EmptyTreeTest)
{
	Tree tree;
	ASSERT_FALSE(tree.generate(true));
	ASSERT_FALSE(tree.generate(false));
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
    ASSERT_TRUE(tree.generate(true));
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

    try
    {
          Tree tree;
          tree.buildTree(error,begin(error));
    }
    catch(exception &e)
    {
       ASSERT_EQ(string(e.what()),"Unexpected end of string.");
    }
    error = "main main=";
    try
    {
          Tree tree;
          tree.buildTree(error,begin(error));
    }
    catch(exception &e)
    {
       ASSERT_EQ(string(e.what()),"Wrong syntax near main");
    }
    error = "main=main=";
    try
    {
          Tree tree;
          tree.buildTree(error,begin(error));
    }
    catch(exception &e)
    {
       ASSERT_EQ(string(e.what()),"Unexpected occurence of: <=> at 9");
    }
    error = "(}";
    try
    {
          Tree tree;
          tree.buildTree(error,begin(error));
    }
    catch(exception &e)
    {
       ASSERT_EQ(string(e.what()),"Wrong closing brace: <}> at 1");
    }
    error = "*";
    try
    {
          Tree tree;
          tree.buildTree(error,begin(error));
    }
    catch(exception &e)
    {
       ASSERT_EQ(string(e.what()),"Wrong literal: <*> at 0");
    }

}
