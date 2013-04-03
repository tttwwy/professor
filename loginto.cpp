#include "loginto.h"
#include "ui_loginto.h"


Loginto::Loginto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Loginto)
{
    ui->setupUi(this);
    CreateConnection();
    ui->passwordlineEdit_2->setEchoMode(QLineEdit::Password);

}

Loginto::~Loginto()
{
    delete ui;
}

void Loginto::on_okpushButton_clicked()
{
    userroot.open();
//    QMessageBox::information(this, tr("����!"),tr(userroot.lastError().text().toAscii()));




    QSqlTableModel *user = new QSqlTableModel(this,userroot);
    user->setTable("user");
    user->setFilter(tr("user='%1' and password = '%2'").arg(ui->userlineEdit->text(),ui->passwordlineEdit_2->text()));
    user->select();

    if (user->rowCount() > 0)
    {
        username = ui->userlineEdit->text();
        userroot.close();
        accept();
    }
    else
    {
        QMessageBox::information(this, tr("����!"),tr("�û������������"));
    }
    userroot.close();
}

void Loginto::on_exitpushButton_2_clicked()
{
    close();
}

bool Loginto::CreateConnection()
{
    QDir dir;
    userroot = QSqlDatabase::addDatabase("QODBC","user");  //testΪ���ݿ������
    userroot.setDatabaseName(tr("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=data/root.mdb"));
    userroot.setPassword("xiaose");

//QMessageBox::warning(0,"PATH",dir.absoluteFilePath("root.mdb"),QMessageBox::Yes);//�鿴·��
}
