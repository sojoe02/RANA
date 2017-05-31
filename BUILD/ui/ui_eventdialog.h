/********************************************************************************
** Form generated from reading UI file 'eventdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENTDIALOG_H
#define UI_EVENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_EventDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *event_savePushButton;
    QPushButton *event_browsePushButton;
    QPushButton *event_exitPushButton;
    QLineEdit *event_saveLineEdit;

    void setupUi(QDialog *EventDialog)
    {
        if (EventDialog->objectName().isEmpty())
            EventDialog->setObjectName(QStringLiteral("EventDialog"));
        EventDialog->resize(276, 88);
        gridLayout = new QGridLayout(EventDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(EventDialog);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 2, 1, 1, 1);

        event_savePushButton = new QPushButton(EventDialog);
        event_savePushButton->setObjectName(QStringLiteral("event_savePushButton"));

        gridLayout->addWidget(event_savePushButton, 4, 2, 1, 1);

        event_browsePushButton = new QPushButton(EventDialog);
        event_browsePushButton->setObjectName(QStringLiteral("event_browsePushButton"));

        gridLayout->addWidget(event_browsePushButton, 4, 1, 1, 1);

        event_exitPushButton = new QPushButton(EventDialog);
        event_exitPushButton->setObjectName(QStringLiteral("event_exitPushButton"));

        gridLayout->addWidget(event_exitPushButton, 4, 3, 1, 1);

        event_saveLineEdit = new QLineEdit(EventDialog);
        event_saveLineEdit->setObjectName(QStringLiteral("event_saveLineEdit"));

        gridLayout->addWidget(event_saveLineEdit, 3, 1, 1, 3);


        retranslateUi(EventDialog);

        QMetaObject::connectSlotsByName(EventDialog);
    } // setupUi

    void retranslateUi(QDialog *EventDialog)
    {
        EventDialog->setWindowTitle(QApplication::translate("EventDialog", "Save Event Dialog", 0));
        label->setText(QApplication::translate("EventDialog", "<html><head/><body><p><span style=\" font-weight:600;\">Path:</span></p></body></html>", 0));
        event_savePushButton->setText(QApplication::translate("EventDialog", "Save", 0));
        event_browsePushButton->setText(QApplication::translate("EventDialog", "Browse", 0));
        event_exitPushButton->setText(QApplication::translate("EventDialog", "Cancel", 0));
        event_saveLineEdit->setText(QApplication::translate("EventDialog", "events.kas", 0));
    } // retranslateUi

};

namespace Ui {
    class EventDialog: public Ui_EventDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTDIALOG_H
