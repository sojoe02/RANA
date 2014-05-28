#include <QApplication>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "physics/maphandler.h"

QImage *MainWindow::image = NULL;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);

    ui->graphicsView->setScene(&scene);

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

void MainWindow::on_exitButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_browseMapButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName
            (this, tr("Open File"),QDir::currentPath());

    if(!fileName.isEmpty()){
        delete image;
        image = new QImage(fileName);
        if(image->isNull()){
            QMessageBox::information(this,tr("Image Viewer"),
                                     tr("Cannot Load %1.").arg(fileName));
            return;
        }

        //ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
        scene.addPixmap(QPixmap::fromImage(*image));
        ui->graphicsView->show();
        mapHandler::setImage(image);



        //scaleFactor = 1.0;

    }
}

void MainWindow::write_output(const char *argMsg)
{
    ui->outputTextEdit->append(QString::fromStdString(argMsg));
}
