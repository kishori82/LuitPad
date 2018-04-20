/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sat May 11 14:28:38 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      49,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x08,
      35,   11,   11,   11, 0x28,
      45,   11,   11,   11, 0x08,
      52,   11,   11,   11, 0x08,
      59,   11,   11,   11, 0x08,
      68,   11,   11,   11, 0x08,
      81,   11,   11,   11, 0x08,
      93,   11,   11,   11, 0x08,
     108,   11,   11,   11, 0x08,
     120,   11,   11,   11, 0x08,
     132,   11,   11,   11, 0x08,
     138,   11,   11,   11, 0x08,
     145,   11,   11,   11, 0x08,
     153,   11,   11,   11, 0x08,
     162,   11,   11,   11, 0x08,
     170,   11,   11,   11, 0x08,
     184,   11,   11,   11, 0x08,
     198,   11,   11,   11, 0x08,
     211,   11,   11,   11, 0x08,
     232,   11,   11,   11, 0x08,
     261,   12,  251,   11, 0x08,
     286,   11,  251,   11, 0x28,
     303,   11,   11,   11, 0x08,
     334,  327,   11,   11, 0x08,
     363,   11,   11,   11, 0x08,
     379,   11,   11,   11, 0x08,
     392,   11,   11,   11, 0x08,
     410,  403,   11,   11, 0x08,
     438,   11,   11,   11, 0x08,
     451,   11,   11,   11, 0x08,
     463,   11,   11,   11, 0x08,
     477,   11,   11,   11, 0x08,
     494,  492,   11,   11, 0x08,
     516,  514,   11,   11, 0x08,
     534,  403,   11,   11, 0x08,
     609,  600,  580,   11, 0x08,
     640,   11,  632,   11, 0x08,
     658,  653,   11,   11, 0x08,
     679,   11,   11,   11, 0x08,
     690,   11,   11,   11, 0x08,
     701,   11,   11,   11, 0x08,
     723,   11,   11,   11, 0x08,
     753,   11,   11,   11, 0x08,
     781,   11,   11,   11, 0x08,
     826,  817,  805,   11, 0x08,
     854,   11,   11,   11, 0x08,
     884,   11,   11,   11, 0x08,
     909,   11,   11,   11, 0x08,
     936,  930,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0type\0newFile(FILETYPE)\0"
    "newFile()\0open()\0save()\0saveAs()\0"
    "saveAsHtml()\0saveAsPDF()\0printPrvFile()\0"
    "printFile()\0selectAll()\0cut()\0copy()\0"
    "paste()\0search()\0about()\0registerKey()\0"
    "updateMenus()\0kbdDisplay()\0"
    "updateSettingsMenu()\0updateWindowMenu()\0"
    "MdiChild*\0createMdiChild(FILETYPE)\0"
    "createMdiChild()\0switchLayoutDirection()\0"
    "window\0setActiveSubWindow(QWidget*)\0"
    "underlineText()\0italicText()\0boldText()\0"
    "format\0setFormat(QTextCharFormat&)\0"
    "rightAlign()\0leftAlign()\0centerAlign()\0"
    "justifyAlign()\0f\0textFamily(QString)\0"
    "p\0textSize(QString)\0"
    "mergeFormatOnWordOrSelection(QTextCharFormat)\0"
    "QAbstractItemModel*\0fileName\0"
    "modelFromFile(QString)\0QString\0"
    "hardDiskID()\0mesg\0piratedCopy(QString)\0"
    "f2Action()\0f3Action()\0toggleAutoVowelMode()\0"
    "toggleAutoCharacterFillMode()\0"
    "toggleSingleCharacterMode()\0"
    "toggleHideToolTipMode()\0QStringList\0"
    "wordList\0pickRootWords(QStringList&)\0"
    "printInternalDictionarySlot()\0"
    "writeNewDictionarySlot()\0loadUndefinedWords()\0"
    "child\0createMessageText(MdiChild*)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->newFile((*reinterpret_cast< FILETYPE(*)>(_a[1]))); break;
        case 1: _t->newFile(); break;
        case 2: _t->open(); break;
        case 3: _t->save(); break;
        case 4: _t->saveAs(); break;
        case 5: _t->saveAsHtml(); break;
        case 6: _t->saveAsPDF(); break;
        case 7: _t->printPrvFile(); break;
        case 8: _t->printFile(); break;
        case 9: _t->selectAll(); break;
        case 10: _t->cut(); break;
        case 11: _t->copy(); break;
        case 12: _t->paste(); break;
        case 13: _t->search(); break;
        case 14: _t->about(); break;
        case 15: _t->registerKey(); break;
        case 16: _t->updateMenus(); break;
        case 17: _t->kbdDisplay(); break;
        case 18: _t->updateSettingsMenu(); break;
        case 19: _t->updateWindowMenu(); break;
        case 20: { MdiChild* _r = _t->createMdiChild((*reinterpret_cast< FILETYPE(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< MdiChild**>(_a[0]) = _r; }  break;
        case 21: { MdiChild* _r = _t->createMdiChild();
            if (_a[0]) *reinterpret_cast< MdiChild**>(_a[0]) = _r; }  break;
        case 22: _t->switchLayoutDirection(); break;
        case 23: _t->setActiveSubWindow((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 24: _t->underlineText(); break;
        case 25: _t->italicText(); break;
        case 26: _t->boldText(); break;
        case 27: _t->setFormat((*reinterpret_cast< QTextCharFormat(*)>(_a[1]))); break;
        case 28: _t->rightAlign(); break;
        case 29: _t->leftAlign(); break;
        case 30: _t->centerAlign(); break;
        case 31: _t->justifyAlign(); break;
        case 32: _t->textFamily((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 33: _t->textSize((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 34: _t->mergeFormatOnWordOrSelection((*reinterpret_cast< const QTextCharFormat(*)>(_a[1]))); break;
        case 35: { QAbstractItemModel* _r = _t->modelFromFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QAbstractItemModel**>(_a[0]) = _r; }  break;
        case 36: { QString _r = _t->hardDiskID();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 37: _t->piratedCopy((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 38: _t->f2Action(); break;
        case 39: _t->f3Action(); break;
        case 40: _t->toggleAutoVowelMode(); break;
        case 41: _t->toggleAutoCharacterFillMode(); break;
        case 42: _t->toggleSingleCharacterMode(); break;
        case 43: _t->toggleHideToolTipMode(); break;
        case 44: { QStringList _r = _t->pickRootWords((*reinterpret_cast< QStringList(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 45: _t->printInternalDictionarySlot(); break;
        case 46: _t->writeNewDictionarySlot(); break;
        case 47: _t->loadUndefinedWords(); break;
        case 48: _t->createMessageText((*reinterpret_cast< MdiChild*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
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
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
