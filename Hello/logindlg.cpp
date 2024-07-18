#include "logindlg.h"
#include "ui_logindlg.h"
#include <QMessageBox>

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
    //设置显示方式
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    //设置提示内容
    ui->userlineEdit->setPlaceholderText("请输入账号");
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::on_loginBtn_clicked()
{
    //trimmed 去除前后空格
    if(ui->userlineEdit->text().trimmed() == QString("11") &&
            ui->pwdLineEdit->text() == QString("123"))
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this,"error","username or password error",QMessageBox::Ok);

        ui->userlineEdit->clear();
        ui->pwdLineEdit->clear();
        //清除后设置焦点
        ui->userlineEdit->setFocus();
    }
}
