/****************************************************************************
** Meta object code from reading C++ file 'atomeditor.h'
**
** Created: Wed Aug 1 18:54:32 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../code/atomeditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'atomeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AtomEditor[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x08,
      39,   33,   11,   11, 0x08,
      65,   56,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AtomEditor[] = {
    "AtomEditor\0\0type\0changeType(int)\0state\0"
    "changeState(int)\0reaction\0"
    "changeReaction(QString)\0"
};

const QMetaObject AtomEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AtomEditor,
      qt_meta_data_AtomEditor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AtomEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AtomEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AtomEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AtomEditor))
        return static_cast<void*>(const_cast< AtomEditor*>(this));
    return QDialog::qt_metacast(_clname);
}

int AtomEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: changeState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: changeReaction((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
