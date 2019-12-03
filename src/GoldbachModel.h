#ifndef GOLDBACHMODEL_H
#define GOLDBACHMODEL_H

#include <QAbstractListModel>
#include <QVector>

class GoldbachWorker;

class GoldbachModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(GoldbachModel)

  protected:
    //GoldbachWorker* worker = nullptr;
    QVector<GoldbachWorker*> workers;
    QVector<int> progressPercents;
    int overallProgressPercent = 0;
    int finishedWorkerCount = 0;
    QVector< QVector<QString> > results;
    int fetchedRowCount = 0;

  public:
    explicit GoldbachModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    const QString& findValue(int index) const;
    int countResults() const;
    QVector<QString> fetchAllSums() const;

    void calculate(long long number);
    void stop();

  protected:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

  signals:
    void calculationDone(long long sumCount);
    void progressUpdated(int percent) const;

  private slots:
    void workerDone(long long sumCount);
    void updateProgress(int workerId, int percent);
};

#endif // GOLDBACHMODEL_H
