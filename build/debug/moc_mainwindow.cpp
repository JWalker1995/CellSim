/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Aug 1 20:37:19 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../code/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   12,   11,   11, 0x0a,
      45,   35,   11,   11, 0x0a,
      70,   65,   11,   11, 0x0a,
      89,   87,   11,   11, 0x0a,
     104,   87,   11,   11, 0x0a,
     118,   11,   11,   11, 0x0a,
     133,   11,   11,   11, 0x08,
     159,   11,   11,   11, 0x08,
     185,   11,   11,   11, 0x08,
     210,   11,   11,   11, 0x08,
     244,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0run\0playPauseSim(bool)\0"
    "speedSlow\0setFrameSpeed(bool)\0path\0"
    "openDoc(QString)\0i\0changeDoc(int)\0"
    "closeDoc(int)\0updateStatus()\0"
    "on_actionSave_triggered()\0"
    "on_actionOpen_triggered()\0"
    "on_actionNew_triggered()\0"
    "on_actionTile_Windows_triggered()\0"
    "on_actionNew_Window_triggered()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: playPauseSim((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: setFrameSpeed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: openDoc((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: changeDoc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: closeDoc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: updateStatus(); break;
        case 6: on_actionSave_triggered(); break;
        case 7: on_actionOpen_triggered(); break;
        case 8: on_actionNew_triggered(); break;
        case 9: on_actionTile_Windows_triggered(); break;
        case 10: on_actionNew_Window_triggered(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
