#include <QFileDialog>

#include "ui_eventdialog.h"
#include "src/eventdialog.h"

EventDialog::EventDialog(Control *control, QWidget *parent)
	:
	QDialog(parent),
	ui(new Ui::EventDialog), control(control)
{
  ui->setupUi(this);
  this->setWindowTitle("Save Events");
}

EventDialog::~EventDialog()
{
  delete ui;
}

void
EventDialog::on_event_savePushButton_clicked()
{
  QString path = ui->event_saveLineEdit->text();
  control->saveEvents(path);
  this->close();

}

void
EventDialog::on_event_exitPushButton_clicked()
{
  this->close();

}

void
EventDialog::on_event_browsePushButton_clicked()
{
  QString fileName = QFileDialog::getSaveFileName
	  (this, tr("save event path"), QDir::currentPath(), tr("Save File Name (*.kas)"));

  if(!fileName.contains(".kas"))
	{
	  fileName.append(".kas");
	}

  ui->event_saveLineEdit->setText(fileName);
}
