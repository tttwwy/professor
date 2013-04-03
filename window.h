#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMainWindow>
#include <QObject>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QtCore>
#include <QMessageBox>
#include "changepassword.h"
#include <QSqlField>
#include <QPrinter>
#include "delegate.h"
#include <QPrintPreviewDialog>
#include <QPrintPreviewWidget>
#include <QWebView>
#include <QMap>
#include <QString>
#include <QTextDocument>
#include <QDate>
#include <about.h>
#include "mylistview.h"


namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Window(QWidget *parent = 0);
    ~Window();
    void Init();
    void Search();
    void AddItem();
    void Continue();
    void Save();
    void Print();
    bool CreateConnection();


    QSqlTableModel *expert1;
    QSqlTableModel *company;
    QSqlTableModel *expert;
    QSqlTableModel *backup;
    QSqlTableModel *selects;
    QSqlTableModel *result;



    QSqlDatabase db,root;
    ChangePassword chgpassword;
    IsDelegate isdelegate;
    IsDelegate1 isdelegate1;
    ReadOnlyDelegate readdelegate;
    ManDelegate mandelegate;
    QWebView webView;
    QTextDocument doc;
    About about;
private slots:



    void printPreview(QPrinter *printer);

    void on_changepassword_triggered();



    void on_exit_triggered();

    void on_printers_triggered();

    void on_delete_button_clicked();

    void on_add_button_clicked();


    void on_begin_button_clicked();

    void on_continue_button_clicked();

    void on_save_button_2_clicked();

    void on_print_button_clicked();

    void on_save_button_clicked();

    void on_pushButton_clicked();

    void on_action_triggered();

    void findCompany();

    void on_findCompany_textChanged(const QString &arg1);

private:
    Ui::Window *ui;


};


