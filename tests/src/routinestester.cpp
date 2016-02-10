#include <QString>
#include <QtTest>
#include "routines.h"


class RoutinesTester : public QObject
{
    Q_OBJECT

public:
    RoutinesTester();

private Q_SLOTS:
    void advance_Test();
    void removeSpacesTest();
    void readEcqSequenceTest();
};

RoutinesTester::RoutinesTester()
{
}

void RoutinesTester::advance_Test()
{
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
    std::string tested("a a a a a a                 aaf f ");
    removeSpaces(tested);
    QVERIFY2(tested=="aaaaaaaaff","removeSpaces");
}

void RoutinesTester::readEcqSequenceTest()
{

}


QTEST_APPLESS_MAIN(RoutinesTester)

#include "routinestester.moc"
