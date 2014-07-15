#include <QApplication>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <string>


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

    ui->setupUi(this);

    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->graphicsView->setScene(scene);
    ui->runButton->hide();

    control = new Control(this);

    QObject::connect(this,SIGNAL(map_updateSignal(std::list<agentInfo>)),
                     this,SLOT(on_updateMap(std::list<agentInfo>)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateButton_clicked()
{
    if(mapItem != NULL){
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
        Output::Inst()->kprintf("No map has been loaded, puuulease do that mmkay?");
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

    QRgb value = qRgb(0,0,255);

    for(int x = 0; x < mapImage->width(); x++)
    {
        for(int y = 0; y < mapImage->height(); y++)
        {
            if(Phys::getMersenneFloat(0,1) < ui->densityDoubleSpinBox->value())
            {
                mapImage->setPixel(x,y, value);
            }
        }
    }

   defineMap();
}

void MainWindow::write_output(QString argMsg)
{
    QCoreApplication::postEvent(ui->outputTextEdit, new QEvent(QEvent::UpdateRequest),
                                Qt::LowEventPriority);

    QMetaObject::invokeMethod(ui->outputTextEdit, "append", Q_ARG(QString, argMsg));

}

void MainWindow::updateMap(std::list<agentInfo> infolist)
{
    emit map_updateSignal(infolist);
}



void MainWindow::on_updateMap(std::list<agentInfo> infolist)
{
    Output::Inst()->kprintf("updating map fired...");

    mapItem->setPixmap(QPixmap::fromImage(*mapImage));
    mapItem->setZValue(1);

    std::list<agentInfo>::iterator itr;

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
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    // Scale the view / do the zoom
    double scaleFactor = 1.15;

    if(event->delta() > 0) {
        // Zoom in
        factor = .15 + factor;
        ui->graphicsView-> scale(scaleFactor, scaleFactor);

    } else {
        // Zooming out
        factor =  factor - .15;
        ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    ui->zoomLabel->setText(QString().setNum(factor*100));
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

    MapHandler::setImage(mapImage);
    Phys::setEnvironment(mapImage->width(),mapImage->height());
    GridMovement::initGrid(mapImage->width(), mapImage->height());
}

