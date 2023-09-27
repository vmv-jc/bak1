#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cosign.h"
#include <openssl/gmskf.h>
#include <openssl/skf.h>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerDialog; }
QT_END_NAMESPACE

class ServerDialog : public QDialog
{
    Q_OBJECT

public:
    ServerDialog(QWidget *parent = nullptr);
    ~ServerDialog();

private slots:
    void on_createButton_clicked();
    //响应客户端连接请求的槽函数
    void onNewConnection();
    //接收客户端消息的槽函数
    void onReadyRead();
    //定时器到时执行的槽函数
    void onTimeout(void);
private:
    Ui::ServerDialog *ui;
    QTcpServer tcpServer;
    quint16 port;//服务器端口
    QList<QTcpSocket*> tcpClientList;//容器：保存所有和客户端通信的套接字
    BN_CTX *ctx = BN_CTX_new() ;//大数运算
    const EC_GROUP *group = EC_GROUP_new_by_curve_name(1121);//群
    const EC_POINT *P = EC_GROUP_get0_generator(group);//基点
    BIGNUM *p = BN_new();//有限域p
    BIGNUM *a = BN_new();//a
    BIGNUM *b = BN_new();//b
    BIGNUM *q = BN_new();//阶
    QList<BIGNUM *>  dSList;//服务端私钥
	QList<BIGNUM *>  kSList;
	QList<BIGNUM *>  tList;
    QList<EC_POINT *> RCList;
    QList<EC_POINT *> RC_List;
    QList<QString> PIN_skC_List;//客户端私钥PIN
    QList<int> PIN_error_List;//客户端私钥PIN
    QList<EC_POINT *> pkCList;//客户端公钥分片
    QList<EC_POINT *> pkSList;//服务端公钥分片
    QList<EC_POINT *> pkList;//完整公钥
    QTimer timer;//定时器
    HANDLE hdev = NULL;
    HANDLE happ = NULL;
    HANDLE hkey = NULL;
    HANDLE hcont = NULL;
    FILEATTRIBUTE pFileInfo;
};
#endif // SERVERDIALOG_H
