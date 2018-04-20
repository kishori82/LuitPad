/****************************************************************************
** Meta object code from reading C++ file 'profile.h'
**
** Created: Sat May 11 14:29:31 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../profile.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'profile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Profile[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   14,    9,    8, 0x0a,
      46,    8,    9,    8, 0x08,
      62,    8,    9,    8, 0x08,
      76,    8,    9,    8, 0x08,
      98,   92,    8,    8, 0x08,
     130,  123,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Profile[] = {
    "Profile\0\0bool\0profName\0fill_keyboard(QString)\0"
    "changeProfile()\0saveProfile()\0"
    "deleteProfile()\0state\0changeModifierOrder(int)\0"
    "button\0buttonClick(QAbstractButton*)\0"
};

void Profile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Profile *_t = static_cast<Profile *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->fill_keyboard((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->changeProfile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->saveProfile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->deleteProfile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->changeModifierOrder((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->buttonClick((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Profile::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Profile::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Profile,
      qt_meta_data_Profile, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Profile::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Profile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Profile::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Profile))
        return static_cast<void*>(const_cast< Profile*>(this));
    return QWidget::qt_metacast(_clname);
}

int Profile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
