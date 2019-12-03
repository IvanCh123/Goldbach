#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

namespace Ui {
class MainWindow;
}

// Forward declaration
class QProgressBar;
class GoldbachModel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
    Ui::MainWindow* ui = nullptr;
    QProgressBar* progressBar = nullptr;
    GoldbachModel* model = nullptr;
    QTime time;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_lineEditNumber_textEdited(const QString &arg1);
    void on_pushButtonCalculate_clicked();
    void on_pushButtonStop_clicked();

    void calculationDone(long long sumCount);
    void updateProgressBar(int percent);
};

#endif // MAINWINDOW_H
