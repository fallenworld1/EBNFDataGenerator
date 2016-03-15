#include "tree.h"
#include <gtest/gtest.h>


TEST(TreeTests, EmptyTreeTest)
{
    Tree tree;
    ASSERT_THROW(tree.generate(true),routines::DGException)<<"Empty tree cant genereate";
    ASSERT_THROW(tree.generate(false),routines::DGException)<<"Empty tree cant genereate";
    ASSERT_FALSE(tree.canChange())<<"Empty tree have cleared chanbge flag";
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
    auto test = [](Tree &tree, const std::string &error,const std::string &errorString)
    {
        try
        {

            tree.buildTree(error,begin(error));
        }
        catch(exception &e)
        {
            ASSERT_EQ(string(e.what()),errorString)<<"Exception string test";
            ASSERT_FALSE(tree.isValid())<<"Tree::buildTree error. Tree valid after exception";
        }
    };


    Tree tree;
    test(tree, "main",      "Tree::buildTree error. Unexpected end of string."        );
    test(tree, "main main=","Tree::buildTree error. Wrong syntax near main"           );
    test(tree, "main=main=","Tree::buildTree error. Unexpected occurence of: <=> at 9");
    test(tree, "(}",        "Tree::buildTree error. Wrong closing brace: <}> at 1"    );
    test(tree, "*",         "Tree::buildTree error. Wrong literal: <*> at 0"          );
    //test(tree, "*",         "Tree::buildTree error. Wrong literal: <*> at 0"          );
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
TEST(TreeTests, CustomTokenTest)
{
	TreePtr tree= std::make_shared<Tree>();
	StringList result,expect;
	std::string expr = "a=\"1\"|\"2\";";
	tree->buildTree(expr, std::begin(expr));
	CustomToken ct("main",nullptr);
	ASSERT_THROW(ct.proc(result), DGException) << "Childs not set";
	ASSERT_THROW(ct.preCount(), DGException) << "Childs not set";
	ASSERT_FALSE(ct.checkType('s'));
	ASSERT_FALSE(ct.checkChildType('s'));
	ASSERT_EQ(ct.name(), "main");
	ct.setMain(tree);
	ASSERT_EQ(ct.preCount(), tree->preCount());
	tree->generate(true);
	expect = tree->getResults();
	ct.proc(result);
	ASSERT_EQ(result.size(), expect.size());
	for (int i = 0; i < result.size(); ++i)
		ASSERT_EQ(result[i], expect[i]);
}
