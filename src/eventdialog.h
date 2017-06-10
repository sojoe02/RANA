#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>

#include "src/control.h"

namespace Ui
{
class EventDialog;
}

class EventDialog : public QDialog {
  Q_OBJECT

 public:
  explicit EventDialog(Control *control, QWidget *parent = 0);

  ~EventDialog();

 private
  slots
	  :
  void

  on_event_savePushButton_clicked();

  void
  on_event_exitPushButton_clicked();

  void
  on_event_browsePushButton_clicked();

 private:
  Ui::EventDialog *ui;
  Control *control;
};

#endif // EVENTDIALOG_H
