/********************************************************************************
** Form generated from reading UI file 'clientdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTDIALOG_H
#define UI_CLIENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ClientDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLineEdit *serverportEdit;
    QLineEdit *usernameEdit;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *serveripEdit;
    QPushButton *connectButton;
    QPushButton *genkeyButton;
    QGridLayout *gridLayout_4;
    QLabel *label_6;
    QRadioButton *f_radioButton;
    QLabel *label_10;
    QPushButton *choose_file_Button;
    QLabel *label_5;
    QLabel *label_4;
    QLineEdit *sk_PINEdit;
    QRadioButton *s_radioButton;
    QLineEdit *string_Edit;
    QLineEdit *file_Edit;
    QLineEdit *r_Edit;
    QLineEdit *s_Edit;
    QPushButton *setPIN_Button;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *sign_Button;
    QPushButton *verify_Button;
    QGridLayout *gridLayout_2;
    QLineEdit *co_pk_Edit;
    QPushButton *verify_other_Button;
    QLabel *label_7;
    QLineEdit *other_ID_Edit;
    QLabel *label_8;
    QGridLayout *gridLayout_3;
    QLineEdit *SM3_result_Edit;
    QLabel *label_9;
    QPushButton *SM3_Button;
    QGridLayout *gridLayout_5;
    QLabel *label_11;
    QLabel *label_12;
    QPushButton *gen_SM4key_Button;
    QLineEdit *SM4_key_Edit;
    QPushButton *SM4c_Button;
    QPushButton *SM4e_Button;
    QLineEdit *SM4c_Edit;

    void setupUi(QDialog *ClientDialog)
    {
        if (ClientDialog->objectName().isEmpty())
            ClientDialog->setObjectName(QString::fromUtf8("ClientDialog"));
        ClientDialog->resize(489, 820);
        QFont font;
        font.setPointSize(16);
        ClientDialog->setFont(font);
        verticalLayout = new QVBoxLayout(ClientDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(ClientDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        serverportEdit = new QLineEdit(ClientDialog);
        serverportEdit->setObjectName(QString::fromUtf8("serverportEdit"));

        gridLayout->addWidget(serverportEdit, 1, 1, 1, 1);

        usernameEdit = new QLineEdit(ClientDialog);
        usernameEdit->setObjectName(QString::fromUtf8("usernameEdit"));

        gridLayout->addWidget(usernameEdit, 2, 1, 1, 1);

        label_2 = new QLabel(ClientDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(ClientDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        serveripEdit = new QLineEdit(ClientDialog);
        serveripEdit->setObjectName(QString::fromUtf8("serveripEdit"));

        gridLayout->addWidget(serveripEdit, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        connectButton = new QPushButton(ClientDialog);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        verticalLayout->addWidget(connectButton);

        genkeyButton = new QPushButton(ClientDialog);
        genkeyButton->setObjectName(QString::fromUtf8("genkeyButton"));

        verticalLayout->addWidget(genkeyButton);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_6 = new QLabel(ClientDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 7, 0, 1, 2);

        f_radioButton = new QRadioButton(ClientDialog);
        f_radioButton->setObjectName(QString::fromUtf8("f_radioButton"));

        gridLayout_4->addWidget(f_radioButton, 4, 0, 1, 1);

        label_10 = new QLabel(ClientDialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_4->addWidget(label_10, 1, 0, 1, 2);

        choose_file_Button = new QPushButton(ClientDialog);
        choose_file_Button->setObjectName(QString::fromUtf8("choose_file_Button"));

        gridLayout_4->addWidget(choose_file_Button, 4, 1, 1, 1);

        label_5 = new QLabel(ClientDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 5, 0, 1, 2);

        label_4 = new QLabel(ClientDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_4->addWidget(label_4, 3, 1, 1, 1);

        sk_PINEdit = new QLineEdit(ClientDialog);
        sk_PINEdit->setObjectName(QString::fromUtf8("sk_PINEdit"));

        gridLayout_4->addWidget(sk_PINEdit, 1, 2, 1, 1);

        s_radioButton = new QRadioButton(ClientDialog);
        s_radioButton->setObjectName(QString::fromUtf8("s_radioButton"));

        gridLayout_4->addWidget(s_radioButton, 3, 0, 1, 1);

        string_Edit = new QLineEdit(ClientDialog);
        string_Edit->setObjectName(QString::fromUtf8("string_Edit"));

        gridLayout_4->addWidget(string_Edit, 3, 2, 1, 2);

        file_Edit = new QLineEdit(ClientDialog);
        file_Edit->setObjectName(QString::fromUtf8("file_Edit"));

        gridLayout_4->addWidget(file_Edit, 4, 2, 1, 2);

        r_Edit = new QLineEdit(ClientDialog);
        r_Edit->setObjectName(QString::fromUtf8("r_Edit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(r_Edit->sizePolicy().hasHeightForWidth());
        r_Edit->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(r_Edit, 5, 2, 1, 2);

        s_Edit = new QLineEdit(ClientDialog);
        s_Edit->setObjectName(QString::fromUtf8("s_Edit"));

        gridLayout_4->addWidget(s_Edit, 7, 2, 1, 2);

        setPIN_Button = new QPushButton(ClientDialog);
        setPIN_Button->setObjectName(QString::fromUtf8("setPIN_Button"));

        gridLayout_4->addWidget(setPIN_Button, 1, 3, 1, 1);


        verticalLayout->addLayout(gridLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        sign_Button = new QPushButton(ClientDialog);
        sign_Button->setObjectName(QString::fromUtf8("sign_Button"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sign_Button->sizePolicy().hasHeightForWidth());
        sign_Button->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(sign_Button);

        verify_Button = new QPushButton(ClientDialog);
        verify_Button->setObjectName(QString::fromUtf8("verify_Button"));

        horizontalLayout_3->addWidget(verify_Button);


        verticalLayout->addLayout(horizontalLayout_3);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        co_pk_Edit = new QLineEdit(ClientDialog);
        co_pk_Edit->setObjectName(QString::fromUtf8("co_pk_Edit"));

        gridLayout_2->addWidget(co_pk_Edit, 0, 1, 1, 1);

        verify_other_Button = new QPushButton(ClientDialog);
        verify_other_Button->setObjectName(QString::fromUtf8("verify_other_Button"));

        gridLayout_2->addWidget(verify_other_Button, 2, 0, 1, 2);

        label_7 = new QLabel(ClientDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        other_ID_Edit = new QLineEdit(ClientDialog);
        other_ID_Edit->setObjectName(QString::fromUtf8("other_ID_Edit"));

        gridLayout_2->addWidget(other_ID_Edit, 1, 1, 1, 1);

        label_8 = new QLabel(ClientDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        SM3_result_Edit = new QLineEdit(ClientDialog);
        SM3_result_Edit->setObjectName(QString::fromUtf8("SM3_result_Edit"));

        gridLayout_3->addWidget(SM3_result_Edit, 1, 1, 1, 1);

        label_9 = new QLabel(ClientDialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_3->addWidget(label_9, 1, 0, 1, 1);

        SM3_Button = new QPushButton(ClientDialog);
        SM3_Button->setObjectName(QString::fromUtf8("SM3_Button"));

        gridLayout_3->addWidget(SM3_Button, 0, 0, 1, 2);


        verticalLayout->addLayout(gridLayout_3);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_11 = new QLabel(ClientDialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_5->addWidget(label_11, 1, 0, 1, 1);

        label_12 = new QLabel(ClientDialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_5->addWidget(label_12, 0, 0, 1, 1);

        gen_SM4key_Button = new QPushButton(ClientDialog);
        gen_SM4key_Button->setObjectName(QString::fromUtf8("gen_SM4key_Button"));

        gridLayout_5->addWidget(gen_SM4key_Button, 0, 4, 1, 1);

        SM4_key_Edit = new QLineEdit(ClientDialog);
        SM4_key_Edit->setObjectName(QString::fromUtf8("SM4_key_Edit"));

        gridLayout_5->addWidget(SM4_key_Edit, 0, 1, 1, 3);

        SM4c_Button = new QPushButton(ClientDialog);
        SM4c_Button->setObjectName(QString::fromUtf8("SM4c_Button"));

        gridLayout_5->addWidget(SM4c_Button, 3, 0, 1, 3);

        SM4e_Button = new QPushButton(ClientDialog);
        SM4e_Button->setObjectName(QString::fromUtf8("SM4e_Button"));

        gridLayout_5->addWidget(SM4e_Button, 3, 3, 1, 2);

        SM4c_Edit = new QLineEdit(ClientDialog);
        SM4c_Edit->setObjectName(QString::fromUtf8("SM4c_Edit"));

        gridLayout_5->addWidget(SM4c_Edit, 1, 1, 1, 4);


        verticalLayout->addLayout(gridLayout_5);


        retranslateUi(ClientDialog);

        QMetaObject::connectSlotsByName(ClientDialog);
    } // setupUi

    void retranslateUi(QDialog *ClientDialog)
    {
        ClientDialog->setWindowTitle(QApplication::translate("ClientDialog", "\350\201\224\345\220\210\347\255\276\345\220\215\345\256\242\346\210\267\347\253\257", nullptr));
        label_3->setText(QApplication::translate("ClientDialog", "ID\357\274\232", nullptr));
        serverportEdit->setText(QApplication::translate("ClientDialog", "8080", nullptr));
        usernameEdit->setText(QApplication::translate("ClientDialog", "1", nullptr));
        label_2->setText(QApplication::translate("ClientDialog", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243\357\274\232", nullptr));
        label->setText(QApplication::translate("ClientDialog", "\346\234\215\345\212\241\345\231\250\345\234\260\345\235\200\357\274\232", nullptr));
        serveripEdit->setText(QApplication::translate("ClientDialog", "127.0.0.1", nullptr));
        connectButton->setText(QApplication::translate("ClientDialog", "\350\277\236\346\216\245\346\234\215\345\212\241\345\231\250", nullptr));
        genkeyButton->setText(QApplication::translate("ClientDialog", "\347\224\237\346\210\220\345\205\254\347\247\201\351\222\245\345\257\271", nullptr));
        label_6->setText(QApplication::translate("ClientDialog", "s:", nullptr));
        f_radioButton->setText(QString());
        label_10->setText(QApplication::translate("ClientDialog", "\347\247\201\351\222\245PIN:", nullptr));
        choose_file_Button->setText(QApplication::translate("ClientDialog", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        label_5->setText(QApplication::translate("ClientDialog", "r:", nullptr));
        label_4->setText(QApplication::translate("ClientDialog", "\345\255\227\347\254\246\344\270\262:", nullptr));
        s_radioButton->setText(QString());
        setPIN_Button->setText(QApplication::translate("ClientDialog", "\350\256\276\347\275\256PIN", nullptr));
        sign_Button->setText(QApplication::translate("ClientDialog", "\347\255\276\345\220\215", nullptr));
        verify_Button->setText(QApplication::translate("ClientDialog", "\351\252\214\347\255\276", nullptr));
        verify_other_Button->setText(QApplication::translate("ClientDialog", "\345\257\271\344\273\226\344\272\272\346\225\260\346\215\256\351\252\214\347\255\276", nullptr));
        label_7->setText(QApplication::translate("ClientDialog", "\350\201\224\345\220\210\345\205\254\351\222\245:", nullptr));
        label_8->setText(QApplication::translate("ClientDialog", "other ID:", nullptr));
        label_9->setText(QApplication::translate("ClientDialog", "SM3\347\273\223\346\236\234\357\274\232", nullptr));
        SM3_Button->setText(QApplication::translate("ClientDialog", "SM3\346\221\230\350\246\201", nullptr));
        label_11->setText(QApplication::translate("ClientDialog", "SM4\345\257\206\346\226\207\357\274\232", nullptr));
        label_12->setText(QApplication::translate("ClientDialog", "SM4\345\257\206\351\222\245", nullptr));
        gen_SM4key_Button->setText(QApplication::translate("ClientDialog", "\347\224\237\346\210\220SM4\345\257\206\351\222\245", nullptr));
        SM4c_Button->setText(QApplication::translate("ClientDialog", "SM4\345\212\240\345\257\206", nullptr));
        SM4e_Button->setText(QApplication::translate("ClientDialog", "SM4\350\247\243\345\257\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClientDialog: public Ui_ClientDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTDIALOG_H
