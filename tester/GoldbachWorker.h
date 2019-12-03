#ifndef GOLDBACHWORKER_H
#define GOLDBACHWORKER_H

#include <QThread>

class GoldbachWorker : public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY(GoldbachWorker)

  private:
    long long number = 0;
    int workerId = -1;
    int workerCount = 0;
    QVector<QString>& results;
    int progressPercent = 0;

  public:
    explicit GoldbachWorker(long long number, int workerId, int workerCount, QVector<QString>& results, QObject* parent = nullptr);
    void run() override;
    static long long calculateStart(long long dataCount, int workerCount, int workerId, long long dataStart = 0);
    static long long calculateFinish(long long dataCount, int workerCount, int workerId, long long dataStart = 0);

  signals:
    void sumFound(const QString& sum) const;
    void calculationDone(long long sumCount) const;
    void progressUpdated(int workerId, int percent) const;

  private:
    void updateProgress(int newPercent);

  private:
    /**
     * @brief Calcula las sumas de Goldbach para el numero dado y las agrega a una pizarra
     * @param number El numero dado por el usuario
     * @return La cantidad de sumas encontradas
     */
    long long calculate(long long number);
    /**
     * Calcula todas las sumas de dos primos que equivalen al numero dado, y las presenta
     * en el area resultado. Incrementa la barra de progreso mientras realiza el calculo
     * de la conjetura fuerte de Goldbach, por ejemplo:
     *
     *   4 == 2 + 2
     *   6 == 3 + 3
     *  10 == 3 + 7 == 5 + 5
     *
     * @param numero Un numero entero par mayor o igual a 4
     * @return la cantidad de sumas de dos primos encontradas
     */
    long long calculateEvenGoldbach(long long number);
    /**
     * Calcula todas las sumas de tres primos que equivalen al numero dado, y las presenta
     * en el area resultado. Incrementa la barra de progreso mientras realiza el calculo
     * de la conjetura debil de Goldbach, por ejemplo:
     *
     *   7 == 2 + 2 + 3
     *   9 == 2 + 2 + 5 == 3 + 3 + 3
     *
     * @param numero Un numero entero impar mayor o igual a 7
     * @return la cantidad de sumas de tres primos encontradas
     */
    long long calculateOddGoldbach(long long number);
    /**
     * Retorna true si numero es primo, false si numero no es primo o menor que 2
     * Por definicion 1 no es primo ni compuesto, este metodo retorna false
     */
    static bool isPrime(long long numero);
};

#endif // GOLDBACHWORKER_H
