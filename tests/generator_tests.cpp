#include "generator.h"
#include "routines.h"
#include <gtest/gtest.h>


TEST(GeneratorTests, CreatingEmptyTest)
{
    Generator gen;
    StringList dict;
    PolicyPtr policy;
    ASSERT_THROW(gen.generate(),routines::DGException);
    ASSERT_THROW(gen.setAddingPolicy("test",policy),routines::DGException);
    ASSERT_THROW(gen.setDictionary("test",dict),routines::DGException);
    ASSERT_NO_THROW(gen.setAddingPolicy(policy));
    ASSERT_NO_THROW(gen.setDictionary(dict));
}
TEST(GeneratorTests, SimpleGeneratingTest)
{
    Generator gen;
    Parser  par;
    std::string str = "quotes = \"a\"; figure_brace = {quotes}; round_brace = (quotes); concat = quotes,figure_brace;or = quotes|figure_brace;";
    gen.getTokens(str,par);
    gen.generate("or");
    auto res = gen.getResults();
    ASSERT_GE(res.size(), 4);
    ASSERT_EQ(res[1],"a");
    ASSERT_EQ(res[3],"aaaaa");
    ASSERT_EQ(res[2],"aaa");

    gen.generate("figure_brace");
    res = gen.getResults();
    ASSERT_GE(res.size(), 3);
    ASSERT_EQ(res[0],"a");
    ASSERT_EQ(res[1],"aaa");
    ASSERT_EQ(res[2],"aaaaa");

    gen.generate("quotes",1,1);
    res = gen.getResults();
    ASSERT_EQ(res.size(),1);
    ASSERT_EQ(res[0],"a");


    gen.generate("round_brace");
    res = gen.getResults();
    ASSERT_EQ(res[0],"a");
    ASSERT_EQ(res.size(),1);
    ASSERT_THROW(gen.generate(),routines::DGException);
    ASSERT_THROW(gen.generate("NotExistedName"),routines::DGException);

    gen.setAddingPolicy(std::make_shared<MinMaxPolicy>(1, 3));
    gen.generate("or");
    res = gen.getResults();
    for (auto &r : res)
    {
        ASSERT_GE(r.size(), 1);
        ASSERT_LE(r.size(), 3);
    }
    StringList dict{ "1", "2" };

    gen.setDictionary("quotes",dict);
    gen.generate("or");
    res = gen.getResults();
    auto end = std::end(res);
    for (auto &d : dict)
    {
        ASSERT_FALSE(std::find(std::begin(res), end, d) ==  end);
    }

    str = "a=b;b=a;";
    gen.getTokens(str,par);
    ASSERT_NO_FATAL_FAILURE(gen.generate("a"));
}
TEST(GeneratorTests,ThrowingTest)
{
    using namespace std;
    Generator g;
    Parser p;
    g.getTokens("a=[\"1\"]|\"2\";",p);
    try
    {
        g.setAddingPolicy("1",nullptr);
    }
    catch(exception &e)
    {
        ASSERT_EQ(string(e.what()),"Generator::setAddingPolicy error. Error while setting dictionary. Custom token <1> not found")<<"Exception string test";

    }

}
