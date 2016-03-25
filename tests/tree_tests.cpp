#include "tree.h"
#include "treebuilder.h"
#include <gtest/gtest.h>


TEST(TreeTests, EmptyTreeTest)
{
    Tree tree;
    ASSERT_THROW(tree.generate(true),routines::DGException)<<"Empty tree cant genereate";
    ASSERT_THROW(tree.generate(false),routines::DGException)<<"Empty tree cant genereate";
    //ASSERT_FALSE(tree.canChange())<<"Empty tree have cleared chanbge flag";
    auto r = tree.getResults();
    ASSERT_TRUE(r.empty());
    auto t = tree.getCustomTokensList();
    ASSERT_TRUE(t.empty());
    auto n = tree.name();
    ASSERT_TRUE(n.empty());

}
TEST(TreeBuilderTests, SimpleSuccessParsingTest)
{
    TreeBuilder builder;
    TreePtr tree;
    std::string expr;
    auto it = expr.cbegin();
    ASSERT_THROW(builder.buildTree(expr,it),routines::DGException);
    expr = "  ##comment##  main    =    \"#\"\t\t;";
    it = expr.cbegin();
    tree = builder.buildTree(expr,it);
    //ASSERT_FALSE(tree.canChange());
    ASSERT_EQ(it,expr.end());
    tree->generate(true);
    auto r = tree->getResults();
    ASSERT_EQ(r.size(),1);
    ASSERT_EQ(r[0],"#");
    auto t = tree->getCustomTokensList();
    ASSERT_EQ(t.size(),1);
    auto n = tree->name();
    ASSERT_EQ(n,"main");



}
TEST(TreeBuilderTests, SimpleFailParsingTest)
{
    using namespace std;
    TreeBuilder builder;
    auto test = [&](const std::string &error,const std::string &errorString)
    {
        try
        {
            auto it = error.cbegin();
            builder.buildTree(error,it);
        }
        catch(exception &e)
        {
            ASSERT_EQ(string(e.what()),errorString)<<"Exception string test";
            //ASSERT_FALSE(tree.isValid())<<"Tree::buildTree error. Tree valid after exception";
            return;
        }
        FAIL()<<"Exception not thrown. Error string: "<<error;
    };


    test("main",                "Tree::buildTree error. Unexpected end of string < > at 4"            );
    test("m_a_i_n main=",       "Tree::buildTree error. Unexpected occurence of: <m> at 8"    );
    test("main=main=",          "Tree::buildTree error. Unexpected occurence of: <=> at 9"    );
    test("(}",                  "Tree::buildTree error. Unexpected occurence of: <}> at 1"    );
    test("()",                  "Tree::buildTree error. Unexpected occurence of: <)> at 1"    );
    test("(abc}",               "Tree::buildTree error. Wrong closing brace: <}> at 4"        );
    test("(abc))",              "Tree::buildTree error. Unexpected closing brace: <)> at 5"   );
    test("a = ((abc);",         "Tree::buildTree error. Unterminated brace in string<(> at 10");
    test("*",                   "Tree::buildTree error. Wrong literal in token name: <*> at 0");
    test("main=\"nnn;",         "routines::readLiteralName error. Unexpected end of string."  );
    test("| main = k;",         "Tree::buildTree error. Unexpected occurence of: <|> at 0"    );
    test("main = custom\"ll\"", "Tree::buildTree error. Unexpected occurence of: <\"> at 16"  );
    test("#main = custom\"ll\"", "Tree::buildTree error. Unexpected symbol: <#> at 1"  );

}
TEST(TreeTests, GeneratingTest)
{
    TreePtr tree;
    TreeBuilder builder;
    std::string expr = "a={[\"1\"]|[\"2\"]};   ";
    auto it = expr.cbegin();
    tree=builder.buildTree(expr,it);
    ASSERT_TRUE(tree->isValid());
    tree->generate(true);
    auto results =tree->getResults();
    ASSERT_GE(results.size(),4);
    for(auto &r:results){
        ASSERT_TRUE((r.empty()
                     || std::find(std::begin(r),std::end(r),'1')!=std::end(r)
                || std::find(std::begin(r),std::end(r),'2')!=std::end(r)));

    }

    StringList dictionary;
    dictionary.emplace_back("3");
    dictionary.emplace_back("4");
    tree->setDictionary(dictionary);
    ASSERT_TRUE(tree->isValid());
    tree->generate(true);
    results =tree->getResults();
    auto end = std::end(results);
    for(auto &d:dictionary){
        ASSERT_FALSE(std::find(std::begin(results),end,d) == end);
    }
    tree->setPolicy(std::make_shared<MinMaxPolicy>(0,2));
    ASSERT_TRUE(tree->isValid());
    tree->generate(true);
    results =tree->getResults();
    end = std::end(results);
    for(auto &d:dictionary){
        ASSERT_FALSE(std::find(std::begin(results),end,d) == end);
    }
    for(auto &r:results){
        ASSERT_LE(r.size(),2);
    }
    expr ="a=\"1\",\"2\"|\"3\";  ";
    it = expr.cbegin();
    tree = builder.buildTree(expr,it);
    tree->generate(true);
    results =tree->getResults();
    ASSERT_EQ(results[0],"12");
    ASSERT_EQ(results[1],"3");
}
TEST(TreeTests, CustomTokenTest)
{
    TreePtr tree;
    TreeBuilder builder;
    StringList result,expect;
    std::string expr = "a=\"1\"|\"2\";";
    auto it = expr.cbegin();
    tree = builder.buildTree(expr,it);
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

