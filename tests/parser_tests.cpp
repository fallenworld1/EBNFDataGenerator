#include "parser.h"
#include <gtest/gtest.h>

TEST(ParserTests, parsingStringTest)
{
    std::string str = "quotes = \"a\"; figure_brace = {quotes}; round_brace = (quotes); concat = quotes,figure_brace;or = quotes|figure_brace;";
    Parser parser;
    parser.parse(str);
    const TreesContainer &tokens = parser.resultsOfParsing();
    ASSERT_EQ(tokens.size(),5)<<"Size check";
    ASSERT_EQ(tokens.at("quotes")->getCustomTokensList().size(),1);
    ASSERT_EQ(tokens.at("figure_brace")->getCustomTokensList().size(),2);
    ASSERT_EQ(tokens.at("round_brace")->getCustomTokensList().size(),2);
    ASSERT_EQ(tokens.at("concat")->getCustomTokensList().size(),3);
    ASSERT_EQ(tokens.at("or")->getCustomTokensList().size(),3);

    str = "figure_brace = {quotes}; round_brace = (quotes); concat = quotes,figure_brace;or = quotes|figure_brace;";
    try
    {
        parser.parse(str);
    }
    catch(std::exception &e)
    {
        ASSERT_EQ(std::string(e.what()),"Parser::link error. Token <quotes> not defined.");
    }

    str = " quotes = a; quotes = a; ";
    try
    {
        parser.parse(str);
    }
    catch(std::exception &e)
    {
        ASSERT_EQ(std::string(e.what()),"Parser::parse error. Token <quotes> is implemented more than once.");
    }
    str = "a=;";
    ASSERT_NO_THROW(parser.parse(str));


    str.clear();
    ASSERT_THROW(parser.parse(str),routines::DGException);

}
TEST(ParserTests, ConcatTest2)
{
	Parser parser;
	parser.parse("a=b,d,c;b=c;c=b;d=\"1\";");
	StringList results;
	auto tree = parser.resultsOfParsing().at("a");
	tree->generate(true);
	results = tree->getResults();
	ASSERT_EQ(results.size(), 1);
	ASSERT_EQ(results[0], "1");

	
}