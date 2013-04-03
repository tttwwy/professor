#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QString>


namespace Ui {
class ChangePassword;
}

class ChangePassword : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChangePassword(QWidget *parent = 0);
    ~ChangePassword();
    QSqlDatabase root;

    bool CreateConnection();
    QString username;
private slots:
    void on_pushButton_2_clicked();
    bool on_pushButton_clicked();


private:
    Ui::ChangePassword *ui;

};

#endif // CHANGEPASSWORD_H
