#ifndef LOGINTO_H
#define LOGINTO_H
#include <QSqlDatabase>
#include <QDialog>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QString>
#include <QMessageBox>
#include <QDir>
#include <QtSql>
#include <QDebug>
#include <QApplication>
#include <QObject>


namespace Ui {
class Loginto;
}

class Loginto : public QDialog
{
    Q_OBJECT
    
public:
    explicit Loginto(QWidget *parent = 0);
    ~Loginto();
    QString username;
    
private slots:
    void on_okpushButton_clicked();

    void on_exitpushButton_2_clicked();

private:
    Ui::Loginto *ui;
    QSqlDatabase userroot;
    bool CreateConnection();
};

#endif // LOGINTO_H
