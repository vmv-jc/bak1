/****************************************************************************
** Meta object code from reading C++ file 'clientdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "clientdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ClientDialog_t {
    QByteArrayData data[17];
    char stringdata0[341];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClientDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClientDialog_t qt_meta_stringdata_ClientDialog = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ClientDialog"
QT_MOC_LITERAL(1, 13, 24), // "on_connectButton_clicked"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 11), // "onConnected"
QT_MOC_LITERAL(4, 51, 14), // "onDisconnected"
QT_MOC_LITERAL(5, 66, 11), // "onReadyRead"
QT_MOC_LITERAL(6, 78, 7), // "onError"
QT_MOC_LITERAL(7, 86, 23), // "on_genkeyButton_clicked"
QT_MOC_LITERAL(8, 110, 22), // "on_sign_Button_clicked"
QT_MOC_LITERAL(9, 133, 24), // "on_verify_Button_clicked"
QT_MOC_LITERAL(10, 158, 29), // "on_choose_file_Button_clicked"
QT_MOC_LITERAL(11, 188, 24), // "on_setPIN_Button_clicked"
QT_MOC_LITERAL(12, 213, 30), // "on_verify_other_Button_clicked"
QT_MOC_LITERAL(13, 244, 21), // "on_SM3_Button_clicked"
QT_MOC_LITERAL(14, 266, 22), // "on_SM4e_Button_clicked"
QT_MOC_LITERAL(15, 289, 22), // "on_SM4c_Button_clicked"
QT_MOC_LITERAL(16, 312, 28) // "on_gen_SM4key_Button_clicked"

    },
    "ClientDialog\0on_connectButton_clicked\0"
    "\0onConnected\0onDisconnected\0onReadyRead\0"
    "onError\0on_genkeyButton_clicked\0"
    "on_sign_Button_clicked\0on_verify_Button_clicked\0"
    "on_choose_file_Button_clicked\0"
    "on_setPIN_Button_clicked\0"
    "on_verify_other_Button_clicked\0"
    "on_SM3_Button_clicked\0on_SM4e_Button_clicked\0"
    "on_SM4c_Button_clicked\0"
    "on_gen_SM4key_Button_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClientDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    0,   91,    2, 0x08 /* Private */,
       5,    0,   92,    2, 0x08 /* Private */,
       6,    0,   93,    2, 0x08 /* Private */,
       7,    0,   94,    2, 0x08 /* Private */,
       8,    0,   95,    2, 0x08 /* Private */,
       9,    0,   96,    2, 0x08 /* Private */,
      10,    0,   97,    2, 0x08 /* Private */,
      11,    0,   98,    2, 0x08 /* Private */,
      12,    0,   99,    2, 0x08 /* Private */,
      13,    0,  100,    2, 0x08 /* Private */,
      14,    0,  101,    2, 0x08 /* Private */,
      15,    0,  102,    2, 0x08 /* Private */,
      16,    0,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ClientDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ClientDialog *_t = static_cast<ClientDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_connectButton_clicked(); break;
        case 1: _t->onConnected(); break;
        case 2: _t->onDisconnected(); break;
        case 3: _t->onReadyRead(); break;
        case 4: _t->onError(); break;
        case 5: _t->on_genkeyButton_clicked(); break;
        case 6: _t->on_sign_Button_clicked(); break;
        case 7: _t->on_verify_Button_clicked(); break;
        case 8: _t->on_choose_file_Button_clicked(); break;
        case 9: _t->on_setPIN_Button_clicked(); break;
        case 10: _t->on_verify_other_Button_clicked(); break;
        case 11: _t->on_SM3_Button_clicked(); break;
        case 12: _t->on_SM4e_Button_clicked(); break;
        case 13: _t->on_SM4c_Button_clicked(); break;
        case 14: _t->on_gen_SM4key_Button_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject ClientDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ClientDialog.data,
    qt_meta_data_ClientDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ClientDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClientDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClientDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ClientDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
