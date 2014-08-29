#include <QFileDialog>

#include "eventdialog.h"
#include "ui_eventdialog.h"

EventDialog::EventDialog(Control *control, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EventDialog), control(control)
{
	ui->setupUi(this);
}

EventDialog::~EventDialog()
{
	delete ui;
}

void EventDialog::on_event_savePushButton_clicked()
{


}

void EventDialog::on_event_exitPushButton_clicked()
{
	this->close();
}

void EventDialog::on_event_browsePushButton_clicked()
{
	QFileDialog dialog;

	QString fileName = dialog.getSaveFileName
			(this,tr("save event path"),
			 QDir::currentPath(),tr("Save File (*.kas)"));

	if(!fileName.contains("."))
	{
			fileName.append(".kas");
	}


	ui->event_saveLineEdit->setText(fileName);

	QString path = ui->event_saveLineEdit->text();
}
