#include "routines.h"

#include <gtest/gtest.h>



TEST(RoutinesTest,advanceTest)
{   
	using namespace routines;
    std::string tested("12345");
	auto begin = tested.begin(), end = begin, it = begin;
	ASSERT_FALSE(advance_(begin, end, 10)) << "Advance begin = end";
	ASSERT_THROW(advanceOver(begin, begin, end, 10), DGException) << "Bad advance Over";
	end = tested.end();
	ASSERT_FALSE(advance_(it, end, 10)) << "Advance out of range(10)";
	ASSERT_EQ(it, end) << "Advance out of range(10)";

	advanceOver(it, begin, end, 2);
	ASSERT_EQ(it, begin+2) << "Advance out of range(10)";

    begin = tested.begin();
    ASSERT_FALSE(advance_(begin,end,6))<<"Advance out of range(6)";
    begin = tested.begin();
    ASSERT_FALSE(advance_(begin,end,5))<<"Advance out of range(5)";
    begin = tested.begin();
    ASSERT_TRUE(advance_(begin,end,3))<<"Advance in range(3)";
}

TEST(RoutinesTest,removeSpacesTest)
{

    std::string tested("a a a a a a                 aaf f ");
    routines::removeSpaces(tested);
    ASSERT_EQ(tested,"aaaaaaaaff")<<"removeSpaces";
}

TEST(RoutinesTest,readEcqSequenceTest)
{
    std::vector<std::string> input;
    std::vector<char> symbol;

    input.emplace_back("x99");
    input.emplace_back("253");
    input.emplace_back("\"");

    symbol.push_back('\x99');
    symbol.push_back(char(253));
    symbol.push_back('\"');

    auto inputI = begin(input),inputEnd = end(input);
    auto symbolI = begin(symbol),symbolEnd = end(symbol);

    for(;inputI != inputEnd && symbolI != symbolEnd; ++inputI,++symbolI)
    {

        std::string result;
        ConstStrIt begin=inputI->begin(),end = inputI->end();

        routines::readEcqSequence(begin,end,result);

        ASSERT_EQ(*symbolI,result[0])<<*inputI<<"=="<<result;
    }
}


TEST(RoutinesTest,readLiteralTest)
{
//	ASSERT_THROW()
    std::vector<std::string> input;
    std::vector<std::string> testedOutput;

    input.emplace_back("abc\"");
    input.emplace_back("фыва\"");
    input.emplace_back("\\\"aab\\\"\"");

    testedOutput.push_back("abc");
    testedOutput.push_back("фыва");
    testedOutput.push_back("\"aab\"");

    auto inputI = begin(input),inputEnd = end(input);
    auto testedI = begin(testedOutput),testedEnd = end(testedOutput);

    for(;inputI != inputEnd && testedI != testedEnd; ++inputI,++testedI)
    {

        std::string result;
        ConstStrIt begin=inputI->begin(),end = inputI->end();

        routines::readLiteralName(begin,end,result);

        ASSERT_EQ(*testedI,result)<<*inputI<<"=="<<result;
    }
}
/*
void RoutinesTester::readLiteralTest_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("simple literal")<<"abc\""<<"abc";

    QTest::newRow("russian literal")<<"фыва\""<<"фыва";
    QTest::newRow("literal whith commas")<<"\\\"aab\\\"\""<<"\"aab\"";
}
*/
TEST(RoutinesTest,throwingTest)
{
    using namespace routines;
    std::string s;
    ConstStrIt begin=s.begin(),end=s.end();
    ASSERT_THROW(readLiteralName(begin,end,s),DGException);
    ASSERT_THROW(readEcqSequence(begin,end,s),DGException);
    s.append("\\1");
    begin=s.begin();
    end=s.end();
    ASSERT_THROW(readLiteralName(begin,end,s),DGException);
    s.append("urvr7hty897j7t6");
    ASSERT_THROW(readLiteralName(begin,end,s),DGException);
}

TEST(RoutinesTest, DGExceptionTestTest)
{
	using namespace routines;
	try
	{
		throw DGException("word", '!');
	}
	catch (std::exception &e)
	{
		ASSERT_EQ(std::string(e.what()),std::string("word<!>")) << "DGException(str, lit) test";
	}
	try
	{
		throw DGException("word", '!',100);
	}
	catch (std::exception &e)
	{
		ASSERT_EQ(std::string(e.what()), std::string("word<!> at 100")) << "DGException(str, lit) test";
	}
}
TEST(RoutinesTest, ConcatNRandUnitTest)
{
	using namespace routines;
	std::vector<std::string> numbers;
	std::string result;
	//auto b = std::begin(numbers), end = std::end(numbers);
	ASSERT_THROW(ConcatNRandUnit(10, numbers, result), DGException) << "Empty container check";
	numbers.emplace_back("1");
	ConcatNRandUnit(5, numbers, result);
	ASSERT_EQ(result, "11111");
}
TEST(RoutinesTest, MainTokenNameGuardTest)
{
	std::string name = "main";
	{
		routines::MainTokenNameGuard mg(name);
		name = "other";
		ASSERT_EQ(name, "other");
	}
	ASSERT_EQ(name, "main");
}