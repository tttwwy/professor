#include "changepassword.h"
#include "ui_changepassword.h"
#include "loginto.h"
ChangePassword::ChangePassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
    ui->old->setEchoMode((QLineEdit::Password));
    ui->new_2->setEchoMode((QLineEdit::Password));
    ui->new_3->setEchoMode((QLineEdit::Password));

    CreateConnection();
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

void ChangePassword::on_pushButton_2_clicked()
{
    close();
}

bool ChangePassword::on_pushButton_clicked()
{

    if (ui->new_2->text()!= ui->new_3->text())
    {
        QMessageBox::information(this, tr("����!"),tr("������������벻һ�£����������룡"));
        ui->new_2->clear();
        ui->new_3->clear();
        return false;
    }
    root.open();

    QSqlTableModel *user = new QSqlTableModel(this,root);
    user->setTable("user");
    user->setFilter(tr("user='%1' and password = '%2'").arg(username,ui->old->text()));
    user->select();

    if (user->rowCount() <= 0)
    {
        QMessageBox::information(this, tr("����!"),tr("��������������������룡"));
        root.close();

        return false;
    }
    else
    {
        user->setData(user->index(0,1),ui->new_2->text());
        user->submitAll();
        root.close();
        QMessageBox::information(this, tr(" �޸ĳɹ�!"),tr("�����޸ĳɹ�����"));
        close();
        return true;

    }
    root.close();
}

bool ChangePassword::CreateConnection()
{
    root = QSqlDatabase::addDatabase("QODBC","password");  //testΪ���ݿ������
    root.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=data/root.mdb");
    root.setPassword("xiaose");


}
