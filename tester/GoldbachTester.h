#ifndef GOLDBACHTESTER_H
#define GOLDBACHTESTER_H

#include <QCoreApplication>
#include <QFileInfo>
#include <QMap>

class GoldbachModel;
class QFileInfo;

class GoldbachTester : public QCoreApplication
{
    Q_OBJECT
    Q_DISABLE_COPY(GoldbachTester)

  protected:
    QMap<GoldbachModel*, QFileInfo> models;
    int finishedModelCount = 0;

  public:
    explicit GoldbachTester(int &argc, char **argv);
    int run();
    int testDirectory(const QString& dirPath);
    static int printHelp();
    int testFile(const QFileInfo& fileInfo);
    int testContents(long long number, const QFileInfo& fileInfo);
    bool compareContents(GoldbachModel* goldbachModel, const QFileInfo& fileInfo);
    static QVector<QString> loadLines(const QFileInfo& fileInfo);

  protected slots:
    void modelFinished(long long sumCount);
};

#endif // GOLDBACHTESTER_H
