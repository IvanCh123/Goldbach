#include <iostream>
#include <QDir>
#include <QTextStream>

#include "GoldbachModel.h"
#include "GoldbachTester.h"

GoldbachTester::GoldbachTester(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
}

int GoldbachTester::run()
{
    if ( this->arguments().count() <= 1 )
        return printHelp();

    for ( int index = 1; index < this->arguments().count(); ++index )
        this->testDirectory( this->arguments()[index] );
        //std::cout << index << ": {" << qPrintable(this->arguments()[index]) << "}\n";

    return this->exec();
}

int GoldbachTester::testDirectory(const QString &dirPath)
{
    QDir dir(dirPath);
    if ( not dir.exists() )
    {
        std::cerr << "Could not open directory " << qPrintable(dirPath) << std::endl;
        return EXIT_FAILURE;
    }

    dir.setFilter(QDir::Files);
    //dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    std::cout << qPrintable(dirPath) << ":" << std::endl;

    for (int fileIndex = 0; fileIndex < list.size(); ++fileIndex)
        this->testFile( list[fileIndex] );

    return EXIT_SUCCESS;
}

int GoldbachTester::printHelp()
{
    std::cout << "Usage: GoldbachTester <DIRECTORIES>\n";

    return EXIT_FAILURE;
}

int GoldbachTester::testFile(const QFileInfo &fileInfo)
{
    bool isValid = true;
    long long number = fileInfo.baseName().toLongLong(&isValid);

    if ( isValid )
    {
        return this->testContents(number, fileInfo);
    }
    else
    {
        std::cerr << "Error: invalid number in: " << qPrintable(fileInfo.fileName()) << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int GoldbachTester::testContents(long long number, const QFileInfo &fileInfo)
{
    std::cout << "Testing: " << qPrintable(fileInfo.fileName()) << "..." << std::endl;
    GoldbachModel* goldbachModel = new GoldbachModel(this);
    this->connect( goldbachModel, &GoldbachModel::calculationDone, this, &GoldbachTester::modelFinished );
    this->models.insert(goldbachModel, fileInfo);
    goldbachModel->calculate(number);

    return EXIT_SUCCESS;
}

void GoldbachTester::modelFinished(long long sumCount)
{
    Q_UNUSED(sumCount);

    GoldbachModel* goldbachModel = dynamic_cast<GoldbachModel*>( sender() );
    Q_ASSERT(goldbachModel);
    const QFileInfo& fileInfo = this->models.value( goldbachModel, QFileInfo() );
    Q_ASSERT( fileInfo.exists() );

    this->compareContents(goldbachModel, fileInfo);

    if ( ++this->finishedModelCount >= this->models.count() )
        this->quit();
}

bool GoldbachTester::compareContents(GoldbachModel *goldbachModel, const QFileInfo &fileInfo)
{
    const QVector<QString>& modelSums = goldbachModel->fetchAllSums();
    const QVector<QString>& fileSums = loadLines(fileInfo);

    if ( modelSums == fileSums )
        return true;

    std::cerr << "Test case failed: " << qPrintable(fileInfo.fileName()) << std::endl;
    return false;
}

QVector<QString> GoldbachTester::loadLines(const QFileInfo &fileInfo)
{
    QVector<QString> lines;

    QFile file( fileInfo.absoluteFilePath() );
    if ( ! file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        std::cerr << "Could not open: " << qPrintable(fileInfo.fileName()) << std::endl;
        return lines;
    }

    QTextStream textStream( &file );

    QString currentLine;
    while ( textStream.readLineInto(&currentLine) )
    {
        const QString& trimmed = currentLine.trimmed();
        if ( trimmed.length() > 0 )
            lines.append( currentLine );
    }

    return lines;
}
