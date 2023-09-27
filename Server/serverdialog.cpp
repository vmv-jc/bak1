#include "serverdialog.h"
#include "ui_serverdialog.h"
//#include "skfapi.h"
#define	TRUE	1
#define FALSE	0
#define ERROR_THROW(r) {if((r) != SAR_OK) goto END_OF_FUN;}

ServerDialog::ServerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ServerDialog)
{
    ui->setupUi(this);
	EC_GROUP_get_curve_GFp(group,p,a,b,ctx);
    EC_GROUP_get_order(group,q,ctx);
    //当有客户端向服务器发送连接请求，发送信号：newConnection
    connect(&tcpServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    //定时器到时执行的槽函数
    connect(&timer,SIGNAL(timeout()),SLOT(onTimeout()));

    int rv = SKF_LoadLibrary((LPSTR)"gm3000.1.0", NULL);
    char   szDevName[256] = {0};
    ULONG	ulDevNameLen = 256;
    char	szAppName[256] = {0};
    ULONG	ulAppNameLen = 256;
    char	szContName[256] = {0};
    ULONG	ulContName = 256;
    char	*pUserPin = "123456";
    ULONG	ulRetryCount = 0;

    char *pData = "1234567812345678";
    int  nDatalen = strlen(pData);
    char *pContName = szContName;
    char *pdevname = szDevName;
    char *pappname = szAppName;

    //Ukey开启容器
    SKF_EnumDev(TRUE, (LPSTR)szDevName, &ulDevNameLen);
    SKF_ConnectDev((LPSTR)pdevname, &hdev);
    SKF_EnumApplication(hdev, (LPSTR)szAppName, &ulAppNameLen);
    SKF_OpenApplication(hdev, (LPSTR)pappname, &happ);
    SKF_VerifyPIN(happ, USER_TYPE, (LPSTR)pUserPin, &ulRetryCount);
    SKF_EnumContainer(happ, (LPSTR)szContName, &ulContName);
    SKF_OpenContainer(happ, (LPSTR)pContName, &hcont);
}

ServerDialog::~ServerDialog()
{
    SKF_UnloadLibrary();
	BN_CTX_free(ctx);
    BN_free(p);
    BN_free(a);
    BN_free(b);
    BN_free(q);
    delete ui;
    if(hkey)
        SKF_CloseHandle(hkey);
    if(hcont)
        SKF_CloseContainer(hcont);
    if(happ)
        SKF_CloseApplication(happ);
    if(hdev)
        SKF_DisConnectDev(hdev);
}

//创建服务器按钮槽函数
void ServerDialog::on_createButton_clicked()
{
    //创建服务器端口
    port = ui->portEdit->text().toShort();
    //设置服务器IP和端口
    if(tcpServer.listen(QHostAddress::Any,port)==true){
        qDebug() << QString::fromLocal8Bit("创建服务器成功！");
        //禁用创建服务器、端口输入
        ui->createButton->setEnabled(false);
        ui->portEdit->setEnabled(false);
        //开启定时器
        timer.start(3000);
    } else {
        qDebug() << QString::fromLocal8Bit("创建服务器失败！");
    }
}

//响应客户端连接请求的槽函数
void ServerDialog::onNewConnection()
{
    //获取和客户端通信的套接字
    QTcpSocket* tcpClient = tcpServer.nextPendingConnection();
    //保存套接字到容器
    tcpClientList.append(tcpClient);
	//保存客户端私钥到容器
	BIGNUM *dS = BN_new();
	dSList.append(dS);
	
	BIGNUM *kS = BN_new();
	kSList.append(kS);
	
	BIGNUM *t = BN_new();
	tList.append(t);
	
	//保存客户端公钥分片到容器
	EC_POINT *pkC = EC_POINT_new(group);
	pkCList.append(pkC);
	//保存服务端公钥分片到容器
    EC_POINT *pkS = EC_POINT_new(group);
	pkSList.append(pkS);
	//保存完整公钥到容器
    EC_POINT *pk = EC_POINT_new(group);
	pkList.append(pk);
	EC_POINT *RC = EC_POINT_new(group);
    RCList.append(RC);
	EC_POINT *RC_ = EC_POINT_new(group);
    RC_List.append(RC_);
    QString PIN_skC = "";
    PIN_skC_List.append(PIN_skC);
    PIN_error_List.append(0);
    QString dSfilename = "dS"+QString::number(dSList.length());
    QString pkSfilename = "pkS"+QString::number(pkSList.length());
    QString pinfilename = "pin"+QString::number(PIN_skC_List.length());
    SKF_CreateFile (happ, (LPSTR)dSfilename.toUtf8().data(), (ULONG)64, (ULONG)255,(ULONG)255);
    SKF_CreateFile (happ, (LPSTR)pkSfilename.toUtf8().data(), (ULONG)130, (ULONG)255,(ULONG)255);
    SKF_CreateFile (happ, (LPSTR)pinfilename.toUtf8().data(), (ULONG)20, (ULONG)255,(ULONG)255);

    //当客户端向服务器发送消息时，通信套接字发送信号：readyRead
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
}
//接收客户端消息的槽函数
void ServerDialog::onReadyRead()
{
    //遍历容器哪个客户端给服务器发送了消息
    for(int i=0;i<tcpClientList.size();i++){
        //bytesAvailable：获取当前套接字等待读取消息字节数
        //返回0没有消息
        //返回>0,当前套接字有消息
        if(tcpClientList.at(i)->bytesAvailable()){
            //读取消息并保存
            QByteArray buf = tcpClientList.at(i)->readAll();
            qDebug() << buf.data();
            //处理协议
            int index = buf.indexOf(':');

            if(strcmp(buf.left(index).data(),"pkC")==0){
				//store pkC
				EC_POINT_hex2point(group,buf.mid(index+1).data(),pkCList.value(i),ctx);
                
                //gen dS
                BN_rand_range(dSList.value(i),q);

                //gen pkS
                EC_POINT_mul(group,pkSList.value(i),NULL,P,dSList.at(i),ctx);//P*da

				//send pkS to client
				QString pks_string = EC_POINT_point2hex(group,pkSList.at(i),POINT_CONVERSION_UNCOMPRESSED,ctx);
				pks_string = "pkS:" + pks_string;
				tcpClientList.at(i)->write(pks_string.toUtf8());
				tcpClientList.at(i)->flush();

				//gen pk
				EC_POINT *NP = EC_POINT_new(group);
				EC_POINT_copy(NP,P);
				EC_POINT_invert(group,NP,ctx);
				EC_POINT_mul(group,pkList.value(i),NULL,pkCList.at(i),dSList.at(i),ctx);//p*db*da
				EC_POINT_add(group,pkList.value(i),pkList.at(i),NP,ctx);//p*db*da-p
                EC_POINT_free(NP);

                QString dSfilename = "dS"+QString::number(i+1);
                QString pkSfilename = "pkS"+QString::number(i+1);

                char *pData = BN_bn2hex(dSList.value(i));
                int  nDatalen = strlen(pData);
                SKF_WriteFile (happ, (LPSTR)dSfilename.toUtf8().data(), 0, (BYTE *)pData, nDatalen);
                //SKF_ReadFile(happ,(LPSTR)dSfilename.toUtf8().data(),0,nDatalen,(BYTE *)pbOutData,&pulOutLen);

                SKF_GetFileInfo (happ, (LPSTR)pkSfilename.toUtf8().data(), &pFileInfo);
                pData = EC_POINT_point2hex(group,pkSList.value(i),POINT_CONVERSION_UNCOMPRESSED,ctx);
                nDatalen = strlen(pData);
                SKF_WriteFile (happ, (LPSTR)pkSfilename.toUtf8().data(), 0, (BYTE *)pData, nDatalen);
                //SKF_ReadFile(happ,(LPSTR)pkSfilename.toUtf8().data(),0,nDatalen,(BYTE *)pbOutData1,&pulOutLen1);

            }else if(strcmp(buf.left(index).data(),"RC")==0){
				//store RC
                EC_POINT_hex2point(group,buf.mid(index+1).data(),RCList.value(i),ctx);
			}else if(strcmp(buf.left(index).data(),"RC_")==0){
				//store RC_ , verify ra_ = db*ra' and send RS and RS_
				EC_POINT_hex2point(group,buf.mid(index+1).data(),RC_List.value(i),ctx);  
				

				//verify
				EC_POINT *RC_t = EC_POINT_new(group);
				EC_POINT_mul(group,RC_t,NULL,RCList.at(i),dSList.at(i),ctx);

				if(EC_POINT_cmp(group,RC_List.at(i),RC_t,ctx)==0){
					//printf("ra_ = db*ra'\n");
					EC_POINT_free(RC_t);
				}else{
					EC_POINT_free(RC_t);
					qDebug() << "key error";
					return;
				}
				
				//send RS and RS'
				EC_POINT *RS = EC_POINT_new(group);
				EC_POINT *RS_ = EC_POINT_new(group);
				BN_rand_range(kSList.value(i),q);
				EC_POINT_mul(group,RS,NULL,pkCList.at(i),kSList.at(i),ctx);
				EC_POINT_mul(group,RS_,NULL,P,kSList.at(i),ctx);
				
				QString send = EC_POINT_point2hex(group,RS,POINT_CONVERSION_UNCOMPRESSED,ctx);
				send = "RS:" + send;
				tcpClientList.at(i)->write(send.toUtf8());
				tcpClientList.at(i)->flush();
				sleep(0.1);
				send = EC_POINT_point2hex(group,RS_,POINT_CONVERSION_UNCOMPRESSED,ctx);
				send = "RS_:" + send;
				tcpClientList.at(i)->write(send.toUtf8());
				tcpClientList.at(i)->flush();
				
				EC_POINT_free(RS);
				EC_POINT_free(RS_);
				
			}else if(strcmp(buf.left(index).data(),"s'")==0){
				BIGNUM *s_ = BN_new();
				BN_hex2bn(&s_,buf.mid(index+1).data());
				BIGNUM *dS_ = BN_new();
				BN_mod_inverse(dS_,dSList.at(i),q,ctx);
				BN_add(tList.value(i),s_,kSList.at(i));
				BN_free(s_);
				BN_nnmod(tList.value(i),tList.at(i),q,ctx);
				BN_mul(tList.value(i),dS_,tList.at(i),ctx);
				BN_free(dS_);
				BN_nnmod(tList.value(i),tList.at(i),q,ctx);
				QString send = BN_bn2hex(tList.at(i));
				send = "t:" + send;
				tcpClientList.at(i)->write(send.toUtf8());
				tcpClientList.at(i)->flush();
            }else if(strcmp(buf.left(index).data(),"setPIN")==0){//服务端设客户端私钥PIN
                PIN_skC_List.replace(i,QString(buf.mid(index+1)));
                QString pinfilename = "pin"+QString::number(i+1);
                char pbOutData[256] = {0};
                ULONG pulOutLen = 256;
                char *pData = (char*)PIN_skC_List.value(i).toStdString().c_str();
                int  nDatalen = strlen(pData);
                SKF_WriteFile (happ, (LPSTR)pinfilename.toUtf8().data(), 0, (BYTE *)pData, nDatalen);
                //SKF_ReadFile(happ,(LPSTR)pinfilename.toUtf8().data(),0,6,(BYTE *)pbOutData,&pulOutLen);

            }else if(strcmp(buf.left(index).data(),"PIN")==0){
                QString pinfilename = "pin"+QString::number(i+1);
                char pbOutData[256] = {0};
                ULONG pulOutLen = 256;
                SKF_ReadFile(happ,(LPSTR)pinfilename.toUtf8().data(),0,6,(BYTE *)pbOutData,&pulOutLen);
                if(strcmp(buf.mid(index+1),pbOutData)==0){
                    //PIN 正确
                    QString send = "PIN:ok";
                    tcpClientList.at(i)->write(send.toUtf8());
                    tcpClientList.at(i)->flush();
                }else {
                    //PIN 错误
                    PIN_error_List.replace(i,PIN_error_List.at(i)+1);
                    QString send = "PIN:"+QString::number(PIN_error_List.at(i));
                    tcpClientList.at(i)->write(send.toUtf8());
                    tcpClientList.at(i)->flush();
                    if(PIN_error_List.at(i)==5){
                        PIN_error_List.replace(i,0);
                    }
                }
            }
        }
    }
}

//定时器到时执行的槽函数
void ServerDialog::onTimeout(void)
{
    //遍历检查容器中保存的客户端通信套接字是否已经断开连接，如果是则删除
    for(int i=0;i<tcpClientList.size();i++){
        if(tcpClientList.at(i)->state()==QAbstractSocket::UnconnectedState){
            tcpClientList.removeAt(i);
			dSList.removeAt(i);
			pkCList.removeAt(i);
			pkSList.removeAt(i);
			pkList.removeAt(i);
            --i;
        }
    }
}
