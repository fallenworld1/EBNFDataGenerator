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
    auto test = [](const std::string &input,const char output)
    {
        std::string result;
        ConstStrIt begin=std::begin(input),end = std::end(input);

        routines::readEcqSequence(begin,end,result);

        ASSERT_EQ(output,result[0])<<output<<"=="<<result;
    };
    test("x99",'\x99');
    test("253",char(253));
    test("\"",'\"');


}


TEST(RoutinesTest,readLiteralTest)
{
    auto test = [](const std::string &input,const std::string &output)
    {
        std::string result;
        ConstStrIt begin=std::begin(input),end = std::end(input);

        routines::readLiteralName(begin,end,result);

        ASSERT_EQ(output,result)<<output<<"=="<<result;
    };

    test("abc\"","abc");
    test("фыва\"","фыва");
    test("\\\"aab\\\"\"","\"aab\"");
}

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
