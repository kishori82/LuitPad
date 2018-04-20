/****************************************************************************
** Meta object code from reading C++ file 'mdichild.h'
**
** Created: Sat May 11 14:29:30 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mdichild.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mdichild.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MdiChild[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      28,    9,    9,    9, 0x0a,
      54,    9,    9,    9, 0x0a,
      68,    9,    9,    9, 0x0a,
      89,    9,    9,    9, 0x0a,
     107,    9,    9,    9, 0x0a,
     142,    9,    9,    9, 0x08,
     164,    9,    9,    9, 0x0a,
     173,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MdiChild[] = {
    "MdiChild\0\0showToolTipText()\0"
    "showVowelModToolTipText()\0showToolTip()\0"
    "computeToolTipText()\0resizeImageSize()\0"
    "updateCompleterModelOnTextChange()\0"
    "documentWasModified()\0Ignore()\0"
    "getMeanings()\0"
};

void MdiChild::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MdiChild *_t = static_cast<MdiChild *>(_o);
        switch (_id) {
        case 0: _t->showToolTipText(); break;
        case 1: _t->showVowelModToolTipText(); break;
        case 2: _t->showToolTip(); break;
        case 3: _t->computeToolTipText(); break;
        case 4: _t->resizeImageSize(); break;
        case 5: _t->updateCompleterModelOnTextChange(); break;
        case 6: _t->documentWasModified(); break;
        case 7: _t->Ignore(); break;
        case 8: _t->getMeanings(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MdiChild::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MdiChild::staticMetaObject = {
    { &TextEdit::staticMetaObject, qt_meta_stringdata_MdiChild,
      qt_meta_data_MdiChild, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MdiChild::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MdiChild::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MdiChild::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MdiChild))
        return static_cast<void*>(const_cast< MdiChild*>(this));
    return TextEdit::qt_metacast(_clname);
}

int MdiChild::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
