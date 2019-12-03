#include <QtMath>
#include <QVector>

#include "GoldbachWorker.h"

GoldbachWorker::GoldbachWorker(long long number, int workerId, int workerCount, QVector<QString>& results, QObject *parent)
    : QThread{parent}
    , number{number}
    , workerId{workerId}
    , workerCount{workerCount}
    , results{results}
{
}

void GoldbachWorker::run()
{
    long long sumCount = this->calculate(this->number);
    emit calculationDone(sumCount);
}

long long GoldbachWorker::calculateStart(long long dataCount, int workerCount, int workerId, long long dataStart)
{
    Q_ASSERT(dataStart < dataCount);
    const long long equitative = workerId * ((dataCount - dataStart) / workerCount);
    const long long overload = qMin(static_cast<long long>(workerId), (dataCount - dataStart) % workerCount);
    return dataStart + equitative + overload;
}

long long GoldbachWorker::calculateFinish(long long dataCount, int workerCount, int workerId, long long dataStart)
{
    return calculateStart(dataCount, workerCount, workerId + 1, dataStart);
}

void GoldbachWorker::updateProgress(int newPercent)
{
    if ( this->progressPercent != newPercent )
    {
        this->progressPercent = newPercent;
        emit this->progressUpdated( this->workerId, this->progressPercent );
    }
}

long long GoldbachWorker::calculate(long long number)
{
    if ( number < 4 || number == 5 ) return 0;
    this->progressPercent = 0;
    return number % 2 == 0 ? calculateEvenGoldbach(number) : calculateOddGoldbach(number);
}

//#include <iostream>
long long GoldbachWorker::calculateEvenGoldbach(long long number)
{
    long long resultCount = 0;
    long long start  = calculateStart (number / 2 + 1, this->workerCount, this->workerId, 2);
    long long finish = calculateFinish(number / 2 + 1, this->workerCount, this->workerId, 2);
//    std::cout << this->workerId << ": [" << start << ", " << finish << "[" << std::endl;

    for ( long long a = start; a < finish && this->isInterruptionRequested() == false; ++a )
    {
        this->updateProgress(static_cast<int>(100LL * (a - start + 1) / (finish - start)));

        if ( ! isPrime(a) ) continue;
        long long b = number - a;
        if ( b >= a && isPrime(b) )
        {
            this->results.append( tr("%1 + %2").arg(a).arg(b) );
            ++resultCount;
        }
    }
    return resultCount;
}

long long GoldbachWorker::calculateOddGoldbach(long long number)
{
    long long resultCount = 0;
    long long start  = calculateStart (number / 3 + 1, this->workerCount, this->workerId, 2);
    long long finish = calculateFinish(number / 3 + 1, this->workerCount, this->workerId, 2);
//    std::cout << this->workerId << ": [" << start << ", " << finish << "[" << std::endl;

    for ( long long a = start; a < finish; ++a )
    {
        this->updateProgress( static_cast<int>(100LL * (a - start + 1) / (finish - start)) );

        if ( ! isPrime(a) ) continue;
        for ( long long b = a; b < number; ++b )
        {
            if ( this->isInterruptionRequested() )
                return resultCount;

            if ( ! isPrime(b) ) continue;
            long long c = number - a - b;
            if ( c >= b && isPrime(c) )
            {
                this->results.append( tr("%1 + %2 + %3").arg(a).arg(b).arg(c) );
                ++resultCount;
            }
        }
    }
    return resultCount;
}

bool GoldbachWorker::isPrime(long long number)
{
    if ( number < 2 ) return false;
    if( number == 2) return true;
    if (number % 2 == 0) return false;

    long long last = static_cast<long long>( qSqrt( number) );
    for ( long long i = 3; i <= last; i+=2 )
        if ( number % i == 0 )
            return false;

    return true;
}
