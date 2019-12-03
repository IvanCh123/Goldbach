#include <QProgressBar>

#include "GoldbachModel.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    this->progressBar = new QProgressBar();
    this->ui->statusBar->addPermanentWidget(progressBar);
    this->ui->statusBar->showMessage(tr("Ready"));
}

MainWindow::~MainWindow()
{
	delete ui;
}

#include <iostream>
void MainWindow::on_lineEditNumber_textEdited(const QString &arg1)
{
//    std::cout << qPrintable(arg1) << std::endl;
    bool enable = arg1.trimmed().length() > 0;
    this->ui->pushButtonCalculate->setEnabled(enable);
}

void MainWindow::on_pushButtonStop_clicked()
{
    Q_ASSERT(this->model);
    this->model->stop();
}

void MainWindow::on_pushButtonCalculate_clicked()
{
    bool isValid = true;
    const QString& text = this->ui->lineEditNumber->text();

    long long int number = text.toLongLong(&isValid);

    if ( number < 4 || number == 5 )
        isValid = false;

    if ( isValid )
    {
//        this->ui->plainTextEditResults->clear();
        if ( this->model )
            this->model->deleteLater();

        this->model = new GoldbachModel(this);
        this->ui->listViewResults->setModel(model);

        this->progressBar->reset();

        this->ui->pushButtonCalculate->setEnabled(false);
        this->ui->pushButtonStop->setEnabled(true);
        ui->statusBar->showMessage( tr("Calculating...") );

        this->time.start();

        this->connect(this->model, &GoldbachModel::calculationDone, this, &MainWindow::calculationDone);
        this->connect(this->model, &GoldbachModel::progressUpdated, this, &MainWindow::updateProgressBar);

        this->model->calculate(number);
    }
    else
    {
        ui->statusBar->showMessage( tr("Invalid number: %1").arg(text) );
    }
}

void MainWindow::calculationDone(long long sumCount)
{
    double seconds = this->time.elapsed() / 1000.0;
    ui->statusBar->showMessage( tr("%1 sums found in %2 seconds").arg(sumCount).arg(seconds) );

    this->ui->pushButtonCalculate->setEnabled(true);
    this->ui->pushButtonStop->setEnabled(false);
}

void MainWindow::updateProgressBar(int percent)
{
    this->progressBar->setValue(percent);
}
