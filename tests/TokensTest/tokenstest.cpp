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
    QVERIFY_EXCEPTION_THROWN(fbt.proc(result),myException);

    tt1->proc(tested);
    for(int i=1;i<BaseToken::MinElemCountToMultiplying;++i) result.push_back(result[0]+result[i-1]);
    fbt.setChild(tt1);
    fbt.proc(result);
    for(auto &str:result){
        message.clear();
        message.append("Unexpected result");
        message.append(str);
        QVERIFY2(Routines::contain(tested,str),message.data());
    }
}

QTEST_APPLESS_MAIN(TokensTest)

#include "tokenstest.moc"
