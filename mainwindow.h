#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QGraphicsScene>
#include <map>

#include "graphics/agentItem.h"
#include "utility.h"
#include "control.h"

class Control;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void updateMap(QImage *image);

    ~MainWindow();

    void changeRunButton(QString text);

    void advanceProgess(int percentage);
    void write_output(QString argMsg);
    void updateMap(std::list<agentInfo> infolist);
    void runButtonHide();

public slots:

    void wheelEvent(QWheelEvent *event);


private slots:

    void on_generateButton_clicked();

    void on_exitButton_clicked();

    void on_browseMapButton_clicked();

    void on_generateMap_clicked();

    void on_browseLuaAgentButton_clicked();

    void on_runButton_clicked();

    void on_updateMap(std::list<agentInfo> infolist);


signals:

    void map_updateSignal(std::list<agentInfo> infolist);

private:
    Ui::MainWindow *ui;

    void updatePosition(int Id, int x, int y);

    void advanceProgess();

    QImage *mapImage;
    QGraphicsPixmapItem *mapItem;
    QGraphicsScene *scene;
    QMap<int, agentItem* > graphAgents;
    QMutex lock;

    double factor;

    Control *control;

    void defineMap();
};

#endif // MAINWINDOW_H
