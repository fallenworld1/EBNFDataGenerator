#include <QString>
#include <QtTest>
#include "tokens.h"

class TokensTest : public QObject
{
    Q_OBJECT
    BasePtr tt1;
    BasePtr tt2;
public:
    TokensTest();

private Q_SLOTS:
    void creating();
    void orTokenTest();
    void figureBraceTokenTest();
    void concatToken();
};

TokensTest::TokensTest()
{
    tt1 = std::make_shared<TextToken>("1");
    tt2 = std::make_shared<TextToken>("2");
}

void TokensTest::creating()
{
    QVERIFY2(true, "Failure");
}

void TokensTest::orTokenTest()
{
    OrToken o;
    ResultType result, tested;
    std::string message;
    tt1->proc(tested);
    tt2->proc(tested);
    QVERIFY_EXCEPTION_THROWN(o.proc(result),myException);
    o.setChild(tt1);
    o.setChild(tt2);
    o.proc(result);
    for(auto &str:result){
        message.clear();
        message.append("Unexpected result");
        message.append(str);
        QVERIFY2(Routines::contain(tested,str),message.data());
    }

}

void TokensTest::figureBraceTokenTest()
{
    FigureBraceToken fbt;
    ResultType result,tested;
    std::string message;
    QVERIFY_EXCEPTION_THROWN(fbt.proc(result),myException);
    tt1->proc(tested);
    auto str = tested.front(), insert = str;
    for(int i=1;i<BaseToken::DefaultFigureBraceStep;++i) insert+=str;
    str+=str;
    tested.push_back(str);
    size_t count = BaseToken::DefaultFigureBraceRepeatCount/BaseToken::DefaultFigureBraceStep;
    for(size_t i=0;i<count;++i){
        str+=insert;
        tested.push_back(str);
    }
    fbt.setChild(tt1);
    fbt.proc(result);
    for(auto &str:result){
        message.clear();
        message.append("Unexpected result");
        message.append(str);
        QVERIFY2(Routines::contain(tested,str),message.data());
    }
}

void TokensTest::concatToken()
{
    ConcatToken cct;
    ResultType result,temp;
    std::string message;
    QVERIFY_EXCEPTION_THROWN(cct.proc(result),myException);
    tt1->proc(temp);
    tt2->proc(temp);
    std::string tested;
    for(const auto&s:temp) tested+=s;
    cct.setChild(tt1);
    QVERIFY_EXCEPTION_THROWN(cct.proc(result),myException);
    cct.setChild(tt2);
    cct.proc(result);
    message+=tested;
    message.append("==");
    message+=result.front();
    QVERIFY2(result.front()==tested,message.data());

}

QTEST_APPLESS_MAIN(TokensTest)

#include "tokenstest.moc"
