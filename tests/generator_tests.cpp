#include "generator.h"
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
    std::string str = "quotes = \"a\"; figure_brace = {quotes}; round_brace = (quotes); concat = quotes,figure_brace;or = quotes|figure_brace;";
    gen.getTokens(str,Parser());
    gen.generate("or");
    auto res = gen.getResults();
    ASSERT_EQ(res[0],"");
    ASSERT_EQ(res[2],"aaa");
    ASSERT_EQ(res[4],"a");
    gen.generate("figure_brace");
    res = gen.getResults();
    ASSERT_EQ(res[0],"");
    ASSERT_EQ(res[1],"a");
    ASSERT_EQ(res[3],"aaaaa");
    gen.generate("quotes",1,1);
    res = gen.getResults();
    ASSERT_EQ(res[0],"a");
    ASSERT_EQ(res.size(),1);
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
	for (auto &d : dict){
		ASSERT_NE(std::find(std::begin(res), std::end(res), d), std::end(res));
	}

}