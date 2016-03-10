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
TEST(TreeTests, ParsingTest)
{
    Tree tree;
    std::string expr;
    ASSERT_THROW(tree.buildTree(expr,expr.begin()),routines::DGException);
    expr = "main = \"1\";";
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
