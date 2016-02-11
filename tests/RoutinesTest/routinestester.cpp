#include "routines.h"

#include <QString>
#include <QtTest>


class RoutinesTester : public QObject
{
    Q_OBJECT

public:
    RoutinesTester();
    using str=std::string;
private Q_SLOTS:
    void advance_Test();
    void removeSpacesTest();
    void readEcqSequenceTest();
    void readEcqSequenceTest_data();
    void readLiteralTest();
    void readLiteralTest_data();
    void throwingTest();
};

RoutinesTester::RoutinesTester()
{
}

void RoutinesTester::advance_Test()
{   using Routines::advance_;
    QString tested("12345");
    auto begin = tested.begin(),end = tested.end();
    QVERIFY2(advance_(begin,end,10)==false,"Advance out of range(10)");
    begin = tested.begin();
    QVERIFY2(advance_(begin,end,6)==false,"Advance out of range(6)");
    begin = tested.begin();
    QVERIFY2(advance_(begin,end,5)==false,"Advance out of range(5)");
    begin = tested.begin();
    QVERIFY2(advance_(begin,end,3),"Advance in range(3)");
}

void RoutinesTester::removeSpacesTest()
{

    str tested("a a a a a a                 aaf f ");
    Routines::removeSpaces(tested);
    QVERIFY2(tested=="aaaaaaaaff","removeSpaces");
}

void RoutinesTester::readEcqSequenceTest()
{
    QFETCH(QString,input);
    QFETCH(char,symbol);
    using Routines::constStrIt;
    using Routines::readEcqSequence;
    str tested,result,message;
    tested = input.toStdString();
    constStrIt begin=tested.begin(),end = tested.end();
    readEcqSequence(begin,end,result);
    message.append(tested);
    tested.clear();
    tested.push_back(symbol);
    message.append("==");
    message.append(result);
    QVERIFY2(tested==result,message.data());

}

void RoutinesTester::readEcqSequenceTest_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<char>("symbol");

    QTest::newRow("xNN")<<"x99"<<'\x99';

    QTest::newRow("NNN")<<"253"<<(char)253;//!!todo how that exactly interpret from "\253"?
    QTest::newRow("\\x")<<"\""<<'\"';
}

void RoutinesTester::readLiteralTest()
{
    QFETCH(QString,input);
    QFETCH(QString,output);
    using Routines::constStrIt;
    using Routines::readLiteralName;
    str tested,result,message;
    tested = input.toStdString();
    constStrIt begin=tested.begin(),end = tested.end();
    readLiteralName(begin,end,result);
    tested=output.toStdString();
    message.append(tested);
    message.append("==");
    message.append(result);
    QVERIFY2(tested==result,message.data());
}

void RoutinesTester::readLiteralTest_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("simple literal")<<"abc\""<<"abc";

    QTest::newRow("russian literal")<<"фыва\""<<"фыва";
    QTest::newRow("literal whith commas")<<"\\\"aab\\\"\""<<"\"aab\"";
}

void RoutinesTester::throwingTest()
{   using namespace Routines;
    str s;
    constStrIt begin=s.begin(),end=s.end();
    QVERIFY_EXCEPTION_THROWN(readLiteralName(begin,end,s),myException);
    QVERIFY_EXCEPTION_THROWN(readEcqSequence(begin,end,s),myException);
    s.append("\\1");
    begin=s.begin();
    end=s.end();
    QVERIFY_EXCEPTION_THROWN(readLiteralName(begin,end,s),myException);
    s.append("urvr7hty897j7t6");
    QVERIFY_EXCEPTION_THROWN(readLiteralName(begin,end,s),myException);


}


QTEST_APPLESS_MAIN(RoutinesTester)

#include "routinestester.moc"
