#include <QString>
#include <QtTest>
#include "tree.h"


class DataGeneratorTester : public QObject
{
    Q_OBJECT

public:
    DataGeneratorTester();

private Q_SLOTS:
    void testCase1();
};

DataGeneratorTester::DataGeneratorTester()
{
}

void DataGeneratorTester::testCase1()
{
    QVERIFY2(false, "Failure");
}

QTEST_APPLESS_MAIN(DataGeneratorTester)

#include "tst_datageneratortester.moc"
