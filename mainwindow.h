#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void write_output(const char *argMsg);
    ~MainWindow();

private slots:

    void on_generateButton_clicked();

    void on_exitButton_clicked();

    void on_browseMapButton_clicked();

private:
    Ui::MainWindow *ui;

    void advanceProgess();

    static QImage *image;
    QGraphicsScene scene;

};

#endif // MAINWINDOW_H
