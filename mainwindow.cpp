#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateButton_clicked()
{
    if(ui->progressBar->value()==100)
    {
        ui->progressBar->setValue(0);
    }
    ui->progressBar->setValue(ui->progressBar->value()+10);
}

void MainWindow::advanceProgess()
{


}
