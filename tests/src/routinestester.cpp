#include "../../src/routines.h"

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
    using Routines::constStrIt;
    using Routines::readEcqSequence;
    str tested  ="x99";
    str result="\x99", functionResult;
    constStrIt begin=tested.begin(),end = tested.end();
    readEcqSequence(begin,end,functionResult);
    QVERIFY2(tested==functionResult,"Reading esq-sequence fails");
    tested  ="111";
    result="\111";
    functionResult.clear();

}

void RoutinesTester::readEcqSequenceTest_data()
{

}


QTEST_APPLESS_MAIN(RoutinesTester)

#include "routinestester.moc"
