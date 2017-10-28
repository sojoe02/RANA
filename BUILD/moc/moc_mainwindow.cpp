/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[83];
    char stringdata0[1671];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 16), // "map_updateSignal"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 8), // "INFOLIST"
QT_MOC_LITERAL(4, 38, 8), // "infolist"
QT_MOC_LITERAL(5, 47, 17), // "writeStringSignal"
QT_MOC_LITERAL(6, 65, 9), // "something"
QT_MOC_LITERAL(7, 75, 17), // "writeStatusSignal"
QT_MOC_LITERAL(8, 93, 14), // "internalEvents"
QT_MOC_LITERAL(9, 108, 14), // "externalEvents"
QT_MOC_LITERAL(10, 123, 18), // "writeRegularSignal"
QT_MOC_LITERAL(11, 142, 21), // "enableRunButtonSignal"
QT_MOC_LITERAL(12, 164, 21), // "addGraphicAgentSignal"
QT_MOC_LITERAL(13, 186, 2), // "id"
QT_MOC_LITERAL(14, 189, 4), // "posX"
QT_MOC_LITERAL(15, 194, 4), // "posY"
QT_MOC_LITERAL(16, 199, 4), // "rgba"
QT_MOC_LITERAL(17, 204, 5), // "color"
QT_MOC_LITERAL(18, 210, 5), // "angle"
QT_MOC_LITERAL(19, 216, 24), // "removeGraphicAgentSignal"
QT_MOC_LITERAL(20, 241, 2), // "Id"
QT_MOC_LITERAL(21, 244, 13), // "writePPSignal"
QT_MOC_LITERAL(22, 258, 10), // "wheelEvent"
QT_MOC_LITERAL(23, 269, 12), // "QWheelEvent*"
QT_MOC_LITERAL(24, 282, 5), // "event"
QT_MOC_LITERAL(25, 288, 25), // "on_generateButton_clicked"
QT_MOC_LITERAL(26, 314, 10), // "actionExit"
QT_MOC_LITERAL(27, 325, 26), // "on_browseMapButton_clicked"
QT_MOC_LITERAL(28, 352, 31), // "on_browseLuaAgentButton_clicked"
QT_MOC_LITERAL(29, 384, 20), // "on_runButton_clicked"
QT_MOC_LITERAL(30, 405, 12), // "on_updateMap"
QT_MOC_LITERAL(31, 418, 14), // "on_udateStatus"
QT_MOC_LITERAL(32, 433, 14), // "on_writeOutput"
QT_MOC_LITERAL(33, 448, 6), // "string"
QT_MOC_LITERAL(34, 455, 21), // "on_writeRegularOutput"
QT_MOC_LITERAL(35, 477, 28), // "on_delaySpinBox_valueChanged"
QT_MOC_LITERAL(36, 506, 4), // "arg1"
QT_MOC_LITERAL(37, 511, 26), // "on_zoomSlider_valueChanged"
QT_MOC_LITERAL(38, 538, 5), // "value"
QT_MOC_LITERAL(39, 544, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(40, 566, 15), // "actionPrintInfo"
QT_MOC_LITERAL(41, 582, 18), // "on_enableRunButton"
QT_MOC_LITERAL(42, 601, 7), // "enabled"
QT_MOC_LITERAL(43, 609, 18), // "on_addGraphicAgent"
QT_MOC_LITERAL(44, 628, 21), // "on_removeGraphicAgent"
QT_MOC_LITERAL(45, 650, 25), // "on_initializeTimerTimeout"
QT_MOC_LITERAL(46, 676, 18), // "on_runTimerTimeout"
QT_MOC_LITERAL(47, 695, 11), // "ppIsChecked"
QT_MOC_LITERAL(48, 707, 38), // "on_vis_processEventsPushButto..."
QT_MOC_LITERAL(49, 746, 16), // "on_writePPOutput"
QT_MOC_LITERAL(50, 763, 33), // "on_vis_readInfoPushButton_cli..."
QT_MOC_LITERAL(51, 797, 34), // "on_vis_agentPathPushButton_cl..."
QT_MOC_LITERAL(52, 832, 42), // "on_vis_mapTypeComboBox_curren..."
QT_MOC_LITERAL(53, 875, 19), // "on_zMapTimerTimeout"
QT_MOC_LITERAL(54, 895, 21), // "on_resizeTimerTimeout"
QT_MOC_LITERAL(55, 917, 11), // "eventDialog"
QT_MOC_LITERAL(56, 929, 36), // "on_vis_eventBrowsePushButton_..."
QT_MOC_LITERAL(57, 966, 36), // "on_vis_activeMapSpinBox_value..."
QT_MOC_LITERAL(58, 1003, 44), // "on_vis_stopEventProcessingPus..."
QT_MOC_LITERAL(59, 1048, 42), // "on_actionDisable_Simulation_O..."
QT_MOC_LITERAL(60, 1091, 35), // "on_vis_eventZoomSlider_valueC..."
QT_MOC_LITERAL(61, 1127, 26), // "on_tabWidget_tabBarClicked"
QT_MOC_LITERAL(62, 1154, 5), // "index"
QT_MOC_LITERAL(63, 1160, 11), // "resizeEvent"
QT_MOC_LITERAL(64, 1172, 13), // "QResizeEvent*"
QT_MOC_LITERAL(65, 1186, 27), // "on_tabWidget_currentChanged"
QT_MOC_LITERAL(66, 1214, 34), // "on_vis_eventPlayPushButton_cl..."
QT_MOC_LITERAL(67, 1249, 36), // "on_vis_clearOutputPushButton_..."
QT_MOC_LITERAL(68, 1286, 10), // "helpDialog"
QT_MOC_LITERAL(69, 1297, 40), // "on_action_Enable_Visualisatio..."
QT_MOC_LITERAL(70, 1338, 7), // "checked"
QT_MOC_LITERAL(71, 1346, 33), // "on_generateEmptyMapButton_cli..."
QT_MOC_LITERAL(72, 1380, 29), // "on_zoomSlider_actionTriggered"
QT_MOC_LITERAL(73, 1410, 6), // "action"
QT_MOC_LITERAL(74, 1417, 25), // "on_zoomSlider_sliderMoved"
QT_MOC_LITERAL(75, 1443, 8), // "position"
QT_MOC_LITERAL(76, 1452, 28), // "on_macroSpinBox_valueChanged"
QT_MOC_LITERAL(77, 1481, 30), // "on_timeResSpinBox_valueChanged"
QT_MOC_LITERAL(78, 1512, 36), // "on_vis_disableAgentsCheckBox_..."
QT_MOC_LITERAL(79, 1549, 30), // "on_vis_disableAgentIDs_toggled"
QT_MOC_LITERAL(80, 1580, 29), // "on_showAngle_checkbox_clicked"
QT_MOC_LITERAL(81, 1610, 30), // "on_vis_disableAgentIDs_clicked"
QT_MOC_LITERAL(82, 1641, 29) // "on_showAngle_checkbox_toggled"

    },
    "MainWindow\0map_updateSignal\0\0INFOLIST\0"
    "infolist\0writeStringSignal\0something\0"
    "writeStatusSignal\0internalEvents\0"
    "externalEvents\0writeRegularSignal\0"
    "enableRunButtonSignal\0addGraphicAgentSignal\0"
    "id\0posX\0posY\0rgba\0color\0angle\0"
    "removeGraphicAgentSignal\0Id\0writePPSignal\0"
    "wheelEvent\0QWheelEvent*\0event\0"
    "on_generateButton_clicked\0actionExit\0"
    "on_browseMapButton_clicked\0"
    "on_browseLuaAgentButton_clicked\0"
    "on_runButton_clicked\0on_updateMap\0"
    "on_udateStatus\0on_writeOutput\0string\0"
    "on_writeRegularOutput\0"
    "on_delaySpinBox_valueChanged\0arg1\0"
    "on_zoomSlider_valueChanged\0value\0"
    "on_pushButton_clicked\0actionPrintInfo\0"
    "on_enableRunButton\0enabled\0"
    "on_addGraphicAgent\0on_removeGraphicAgent\0"
    "on_initializeTimerTimeout\0on_runTimerTimeout\0"
    "ppIsChecked\0on_vis_processEventsPushButton_clicked\0"
    "on_writePPOutput\0on_vis_readInfoPushButton_clicked\0"
    "on_vis_agentPathPushButton_clicked\0"
    "on_vis_mapTypeComboBox_currentIndexChanged\0"
    "on_zMapTimerTimeout\0on_resizeTimerTimeout\0"
    "eventDialog\0on_vis_eventBrowsePushButton_clicked\0"
    "on_vis_activeMapSpinBox_valueChanged\0"
    "on_vis_stopEventProcessingPushButton_clicked\0"
    "on_actionDisable_Simulation_Output_toggled\0"
    "on_vis_eventZoomSlider_valueChanged\0"
    "on_tabWidget_tabBarClicked\0index\0"
    "resizeEvent\0QResizeEvent*\0"
    "on_tabWidget_currentChanged\0"
    "on_vis_eventPlayPushButton_clicked\0"
    "on_vis_clearOutputPushButton_clicked\0"
    "helpDialog\0on_action_Enable_Visualisation_triggered\0"
    "checked\0on_generateEmptyMapButton_clicked\0"
    "on_zoomSlider_actionTriggered\0action\0"
    "on_zoomSlider_sliderMoved\0position\0"
    "on_macroSpinBox_valueChanged\0"
    "on_timeResSpinBox_valueChanged\0"
    "on_vis_disableAgentsCheckBox_toggled\0"
    "on_vis_disableAgentIDs_toggled\0"
    "on_showAngle_checkbox_clicked\0"
    "on_vis_disableAgentIDs_clicked\0"
    "on_showAngle_checkbox_toggled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      59,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  309,    2, 0x06 /* Public */,
       5,    1,  312,    2, 0x06 /* Public */,
       7,    2,  315,    2, 0x06 /* Public */,
      10,    1,  320,    2, 0x06 /* Public */,
      11,    1,  323,    2, 0x06 /* Public */,
      12,    5,  326,    2, 0x06 /* Public */,
      19,    1,  337,    2, 0x06 /* Public */,
      21,    1,  340,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      22,    1,  343,    2, 0x0a /* Public */,
      25,    0,  346,    2, 0x08 /* Private */,
      26,    0,  347,    2, 0x08 /* Private */,
      27,    0,  348,    2, 0x08 /* Private */,
      28,    0,  349,    2, 0x08 /* Private */,
      29,    0,  350,    2, 0x08 /* Private */,
      30,    1,  351,    2, 0x08 /* Private */,
      31,    2,  354,    2, 0x08 /* Private */,
      32,    1,  359,    2, 0x08 /* Private */,
      34,    1,  362,    2, 0x08 /* Private */,
      35,    1,  365,    2, 0x08 /* Private */,
      37,    1,  368,    2, 0x08 /* Private */,
      39,    0,  371,    2, 0x08 /* Private */,
      40,    0,  372,    2, 0x08 /* Private */,
      41,    1,  373,    2, 0x08 /* Private */,
      43,    5,  376,    2, 0x08 /* Private */,
      44,    1,  387,    2, 0x08 /* Private */,
      45,    0,  390,    2, 0x08 /* Private */,
      46,    0,  391,    2, 0x08 /* Private */,
      47,    0,  392,    2, 0x08 /* Private */,
      48,    0,  393,    2, 0x08 /* Private */,
      49,    1,  394,    2, 0x08 /* Private */,
      50,    0,  397,    2, 0x08 /* Private */,
      51,    0,  398,    2, 0x08 /* Private */,
      52,    1,  399,    2, 0x08 /* Private */,
      53,    0,  402,    2, 0x08 /* Private */,
      54,    0,  403,    2, 0x08 /* Private */,
      55,    0,  404,    2, 0x08 /* Private */,
      56,    0,  405,    2, 0x08 /* Private */,
      57,    1,  406,    2, 0x08 /* Private */,
      58,    0,  409,    2, 0x08 /* Private */,
      59,    1,  410,    2, 0x08 /* Private */,
      60,    1,  413,    2, 0x08 /* Private */,
      61,    1,  416,    2, 0x08 /* Private */,
      63,    1,  419,    2, 0x08 /* Private */,
      65,    1,  422,    2, 0x08 /* Private */,
      66,    0,  425,    2, 0x08 /* Private */,
      67,    0,  426,    2, 0x08 /* Private */,
      68,    0,  427,    2, 0x08 /* Private */,
      69,    1,  428,    2, 0x08 /* Private */,
      71,    0,  431,    2, 0x08 /* Private */,
      72,    1,  432,    2, 0x08 /* Private */,
      74,    1,  435,    2, 0x08 /* Private */,
      76,    1,  438,    2, 0x08 /* Private */,
      77,    1,  441,    2, 0x08 /* Private */,
      78,    1,  444,    2, 0x08 /* Private */,
      79,    1,  447,    2, 0x08 /* Private */,
      80,    1,  450,    2, 0x08 /* Private */,
      81,    1,  453,    2, 0x08 /* Private */,
      81,    0,  456,    2, 0x08 /* Private */,
      82,    1,  457,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::ULongLong,    8,    9,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, 0x80000000 | 16, QMetaType::Double,   13,   14,   15,   17,   18,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 23,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::ULongLong,    8,    9,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   42,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, 0x80000000 | 16, QMetaType::Double,   13,   14,   15,   17,   18,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   36,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   36,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void, QMetaType::Int,   62,
    QMetaType::Void, 0x80000000 | 64,   24,
    QMetaType::Void, QMetaType::Int,   62,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   70,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   73,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void, QMetaType::Bool,   70,
    QMetaType::Void, QMetaType::Bool,   70,
    QMetaType::Void, QMetaType::Bool,   70,
    QMetaType::Void, QMetaType::Bool,   70,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   70,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->map_updateSignal((*reinterpret_cast< INFOLIST(*)>(_a[1]))); break;
        case 1: _t->writeStringSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->writeStatusSignal((*reinterpret_cast< unsigned long long(*)>(_a[1])),(*reinterpret_cast< unsigned long long(*)>(_a[2]))); break;
        case 3: _t->writeRegularSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->enableRunButtonSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->addGraphicAgentSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< rgba(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5]))); break;
        case 6: _t->removeGraphicAgentSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->writePPSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->wheelEvent((*reinterpret_cast< QWheelEvent*(*)>(_a[1]))); break;
        case 9: _t->on_generateButton_clicked(); break;
        case 10: _t->actionExit(); break;
        case 11: _t->on_browseMapButton_clicked(); break;
        case 12: _t->on_browseLuaAgentButton_clicked(); break;
        case 13: _t->on_runButton_clicked(); break;
        case 14: _t->on_updateMap((*reinterpret_cast< INFOLIST(*)>(_a[1]))); break;
        case 15: _t->on_udateStatus((*reinterpret_cast< unsigned long long(*)>(_a[1])),(*reinterpret_cast< unsigned long long(*)>(_a[2]))); break;
        case 16: _t->on_writeOutput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->on_writeRegularOutput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->on_delaySpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->on_zoomSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->on_pushButton_clicked(); break;
        case 21: _t->actionPrintInfo(); break;
        case 22: _t->on_enableRunButton((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: _t->on_addGraphicAgent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< rgba(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5]))); break;
        case 24: _t->on_removeGraphicAgent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->on_initializeTimerTimeout(); break;
        case 26: _t->on_runTimerTimeout(); break;
        case 27: _t->ppIsChecked(); break;
        case 28: _t->on_vis_processEventsPushButton_clicked(); break;
        case 29: _t->on_writePPOutput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 30: _t->on_vis_readInfoPushButton_clicked(); break;
        case 31: _t->on_vis_agentPathPushButton_clicked(); break;
        case 32: _t->on_vis_mapTypeComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 33: _t->on_zMapTimerTimeout(); break;
        case 34: _t->on_resizeTimerTimeout(); break;
        case 35: _t->eventDialog(); break;
        case 36: _t->on_vis_eventBrowsePushButton_clicked(); break;
        case 37: _t->on_vis_activeMapSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->on_vis_stopEventProcessingPushButton_clicked(); break;
        case 39: _t->on_actionDisable_Simulation_Output_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 40: _t->on_vis_eventZoomSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: _t->on_tabWidget_tabBarClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 43: _t->on_tabWidget_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->on_vis_eventPlayPushButton_clicked(); break;
        case 45: _t->on_vis_clearOutputPushButton_clicked(); break;
        case 46: _t->helpDialog(); break;
        case 47: _t->on_action_Enable_Visualisation_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 48: _t->on_generateEmptyMapButton_clicked(); break;
        case 49: _t->on_zoomSlider_actionTriggered((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 50: _t->on_zoomSlider_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 51: _t->on_macroSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 52: _t->on_timeResSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->on_vis_disableAgentsCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 54: _t->on_vis_disableAgentIDs_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 55: _t->on_showAngle_checkbox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 56: _t->on_vis_disableAgentIDs_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 57: _t->on_vis_disableAgentIDs_clicked(); break;
        case 58: _t->on_showAngle_checkbox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(INFOLIST );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::map_updateSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (MainWindow::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::writeStringSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (MainWindow::*_t)(unsigned long long , unsigned long long );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::writeStatusSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (MainWindow::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::writeRegularSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (MainWindow::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::enableRunButtonSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (MainWindow::*_t)(int , int , int , rgba , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::addGraphicAgentSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::removeGraphicAgentSignal)) {
                *result = 6;
            }
        }
        {
            typedef void (MainWindow::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::writePPSignal)) {
                *result = 7;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 59)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 59;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 59)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 59;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::map_updateSignal(INFOLIST _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::writeStringSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::writeStatusSignal(unsigned long long _t1, unsigned long long _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::writeRegularSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::enableRunButtonSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainWindow::addGraphicAgentSignal(int _t1, int _t2, int _t3, rgba _t4, double _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::removeGraphicAgentSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MainWindow::writePPSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
