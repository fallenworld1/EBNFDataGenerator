#include "tokens.h"
#include <gtest/gtest.h>


TEST(TokensTest,checkTypeTest)
{
    BasePtr tested = std::make_shared<TextToken>("");
    ASSERT_FALSE(tested->checkType('f'))<<"TextTokenCheck";

    tested = std::make_shared<CustomToken>("",nullptr);
    ASSERT_FALSE(tested->checkType('f'))<<"TextTokenCheck";

    tested = std::make_shared<ConcatToken>();
    ASSERT_FALSE(tested->checkType('{'))<<"ConcatTokenCheck";
    ASSERT_TRUE(tested->checkType(','))<<"ConcatTokenCheck";

    tested = std::make_shared<FigureBraceToken>();
    ASSERT_FALSE(tested->checkType('h'))<<"ConcatTokenCheck";
    ASSERT_TRUE(tested->checkType('{'))<<"ConcatTokenCheck";
    ASSERT_TRUE(tested->checkType('}'))<<"ConcatTokenCheck";

    tested = std::make_shared<SquareBraceToken>();
    ASSERT_FALSE(tested->checkType(')'))<<"ConcatTokenCheck";
    ASSERT_TRUE(tested->checkType(']'))<<"ConcatTokenCheck";
    ASSERT_TRUE(tested->checkType('['))<<"ConcatTokenCheck";

    tested = std::make_shared<RoundBraceToken>();
    ASSERT_FALSE(tested->checkType('r'))<<"ConcatTokenCheck";
    ASSERT_TRUE(tested->checkType('('))<<"ConcatTokenCheck";
    ASSERT_TRUE(tested->checkType(')'))<<"ConcatTokenCheck";

    tested = std::make_shared<OrToken>();
    ASSERT_FALSE(tested->checkType('{'))<<"ConcatTokenCheck";
    ASSERT_TRUE(tested->checkType('|'))<<"ConcatTokenCheck";
}

class ServiceClass : public testing::Test
{
protected:
	void SetUp()override{}
	void TearDown()override{}
	BasePtr tt1;
	BasePtr tt2;
public:
	ServiceClass()
	{
		tt1 = std::make_shared<TextToken>("1");
		tt2 = std::make_shared<TextToken>("2");
	}
	

	
};

TEST_F(ServiceClass,orTokenTest)
{
    OrToken o;
    StringList result, tested;
    std::string message;
    tt1->proc(tested);
    tt2->proc(tested);
    
	ASSERT_THROW(o.proc(result),DGException) << "Childs not set";
    o.setChild(tt2);//tt2 goes left
	ASSERT_THROW(o.proc(result), DGException) << "Childs not set";
    o.setChild(tt1);//tt1 goes right

	o.proc(result);
	ASSERT_EQ(tested[0], result[0]);
	ASSERT_EQ(tested[1], result[1]);
}


TEST_F(ServiceClass,figureBraceTokenTest)
{
    FigureBraceToken fbt;
    StringList result,tested;
    std::string message;
	ASSERT_THROW(fbt.proc(result), DGException) << "Childs not set";
	tested.emplace_back("");
	tt1->proc(tested);
	
	auto str = tested.back(), inserted = str;
    int step = BaseToken::DefaultFigureBraceStep, depth = BaseToken::DefaultFigureBraceDepth;
    for (int i = step; i < depth; i += step)
	{
		int j = step;
		while (j-->0)
		{
			inserted += str;
		}
		tested.emplace_back(inserted);
	}
	fbt.setChild(tt1);
	fbt.proc(result);

	ASSERT_EQ(result.size(), tested.size());
    for (int i = static_cast<int>(result.size()) - 1; i >= 0; --i)
	{
		ASSERT_EQ(result[i], tested[i]);
	}
}
TEST_F(ServiceClass, concatToken)
{
	using namespace routines;
    ConcatToken cct;
    StringList result,temp;
	std::string message,tested;
   
    tt1->proc(temp);
    tt2->proc(temp);
	temp[0] += temp[1];

	ASSERT_THROW(cct.proc(result), DGException) << "Childs not set";
	cct.setChild(tt1);
	ASSERT_THROW(cct.proc(result), DGException) << "Childs not set";
    cct.setChild(tt2);
    cct.proc(result);

	ASSERT_EQ(result[0], temp[0]);

}
TEST_F(ServiceClass, SquareBraceTokenTest)
{
	SquareBraceToken sbt;
	StringList result, temp;
	ASSERT_THROW(sbt.proc(result), DGException) << "Childs not set";
	tt1->proc(temp);
	temp.emplace_back("");
	sbt.setChild(tt1);
	sbt.proc(result);
	ASSERT_EQ(result[0], temp[0]);
	ASSERT_EQ(result[1], temp[1]);
}

