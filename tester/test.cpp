#include "GoldbachTester.h"
#include "GoldbachWorker.h"

void testCalculateStart()
{
    Q_ASSERT( GoldbachWorker::calculateStart(21, 7, 0, 2) == 2 );
    Q_ASSERT( GoldbachWorker::calculateStart(21, 7, 1, 2) == 5 );
    Q_ASSERT( GoldbachWorker::calculateStart(21, 7, 4, 2) == 14 );
    Q_ASSERT( GoldbachWorker::calculateStart(21, 7, 5, 2) == 17 );
    Q_ASSERT( GoldbachWorker::calculateStart(21, 7, 6, 2) == 19 );
    Q_ASSERT( GoldbachWorker::calculateStart(21, 7, 7, 2) == 21 );
}

#include <iostream>
int main(int argc, char* argv[])
{
//    for ( int index = 0; index < argc; ++index )
//        std::cout << index << ": {" << argv[index] << "}\n";

//    testCalculateStart();
    GoldbachTester application(argc, argv);
    return application.run();
}
