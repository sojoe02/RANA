/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Open;
    QAction *action_Exit;
    QAction *action_Info;
    QAction *action_Enable_Visualisation;
    QAction *actionSave_Current_Events;
    QAction *actionDisable_Simulation_Output;
    QWidget *centralWidget;
    QGridLayout *gridLayout_6;
    QWidget *simGeneralWidget;
    QGridLayout *gridLayout_12;
    QPushButton *runButton;
    QLabel *label_8;
    QLabel *label_10;
    QLabel *label_status1;
    QPushButton *generateButton;
    QLabel *label_9;
    QProgressBar *progressBar;
    QSpinBox *delaySpinBox;
    QTabWidget *tabWidget;
    QWidget *simControlTab;
    QGridLayout *gridLayout_10;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_3;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QWidget *widget_4;
    QGridLayout *gridLayout;
    QFrame *line_2;
    QFrame *line_4;
    QFrame *line_3;
    QLabel *label_status3;
    QLabel *label_15;
    QLabel *label_3;
    QLabel *label_status4;
    QPushButton *browseMapButton;
    QSpinBox *timeResSpinBox;
    QPushButton *browseLuaAgentButton;
    QSpacerItem *verticalSpacer;
    QLineEdit *agentPathLineEdit;
    QGridLayout *gridLayout_13;
    QSpinBox *emptyPXspinBox;
    QLabel *label_39;
    QLabel *label_11;
    QSpinBox *emptyPYspinBox;
    QPushButton *generateEmptyMapButton;
    QLabel *label_7;
    QLabel *label_2;
    QLabel *label_31;
    QLabel *label_20;
    QPushButton *pushButton;
    QSpinBox *advThreadSpinBox;
    QLabel *label_40;
    QLabel *label_5;
    QSpinBox *runTimeSpinBox;
    QLabel *label_6;
    QDoubleSpinBox *scaleDoubleSpinBox;
    QSpinBox *macroSpinBox;
    QLineEdit *mapPathLineEdit;
    QLabel *label_13;
    QSpinBox *luaSpinBox;
    QSpacerItem *horizontalSpacer;
    QTextEdit *outputTextBrowser;
    QWidget *simLiveView;
    QGridLayout *gridLayout_4;
    QGraphicsView *graphicsView;
    QLabel *label_12;
    QLabel *label_14;
    QLabel *zoomLabel;
    QSlider *zoomSlider;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout;
    QCheckBox *vis_disableAgentsCheckBox;
    QCheckBox *vis_disableAgentIDs;
    QCheckBox *showAngle_checkbox;
    QWidget *vis_controlTab;
    QGridLayout *gridLayout_3;
    QWidget *widget_2;
    QGridLayout *gridLayout_8;
    QPushButton *vis_clearOutputPushButton;
    QLineEdit *vis_agentPathLineEdit;
    QLabel *label_4;
    QSpinBox *vis_fromTimeSpinBox;
    QSpinBox *vis_resolutionSpinBox;
    QPushButton *vis_processEventsPushButton;
    QPushButton *vis_agentPathPushButton;
    QLabel *label_16;
    QDoubleSpinBox *vis_zThressholdDoubleSpinBox;
    QProgressBar *vis_progressBar;
    QLabel *vis_toTimeLabel;
    QSpinBox *vis_toTimeSpinBox;
    QLabel *label_24;
    QLabel *label_17;
    QPushButton *vis_eventBrowsePushButton;
    QPushButton *vis_readInfoPushButton;
    QLineEdit *vis_eventPathLineEdit;
    QDoubleSpinBox *vis_timeResolutionDoubleSpinBox;
    QSpacerItem *verticalSpacer_3;
    QLineEdit *vis_eventRegExLineEdit;
    QLabel *label_25;
    QLabel *label_29;
    QLabel *label_28;
    QPushButton *vis_stopEventProcessingPushButton;
    QTextBrowser *vis_outputTextBrowser;
    QWidget *vis_mapTab;
    QGridLayout *gridLayout_9;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *label_26;
    QComboBox *vis_mapTypeComboBox;
    QLabel *label_27;
    QSpinBox *vis_activeMapSpinBox;
    QLabel *label_30;
    QLabel *vis_zValueLabel;
    QLabel *label_33;
    QLabel *vis_activeTimeResolutionLabel;
    QLabel *label_35;
    QLabel *vis_activeTimeLabel;
    QWidget *vis_replaceWidget;
    QLabel *label_36;
    QSpinBox *vis_controlEventPlaySpinBox;
    QPushButton *vis_eventPlayPushButton;
    QGraphicsView *vis_graphicsView;
    QWidget *widget_7;
    QGridLayout *gridLayout_5;
    QLabel *vis_zoomValueLabel;
    QLabel *label_32;
    QLabel *label_34;
    QSlider *vis_eventZoomSlider;
    QGraphicsView *vis_mapGraphicsView;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menu_Help;
    QMenu *menu_Options;
    QMenu *menuOptions;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(935, 689);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/ranalogo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setDocumentMode(false);
        MainWindow->setTabShape(QTabWidget::Rounded);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QStringLiteral("action_Open"));
        action_Exit = new QAction(MainWindow);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        action_Info = new QAction(MainWindow);
        action_Info->setObjectName(QStringLiteral("action_Info"));
        action_Enable_Visualisation = new QAction(MainWindow);
        action_Enable_Visualisation->setObjectName(QStringLiteral("action_Enable_Visualisation"));
        action_Enable_Visualisation->setCheckable(true);
        actionSave_Current_Events = new QAction(MainWindow);
        actionSave_Current_Events->setObjectName(QStringLiteral("actionSave_Current_Events"));
        actionDisable_Simulation_Output = new QAction(MainWindow);
        actionDisable_Simulation_Output->setObjectName(QStringLiteral("actionDisable_Simulation_Output"));
        actionDisable_Simulation_Output->setCheckable(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_6 = new QGridLayout(centralWidget);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        simGeneralWidget = new QWidget(centralWidget);
        simGeneralWidget->setObjectName(QStringLiteral("simGeneralWidget"));
        gridLayout_12 = new QGridLayout(simGeneralWidget);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        runButton = new QPushButton(simGeneralWidget);
        runButton->setObjectName(QStringLiteral("runButton"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(runButton->sizePolicy().hasHeightForWidth());
        runButton->setSizePolicy(sizePolicy);

        gridLayout_12->addWidget(runButton, 1, 4, 1, 1);

        label_8 = new QLabel(simGeneralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_12->addWidget(label_8, 1, 7, 1, 1);

        label_10 = new QLabel(simGeneralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_12->addWidget(label_10, 1, 9, 1, 1);

        label_status1 = new QLabel(simGeneralWidget);
        label_status1->setObjectName(QStringLiteral("label_status1"));
        label_status1->setMinimumSize(QSize(60, 0));
        label_status1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_status1->setWordWrap(false);

        gridLayout_12->addWidget(label_status1, 1, 8, 1, 1);

        generateButton = new QPushButton(simGeneralWidget);
        generateButton->setObjectName(QStringLiteral("generateButton"));
        sizePolicy.setHeightForWidth(generateButton->sizePolicy().hasHeightForWidth());
        generateButton->setSizePolicy(sizePolicy);

        gridLayout_12->addWidget(generateButton, 1, 0, 1, 1);

        label_9 = new QLabel(simGeneralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_12->addWidget(label_9, 1, 11, 1, 1);

        progressBar = new QProgressBar(simGeneralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy1);
        progressBar->setMinimumSize(QSize(0, 0));
        progressBar->setBaseSize(QSize(350, 0));
        progressBar->setValue(0);

        gridLayout_12->addWidget(progressBar, 1, 10, 1, 1);

        delaySpinBox = new QSpinBox(simGeneralWidget);
        delaySpinBox->setObjectName(QStringLiteral("delaySpinBox"));
        delaySpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        delaySpinBox->setMaximum(10000);
        delaySpinBox->setSingleStep(1);

        gridLayout_12->addWidget(delaySpinBox, 1, 12, 1, 1);


        gridLayout_6->addWidget(simGeneralWidget, 3, 0, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        simControlTab = new QWidget();
        simControlTab->setObjectName(QStringLiteral("simControlTab"));
        gridLayout_10 = new QGridLayout(simControlTab);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        scrollArea = new QScrollArea(simControlTab);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy2);
        scrollArea->setMinimumSize(QSize(450, 0));
        scrollArea->setBaseSize(QSize(0, 0));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 448, 535));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget_3 = new QWidget(scrollAreaWidgetContents);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy3);
        widget_3->setMinimumSize(QSize(0, 0));
        gridLayout_2 = new QGridLayout(widget_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label = new QLabel(widget_3);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 5, 0, 1, 1);

        widget_4 = new QWidget(widget_3);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        sizePolicy1.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy1);
        widget_4->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(widget_4);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        line_2 = new QFrame(widget_4);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 0, 4, 1, 1);

        line_4 = new QFrame(widget_4);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_4, 2, 4, 1, 1);

        line_3 = new QFrame(widget_4);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 1, 4, 1, 1);

        label_status3 = new QLabel(widget_4);
        label_status3->setObjectName(QStringLiteral("label_status3"));
        label_status3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_status3, 1, 3, 1, 1);

        label_15 = new QLabel(widget_4);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout->addWidget(label_15, 1, 5, 1, 1);

        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 2, 1, 1);

        label_status4 = new QLabel(widget_4);
        label_status4->setObjectName(QStringLiteral("label_status4"));
        label_status4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_status4, 1, 6, 1, 1);


        gridLayout_2->addWidget(widget_4, 22, 0, 1, 3);

        browseMapButton = new QPushButton(widget_3);
        browseMapButton->setObjectName(QStringLiteral("browseMapButton"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(browseMapButton->sizePolicy().hasHeightForWidth());
        browseMapButton->setSizePolicy(sizePolicy4);

        gridLayout_2->addWidget(browseMapButton, 7, 2, 1, 1);

        timeResSpinBox = new QSpinBox(widget_3);
        timeResSpinBox->setObjectName(QStringLiteral("timeResSpinBox"));
        sizePolicy.setHeightForWidth(timeResSpinBox->sizePolicy().hasHeightForWidth());
        timeResSpinBox->setSizePolicy(sizePolicy);
        timeResSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timeResSpinBox->setAccelerated(true);
        timeResSpinBox->setMinimum(1);
        timeResSpinBox->setMaximum(12);
        timeResSpinBox->setSingleStep(1);
        timeResSpinBox->setValue(6);
        timeResSpinBox->setDisplayIntegerBase(10);

        gridLayout_2->addWidget(timeResSpinBox, 1, 2, 1, 1);

        browseLuaAgentButton = new QPushButton(widget_3);
        browseLuaAgentButton->setObjectName(QStringLiteral("browseLuaAgentButton"));

        gridLayout_2->addWidget(browseLuaAgentButton, 5, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 21, 0, 1, 1);

        agentPathLineEdit = new QLineEdit(widget_3);
        agentPathLineEdit->setObjectName(QStringLiteral("agentPathLineEdit"));
        sizePolicy1.setHeightForWidth(agentPathLineEdit->sizePolicy().hasHeightForWidth());
        agentPathLineEdit->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(agentPathLineEdit, 6, 0, 1, 3);

        gridLayout_13 = new QGridLayout();
        gridLayout_13->setSpacing(6);
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        emptyPXspinBox = new QSpinBox(widget_3);
        emptyPXspinBox->setObjectName(QStringLiteral("emptyPXspinBox"));
        sizePolicy4.setHeightForWidth(emptyPXspinBox->sizePolicy().hasHeightForWidth());
        emptyPXspinBox->setSizePolicy(sizePolicy4);
        emptyPXspinBox->setMinimumSize(QSize(75, 0));
        emptyPXspinBox->setMaximum(10000);
        emptyPXspinBox->setSingleStep(50);
        emptyPXspinBox->setValue(200);

        gridLayout_13->addWidget(emptyPXspinBox, 1, 1, 1, 1);

        label_39 = new QLabel(widget_3);
        label_39->setObjectName(QStringLiteral("label_39"));

        gridLayout_13->addWidget(label_39, 1, 0, 1, 1);

        label_11 = new QLabel(widget_3);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_13->addWidget(label_11, 1, 2, 1, 1);

        emptyPYspinBox = new QSpinBox(widget_3);
        emptyPYspinBox->setObjectName(QStringLiteral("emptyPYspinBox"));
        sizePolicy4.setHeightForWidth(emptyPYspinBox->sizePolicy().hasHeightForWidth());
        emptyPYspinBox->setSizePolicy(sizePolicy4);
        emptyPYspinBox->setMinimumSize(QSize(75, 0));
        emptyPYspinBox->setMaximum(10000);
        emptyPYspinBox->setSingleStep(50);
        emptyPYspinBox->setValue(200);

        gridLayout_13->addWidget(emptyPYspinBox, 1, 4, 1, 1);


        gridLayout_2->addLayout(gridLayout_13, 10, 0, 1, 1);

        generateEmptyMapButton = new QPushButton(widget_3);
        generateEmptyMapButton->setObjectName(QStringLiteral("generateEmptyMapButton"));

        gridLayout_2->addWidget(generateEmptyMapButton, 10, 2, 1, 1);

        label_7 = new QLabel(widget_3);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_2->addWidget(label_7, 7, 0, 1, 1);

        label_2 = new QLabel(widget_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 4, 0, 1, 1);

        label_31 = new QLabel(widget_3);
        label_31->setObjectName(QStringLiteral("label_31"));

        gridLayout_2->addWidget(label_31, 9, 0, 1, 1);

        label_20 = new QLabel(widget_3);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_2->addWidget(label_20, 19, 0, 1, 1);

        pushButton = new QPushButton(widget_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_2->addWidget(pushButton, 23, 2, 1, 1);

        advThreadSpinBox = new QSpinBox(widget_3);
        advThreadSpinBox->setObjectName(QStringLiteral("advThreadSpinBox"));
        advThreadSpinBox->setLayoutDirection(Qt::LeftToRight);
        advThreadSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        advThreadSpinBox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        advThreadSpinBox->setMinimum(1);
        advThreadSpinBox->setValue(4);

        gridLayout_2->addWidget(advThreadSpinBox, 19, 2, 1, 1);

        label_40 = new QLabel(widget_3);
        label_40->setObjectName(QStringLiteral("label_40"));

        gridLayout_2->addWidget(label_40, 12, 0, 1, 1);

        label_5 = new QLabel(widget_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setToolTipDuration(-1);

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        runTimeSpinBox = new QSpinBox(widget_3);
        runTimeSpinBox->setObjectName(QStringLiteral("runTimeSpinBox"));
        sizePolicy.setHeightForWidth(runTimeSpinBox->sizePolicy().hasHeightForWidth());
        runTimeSpinBox->setSizePolicy(sizePolicy);
        runTimeSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        runTimeSpinBox->setAccelerated(true);
        runTimeSpinBox->setMinimum(1);
        runTimeSpinBox->setMaximum(31536000);
        runTimeSpinBox->setSingleStep(100);
        runTimeSpinBox->setValue(100);

        gridLayout_2->addWidget(runTimeSpinBox, 3, 2, 1, 1);

        label_6 = new QLabel(widget_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 2, 0, 1, 1);

        scaleDoubleSpinBox = new QDoubleSpinBox(widget_3);
        scaleDoubleSpinBox->setObjectName(QStringLiteral("scaleDoubleSpinBox"));
        scaleDoubleSpinBox->setFrame(true);
        scaleDoubleSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        scaleDoubleSpinBox->setAccelerated(true);
        scaleDoubleSpinBox->setDecimals(5);
        scaleDoubleSpinBox->setMinimum(0.0001);
        scaleDoubleSpinBox->setMaximum(100);
        scaleDoubleSpinBox->setSingleStep(0.001);
        scaleDoubleSpinBox->setValue(1);

        gridLayout_2->addWidget(scaleDoubleSpinBox, 12, 2, 1, 1);

        macroSpinBox = new QSpinBox(widget_3);
        macroSpinBox->setObjectName(QStringLiteral("macroSpinBox"));
        sizePolicy.setHeightForWidth(macroSpinBox->sizePolicy().hasHeightForWidth());
        macroSpinBox->setSizePolicy(sizePolicy);
        macroSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        macroSpinBox->setMinimum(1);
        macroSpinBox->setMaximum(12);
        macroSpinBox->setSingleStep(1);
        macroSpinBox->setValue(3);

        gridLayout_2->addWidget(macroSpinBox, 2, 2, 1, 1);

        mapPathLineEdit = new QLineEdit(widget_3);
        mapPathLineEdit->setObjectName(QStringLiteral("mapPathLineEdit"));

        gridLayout_2->addWidget(mapPathLineEdit, 8, 0, 1, 3);

        label_13 = new QLabel(widget_3);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_2->addWidget(label_13, 3, 0, 1, 1);

        luaSpinBox = new QSpinBox(widget_3);
        luaSpinBox->setObjectName(QStringLiteral("luaSpinBox"));
        sizePolicy.setHeightForWidth(luaSpinBox->sizePolicy().hasHeightForWidth());
        luaSpinBox->setSizePolicy(sizePolicy);
        luaSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        luaSpinBox->setMinimum(1);
        luaSpinBox->setMaximum(65536);
        luaSpinBox->setValue(1);

        gridLayout_2->addWidget(luaSpinBox, 4, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 21, 1, 1, 1);


        verticalLayout_2->addWidget(widget_3);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_10->addWidget(scrollArea, 0, 1, 1, 1);

        outputTextBrowser = new QTextEdit(simControlTab);
        outputTextBrowser->setObjectName(QStringLiteral("outputTextBrowser"));
        sizePolicy3.setHeightForWidth(outputTextBrowser->sizePolicy().hasHeightForWidth());
        outputTextBrowser->setSizePolicy(sizePolicy3);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush1(QColor(200, 200, 200, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        QBrush brush2(QColor(190, 190, 190, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        QBrush brush3(QColor(239, 235, 231, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        outputTextBrowser->setPalette(palette);

        gridLayout_10->addWidget(outputTextBrowser, 0, 2, 1, 1);

        tabWidget->addTab(simControlTab, QString());
        simLiveView = new QWidget();
        simLiveView->setObjectName(QStringLiteral("simLiveView"));
        gridLayout_4 = new QGridLayout(simLiveView);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        graphicsView = new QGraphicsView(simLiveView);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        gridLayout_4->addWidget(graphicsView, 0, 1, 1, 4);

        label_12 = new QLabel(simLiveView);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_4->addWidget(label_12, 1, 4, 1, 1);

        label_14 = new QLabel(simLiveView);
        label_14->setObjectName(QStringLiteral("label_14"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy5);
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_14, 1, 2, 1, 1);

        zoomLabel = new QLabel(simLiveView);
        zoomLabel->setObjectName(QStringLiteral("zoomLabel"));
        sizePolicy5.setHeightForWidth(zoomLabel->sizePolicy().hasHeightForWidth());
        zoomLabel->setSizePolicy(sizePolicy5);
        zoomLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(zoomLabel, 1, 3, 1, 1);

        zoomSlider = new QSlider(simLiveView);
        zoomSlider->setObjectName(QStringLiteral("zoomSlider"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(zoomSlider->sizePolicy().hasHeightForWidth());
        zoomSlider->setSizePolicy(sizePolicy6);
        zoomSlider->setMinimum(0);
        zoomSlider->setMaximum(2000);
        zoomSlider->setValue(100);
        zoomSlider->setOrientation(Qt::Horizontal);
        zoomSlider->setInvertedAppearance(false);
        zoomSlider->setInvertedControls(false);

        gridLayout_4->addWidget(zoomSlider, 1, 1, 1, 1);

        widget_6 = new QWidget(simLiveView);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        horizontalLayout = new QHBoxLayout(widget_6);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, 0);
        vis_disableAgentsCheckBox = new QCheckBox(widget_6);
        vis_disableAgentsCheckBox->setObjectName(QStringLiteral("vis_disableAgentsCheckBox"));

        horizontalLayout->addWidget(vis_disableAgentsCheckBox);

        vis_disableAgentIDs = new QCheckBox(widget_6);
        vis_disableAgentIDs->setObjectName(QStringLiteral("vis_disableAgentIDs"));

        horizontalLayout->addWidget(vis_disableAgentIDs);

        showAngle_checkbox = new QCheckBox(widget_6);
        showAngle_checkbox->setObjectName(QStringLiteral("showAngle_checkbox"));

        horizontalLayout->addWidget(showAngle_checkbox);


        gridLayout_4->addWidget(widget_6, 2, 1, 1, 4);

        tabWidget->addTab(simLiveView, QString());
        vis_controlTab = new QWidget();
        vis_controlTab->setObjectName(QStringLiteral("vis_controlTab"));
        gridLayout_3 = new QGridLayout(vis_controlTab);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        widget_2 = new QWidget(vis_controlTab);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        QSizePolicy sizePolicy7(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy7);
        widget_2->setMinimumSize(QSize(300, 0));
        widget_2->setMaximumSize(QSize(400, 16777215));
        gridLayout_8 = new QGridLayout(widget_2);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        vis_clearOutputPushButton = new QPushButton(widget_2);
        vis_clearOutputPushButton->setObjectName(QStringLiteral("vis_clearOutputPushButton"));

        gridLayout_8->addWidget(vis_clearOutputPushButton, 27, 2, 1, 1);

        vis_agentPathLineEdit = new QLineEdit(widget_2);
        vis_agentPathLineEdit->setObjectName(QStringLiteral("vis_agentPathLineEdit"));
        sizePolicy1.setHeightForWidth(vis_agentPathLineEdit->sizePolicy().hasHeightForWidth());
        vis_agentPathLineEdit->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(vis_agentPathLineEdit, 5, 0, 1, 3);

        label_4 = new QLabel(widget_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_8->addWidget(label_4, 2, 0, 1, 1);

        vis_fromTimeSpinBox = new QSpinBox(widget_2);
        vis_fromTimeSpinBox->setObjectName(QStringLiteral("vis_fromTimeSpinBox"));
        vis_fromTimeSpinBox->setWrapping(true);
        vis_fromTimeSpinBox->setAccelerated(true);
        vis_fromTimeSpinBox->setMinimum(-1);
        vis_fromTimeSpinBox->setMaximum(-1);
        vis_fromTimeSpinBox->setSingleStep(1);

        gridLayout_8->addWidget(vis_fromTimeSpinBox, 17, 2, 1, 1);

        vis_resolutionSpinBox = new QSpinBox(widget_2);
        vis_resolutionSpinBox->setObjectName(QStringLiteral("vis_resolutionSpinBox"));
        vis_resolutionSpinBox->setMaximum(21);
        vis_resolutionSpinBox->setValue(5);

        gridLayout_8->addWidget(vis_resolutionSpinBox, 16, 2, 1, 1);

        vis_processEventsPushButton = new QPushButton(widget_2);
        vis_processEventsPushButton->setObjectName(QStringLiteral("vis_processEventsPushButton"));

        gridLayout_8->addWidget(vis_processEventsPushButton, 25, 2, 1, 1);

        vis_agentPathPushButton = new QPushButton(widget_2);
        vis_agentPathPushButton->setObjectName(QStringLiteral("vis_agentPathPushButton"));

        gridLayout_8->addWidget(vis_agentPathPushButton, 4, 2, 1, 1);

        label_16 = new QLabel(widget_2);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_8->addWidget(label_16, 16, 0, 1, 1);

        vis_zThressholdDoubleSpinBox = new QDoubleSpinBox(widget_2);
        vis_zThressholdDoubleSpinBox->setObjectName(QStringLiteral("vis_zThressholdDoubleSpinBox"));
        vis_zThressholdDoubleSpinBox->setMaximum(1);
        vis_zThressholdDoubleSpinBox->setSingleStep(0.01);
        vis_zThressholdDoubleSpinBox->setValue(0.02);

        gridLayout_8->addWidget(vis_zThressholdDoubleSpinBox, 14, 2, 1, 1);

        vis_progressBar = new QProgressBar(widget_2);
        vis_progressBar->setObjectName(QStringLiteral("vis_progressBar"));
        sizePolicy1.setHeightForWidth(vis_progressBar->sizePolicy().hasHeightForWidth());
        vis_progressBar->setSizePolicy(sizePolicy1);
        vis_progressBar->setMinimumSize(QSize(0, 0));
        vis_progressBar->setBaseSize(QSize(350, 0));
        vis_progressBar->setValue(0);

        gridLayout_8->addWidget(vis_progressBar, 28, 0, 1, 3);

        vis_toTimeLabel = new QLabel(widget_2);
        vis_toTimeLabel->setObjectName(QStringLiteral("vis_toTimeLabel"));

        gridLayout_8->addWidget(vis_toTimeLabel, 18, 0, 1, 1);

        vis_toTimeSpinBox = new QSpinBox(widget_2);
        vis_toTimeSpinBox->setObjectName(QStringLiteral("vis_toTimeSpinBox"));
        vis_toTimeSpinBox->setWrapping(true);
        vis_toTimeSpinBox->setAccelerated(true);
        vis_toTimeSpinBox->setMaximum(-1);
        vis_toTimeSpinBox->setSingleStep(1);

        gridLayout_8->addWidget(vis_toTimeSpinBox, 18, 2, 1, 1);

        label_24 = new QLabel(widget_2);
        label_24->setObjectName(QStringLiteral("label_24"));

        gridLayout_8->addWidget(label_24, 4, 0, 1, 1);

        label_17 = new QLabel(widget_2);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_8->addWidget(label_17, 17, 0, 1, 1);

        vis_eventBrowsePushButton = new QPushButton(widget_2);
        vis_eventBrowsePushButton->setObjectName(QStringLiteral("vis_eventBrowsePushButton"));

        gridLayout_8->addWidget(vis_eventBrowsePushButton, 2, 2, 1, 1);

        vis_readInfoPushButton = new QPushButton(widget_2);
        vis_readInfoPushButton->setObjectName(QStringLiteral("vis_readInfoPushButton"));

        gridLayout_8->addWidget(vis_readInfoPushButton, 10, 2, 1, 1);

        vis_eventPathLineEdit = new QLineEdit(widget_2);
        vis_eventPathLineEdit->setObjectName(QStringLiteral("vis_eventPathLineEdit"));

        gridLayout_8->addWidget(vis_eventPathLineEdit, 3, 0, 1, 3);

        vis_timeResolutionDoubleSpinBox = new QDoubleSpinBox(widget_2);
        vis_timeResolutionDoubleSpinBox->setObjectName(QStringLiteral("vis_timeResolutionDoubleSpinBox"));
        vis_timeResolutionDoubleSpinBox->setAccelerated(true);
        vis_timeResolutionDoubleSpinBox->setMinimum(0.01);
        vis_timeResolutionDoubleSpinBox->setSingleStep(0.01);
        vis_timeResolutionDoubleSpinBox->setValue(0.25);

        gridLayout_8->addWidget(vis_timeResolutionDoubleSpinBox, 13, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_3, 23, 1, 1, 1);

        vis_eventRegExLineEdit = new QLineEdit(widget_2);
        vis_eventRegExLineEdit->setObjectName(QStringLiteral("vis_eventRegExLineEdit"));

        gridLayout_8->addWidget(vis_eventRegExLineEdit, 20, 0, 1, 3);

        label_25 = new QLabel(widget_2);
        label_25->setObjectName(QStringLiteral("label_25"));

        gridLayout_8->addWidget(label_25, 14, 0, 1, 1);

        label_29 = new QLabel(widget_2);
        label_29->setObjectName(QStringLiteral("label_29"));

        gridLayout_8->addWidget(label_29, 13, 0, 1, 1);

        label_28 = new QLabel(widget_2);
        label_28->setObjectName(QStringLiteral("label_28"));

        gridLayout_8->addWidget(label_28, 19, 0, 1, 1);

        vis_stopEventProcessingPushButton = new QPushButton(widget_2);
        vis_stopEventProcessingPushButton->setObjectName(QStringLiteral("vis_stopEventProcessingPushButton"));

        gridLayout_8->addWidget(vis_stopEventProcessingPushButton, 26, 2, 1, 1);


        gridLayout_3->addWidget(widget_2, 0, 0, 2, 1);

        vis_outputTextBrowser = new QTextBrowser(vis_controlTab);
        vis_outputTextBrowser->setObjectName(QStringLiteral("vis_outputTextBrowser"));
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(vis_outputTextBrowser->sizePolicy().hasHeightForWidth());
        vis_outputTextBrowser->setSizePolicy(sizePolicy8);

        gridLayout_3->addWidget(vis_outputTextBrowser, 1, 1, 1, 1);

        tabWidget->addTab(vis_controlTab, QString());
        vis_mapTab = new QWidget();
        vis_mapTab->setObjectName(QStringLiteral("vis_mapTab"));
        gridLayout_9 = new QGridLayout(vis_mapTab);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        widget = new QWidget(vis_mapTab);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(150, 0));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_26 = new QLabel(widget);
        label_26->setObjectName(QStringLiteral("label_26"));

        verticalLayout->addWidget(label_26);

        vis_mapTypeComboBox = new QComboBox(widget);
        vis_mapTypeComboBox->setObjectName(QStringLiteral("vis_mapTypeComboBox"));

        verticalLayout->addWidget(vis_mapTypeComboBox);

        label_27 = new QLabel(widget);
        label_27->setObjectName(QStringLiteral("label_27"));

        verticalLayout->addWidget(label_27);

        vis_activeMapSpinBox = new QSpinBox(widget);
        vis_activeMapSpinBox->setObjectName(QStringLiteral("vis_activeMapSpinBox"));
        vis_activeMapSpinBox->setWrapping(true);
        vis_activeMapSpinBox->setAccelerated(true);
        vis_activeMapSpinBox->setMinimum(0);
        vis_activeMapSpinBox->setMaximum(99999);
        vis_activeMapSpinBox->setSingleStep(1);
        vis_activeMapSpinBox->setValue(0);

        verticalLayout->addWidget(vis_activeMapSpinBox);

        label_30 = new QLabel(widget);
        label_30->setObjectName(QStringLiteral("label_30"));

        verticalLayout->addWidget(label_30);

        vis_zValueLabel = new QLabel(widget);
        vis_zValueLabel->setObjectName(QStringLiteral("vis_zValueLabel"));

        verticalLayout->addWidget(vis_zValueLabel);

        label_33 = new QLabel(widget);
        label_33->setObjectName(QStringLiteral("label_33"));

        verticalLayout->addWidget(label_33);

        vis_activeTimeResolutionLabel = new QLabel(widget);
        vis_activeTimeResolutionLabel->setObjectName(QStringLiteral("vis_activeTimeResolutionLabel"));

        verticalLayout->addWidget(vis_activeTimeResolutionLabel);

        label_35 = new QLabel(widget);
        label_35->setObjectName(QStringLiteral("label_35"));

        verticalLayout->addWidget(label_35);

        vis_activeTimeLabel = new QLabel(widget);
        vis_activeTimeLabel->setObjectName(QStringLiteral("vis_activeTimeLabel"));

        verticalLayout->addWidget(vis_activeTimeLabel);

        vis_replaceWidget = new QWidget(widget);
        vis_replaceWidget->setObjectName(QStringLiteral("vis_replaceWidget"));
        QSizePolicy sizePolicy9(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(vis_replaceWidget->sizePolicy().hasHeightForWidth());
        vis_replaceWidget->setSizePolicy(sizePolicy9);

        verticalLayout->addWidget(vis_replaceWidget);

        label_36 = new QLabel(widget);
        label_36->setObjectName(QStringLiteral("label_36"));

        verticalLayout->addWidget(label_36);

        vis_controlEventPlaySpinBox = new QSpinBox(widget);
        vis_controlEventPlaySpinBox->setObjectName(QStringLiteral("vis_controlEventPlaySpinBox"));
        vis_controlEventPlaySpinBox->setAccelerated(true);
        vis_controlEventPlaySpinBox->setMinimum(10);
        vis_controlEventPlaySpinBox->setMaximum(10000);
        vis_controlEventPlaySpinBox->setSingleStep(10);
        vis_controlEventPlaySpinBox->setValue(200);

        verticalLayout->addWidget(vis_controlEventPlaySpinBox);

        vis_eventPlayPushButton = new QPushButton(widget);
        vis_eventPlayPushButton->setObjectName(QStringLiteral("vis_eventPlayPushButton"));

        verticalLayout->addWidget(vis_eventPlayPushButton);


        gridLayout_9->addWidget(widget, 0, 1, 1, 1);

        vis_graphicsView = new QGraphicsView(vis_mapTab);
        vis_graphicsView->setObjectName(QStringLiteral("vis_graphicsView"));
        QSizePolicy sizePolicy10(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(vis_graphicsView->sizePolicy().hasHeightForWidth());
        vis_graphicsView->setSizePolicy(sizePolicy10);

        gridLayout_9->addWidget(vis_graphicsView, 0, 2, 1, 3);

        widget_7 = new QWidget(vis_mapTab);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        QSizePolicy sizePolicy11(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy11.setHorizontalStretch(0);
        sizePolicy11.setVerticalStretch(0);
        sizePolicy11.setHeightForWidth(widget_7->sizePolicy().hasHeightForWidth());
        widget_7->setSizePolicy(sizePolicy11);
        gridLayout_5 = new QGridLayout(widget_7);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        vis_zoomValueLabel = new QLabel(widget_7);
        vis_zoomValueLabel->setObjectName(QStringLiteral("vis_zoomValueLabel"));
        vis_zoomValueLabel->setMinimumSize(QSize(50, 0));

        gridLayout_5->addWidget(vis_zoomValueLabel, 1, 2, 1, 1);

        label_32 = new QLabel(widget_7);
        label_32->setObjectName(QStringLiteral("label_32"));

        gridLayout_5->addWidget(label_32, 1, 1, 1, 1);

        label_34 = new QLabel(widget_7);
        label_34->setObjectName(QStringLiteral("label_34"));

        gridLayout_5->addWidget(label_34, 1, 3, 1, 1);

        vis_eventZoomSlider = new QSlider(widget_7);
        vis_eventZoomSlider->setObjectName(QStringLiteral("vis_eventZoomSlider"));
        vis_eventZoomSlider->setMinimum(1);
        vis_eventZoomSlider->setMaximum(2000);
        vis_eventZoomSlider->setSingleStep(50);
        vis_eventZoomSlider->setPageStep(100);
        vis_eventZoomSlider->setValue(100);
        vis_eventZoomSlider->setOrientation(Qt::Horizontal);
        vis_eventZoomSlider->setInvertedAppearance(false);
        vis_eventZoomSlider->setInvertedControls(false);

        gridLayout_5->addWidget(vis_eventZoomSlider, 1, 0, 1, 1);


        gridLayout_9->addWidget(widget_7, 1, 1, 1, 4);

        vis_mapGraphicsView = new QGraphicsView(vis_mapTab);
        vis_mapGraphicsView->setObjectName(QStringLiteral("vis_mapGraphicsView"));
        sizePolicy10.setHeightForWidth(vis_mapGraphicsView->sizePolicy().hasHeightForWidth());
        vis_mapGraphicsView->setSizePolicy(sizePolicy10);
        vis_mapGraphicsView->setMinimumSize(QSize(80, 0));
        vis_mapGraphicsView->setMaximumSize(QSize(80, 16777215));
        vis_mapGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        vis_mapGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        vis_mapGraphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        gridLayout_9->addWidget(vis_mapGraphicsView, 0, 5, 1, 1);

        tabWidget->addTab(vis_mapTab, QString());

        gridLayout_6->addWidget(tabWidget, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 935, 19));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QStringLiteral("menu_Help"));
        menu_Options = new QMenu(menuBar);
        menu_Options->setObjectName(QStringLiteral("menu_Options"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
#ifndef QT_NO_SHORTCUT
        label->setBuddy(browseLuaAgentButton);
        label_7->setBuddy(browseMapButton);
        label_2->setBuddy(luaSpinBox);
        label_31->setBuddy(scaleDoubleSpinBox);
        label_5->setBuddy(timeResSpinBox);
        label_6->setBuddy(macroSpinBox);
        label_13->setBuddy(runTimeSpinBox);
        label_4->setBuddy(vis_eventBrowsePushButton);
        label_16->setBuddy(vis_resolutionSpinBox);
        vis_toTimeLabel->setBuddy(vis_toTimeSpinBox);
        label_24->setBuddy(vis_agentPathPushButton);
        label_17->setBuddy(vis_fromTimeSpinBox);
        label_25->setBuddy(vis_zThressholdDoubleSpinBox);
        label_29->setBuddy(vis_timeResolutionDoubleSpinBox);
        label_28->setBuddy(vis_eventRegExLineEdit);
        label_26->setBuddy(vis_mapTypeComboBox);
        label_27->setBuddy(vis_activeMapSpinBox);
        label_36->setBuddy(vis_controlEventPlaySpinBox);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(tabWidget, timeResSpinBox);
        QWidget::setTabOrder(timeResSpinBox, macroSpinBox);
        QWidget::setTabOrder(macroSpinBox, runTimeSpinBox);
        QWidget::setTabOrder(runTimeSpinBox, luaSpinBox);
        QWidget::setTabOrder(luaSpinBox, browseLuaAgentButton);
        QWidget::setTabOrder(browseLuaAgentButton, agentPathLineEdit);
        QWidget::setTabOrder(agentPathLineEdit, browseMapButton);
        QWidget::setTabOrder(browseMapButton, mapPathLineEdit);
        QWidget::setTabOrder(mapPathLineEdit, pushButton);
        QWidget::setTabOrder(pushButton, delaySpinBox);
        QWidget::setTabOrder(delaySpinBox, graphicsView);
        QWidget::setTabOrder(graphicsView, zoomSlider);
        QWidget::setTabOrder(zoomSlider, vis_eventBrowsePushButton);
        QWidget::setTabOrder(vis_eventBrowsePushButton, vis_eventPathLineEdit);
        QWidget::setTabOrder(vis_eventPathLineEdit, vis_agentPathPushButton);
        QWidget::setTabOrder(vis_agentPathPushButton, vis_agentPathLineEdit);
        QWidget::setTabOrder(vis_agentPathLineEdit, vis_readInfoPushButton);
        QWidget::setTabOrder(vis_readInfoPushButton, vis_timeResolutionDoubleSpinBox);
        QWidget::setTabOrder(vis_timeResolutionDoubleSpinBox, vis_zThressholdDoubleSpinBox);
        QWidget::setTabOrder(vis_zThressholdDoubleSpinBox, vis_resolutionSpinBox);
        QWidget::setTabOrder(vis_resolutionSpinBox, vis_fromTimeSpinBox);
        QWidget::setTabOrder(vis_fromTimeSpinBox, vis_toTimeSpinBox);
        QWidget::setTabOrder(vis_toTimeSpinBox, vis_eventRegExLineEdit);
        QWidget::setTabOrder(vis_eventRegExLineEdit, vis_processEventsPushButton);
        QWidget::setTabOrder(vis_processEventsPushButton, vis_clearOutputPushButton);
        QWidget::setTabOrder(vis_clearOutputPushButton, vis_outputTextBrowser);
        QWidget::setTabOrder(vis_outputTextBrowser, vis_mapTypeComboBox);
        QWidget::setTabOrder(vis_mapTypeComboBox, vis_activeMapSpinBox);
        QWidget::setTabOrder(vis_activeMapSpinBox, vis_controlEventPlaySpinBox);
        QWidget::setTabOrder(vis_controlEventPlaySpinBox, vis_eventPlayPushButton);
        QWidget::setTabOrder(vis_eventPlayPushButton, vis_eventZoomSlider);
        QWidget::setTabOrder(vis_eventZoomSlider, vis_graphicsView);
        QWidget::setTabOrder(vis_graphicsView, vis_mapGraphicsView);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_Options->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menu_File->addAction(action_Exit);
        menu_Help->addAction(action_Info);
        menu_Options->addSeparator();
        menu_Options->addAction(actionSave_Current_Events);
        menu_Options->addAction(action_Enable_Visualisation);
        menuOptions->addAction(actionDisable_Simulation_Output);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "RANA", 0));
        action_Open->setText(QApplication::translate("MainWindow", "&Open", 0));
        action_Exit->setText(QApplication::translate("MainWindow", "&Exit", 0));
        action_Info->setText(QApplication::translate("MainWindow", "&About", 0));
        action_Enable_Visualisation->setText(QApplication::translate("MainWindow", "&Toggle Event Processing", 0));
        actionSave_Current_Events->setText(QApplication::translate("MainWindow", "&Save Current Events", 0));
        actionDisable_Simulation_Output->setText(QApplication::translate("MainWindow", "&Disable Simulation Debug Output", 0));
#ifndef QT_NO_TOOLTIP
        runButton->setToolTip(QApplication::translate("MainWindow", "Start the simulation", 0));
#endif // QT_NO_TOOLTIP
        runButton->setText(QApplication::translate("MainWindow", "Run", 0));
        label_8->setText(QApplication::translate("MainWindow", "Time:", 0));
        label_10->setText(QApplication::translate("MainWindow", "[s]", 0));
        label_status1->setText(QApplication::translate("MainWindow", "0", 0));
#ifndef QT_NO_TOOLTIP
        generateButton->setToolTip(QApplication::translate("MainWindow", "Initialize the simulation and the agents", 0));
#endif // QT_NO_TOOLTIP
        generateButton->setText(QApplication::translate("MainWindow", "Initialize", 0));
        generateButton->setShortcut(QApplication::translate("MainWindow", "I", 0));
        label_9->setText(QApplication::translate("MainWindow", "Delay:", 0));
#ifndef QT_NO_TOOLTIP
        delaySpinBox->setToolTip(QApplication::translate("MainWindow", "Delay between steps in milliseconds", 0));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("MainWindow", "<b>Agent Path:", 0));
        label_status3->setText(QApplication::translate("MainWindow", "0", 0));
        label_15->setText(QApplication::translate("MainWindow", "External Events:", 0));
        label_3->setText(QApplication::translate("MainWindow", "References:", 0));
        label_status4->setText(QApplication::translate("MainWindow", "0", 0));
#ifndef QT_NO_TOOLTIP
        browseMapButton->setToolTip(QApplication::translate("MainWindow", "Load up an existing bitmap as a map", 0));
#endif // QT_NO_TOOLTIP
        browseMapButton->setText(QApplication::translate("MainWindow", "Browse", 0));
#ifndef QT_NO_TOOLTIP
        timeResSpinBox->setToolTip(QApplication::translate("MainWindow", "The precision with which events are distributed in seconds", 0));
#endif // QT_NO_TOOLTIP
        timeResSpinBox->setSuffix(QString());
        timeResSpinBox->setPrefix(QApplication::translate("MainWindow", "1e-", 0));
#ifndef QT_NO_TOOLTIP
        browseLuaAgentButton->setToolTip(QApplication::translate("MainWindow", "Load up an agent behavior", 0));
#endif // QT_NO_TOOLTIP
        browseLuaAgentButton->setText(QApplication::translate("MainWindow", "Browse", 0));
        agentPathLineEdit->setText(QString());
        label_39->setText(QApplication::translate("MainWindow", "Width:", 0));
        label_11->setText(QApplication::translate("MainWindow", "Height:", 0));
#ifndef QT_NO_TOOLTIP
        generateEmptyMapButton->setToolTip(QApplication::translate("MainWindow", "Generate an empty (black) map with height,width.", 0));
#endif // QT_NO_TOOLTIP
        generateEmptyMapButton->setText(QApplication::translate("MainWindow", "GenerateMap", 0));
        label_7->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Load Map:</span></p></body></html>", 0));
        label_2->setText(QApplication::translate("MainWindow", "<b>Number of Agents:", 0));
#ifndef QT_NO_TOOLTIP
        label_31->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>How many meters each pixel on the map covers(this is only important if the agent uses collision detection and map manipulation).</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_31->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Generate Map:</span></p></body></html>", 0));
        label_20->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Simulation Threads:</span></p></body></html>", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Clear Output", 0));
#ifndef QT_NO_TOOLTIP
        advThreadSpinBox->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Amount of CPU threads to utilize for the next simulation.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label_40->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>How many meters each pixel covers</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_40->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Map Scale:</span></p></body></html>", 0));
#ifndef QT_NO_TOOLTIP
        label_5->setToolTip(QApplication::translate("MainWindow", "Number of microsteps pr second", 0));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Event Distribution Precision (s):</span></p></body></html>", 0));
#ifndef QT_NO_TOOLTIP
        label_6->setToolTip(QApplication::translate("MainWindow", "Number of mirosteps pr macrostep", 0));
#endif // QT_NO_TOOLTIP
        label_6->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Step Precision(s):</span></p></body></html>", 0));
#ifndef QT_NO_TOOLTIP
        scaleDoubleSpinBox->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Define the map scale. It defines how many meters each pixel of the map covers.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        macroSpinBox->setToolTip(QApplication::translate("MainWindow", "Defines the amount of 'takeStep' pr second (which is 1/stepPrecision)", 0));
#endif // QT_NO_TOOLTIP
        macroSpinBox->setPrefix(QApplication::translate("MainWindow", "1e-", 0));
        label_13->setText(QApplication::translate("MainWindow", "<b>Runtime(s):", 0));
        tabWidget->setTabText(tabWidget->indexOf(simControlTab), QApplication::translate("MainWindow", "Simulation", 0));
        label_12->setText(QApplication::translate("MainWindow", "%", 0));
        label_14->setText(QApplication::translate("MainWindow", "Zoom", 0));
        zoomLabel->setText(QApplication::translate("MainWindow", "100", 0));
#ifndef QT_NO_TOOLTIP
        zoomSlider->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Slide to zoom</p><p><br/></p><p><br/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        vis_disableAgentsCheckBox->setToolTip(QApplication::translate("MainWindow", "Disable graphic representation of agents (CTRL-A)", 0));
#endif // QT_NO_TOOLTIP
        vis_disableAgentsCheckBox->setText(QApplication::translate("MainWindow", "Disable Agents", 0));
        vis_disableAgentsCheckBox->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", 0));
#ifndef QT_NO_TOOLTIP
        vis_disableAgentIDs->setToolTip(QApplication::translate("MainWindow", "Disable the ID numbers of the agents (CTRL-I)", 0));
#endif // QT_NO_TOOLTIP
        vis_disableAgentIDs->setText(QApplication::translate("MainWindow", "Disable IDs", 0));
        vis_disableAgentIDs->setShortcut(QApplication::translate("MainWindow", "Ctrl+I", 0));
        showAngle_checkbox->setText(QApplication::translate("MainWindow", "Disable Angle", 0));
        tabWidget->setTabText(tabWidget->indexOf(simLiveView), QApplication::translate("MainWindow", "Live View", 0));
        vis_clearOutputPushButton->setText(QApplication::translate("MainWindow", "Clear Output", 0));
        label_4->setText(QApplication::translate("MainWindow", "<b>Data File:", 0));
        vis_processEventsPushButton->setText(QApplication::translate("MainWindow", "Process Events", 0));
        vis_agentPathPushButton->setText(QApplication::translate("MainWindow", "Browse", 0));
#ifndef QT_NO_TOOLTIP
        label_16->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Amount of frames pr. second</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_16->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Map Resolution:</span></p></body></html>", 0));
        vis_toTimeLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">To Time[s]:</span></p></body></html>", 0));
        label_24->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Agent Path:</span></p></body></html>", 0));
        label_17->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">From Time[s]:</span></p></body></html>", 0));
        vis_eventBrowsePushButton->setText(QApplication::translate("MainWindow", "Browse", 0));
        vis_readInfoPushButton->setText(QApplication::translate("MainWindow", "Get Event Info", 0));
#ifndef QT_NO_TOOLTIP
        vis_eventRegExLineEdit->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>This takes a RegEx string, that matched against each events 'description' string. Only events containing a matching description will be processed.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label_25->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>The fraction of the origin Z level that the recursive propagation algorithm should stop at.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_25->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Z Value Thresshold:</span></p></body></html>", 0));
#ifndef QT_NO_TOOLTIP
        label_29->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Amount of seconds each timeslot will cover.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_29->setText(QApplication::translate("MainWindow", "<b>Time Resolution:", 0));
#ifndef QT_NO_TOOLTIP
        label_28->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>This takes a RegEx string, that matched against each events 'description' string. Only events containing a matching description will be processed.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_28->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Event Description RegEx:</span></p></body></html>", 0));
        vis_stopEventProcessingPushButton->setText(QApplication::translate("MainWindow", "Stop Processing", 0));
        tabWidget->setTabText(tabWidget->indexOf(vis_controlTab), QApplication::translate("MainWindow", "Event Processing", 0));
        label_26->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Map Type:</span></p></body></html>", 0));
        vis_mapTypeComboBox->clear();
        vis_mapTypeComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Cumulative", 0)
         << QApplication::translate("MainWindow", "Frequency", 0)
         << QApplication::translate("MainWindow", "Average", 0)
         << QApplication::translate("MainWindow", "Highest", 0)
        );
        label_27->setText(QApplication::translate("MainWindow", "<b>Active Map:", 0));
        label_30->setText(QApplication::translate("MainWindow", "<b>Block Information:", 0));
        vis_zValueLabel->setText(QApplication::translate("MainWindow", "Z-Value", 0));
        label_33->setText(QApplication::translate("MainWindow", "<b>TimeResolution:", 0));
        vis_activeTimeResolutionLabel->setText(QApplication::translate("MainWindow", "Resolution", 0));
        label_35->setText(QApplication::translate("MainWindow", "<b>Current Time[s]:", 0));
        vis_activeTimeLabel->setText(QApplication::translate("MainWindow", "Time", 0));
        label_36->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Frame Delay[ms]:</span></p></body></html>", 0));
        vis_eventPlayPushButton->setText(QApplication::translate("MainWindow", "Start Playback", 0));
        vis_zoomValueLabel->setText(QApplication::translate("MainWindow", "100", 0));
        label_32->setText(QApplication::translate("MainWindow", "Zoom", 0));
        label_34->setText(QApplication::translate("MainWindow", "%", 0));
        tabWidget->setTabText(tabWidget->indexOf(vis_mapTab), QApplication::translate("MainWindow", "Event Map", 0));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0));
        menu_Help->setTitle(QApplication::translate("MainWindow", "&Help", 0));
        menu_Options->setTitle(QApplication::translate("MainWindow", "&Events", 0));
        menuOptions->setTitle(QApplication::translate("MainWindow", "&Options", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
