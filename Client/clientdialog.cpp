#include "clientdialog.h"
#include "ui_clientdialog.h"
#include "cosign.h"

ClientDialog::ClientDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClientDialog)
{
    ui->setupUi(this);
    EC_GROUP_get_curve_GFp(group,p,a,b,ctx);
    EC_GROUP_get_order(group,q,ctx);
    bg->addButton(ui->s_radioButton,0);//一个值为0
    bg->addButton(ui->f_radioButton,1);//一个值为1
    ui->sign_Button->setEnabled(false);
    ui->verify_Button->setEnabled(false);
    ui->setPIN_Button->setEnabled(false);
    ui->choose_file_Button->setEnabled(false);
    ui->genkeyButton->setEnabled(false);
    ui->s_radioButton->setChecked(true);
    status=false;//离线状态
    ID = ui->usernameEdit->text();
    ui -> file_Edit->setReadOnly(true);
    connect(&tcpSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(&tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError()));
}

ClientDialog::~ClientDialog()
{
    BN_CTX_free(ctx);
    BN_free(p);
    BN_free(a);
    BN_free(b);
    BN_free(q);
	BN_free(r);
    BN_free(dC);
	BN_free(kC);
    EC_POINT_free(pkC);
    EC_POINT_free(pkS);
    EC_POINT_free(pk);
    EC_POINT_free(RS);
	EC_POINT_free(RC);
    EC_POINT_free(RS_);
    delete ui;

}

//连接服务器槽函数
void ClientDialog::on_connectButton_clicked()
{
    //当前离线，建立连接
    if(status == false){
        //获取服务器IP
        serverIP.setAddress(ui->serveripEdit->text());
        //获取服务器端口
        serverPort = ui->serverportEdit->text().toShort();
        //获取ID
        ID = ui->usernameEdit->text();
        //向联合签名服务器发送连接请求
        //成功信号：connected,失败：error
        tcpSocket.connectToHost(serverIP,serverPort);
    }
    //当前在线，断开连接
    else{
        //向联合签名服务器发送断开联合签名服务器提示信息
        QString msg = ID + QString::fromLocal8Bit( "断开联合签名服务器");
        //toUtf8:QString转换为QByteArray
        tcpSocket.write(msg.toUtf8());
        //关闭与联合签名服务器连接,发送信号：disconnected
        tcpSocket.disconnectFromHost();
    }
}
//与联合签名服务器连接成功时执行的槽函数
void ClientDialog::onConnected()
{
    status = true;//在线
    //可以签名验签
    ui->genkeyButton->setEnabled(true);
    ui->serveripEdit->setEnabled(false);//禁用连接相关
    ui->serverportEdit->setEnabled(false);
    ui->usernameEdit->setEnabled(false);
    ui->connectButton->setText(QString::fromLocal8Bit("断开联合签名服务器"));

    //向服务器发送连接提示信息
    QString msg = ID + QString::fromLocal8Bit("连接联合签名服务器");
    //toUtf8:QString转换为QByteArray
    tcpSocket.write(msg.toUtf8());
}

//与服务器断开连接时执行的槽函数
void ClientDialog::onDisconnected()
{
    status = false;//离线
    //不可以签名验签
    ui->sign_Button->setEnabled(false);
    ui->verify_Button->setEnabled(false);
    ui->setPIN_Button->setEnabled(false);
    ui->choose_file_Button->setEnabled(false);
    ui->genkeyButton->setEnabled(false);
    ui->serveripEdit->setEnabled(true);//开启连接相关
    ui->serverportEdit->setEnabled(true);
    ui->usernameEdit->setEnabled(true);
    ui->connectButton->setText(QString::fromLocal8Bit("连接联合签名服务器"));
}

//接收通信的槽函数
void ClientDialog::onReadyRead()
{
    if(tcpSocket.bytesAvailable()){
        //接收消息
        QByteArray buf = tcpSocket.readAll();

		int index = buf.indexOf(':');
		qDebug() << buf.data();
		//接受pkS后生成公钥
		if(strcmp(buf.left(index).data(),"pkS")==0){
			EC_POINT_hex2point(group,buf.mid(index+1).data(),pkS,ctx);

			//gen pk
			EC_POINT *NP = EC_POINT_new(group);
			EC_POINT_copy(NP,P);
			EC_POINT_invert(group,NP,ctx);
			EC_POINT_mul(group,pk,NULL,pkS,dC,ctx);//p*db*da
			EC_POINT_add(group,pk,pk,NP,ctx);//p*db*da-p
            qDebug() <<"pk:"<< EC_POINT_point2hex(group,pk,POINT_CONVERSION_UNCOMPRESSED,ctx)<<endl;
            gettimeofday(&tpend,NULL);
            timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;
            qDebug()<<timeuse<<"s";
            EC_POINT_free(NP);
            ui->sign_Button->setEnabled(true);
            ui->verify_Button->setEnabled(true);
            ui->setPIN_Button->setEnabled(true);
            ui->choose_file_Button->setEnabled(true);
        } else if(strcmp(buf.left(index).data(),"RS")==0){
			//store RS
			EC_POINT_hex2point(group,buf.mid(index+1).data(),RS,ctx);      
		}else if(strcmp(buf.left(index).data(),"RS_")==0){
			EC_POINT_hex2point(group,buf.mid(index+1).data(),RS_,ctx);      
			
			//verify
			EC_POINT *RS_t = EC_POINT_new(group);
			EC_POINT_mul(group,RS_t,NULL,RS_,dC,ctx);
			
			if(EC_POINT_cmp(group,RS,RS_t,ctx)==0){
				//printf("rb = da*rb'\n");
                EC_POINT_free(RS_t);
			}else{
                qDebug() << "key error";
                EC_POINT_free(RS_t);
				return;
			}
            gettimeofday(&tpstart,NULL);
            //获取待签名的字符串
            QString msg = "";
            int bgid = bg->checkedId();
            if(bgid == 0){
                msg = ui->string_Edit->text();
                if(msg == ""){
                    QMessageBox::critical(this,"ERROR", "msg inpute null");
                    return;
                }

            }else{
                QString path = ui ->file_Edit->text();
                if(path == ""){
                    QMessageBox::critical(this,"ERROR", "path inpute null");
                    return;
                }
                QFile file(path);
                file.open(QIODevice::ReadOnly);
                QString msg = file.readAll();
                file.close();
            }

			BIGNUM *s_ = BN_new();

			//compute R'
			EC_POINT *R_ = EC_POINT_new(group);
			EC_POINT_add(group,R_,RC,RS,ctx);

            //compute za
            unsigned char dgst[EVP_MAX_MD_SIZE];
            unsigned int dgstlen;
            const EVP_MD *msg_md = EVP_sm3();
            dgstlen = sizeof (dgst);

            //get ENTL
            char ENTL[EVP_MAX_MD_SIZE];
            sprintf(ENTL,"%d",strlen(ID.toUtf8().data()));
            qDebug() <<"ENTL"<< ENTL;
            EVP_MD_CTX *md_ctx = NULL;
            if(!(md_ctx = EVP_MD_CTX_new())
               || !EVP_DigestInit_ex(md_ctx,msg_md,NULL)
               || !EVP_DigestUpdate(md_ctx,ENTL,strlen(ENTL))
               || !EVP_DigestUpdate(md_ctx,ID.toUtf8().data(),strlen(ID.toUtf8().data()))
               || !EVP_DigestUpdate(md_ctx,BN_bn2hex(a),strlen(BN_bn2hex(a)))
               || !EVP_DigestUpdate(md_ctx,BN_bn2hex(b),strlen(BN_bn2hex(b)))
               || !EVP_DigestUpdate(md_ctx,EC_POINT_point2hex(group,P,POINT_CONVERSION_UNCOMPRESSED,ctx),strlen(EC_POINT_point2hex(group,P,POINT_CONVERSION_UNCOMPRESSED,ctx)))
               || !EVP_DigestUpdate(md_ctx,EC_POINT_point2hex(group,pk,POINT_CONVERSION_UNCOMPRESSED,ctx),strlen(EC_POINT_point2hex(group,pk,POINT_CONVERSION_UNCOMPRESSED,ctx)))
               || !EVP_DigestFinal(md_ctx,dgst,&dgstlen)){
            }
            char two[2];
            QString dgst_s = "";
            for(int i=0;i<32;i++){
                sprintf(two,"%02x ",dgst[i]);
                dgst_s += two;
                dgst_s = dgst_s.left(dgst_s.length() - 1);
            }
            //compute r
            if(!(md_ctx = EVP_MD_CTX_new())
               || !EVP_DigestInit_ex(md_ctx,msg_md,NULL)
               || !EVP_DigestUpdate(md_ctx,dgst_s.toUtf8().data(),strlen(dgst_s.toUtf8().data()))
               || !EVP_DigestUpdate(md_ctx,msg.toUtf8().data(),strlen(msg.toUtf8().data()))
               || !EVP_DigestFinal(md_ctx,dgst,&dgstlen)){
            }
            dgst_s = "";
            for(int i=0;i<32;i++){
                sprintf(two,"%02x ",dgst[i]);
                dgst_s += two;
                dgst_s = dgst_s.left(dgst_s.length() - 1);
            }

            BIGNUM *h = BN_new();
            BN_hex2bn(&h,dgst_s.toUtf8().data());

			//get xa
			BIGNUM *xa = BN_new();
			EC_POINT_get_affine_coordinates_GFp(group,R_,xa,NULL,ctx);
			BN_nnmod(xa,xa,q,ctx);

			BN_add(r,h,xa);
			BN_nnmod(r,r,q,ctx);
			BN_free(xa);
			BN_free(h);

			//compute s'
			BN_add(s_,kC,r);
			BN_nnmod(s_,s_,q,ctx);
			BIGNUM *dC_ = BN_new();
			BN_mod_inverse(dC_,dC,q,ctx);
			BN_mul(s_,dC_,s_,ctx);
			BN_free(dC_);
			BN_nnmod(s_,s_,q,ctx);

			//send s'
			QString send = BN_bn2hex(s_);
			send = "s':" + send;
			tcpSocket.write(send.toUtf8());
			tcpSocket.flush();
			BN_free(s_);
		}else if(strcmp(buf.left(index).data(),"t")==0){
			//get t
			BIGNUM *t = BN_new();
			BN_hex2bn(&t,buf.mid(index+1).data());

			//out sign(r,s)
			BIGNUM *s = BN_new();
			BN_sub(s,t,r);
            BN_nnmod(s,s,q,ctx);
            ui ->r_Edit->setText(BN_bn2hex(r));
            ui ->s_Edit->setText(BN_bn2hex(s));
            gettimeofday(&tpend,NULL);
            timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;
            qDebug()<<timeuse<<"s";
			BN_free(s);
			BN_free(t);
        }else if(strcmp(buf.left(index).data(),"PIN")==0){
            if(strcmp(buf.mid(index+1).data(),"ok")==0){
                //gen R and R`
                BN_rand_range(kC,q);
                EC_POINT *RC_ = EC_POINT_new(group);
                EC_POINT_mul(group,RC,NULL,P,kC,ctx);
                EC_POINT_mul(group,RC_,NULL,pkS,kC,ctx);

                //send RC and RC'
                QString send = EC_POINT_point2hex(group,RC,POINT_CONVERSION_UNCOMPRESSED,ctx);
                send = "RC:" + send;
                tcpSocket.write(send.toUtf8());
                tcpSocket.flush();
                sleep(0.1);
                send = EC_POINT_point2hex(group,RC_,POINT_CONVERSION_UNCOMPRESSED,ctx);
                send = "RC_:" + send;
                tcpSocket.write(send.toUtf8());
                tcpSocket.flush();
                EC_POINT_free(RC_);
            }else {
                QMessageBox::critical(this,"ERROR", "error pin "+buf.mid(index+1 )+"th");
                if(strcmp(buf.mid(index+1),"5")==0){
                    ui->sign_Button->setEnabled(false);
                    ui->verify_Button->setEnabled(false);
                }
            }
        }
    }
}

//网络异常执行的槽函数
void ClientDialog::onError()
{
    //errorString：获取网络异常原因
    QMessageBox::critical(this,"ERROR", tcpSocket.errorString());
}

//浏览文件槽函数
void ClientDialog::on_choose_file_Button_clicked(){
    //浏览文件
    QString path = QFileDialog::getOpenFileName(this,"open file","/","textfile(*.txt);;All file(*.*)");
    ui ->file_Edit->setText(path);
}

//生成密钥的槽函数
void ClientDialog::on_genkeyButton_clicked()
{
    gettimeofday(&tpstart,NULL);
    //gen dC
    BN_rand_range(dC,q);

    //get pkC
    EC_POINT_mul(group,pkC,NULL,P,dC,ctx);//P*da

    //send pkC to server
    QString pkc_string = EC_POINT_point2hex(group,pkC,POINT_CONVERSION_UNCOMPRESSED,ctx);
    pkc_string = "pkC:" + pkc_string;
    tcpSocket.write(pkc_string.toUtf8());
    tcpSocket.flush();   
}

//签名的槽函数
void ClientDialog::on_sign_Button_clicked(){
    //发送PIN
    QString send = ui->sk_PINEdit->text();
    if(send == ""){
        QMessageBox::critical(this,"ERROR", "PIN inpute null");
        return;
    }
    send = "PIN:" + send;
    tcpSocket.write(send.toUtf8());
    tcpSocket.flush();
}

//对验签的槽函数
void ClientDialog::on_verify_Button_clicked(){
    gettimeofday(&tpstart,NULL);
    bool success = false;
    QString s_s = ui->s_Edit->text();
    QString r_s = ui->r_Edit->text();
    if(s_s == "" ||r_s == ""){
        QMessageBox::critical(this,"ERROR", "inpute null");
        return;
    }
	//获取待验签的字符串
    QString msg = "";
    int bgid = bg->checkedId();
    if(bgid == 0){
        msg = ui->string_Edit->text();
        if(msg == ""){
            QMessageBox::critical(this,"ERROR", "msg inpute null");
            return;
        }
    }else{
        QString path = ui ->file_Edit->text();
        if(path == ""){
            QMessageBox::critical(this,"ERROR", "path inpute null");
            return;
        }
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QString msg = file.readAll();
        file.close();
    }

    //QMessageBox::information(this, "Tips", "Red chosed!", QMessageBox::Ok);
    BIGNUM *s1 = BN_new();
    BIGNUM *r1 = BN_new();
    BN_hex2bn(&s1,s_s.toUtf8().data());
    BN_hex2bn(&r1,r_s.toUtf8().data());
			
	//s、r不在椭圆曲线上失败
	if(BN_cmp(s1,q)==1||BN_cmp(r1,q)==1){
		success = false;
		QMessageBox::critical(this,"ERROR", "s or r not in the Zq");
		BN_free(r1);
		BN_free(s1);
		return;
	}
	
    BIGNUM *t1 = BN_new();
    BN_add(t1,r1,s1);
    BN_nnmod(t1,t1,q,ctx);
    //t=0失败
    if(BN_cmp(t1,BN_new())==0){
		success = false;
		QMessageBox::critical(this,"ERROR", "t = 0");
		BN_free(r1);
		BN_free(s1);
		BN_free(t1);
		return;
	}
	
    EC_POINT *r__ = EC_POINT_new(group);
    EC_POINT *r__a = EC_POINT_new(group);

    EC_POINT_mul(group,r__,NULL,P,s1,ctx);
	
    EC_POINT_mul(group,r__a,NULL,pk,t1,ctx);
    EC_POINT_add(group,r__,r__,r__a,ctx);
	EC_POINT_free(r__a);
	
    //compute za
    unsigned char dgst[EVP_MAX_MD_SIZE];
    unsigned int dgstlen;
    const EVP_MD *msg_md = EVP_sm3();
    dgstlen = sizeof (dgst);

    //get ENTL
    char ENTL[EVP_MAX_MD_SIZE];
    sprintf(ENTL,"%d",strlen(ID.toUtf8().data()));
    qDebug() <<"ENTL"<< ENTL;
    EVP_MD_CTX *md_ctx = NULL;
    if(!(md_ctx = EVP_MD_CTX_new())
       || !EVP_DigestInit_ex(md_ctx,msg_md,NULL)
       || !EVP_DigestUpdate(md_ctx,ENTL,strlen(ENTL))
       || !EVP_DigestUpdate(md_ctx,ID.toUtf8().data(),strlen(ID.toUtf8().data()))
       || !EVP_DigestUpdate(md_ctx,BN_bn2hex(a),strlen(BN_bn2hex(a)))
       || !EVP_DigestUpdate(md_ctx,BN_bn2hex(b),strlen(BN_bn2hex(b)))
       || !EVP_DigestUpdate(md_ctx,EC_POINT_point2hex(group,P,POINT_CONVERSION_UNCOMPRESSED,ctx),strlen(EC_POINT_point2hex(group,P,POINT_CONVERSION_UNCOMPRESSED,ctx)))
       || !EVP_DigestUpdate(md_ctx,EC_POINT_point2hex(group,pk,POINT_CONVERSION_UNCOMPRESSED,ctx),strlen(EC_POINT_point2hex(group,pk,POINT_CONVERSION_UNCOMPRESSED,ctx)))
       || !EVP_DigestFinal(md_ctx,dgst,&dgstlen)){
    }
    char two[2];
    QString dgst_s = "";
    for(int i=0;i<32;i++){
        sprintf(two,"%02x ",dgst[i]);
        dgst_s += two;
        dgst_s = dgst_s.left(dgst_s.length() - 1);
    }
    qDebug()<<dgst_s;
    //compute r
    if(!(md_ctx = EVP_MD_CTX_new())
       || !EVP_DigestInit_ex(md_ctx,msg_md,NULL)
       || !EVP_DigestUpdate(md_ctx,dgst_s.toUtf8().data(),strlen(dgst_s.toUtf8().data()))
       || !EVP_DigestUpdate(md_ctx,msg.toUtf8().data(),strlen(msg.toUtf8().data()))
       || !EVP_DigestFinal(md_ctx,dgst,&dgstlen)){
    }
    dgst_s = "";
    for(int i=0;i<32;i++){
        sprintf(two,"%02x ",dgst[i]);
        dgst_s += two;
        dgst_s = dgst_s.left(dgst_s.length() - 1);
    }
    qDebug()<<dgst_s;
    qDebug()<<"dgst_s"<<dgst_s;
	BIGNUM *e_ = BN_new();
    BN_hex2bn(&e_,dgst_s.toUtf8().data());

	BIGNUM *r__x = BN_new();
    EC_POINT_get_affine_coordinates_GFp(group,r__,r__x,NULL,ctx);
    BN_nnmod(r__x,r__x,q,ctx);
	EC_POINT_free(r__);
	
	
	BIGNUM *r_ = BN_new();
    BN_add(r_,e_,r__x);
    BN_nnmod(r_,r_,q,ctx);
	
	//r=r'成功
    if(BN_cmp(r_,r1)==0){
        success = true;
        gettimeofday(&tpend,NULL);
        timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;
        qDebug()<<timeuse<<"s";
        QMessageBox::information(this, "Tips", "verify succuss!", QMessageBox::Ok);
    }else{
        success = false;
        gettimeofday(&tpend,NULL);
        timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;
        qDebug()<<timeuse<<"s";

        QMessageBox::critical(this,"ERROR", "verify fail!");
    }
    qDebug() << success;
	BN_free(r__x);
	BN_free(e_);
	BN_free(r_);
	BN_free(r1);
	BN_free(s1);
	BN_free(t1);
}

//设置私钥的槽函数
void ClientDialog::on_setPIN_Button_clicked(){
    QString PIN = ui->sk_PINEdit->text();
    if(PIN == ""){
        QMessageBox::critical(this,"ERROR", "PIN inpute null");
        return;
    }
    QString send = "setPIN:" + PIN;
    tcpSocket.write(send.toUtf8());
    tcpSocket.flush();
}

//对他人数据验签槽函数
void ClientDialog::on_verify_other_Button_clicked(){
    bool success = false;
    QString s_s = ui->s_Edit->text();
    QString r_s = ui->r_Edit->text();
    QString other_id = ui->other_ID_Edit->text();
    QString co_pk = ui->co_pk_Edit->text();
    if(s_s == "" ||r_s == ""||other_id == "" ||co_pk == ""){
        QMessageBox::critical(this,"ERROR", "inpute null");
        return;
    }

    EC_POINT *co_pk_P = EC_POINT_new(group);//客户端公钥分片
    EC_POINT_hex2point(group,co_pk.toUtf8().data(),co_pk_P,ctx);
    //获取待验签的字符串
    QString msg = "";
    int bgid = bg->checkedId();
    if(bgid == 0){
        msg = ui->string_Edit->text();
        if(msg == ""){
            QMessageBox::critical(this,"ERROR", "msg inpute null");
            return;
        }
    }else{
        QString path = ui ->file_Edit->text();
        if(path == ""){
            QMessageBox::critical(this,"ERROR", "path inpute null");
            return;
        }
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QString msg = file.readAll();
        file.close();
    }

    //QMessageBox::information(this, "Tips", "Red chosed!", QMessageBox::Ok);
    BIGNUM *s1 = BN_new();
    BIGNUM *r1 = BN_new();
    BN_hex2bn(&s1,s_s.toUtf8().data());
    BN_hex2bn(&r1,r_s.toUtf8().data());

    //s、r不在椭圆曲线上失败
    if(BN_cmp(s1,q)==1||BN_cmp(r1,q)==1){
        success = false;
        QMessageBox::critical(this,"ERROR", "s or r not in the Zq");
        BN_free(r1);
        BN_free(s1);
        return;
    }

    BIGNUM *t1 = BN_new();
    BN_add(t1,r1,s1);
    BN_nnmod(t1,t1,q,ctx);
    //t=0失败
    if(BN_cmp(t1,BN_new())==0){
        success = false;
        QMessageBox::critical(this,"ERROR", "t = 0");
        BN_free(r1);
        BN_free(s1);
        BN_free(t1);
        return;
    }



    EC_POINT *r__ = EC_POINT_new(group);
    EC_POINT *r__a = EC_POINT_new(group);

    EC_POINT_mul(group,r__,NULL,P,s1,ctx);

    EC_POINT_mul(group,r__a,NULL,co_pk_P,t1,ctx);
    EC_POINT_add(group,r__,r__,r__a,ctx);
    EC_POINT_free(r__a);

    //compute za
    unsigned char dgst[EVP_MAX_MD_SIZE];
    unsigned int dgstlen;
    const EVP_MD *msg_md = EVP_sm3();
    dgstlen = sizeof (dgst);

    //get ENTL
    char ENTL[EVP_MAX_MD_SIZE];
    sprintf(ENTL,"%d",strlen(other_id.toUtf8().data()));
    qDebug() <<"ENTL"<< ENTL;
    EVP_MD_CTX *md_ctx = NULL;
    if(!(md_ctx = EVP_MD_CTX_new())
       || !EVP_DigestInit_ex(md_ctx,msg_md,NULL)
       || !EVP_DigestUpdate(md_ctx,ENTL,strlen(ENTL))
       || !EVP_DigestUpdate(md_ctx,other_id.toUtf8().data(),strlen(other_id.toUtf8().data()))
       || !EVP_DigestUpdate(md_ctx,BN_bn2hex(a),strlen(BN_bn2hex(a)))
       || !EVP_DigestUpdate(md_ctx,BN_bn2hex(b),strlen(BN_bn2hex(b)))
       || !EVP_DigestUpdate(md_ctx,EC_POINT_point2hex(group,P,POINT_CONVERSION_UNCOMPRESSED,ctx),strlen(EC_POINT_point2hex(group,P,POINT_CONVERSION_UNCOMPRESSED,ctx)))
       || !EVP_DigestUpdate(md_ctx,co_pk.toUtf8().data(),strlen(co_pk.toUtf8().data()))
       || !EVP_DigestFinal(md_ctx,dgst,&dgstlen)){
    }
    char two[2];
    QString dgst_s = "";
    for(int i=0;i<32;i++){
        sprintf(two,"%02x ",dgst[i]);
        dgst_s += two;
        dgst_s = dgst_s.left(dgst_s.length() - 1);
    }
    qDebug()<<dgst_s;
    //compute r
    if(!(md_ctx = EVP_MD_CTX_new())
       || !EVP_DigestInit_ex(md_ctx,msg_md,NULL)
       || !EVP_DigestUpdate(md_ctx,dgst_s.toUtf8().data(),strlen(dgst_s.toUtf8().data()))
       || !EVP_DigestUpdate(md_ctx,msg.toUtf8().data(),strlen(msg.toUtf8().data()))
       || !EVP_DigestFinal(md_ctx,dgst,&dgstlen)){
    }
    dgst_s = "";
    for(int i=0;i<32;i++){
        sprintf(two,"%02x ",dgst[i]);
        dgst_s += two;
        dgst_s = dgst_s.left(dgst_s.length() - 1);
    }
    qDebug()<<dgst_s;
    BIGNUM *e_ = BN_new();
    BN_hex2bn(&e_,dgst_s.toUtf8().data());

    BIGNUM *r__x = BN_new();
    EC_POINT_get_affine_coordinates_GFp(group,r__,r__x,NULL,ctx);
    BN_nnmod(r__x,r__x,q,ctx);
    EC_POINT_free(r__);

    BIGNUM *r_ = BN_new();
    BN_add(r_,e_,r__x);
    BN_nnmod(r_,r_,q,ctx);
    //r=r'成功
    if(BN_cmp(r_,r1)==0){
        success = true;
        QMessageBox::information(this, "Tips", "verify succuss!", QMessageBox::Ok);
    }else{
        success = false;
        QMessageBox::critical(this,"ERROR", "verify fail!");
    }
    qDebug() << success;
    BN_free(r__x);
    BN_free(e_);
    BN_free(r_);
    BN_free(r1);
    BN_free(s1);
    BN_free(t1);
}

//SM3槽函数
void ClientDialog::on_SM3_Button_clicked(){
    //获取待摘要的字符串
    QString msg = "";
    int bgid = bg->checkedId();
    if(bgid == 0){
        msg = ui->string_Edit->text();
        if(msg == ""){
            QMessageBox::critical(this,"ERROR", "msg inpute null");
            return;
        }
    }else{
        QString path = ui ->file_Edit->text();
        if(path == ""){
            QMessageBox::critical(this,"ERROR", "path inpute null");
            return;
        }
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QString msg = file.readAll();
        file.close();
    }


    unsigned char dgst[64];
    sm3((unsigned char*)msg.toUtf8().data(),strlen(msg.toUtf8().data()),dgst);
    char two[2];
    QString dgst_s = "";
    for(int i=0;i<32;i++){
        sprintf(two,"%02x ",dgst[i]);
        dgst_s += two;
        dgst_s = dgst_s.left(dgst_s.length() - 1);
    }
    ui->SM3_result_Edit->setText(dgst_s);

}
//SM4解密槽函数
void ClientDialog::on_SM4e_Button_clicked()
{
        QString user_key_s = ui->SM4_key_Edit->text();
        if(user_key_s == ""){
            QMessageBox::critical(this,"ERROR", "sm4 key null");
            return;
        }
        //获取待加密的字符串
        QString cmsg = "";
        cmsg = ui->SM4c_Edit->text();
        if(cmsg == ""){
            QMessageBox::critical(this,"ERROR", "cmsg inpute null");
            return;
        }

        sms4_key_t key;
        unsigned char *user_key;
        user_key = (unsigned char *)qstrdup(user_key_s.toUtf8().constData());
        //user_key = (unsigned char *)user_key_s.toLatin1().data();

        unsigned char *in = (unsigned char *)qstrdup(cmsg.toUtf8().constData());
        //unsigned char *in = (unsigned char *)cmsg.toLatin1().data();

        unsigned char out[10000];
        sms4_set_encrypt_key(&key, user_key);
        sms4_decrypt(in,out,&key);

        qDebug()<<"user_key"<<QString(QLatin1String((char*)user_key));
        qDebug()<<"c  text"<<QString(QLatin1String((char*)in));
        qDebug()<<"ec text"<<QString(QLatin1String((char*)out));
        ui->SM4c_Edit->setText(QString(QLatin1String((char*)out)));
}

//SM4加密槽函数
void ClientDialog::on_SM4c_Button_clicked()
{
    QString user_key_s = ui->SM4_key_Edit->text();
    if(user_key_s == ""){
        QMessageBox::critical(this,"ERROR", "sm4 key null");
        return;
    }
    //获取待加密的字符串
    QString msg = "";
    int bgid = bg->checkedId();
    if(bgid == 0){
        msg = ui->string_Edit->text();
        if(msg == ""){
            QMessageBox::critical(this,"ERROR", "msg inpute null");
            return;
        }
    }else{
        QString path = ui ->file_Edit->text();
        if(path == ""){
            QMessageBox::critical(this,"ERROR", "path inpute null");
            return;
        }
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QString msg = file.readAll();
        file.close();
    }

    sms4_key_t key;
    unsigned char user_key[16]={0};
    int i;
    for(i=0;i<user_key_s.length();i++){
        user_key[i] = (unsigned char)user_key_s.at(i).toLatin1();
    }
    unsigned char *in = (unsigned char *)msg.toStdString().data();
    unsigned char *out =(unsigned char*)malloc(sizeof(in));
    sms4_set_encrypt_key(&key, user_key);
    sms4_ecb_encrypt(in,out,&key,1);
    qDebug()<<"in"<<QString::asprintf("%s", in)<<strlen((char*)in);
    qDebug()<<"out"<<QString::asprintf("%s", out)<<strlen((char*)out);
    sms4_ecb_encrypt(out,in,&key,0);
    qDebug()<<"in"<<QString::asprintf("%s", in)<<strlen((char*)in);
    qDebug()<<"out"<<QString::asprintf("%s", out)<<strlen((char*)out);
    QString sm4_c_text = "";
    for(i=0;i<msg.length();i++){
        sm4_c_text += out[i];
    }
    qDebug()<<"c  text"<<sm4_c_text<<sm4_c_text.length();

    unsigned char enout[10000];
    for(i=0;i<msg.length();i++){
        enout[i] = (unsigned char)user_key_s.at(i).toLatin1();
    }
    unsigned char deout[10000];
    sms4_set_encrypt_key(&key, user_key);
    sms4_decrypt(enout,deout,&key);
    QString sm4_p_text = "";
    for(i=0;i<msg.length();i++){
        sm4_p_text += deout[i];
    }
    qDebug()<<"p  text"<<sm4_p_text<<sm4_p_text.length();
    ui->SM4c_Edit->setText(sm4_c_text);

}
//SM4key设置槽函数
void ClientDialog::on_gen_SM4key_Button_clicked()
{
    unsigned char sm4_key[16] = {0};
    RAND_bytes(sm4_key, sizeof(sm4_key));
    QString sm4_key_s = "";
    for(int i=0;i<16;i++){
        sm4_key_s += sm4_key[i];
    }
    qDebug()<<"sm4_key"<<sm4_key_s<<sm4_key_s.length();
    ui->SM4_key_Edit->setText(sm4_key_s);
}

//处理openssl错误
void ClientDialog::handleErrors(void){
    ERR_print_errors_fp(stderr);
    abort();
}

//sm4对称加密
int ClientDialog::encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,unsigned char *iv, unsigned char *ciphertext){
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_sms4_cbc(), NULL, key, iv))
    handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

//sm4对称解密
int ClientDialog::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *plaintext){
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_DecryptInit_ex(ctx, EVP_sms4_cbc(), NULL, key, iv))
    handleErrors();

    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
    plaintext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}
