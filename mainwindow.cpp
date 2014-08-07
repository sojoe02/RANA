#include <QApplication>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include <thread>
#include <chrono>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "physics/maphandler.h"
#include "physics/phys.h"
#include "physics/gridmovement.h"
#include "output.h"


//QImage *MainWindow::mapItem = NULL;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),factor(1),
    mapImage(NULL), mapItem(NULL)
{
    scene = new QGraphicsScene();
    this->setWindowTitle("RANA QT version 1.0");

    ui->setupUi(this);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->graphicsView->setScene(scene);
    scene->setBackgroundBrush(Qt::gray);
    ui->runButton->hide();

    control = new Control(this);

    qRegisterMetaType<INFOLIST>("INFOLIST");

    QObject::connect(this,SIGNAL(map_updateSignal(INFOLIST)),
                     this,SLOT(on_updateMap(INFOLIST)));

    QObject::connect(this,SIGNAL(writeStringSignal(QString)),
                     this,SLOT(on_writeOutput(QString)));

    QObject::connect(this,SIGNAL(writeStatusSignal(unsigned long long,unsigned long long,unsigned long long,unsigned long long)),
                     this,SLOT(on_udateStatus(unsigned long long,unsigned long long,unsigned long long,unsigned long long)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateButton_clicked()
{
    for(auto iter=graphAgents.begin(); iter!=graphAgents.end(); ++iter) {
        scene->removeItem(*iter);
        //delete *iter;
    }
    graphAgents.clear();
    GridMovement::clearGrid();

    if(mapItem != NULL){
        ui->progressBar->setValue(0);
        QFile path(ui->agentPathLineEdit->text());
        if(path.exists())
        {
            double timeRes = 1/(double)ui->timeResSpinBox->value();
            double macroRes = ui->macroSpinBox->value();
            int agentAmount = ui->luaSpinBox->value();
            QString agentPath = ui->agentPathLineEdit->text();
            std::string stringPath = agentPath.toUtf8().constData();
            control->generateEnvironment(mapImage, 1,timeRes, macroRes,
                                         agentAmount,stringPath);

            Output::Inst()->kprintf("generating environment, %d, %s",
                                    agentAmount, stringPath.c_str());
            ui->runButton->show();
        } else
            Output::Inst()->kprintf("Cannot generate Environment: Valid path not given");
    } else
        Output::Inst()->kprintf("No map has been loaded, please do that...");
}

void MainWindow::advanceProgess(int percentage)
{
    QCoreApplication::postEvent(scene, new QEvent(QEvent::UpdateRequest),
                                Qt::LowEventPriority);
    QMetaObject::invokeMethod(ui->progressBar, "setValue", Q_ARG(int, percentage));
    //ui->progressBar->setValue(percentage);
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_browseMapButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName
            (this, tr("Open Map File"),QDir::currentPath(),
             tr("Image Files (*.png *.jpg *.bmp)"));

    ui->mapPathLineEdit->setText(fileName);

    if(!fileName.isEmpty()){
        delete mapImage;
        mapImage = new QImage(ui->mapPathLineEdit->text());
        if(mapImage->isNull()){
            QMessageBox::information(this,tr("Image Viewer"),
                                     tr("Cannot Load %1.").arg(fileName));
            return;
        }
        defineMap();
    }
}

void MainWindow::on_generateMap_clicked()
{
    if(mapImage != NULL)
        delete mapImage;

    mapImage = new QImage(ui->pxSpinBox->value(), ui->pySpinBox->value(),
                       QImage::Format_RGB32);

    mapImage->fill(Qt::GlobalColor::white);

    QRgb value = qRgb(0,0,0);

    for(int x = 0; x < mapImage->width(); x++)
    {
        for(int y = 0; y < mapImage->height(); y++)
        {
            int Bvalue = 0;
            int Gvalue = 0;
            int Rvalue = 0;

            if(Phys::getMersenneFloat(0,1) < ui->RdoubleSpinBox->value())
            {
                Rvalue = 255;
            }
            if(Phys::getMersenneFloat(0,1) < ui->GDoubleSpinBox->value())
            {
                Gvalue = 255;
            }
            if(Phys::getMersenneFloat(0,1) < ui->BDoubleSpinBox->value())
            {
                Bvalue = 255;
            }
            value = qRgb(Rvalue,Gvalue,Bvalue);
            mapImage->setPixel(x,y, value);
        }
    }

   defineMap();
}
void MainWindow::on_writeOutput(QString string)
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    ui->outputTextEdit->append(string);
}

void MainWindow::write_output(QString argMsg)
{
    emit writeStringSignal(argMsg);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

}

void MainWindow::on_writeErrOutput(QString string)
{
    ui->outputTextEdit->setTextColor(Qt::red);
    ui->outputTextEdit->append(string);
    ui->outputTextEdit->setTextColor(Qt::black);
}

void MainWindow::write_errOutput(QString argMsg)
{
    emit writeErrSignal(argMsg);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

}

void MainWindow::on_udateStatus(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents)
{
    ui->label_status1->setText(QString().setNum(ms));
    ui->label_status2->setText(QString().setNum(eventInit));
    ui->label_status3->setText(QString().setNum(internalEvents));
    ui->label_status4->setText(QString().setNum(externalEvents));
}

void MainWindow::write_status(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents)
{
    emit writeStatusSignal(ms, eventInit, internalEvents, externalEvents);
}

void MainWindow::updateMap(std::list<agentInfo> infolist)
{
    emit map_updateSignal(infolist);
}



void MainWindow::on_updateMap(INFOLIST infolist)
{
    //Output::Inst()->kprintf("updating map fired...");

    mapItem->setPixmap(QPixmap::fromImage(*mapImage));
    mapItem->setZValue(1);

    INFOLIST::iterator itr;

    for(itr = infolist.begin(); itr != infolist.end(); itr++)
    {
        int x = itr->x;
        int y = itr->y;
        int Id = itr->id;

        if(!graphAgents.contains(Id))
        {
            agentItem *gfxItem = new agentItem(QString::number(Id));
            gfxItem->setZValue(2);
            gfxItem->setX(x);
            gfxItem->setY(y);
            scene->addItem(gfxItem);

            graphAgents.insert(Id, gfxItem);
        } else
        {
            QMap<int, agentItem*>::const_iterator i = graphAgents.find(Id);
            agentItem *gfxItem = i.value();
            gfxItem->setX(x);
            gfxItem->setY(y);
        }
    }
}




void MainWindow::wheelEvent(QWheelEvent* event)
{
//    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    // Scale the view / do the zoom
//    double scaleFactor = 1.15;

//    if(event->delta() > 0) {
//        // Zoom in
//        factor = .15 + factor;
//        ui->graphicsView->scale(scaleFactor, scaleFactor);

//    } else {
//        // Zooming out
//        factor =  factor - .15;
//        ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
//    }

//    ui->zoomLabel->setText(QString().setNum(factor*100));
}

void MainWindow::on_browseLuaAgentButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName
            (this, tr("Open Map File"),QDir::currentPath(),
             tr("Lua Files (*.lua)"));

    ui->agentPathLineEdit->setText(fileName);
}

void MainWindow::on_runButton_clicked()
{
    if(control->isRunning()){
        control->stopSimulation();
    } else
        control->runSimulation(ui->runTimeSpinBox->value());
}

void MainWindow::changeRunButton(QString text)
{
    ui->runButton->setText(text);
}

void MainWindow::runButtonHide()
{
    ui->runButton->hide();
}

void MainWindow::defineMap()
{
    if(mapItem == NULL)
    {
        mapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*mapImage));
        scene->addItem(mapItem);
    }else
        mapItem->setPixmap(QPixmap::fromImage(*mapImage));

    Output::Inst()->kprintf("Map information generated");
    MapHandler::setImage(mapImage);
    Phys::setEnvironment(mapImage->width(),mapImage->height());
    //GridMovement::initGrid(mapImage->width(), mapImage->height());
}


void MainWindow::on_delaySpinBox_valueChanged(int arg1)
{
    Output::DelayValue = arg1;
}


void MainWindow::on_zoomSlider_valueChanged(int value)
{

    double scale = (double)value/100;

    ui->zoomLabel->setText(QString().setNum(value));

    ui->graphicsView->setTransform(QTransform::fromScale(scale,scale));


}
