#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QButtonGroup>
#include <QTime>
#include <sys/time.h>
#include "cosign.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ClientDialog; }
QT_END_NAMESPACE

class ClientDialog : public QDialog
{
    Q_OBJECT

public:
    ClientDialog(QWidget *parent = nullptr);
    ~ClientDialog();

private slots:
    //连接服务器槽函数
    void on_connectButton_clicked();
    //与服务器连接成功时执行的槽函数
    void onConnected();
    //与服务器断开连接时执行的槽函数
    void onDisconnected();
    //接收通信的槽函数
    void onReadyRead();
    //网络异常执行的槽函数
    void onError();
    //生成密钥的槽函数
    void on_genkeyButton_clicked();
    //签名的槽函数
    void on_sign_Button_clicked();
    //验签的槽函数
    void on_verify_Button_clicked();
    //浏览文件槽函数
    void on_choose_file_Button_clicked();
    //设置私钥的槽函数
    void on_setPIN_Button_clicked();
    //对他人数据验签槽函数
    void on_verify_other_Button_clicked();
    //SM3槽函数
    void on_SM3_Button_clicked();
    //SM4解密槽函数
    void on_SM4e_Button_clicked();
    //SM4加密槽函数
    void on_SM4c_Button_clicked();
    //SM4key设置槽函数
    void on_gen_SM4key_Button_clicked();

private:
    //处理openssl错误
    void handleErrors(void);
    //sm4对称加密
    int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,unsigned char *iv, unsigned char *ciphertext);
    //sm4对称解密
    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *plaintext);

private:
    Ui::ClientDialog *ui;
    bool status;//状态标识：在线/离线
    QTcpSocket tcpSocket;//与服务器通信套接字
    QHostAddress serverIP;//服务器地址
    quint16 serverPort;//服务器端口
    QString ID;//ID
    QString sk_PIN;//sk_PIN
    QButtonGroup *bg = new QButtonGroup(this);
    BN_CTX *ctx = BN_CTX_new() ;//大数运算
    const EC_GROUP *group = EC_GROUP_new_by_curve_name(1121);//群
    const EC_POINT *P = EC_GROUP_get0_generator(group);//基点
    BIGNUM *p = BN_new();//有限域p
    BIGNUM *a = BN_new();//a
    BIGNUM *b = BN_new();//b
    BIGNUM *q = BN_new();//阶
    BIGNUM *dC = BN_new();//客户端私钥
	BIGNUM *kC = BN_new();
	BIGNUM *r = BN_new();
    EC_POINT *pkC = EC_POINT_new(group);//客户端公钥分片
    EC_POINT *pkS = EC_POINT_new(group);//服务端公钥分片
    EC_POINT *pk = EC_POINT_new(group);//完整公钥
	EC_POINT *RC = EC_POINT_new(group);
    EC_POINT *RS = EC_POINT_new(group);
    EC_POINT *RS_ = EC_POINT_new(group);
    struct timeval tpstart,tpend;
    float timeuse;
};
#endif // CLIENTDIALOG_H
