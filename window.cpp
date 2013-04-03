#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{

    ui->setupUi(this);
    CreateConnection();

    ui->label->setText(tr("��ӭʹ�ú�����ʡ��ҽ��蹤����ѯ�������ι�˾-����ר�ҿ�"));
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setTime(QTime::currentTime());
    company = new QSqlTableModel(this,root);
    company->setTable("company");
    company->setEditStrategy(QSqlTableModel::OnFieldChange);

    expert1 = new QSqlTableModel(this,db);
    expert1->setTable("expert");
    expert1->setEditStrategy(QSqlTableModel::OnManualSubmit);
    expert = new QSqlTableModel(this,db);
    expert->setTable("expert");
    backup = new QSqlTableModel(this,root);
    backup->setTable("backup");

    selects = new QSqlTableModel(this,root);
    selects->setTable("selects");
    result = new QSqlTableModel(this,root);
    result->setTable("result");

    selects->setEditStrategy(QSqlTableModel::OnFieldChange);
    result->setEditStrategy(QSqlTableModel::OnFieldChange);

    //    expert1->setEditStrategy(QSqlTableModel::OnFieldChange);

    ui->treeView->setSortingEnabled(true);
    ui->treeView_2->setSortingEnabled(true);
    ui->treeView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setModel(selects);

    ui->treeView_2->setModel(result);

    ui->tableView->setModel(expert1);
    ui->treeView_3->setModel(company);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView_3->setSortingEnabled(true);
//    ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->treeView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->treeView_3->setEditTriggers(QAbstractItemView::AllEditTriggers);

    expert1->select();

    int r = selects->rowCount();
    for (int i = 0;i < r;i++)
        selects->removeRow(0);

    r = backup->rowCount();
    for (int i = 0;i < r;i++)
        backup->removeRow(0);
    backup->submitAll();


    r =result->rowCount();
    for (int i = 0;i < r;i++)
        result->removeRow(0);

    r =company->rowCount();
    for (int i = 0;i < r;i++)
        company->removeRow(0);
    Init();

}

Window::~Window()
{
    delete ui;
}

void Window::Init()
{
    AddItem();
    ui->continue_button->setEnabled(false);
    ui->save_button_2->setEnabled(false);
    ui->print_button->setEnabled(false);
    ui->treeView->setItemDelegateForColumn(2,&isdelegate);
    ui->treeView->setItemDelegateForColumn(0,&readdelegate);
    ui->treeView->setItemDelegateForColumn(1,&readdelegate);
    ui->treeView->setItemDelegateForColumn(4,&readdelegate);
    ui->treeView->setItemDelegateForColumn(5,&readdelegate);
    ui->treeView->setItemDelegateForColumn(6,&readdelegate);
    ui->treeView->setItemDelegateForColumn(8,&readdelegate);
    ui->treeView->setItemDelegateForColumn(9,&readdelegate);
    ui->tableView->setItemDelegateForColumn(7,&isdelegate1);

    ui->treeView_3->setItemDelegateForColumn(0,&readdelegate);
    ui->treeView_3->setItemDelegateForColumn(1,&isdelegate);
    ui->tableView->hideColumn(7);

}
void Window::Search()
{

    ui->begin_button->setText(tr("���³�ȡ"));
    int n = ui->spinBox->value();
    //    QSqlTableModel *expert = new QSqlTableModel(this,db);
    //    expert->setTable("experts");
    //    QSqlTableModel *backup = new QSqlTableModel(this,root);
    //    backup->setTable("backup");
    //    QSqlTableModel *selects = new QSqlTableModel(this,root);
    //    selects->setTable("selects");
    //    QSqlTableModel *result = new QSqlTableModel(this,root);
    //    result->setTable("result");
    //    selects->setEditStrategy(QSqlTableModel::OnFieldChange);
    expert->select(); //ѡȡ�������������

    backup->select(); //ѡȡ�������������
    selects->select(); //ѡȡ�������������
    result->select();


    int r = selects->rowCount();
    for (int i = 0;i < r;i++)
        selects->removeRow(0);

    r = backup->rowCount();
    for (int i = 0;i < r;i++)
        backup->removeRow(0);
    backup->submitAll();


    r =result->rowCount();
    for (int i = 0;i < r;i++)
        result->removeRow(0);

    QSqlRecord temp1;

    //    expert->setFilter(tr("���='1'"));


    expert->select();
    for (int i = 0;i < expert->rowCount();i++)
    {
        temp1 = expert->record(i);
        if (temp1.value(7).toString()=="1")
        {
            temp1.remove(7);
            backup->insertRecord(0,temp1);
        }
    }

    while(expert->canFetchMore())
    {
        expert->fetchMore();
        for (int i = 0;i < expert->rowCount();i++)
        {
            temp1 = expert->record(i);
            if (temp1.value(7).toString()=="1")
            {
                temp1.remove(7);
                backup->insertRecord(0,temp1);
            }
        }
    }
    ui->label->setText(tr("��������"));

    QString select = "";
    if (ui->comboBox_2->currentIndex()!= 0)
    {
        select += tr("ְ�� ='").append(ui->comboBox_2->currentText()).append("'and ");
    }

    QString filter = company->filter();
    company->setFilter(tr("�Ƿ���� = '��'"));
    company->select();
    for (int i = 0;i < company->rowCount();i++)
    {
        qDebug() << company->record(i);
        if (company->record(i).value(0).toString().size() > 0)
            select += tr("������λ <>'")+ company->record(i).value(0).toString()+tr("'and ");
    }
    company->setFilter(filter);
    company->select();

    select += tr("רҵ= '%1' order by rnd(���)").arg(ui->comboBox_3->currentText());
    backup->setFilter(select);
    qDebug() << select;

    backup->select();
    if (backup->rowCount() < n)
    {
        QMessageBox::information(this, tr("����ר�ҿ�"),tr("��ѡרҵ��ר�������㣡"));
        //        ui->continue_button->setEnabled(true);
        ui->save_button_2->setEnabled(false);
        ui->print_button->setEnabled(false);

    }
    else
    {
        QSqlField field(tr("�Ƿ��ϯ"), QVariant::String);
        field.setValue(tr("��"));



        for (int i = 0;i < n;i++)
        {
            QSqlRecord temp = backup->record(i);
            temp.remove(8);
            temp.insert(2,field);
            selects->insertRecord(0,temp);
        }

        for (int i = 0;i < n;i++)
        {
            backup->removeRow(0);
        }
        backup->submitAll();
        selects->select(); //ѡȡ�������������
        ui->continue_button->setEnabled(true);
        ui->save_button_2->setEnabled(true);
        ui->print_button->setEnabled(false);


    }




}

void Window::AddItem()
{
    QSqlQuery query(db);
    QStringList item;
    query.exec(QObject::tr("select distinct ְ�� from expert"));
    item << tr("����");
    while(query.next())
    {

        if (query.value(0).toString().size() > 0)
            item << query.value(0).toString();
    }
    ui->comboBox_2->clear();
    ui->comboBox_2->addItems(item);

    item.clear();
    query.exec(QObject::tr("select distinct ������λ from expert"));
    QSqlRecord temp;
    QSqlField field(tr("�Ƿ����"), QVariant::String);
    field.setValue(tr("��"));

    for (int i = 0;i < company->rowCount();i++)
    {
        company->removeRow(0);
    }
    while(company->canFetchMore())
    {
        company->fetchMore();
        for (int i = 0;i < company->rowCount();i++)
        {
            company->removeRow(0);
        }
    }

    while(query.next())
    {
        if (query.value(0).toString().size() > 0)
        {
            temp = query.record();
            qDebug() << "begin:" <<temp;
            temp.insert(0,field);
            company->insertRecord(0,temp);
        }
    }
    ui->treeView_3->resizeColumnToContents(0);//ʹ�п���ı�����ƥ��
    ui->treeView_3->resizeColumnToContents(1);
    company->select();



    item.clear();
    query.exec(QObject::tr("select distinct רҵ from expert"));

    while(query.next())
    {
        if (query.value(0).toString().size() > 0)
            item << query.value(0).toString();
    }
    ui->comboBox_3->clear();
    ui->comboBox_3->addItems(item);


}

void Window::Continue()
{
    int n = ui->spinBox->value();
    //    QSqlTableModel *expert = new QSqlTableModel(this,db);
    //    expert->setTable("expert");
    //    QSqlTableModel *backup = new QSqlTableModel(this,root);
    //    backup->setTable("backup");
    //    QSqlTableModel *selects = new QSqlTableModel(this,root);
    //    selects->setTable("selects");
    //    QSqlTableModel *result = new QSqlTableModel(this,root);
    //    result->setTable("result");
    //    selects->setEditStrategy(QSqlTableModel::OnFieldChange);
    //    backup->setEditStrategy(QSqlTableModel::OnFieldChange);

    expert->select(); //ѡȡ�������������
    backup->select(); //ѡȡ�������������
    selects->select(); //ѡȡ�������������
    QString select = "";
    if (ui->comboBox_2->currentIndex()!= 0)
    {
        select += tr("ְ�� ='").append(ui->comboBox_2->currentText()).append("'and ");
    }
    QString filter = company->filter();
    company->setFilter(tr("�Ƿ���� = '��'"));
    company->select();
    for (int i = 0;i < company->rowCount();i++)
    {
        qDebug() << company->record(i);
        if (company->record(i).value(0).toString().size() > 0)
            select += tr("������λ <>'")+ company->record(i).value(0).toString()+tr("'and ");
    }
    company->setFilter(filter);
    company->select();

    select += tr("רҵ= '%1' order by rnd(���)").arg(ui->comboBox_3->currentText());
    backup->setFilter(select);
    backup->select();
    ui->label->setText("��������");
    if (backup->rowCount() < n)
    {
        QMessageBox::information(this, tr("����ר�ҿ�"),tr("��ѡרҵ��ר�������㣡"));
    }
    else
    {

        QSqlField field(tr("�Ƿ��ϯ"), QVariant::String);
        field.setValue(tr("��"));



        for (int i = 0;i < n;i++)
        {
            QSqlRecord temp = backup->record(i);
            temp.remove(8);
            temp.insert(2,field);
            selects->insertRecord(0,temp);
        }

        for (int i = 0;i < n;i++)
        {
            backup->removeRow(0);
        }

        selects->select(); //ѡȡ�������������

    }

}

void Window::Save()
{


    selects->setFilter(tr("�Ƿ��ϯ='��' and ��ע is NULL"));
    selects->select();
    if (selects->rowCount() > 0)
    {
        QMessageBox::information(this, tr("����ר�ҿ�"),tr("�޷���ϯ���ڱ�ע����дԭ��"));
        selects->clear();
        selects->setTable("selects");
        selects->select();
        return;
    }

    selects->clear();
    selects->setTable("selects");
    selects->select();
    int row = selects->rowCount();
    //    QSqlRecord temp;


    for (int i = 0;i < row;i++)
    {
        result->insertRecord(0,selects->record(i));
        //        temp = selects->record(i);
        //        if (temp.value(2) == tr("��"))
        //        {
        //            unattend->insertRecord(0,temp);
        //        }
        //        else
        //        {
        //            temp.remove(2);
        //            temp.remove(2);
        //            qDebug() << temp;
        //            result->insertRecord(0,temp);
        //        }

    }

    for (int i = 0;i < row;i++)
        selects->removeRow(0);

    selects->select();
    result->select();
    ui->print_button->setEnabled(true);
}

void Window::Print()
{
    if (ui->ProjectNo->text().isEmpty())
    {
        QMessageBox::information(this, tr("����ר�ҿ�"),tr("���̱�Ų���Ϊ�գ�"));
        return;
    }
    if (ui->ProjectName->text().isEmpty())
    {
        QMessageBox::information(this, tr("����ר�ҿ�"),tr("�������Ʋ���Ϊ�գ�"));
        return;
    }
    if (ui->lineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("����ר�ҿ�"),tr("����ص㲻��Ϊ�գ�"));
        return;
    }



    QPrinter printer(QPrinter::HighResolution);
    //    QPrintPreviewWidget preview(&printer,this);
    QPrintPreviewDialog preview(&printer,this);
    connect(&preview, SIGNAL(paintRequested(QPrinter *)),this,SLOT(printPreview(QPrinter *)));
    //    printer.setFullPage(true);

    //    preview.show();
    //    preview.adjustSize();
    //    preview.adjustPosition(this);
    //    preview.autoFillBackground();

    preview.resize(800,600);
    //    preview.width();
    //    preview.widthMM();

    preview.exec();

}


bool Window::CreateConnection()
{
    db = QSqlDatabase::addDatabase("QODBC","db");  //testΪ���ݿ������
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=data/data.mdb");
    db.setPassword("1");
    root = QSqlDatabase::addDatabase("QODBC","root");  //testΪ���ݿ������
    root.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=data/root.mdb");
    root.setPassword("xiaose");
    if(!db.open()||!root.open()) return false;
    return true;
}



void Window::printPreview(QPrinter *printer)
{



    QString id = ui->ProjectNo->text();
    QString name = ui->ProjectName->text();
    QString form = ui->ProjectForm_comboBox_4->currentText();
    QString time = QDate::currentDate().toString(tr("yyyy��MM��dd��"))+" "+QTime::currentTime().toString(tr("hh:mm:ss"));;
    QString judgetime = ui->dateEdit->date().toString(tr("yyyy��MM��dd��"))+" "+ui->timeEdit->time().toString(tr("hh:mm:ss"));
    //    QString judgetime = time;
    qDebug() << judgetime;
    qDebug() << ui->lineEdit->text();
    result->setFilter(tr("�Ƿ��ϯ = '��'"));
    result->select();
    int r = result->rowCount();
    if (r == 0)
    {
        QMessageBox::information(this, tr("����ר�ҿ�"),tr("�����ר���б�Ϊ�գ��޷���ӡ��"));
        return;
    }
    QSqlRecord temp;
    QString proname,work,phone;
    QString print;

    print +=   tr("<html><head>"
                  "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
                  "</head>"
                  "<body>"
                  "<div id=\"main\">"
                  "<table cellspacing=\"0\" cellpadding=\"0\" class=\"pageeffect\" style=\""
                  "    margin: auto;"
                  "\"><tbody><tr><td class=\"pageeffect\"><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" class=\"pagewidth\" style=\"overflow-x:hidden;word-break:break-all;width:595.3pt;padding:36.0pt 36.0pt 36.0pt 36.0pt;\"><tbody><tr><td><h1 style=\""
                  "    margin-left: 222px;"
                  "\">ר�ҳ�ȡ������</h1>"
                  ""
                  "<p class=\"MsoNormal\" style=\"text-indent:8pt;\"><b><span style=\"font-size:14.0pt;font-family:����\">���̱�ţ�<span>%1</span></span></b></p>"
                  ""
                  "<table class=\"MsoNormalTable\" border=\"1\" cellspacing=\"0\" cellpadding=\"0\" align=\"left\" width=\"98%\" style=\"width:98.7%;border-collapse:collapse;border:none;"
                  " margin-left:6.75pt;margin-right:6.75pt;"
                  " \">"
                  " <tbody><tr style=\"height:24.6pt\">"
                  "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">��������</span></b></p>"
                  "  </td>"
                  "  <td width=\"78%\" nowrap=\"\" colspan=\"4\" valign=\"top\" style=\"width:78.12%;border:solid windowtext 1.0pt;"
                  "  border-left:none;padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;"
                  "  line-height:150%;font-family:����\">%2</span></b></p>"
                  "  </td>"
                  " </tr>"
                  " <tr style=\"height:24.65pt\">"
                  "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
                  "  border-top:none;"
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.65pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">�б���ʽ</span></b></p>"
                  "  </td>"
                  "  <td width=\"78%\" nowrap=\"\" colspan=\"4\" valign=\"top\" style=\"width:78.12%;border-top:"
                  "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                  "  "
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.65pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">%3<span> </span></span></b></p>"
                  "  </td>"
                  " </tr>"
                  " <tr style=\"height:24.4pt\">"
                  "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
                  "  border-top:none;"
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.4pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">��ȡר��ʱ��</span></b></p>"
                  "  </td>"
                  "  <td width=\"78%\" nowrap=\"\" colspan=\"4\" valign=\"top\" style=\"width:78.12%;border-top:"
                  "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                  "  "
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.4pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;"
                  "  line-height:150%;font-family:����\">%4</span></b></p>"
                  "  </td>"
                  " </tr>"
                  " <tr style=\"height:24.75pt\">"
                  "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
                  "  border-top:none;"
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.75pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">����ʱ��</span></b></p>"
                  "  </td>"

                  "  <td width=\"78%\" nowrap=\"\" colspan=\"4\" valign=\"top\" style=\"width:78.12%;border-top:"
                  "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                  "  "
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.4pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;"
                  "  line-height:150%;font-family:����\">%5</span></b></p>"
                  "  </td>"
                  " </tr>"

                  //                  " <tr style=\"height:24.75pt\">"
                  //                  "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
                  //                  "  border-top:none;"
                  //                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.75pt\">"

                  //                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">����ص�</span></b></p>"
                  //                  "  </td>"

                  //                  "  <td width=\"78%\" nowrap=\"\" colspan=\"4\" valign=\"top\" style=\"width:78.12%;border-top:"
                  //                  "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                  //                  "  "
                  //                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.75pt\">%6</td>"
                  //                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;"
                  //                  "  line-height:150%;font-family:����\">%6</span></b></p>"
                  //                  "  </td>"

                  //                  " </tr>"

                  " <tr style=\"height:24.65pt\">"
                  "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
                  "  border-top:none;"
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.65pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">����ص�</span></b></p>"
                  "  </td>"
                  "  <td width=\"78%\" nowrap=\"\" colspan=\"4\" valign=\"top\" style=\"width:78.12%;border-top:"
                  "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                  "  "
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.65pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">%6<span> </span></span></b></p>"
                  "  </td>"
                  " </tr>"


                  " <tr style=\"height:24.1pt\">"
                  "  <td width=\"100%\" nowrap=\"\" colspan=\"6\" valign=\"top\" style=\"width:100.0%;border:solid windowtext 1.0pt;"
                  "  border-top:none;"
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:24.1pt;text-align: center;\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">ר������</span></b></p>"
                  "  </td>"
                  " </tr>"
                  " <tr style=\"height:25.0pt\">"
                  "  <td width=\"16%\" nowrap=\"\" valign=\"top\" style=\"width:16.74%;border:solid windowtext 1.0pt;"
                  "  border-top:none;"
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:25.0pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">ר������</span></b></p>"
                  "  </td>"
                  "  <td width=\"37%\" nowrap=\"\" colspan=\"3\" valign=\"top\" style=\"width:37.32%;border-top:"
                  "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                  "  "
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:25.0pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">ר�ҹ�����λ</span></b></p>"
                  "  </td>"
                  "  <td width=\"45%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:45.92%;border-top:"
                  "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                  "  "
                  "  padding:0cm 5.4pt 0cm 5.4pt;height:25.0pt\">"
                  "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">��ϵ�绰</span></b></p>"
                  "  </td>"
                  " </tr>").arg(id,name,form,time,judgetime,ui->lineEdit->text());

    QString item = tr(
                " <tr style=\"height:25.75pt\">"
                "  <td width=\"16%\" nowrap=\"\" valign=\"top\" style=\"width:16.74%;border:solid windowtext 1.0pt;"
                "  border-top:none;"
                "  padding:0cm 5.4pt 0cm 5.4pt;height:25.75pt\">"
                "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><span style=\"font-size:11.0pt;line-height:150%;font-family:����\">%1</span></p>"
                "  </td>"
                "  <td width=\"37%\" nowrap=\"\" colspan=\"3\" valign=\"top\" style=\"width:37.32%;border-top:"
                "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                "  "
                "  padding:0cm 5.4pt 0cm 5.4pt;height:25.75pt\">"
                "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><span style=\"font-size:11.0pt;line-height:150%;font-family:����\">%2</span></p>"
                "  </td>"
                "  <td width=\"45%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:45.92%;border-top:"
                "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                "  "
                "  padding:0cm 5.4pt 0cm 5.4pt;height:25.75pt\">"
                "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><span style=\"font-size:11.0pt;line-height:150%;font-family:����\">%3</span></p>"
                "  </td>"
                " </tr>"
                );


    for (int i = 0;i < r;i++)
    {
        temp = result->record(i);
        proname = temp.value(1).toString();
        work = temp.value(6).toString();
        phone = temp.value(7).toString();
        print += item.arg(proname,work,phone);
    }

    result->setFilter(tr("�Ƿ��ϯ='��'"));
    result->select();
    r = result->rowCount();
    QString others,othername,otherthing;
    for (int i = 0;i < r;i++)
    {
        temp = result->record(i);
        othername = temp.value(1).toString();
        otherthing = temp.value(3).toString();
        others+= othername + "(" +otherthing +")  ";
    }

    print += tr(" <tr style=\"height:24.6pt\">"
                "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
                "  border-top:none;"
                "  padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">"
                "  <p class=\"MsoNormal\" align=\"left\" style=\"text-align:left;line-height:150%;"
                "  "
                "  "
                "  \"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">��ȡר����Ա</span></b></p>"
                "  </td>"
                "  <td width=\"27%\" nowrap=\"\" valign=\"top\" style=\"width:27.14%;border-top:none;"
                "  border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                "    padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">&nbsp;</td>"
                "  <td width=\"25%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:25.42%;border-top:"
                "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                "  padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">"
                "  <p class=\"MsoNormal\" align=\"left\" style=\"text-align:left;line-height:150%;"
                "  \"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">֪ͨר����Ա</span></b></p>"
                "  </td>"
                "  <td width=\"25%\" valign=\"top\" style=\"width:25.56%;border-top:none;border-left:"
                "  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                "  "
                "  padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">"
                "  <p class=\"MsoNormal\"><b></b></p>"
                "  </td>"
                " </tr>"
                " <tr style=\"height:24.9pt\">"
                "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
                "  border-top:none;"
                "  padding:0cm 5.4pt 0cm 5.4pt;height:24.9pt\">"
                "  <p class=\"MsoNormal\" align=\"left\" style=\"text-align:left;line-height:150%;"
                "    \"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">�ල��</span></b></p>"
                "  </td>"
                "  <td width=\"27%\" nowrap=\"\" valign=\"top\" style=\"width:27.14%;border-top:none;"
                "  border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                "  "
                "  padding:0cm 5.4pt 0cm 5.4pt;height:24.9pt\">&nbsp;</td>"
                "  <td width=\"25%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:25.42%;border-top:"
                "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                "    padding:0cm 5.4pt 0cm 5.4pt;height:24.9pt\">"
                "  <p class=\"MsoNormal\" align=\"left\" style=\"text-align:left;line-height:150%;"
                "    \"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">��֤��</span></b></p>"
                "  </td>"
                "  <td width=\"25%\" nowrap=\"\" valign=\"top\" style=\"width:25.56%;border-top:none;"
                "  border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
                "  "
                "  padding:0cm 5.4pt 0cm 5.4pt;height:24.9pt\">&nbsp;</td>"
                " </tr>"
                " <tr style=\"height:152.05pt\">"
                "  <td width=\"100%\" nowrap=\"\" colspan=\"6\" valign=\"top\" style=\"width:100.0%;border:solid windowtext 1.0pt;"
                "  border-top:none;"
                "  padding:0cm 5.4pt 0cm 5.4pt;height:152.05pt\">"
                "    <p class=\"MsoNormal\" style=\""
                "    margin-top: 18px;"
                "    margin-left: 10px;"
                "\"><b><span style=\"font-size:12.0pt;font-family:����\">���赥λ������ǩ��</span></b></p>"
                "  </td>"
                " </tr>"
                " <tr style=\"height:126.25pt\">"
                "  <td width=\"100%\" nowrap=\"\" colspan=\"6\" valign=\"top\" style=\"width:100.0%;border:solid windowtext 1.0pt;"
                "  border-top:none;"
                "  padding:0cm 5.4pt 0cm 5.4pt;height:126.25pt\">"
                "    <p class=\"MsoNormal\" style=\""
                "    margin-top: 18px;"
                "    margin-left: 10px;"
                "\"><b><span style=\"font-size:12.0pt;font-family:����\">��ע</span></b></p>"
                "  <p class=\"MsoNormal\"><b><span style=\"font-size:12.0pt;"
                "  font-family:����\"><span>&nbsp;&nbsp;&nbsp; </b><b><span style=\"font-size:12.0pt;font-family:����\">%1</span></b></p>"
                "  </td>"
                " </tr>"
                " </tbody></table>"
                "<p class=\"MsoNormal\">&nbsp;</p>"
                "</td></tr></tbody></table></td></tr></tbody></table>"
                "</div>"
                "</body></html>").arg(others);



    //    webView.setHtml(
    //                tr("<html><head>"
    //                   "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
    //                   "</head>"
    //                   "<body>"
    //                   "<div id=\"main\">"
    //                   "<table cellspacing=\"0\" cellpadding=\"0\" class=\"pageeffect\" style=\""
    //                   "    margin: auto;"
    //                   "\"><tbody><tr><td class=\"pageeffect\"><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" class=\"pagewidth\" style=\"overflow-x:hidden;word-break:break-all;width:595.3pt;padding:36.0pt 36.0pt 36.0pt 36.0pt;\"><tbody><tr><td><h1 style=\""
    //                   "    margin-left: 222px;"
    //                   "\">ר�ҳ�ȡ������</h1>"
    //                   ""
    //                   "<p class=\"MsoNormal\" style=\"text-indent:8pt;\"><b><span style=\"font-size:14.0pt;font-family:����\">���̱�ţ�<span>%1</span></span></b></p>"
    //                   ""
    //                   "<table class=\"MsoNormalTable\" border=\"1\" cellspacing=\"0\" cellpadding=\"0\" align=\"left\" width=\"98%\" style=\"width:98.7%;border-collapse:collapse;border:none;"
    //                   " margin-left:6.75pt;margin-right:6.75pt;"
    //                   " \">"
    //                   " <tbody><tr style=\"height:24.6pt\">"
    //                   "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">��������</span></b></p>"
    //                   "  </td>"
    //                   "  <td width=\"78%\" nowrap=\"\" colspan=\"4\" valign=\"top\" style=\"width:78.12%;border:solid windowtext 1.0pt;"
    //                   "  border-left:none;padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;"
    //                   "  line-height:150%;font-family:����\">%2</span></b></p>"
    //                   "  </td>"
    //                   " </tr>"
    //                   " <tr style=\"height:24.65pt\">"
    //                   "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.65pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">�б���ʽ</span></b></p>"
    //                   "  </td>"
    //                   "  <td width=\"78%\" nowrap=\"\" colspan=\"4\" valign=\"top\" style=\"width:78.12%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.65pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">%3<span> </span></span></b></p>"
    //                   "  </td>"
    //                   " </tr>"
    //                   " <tr style=\"height:24.4pt\">"
    //                   "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.4pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">��ȡר��ʱ��</span></b></p>"
    //                   "  </td>"
    //                   "  <td width=\"78%\" nowrap=\"\" colspan=\"4\" valign=\"top\" style=\"width:78.12%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.4pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;"
    //                   "  line-height:150%;font-family:����\">%4</span></b></p>"
    //                   "  </td>"
    //                   " </tr>"
    //                   " <tr style=\"height:24.75pt\">"
    //                   "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.75pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">����ʱ��</span></b></p>"
    //                   "  </td>"
    //                   "  <td width=\"78%\" nowrap=\"\" colspan=\"4\" valign=\"top\" style=\"width:78.12%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.75pt\">&nbsp;</td>"
    //                   " </tr>"
    //                   " <tr style=\"height:24.1pt\">"
    //                   "  <td width=\"100%\" nowrap=\"\" colspan=\"6\" valign=\"top\" style=\"width:100.0%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.1pt;text-align: center;\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">ר������</span></b></p>"
    //                   "  </td>"
    //                   " </tr>"
    //                   " <tr style=\"height:25.0pt\">"
    //                   "  <td width=\"16%\" nowrap=\"\" valign=\"top\" style=\"width:16.74%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:25.0pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">ר������</span></b></p>"
    //                   "  </td>"
    //                   "  <td width=\"37%\" nowrap=\"\" colspan=\"3\" valign=\"top\" style=\"width:37.32%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:25.0pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">ר�ҹ�����λ</span></b></p>"
    //                   "  </td>"
    //                   "  <td width=\"45%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:45.92%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:25.0pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">��ϵ�绰</span></b></p>"
    //                   "  </td>"
    //                   " </tr>"



    //                   " <tr style=\"height:25.75pt\">"
    //                   "  <td width=\"16%\" nowrap=\"\" valign=\"top\" style=\"width:16.74%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:25.75pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><span style=\"font-size:11.0pt;line-height:150%;font-family:����\">a</span></p>"
    //                   "  </td>"
    //                   "  <td width=\"37%\" nowrap=\"\" colspan=\"3\" valign=\"top\" style=\"width:37.32%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:25.75pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><span style=\"font-size:11.0pt;line-height:150%;font-family:����\">b</span></p>"
    //                   "  </td>"
    //                   "  <td width=\"45%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:45.92%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:25.75pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><span style=\"font-size:11.0pt;line-height:150%;font-family:����\">c</span></p>"
    //                   "  </td>"
    //                   " </tr>"



    //                   " <tr style=\"height:21.15pt\">"
    //                   "  <td width=\"16%\" nowrap=\"\" valign=\"top\" style=\"width:16.74%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:21.15pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><span style=\"font-size:11.0pt;line-height:150%;font-family:����\">d</span></p>"
    //                   "  </td>"
    //                   "  <td width=\"37%\" nowrap=\"\" colspan=\"3\" valign=\"top\" style=\"width:37.32%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:21.15pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><span style=\"font-size:11.0pt;line-height:150%;font-family:����\">e</span></p>"
    //                   "  </td>"
    //                   "  <td width=\"45%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:45.92%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:21.15pt\">"
    //                   "  <p class=\"MsoNormal\" style=\"line-height:150%;\"><span style=\"font-size:11.0pt;line-height:150%;font-family:����\">f</span></p>"
    //                   "  </td>"
    //                   " </tr>"



    //                   " <tr style=\"height:24.6pt\">"
    //                   "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">"
    //                   "  <p class=\"MsoNormal\" align=\"left\" style=\"text-align:left;line-height:150%;"
    //                   "  "
    //                   "  "
    //                   "  \"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">��ȡר����Ա</span></b></p>"
    //                   "  </td>"
    //                   "  <td width=\"27%\" nowrap=\"\" valign=\"top\" style=\"width:27.14%;border-top:none;"
    //                   "  border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "    padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">&nbsp;</td>"
    //                   "  <td width=\"25%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:25.42%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">"
    //                   "  <p class=\"MsoNormal\" align=\"left\" style=\"text-align:left;line-height:150%;"
    //                   "  \"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">֪ͨר����Ա</span></b></p>"
    //                   "  </td>"
    //                   "  <td width=\"25%\" valign=\"top\" style=\"width:25.56%;border-top:none;border-left:"
    //                   "  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.6pt\">"
    //                   "  <p class=\"MsoNormal\"><b></b></p>"
    //                   "  </td>"
    //                   " </tr>"
    //                   " <tr style=\"height:24.9pt\">"
    //                   "  <td width=\"21%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:21.88%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.9pt\">"
    //                   "  <p class=\"MsoNormal\" align=\"left\" style=\"text-align:left;line-height:150%;"
    //                   "    \"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">�ල��</span></b></p>"
    //                   "  </td>"
    //                   "  <td width=\"27%\" nowrap=\"\" valign=\"top\" style=\"width:27.14%;border-top:none;"
    //                   "  border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.9pt\">&nbsp;</td>"
    //                   "  <td width=\"25%\" nowrap=\"\" colspan=\"2\" valign=\"top\" style=\"width:25.42%;border-top:"
    //                   "  none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "    padding:0cm 5.4pt 0cm 5.4pt;height:24.9pt\">"
    //                   "  <p class=\"MsoNormal\" align=\"left\" style=\"text-align:left;line-height:150%;"
    //                   "    \"><b><span style=\"font-size:12.0pt;line-height:150%;font-family:����\">��֤��</span></b></p>"
    //                   "  </td>"
    //                   "  <td width=\"25%\" nowrap=\"\" valign=\"top\" style=\"width:25.56%;border-top:none;"
    //                   "  border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;"
    //                   "  "
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:24.9pt\">&nbsp;</td>"
    //                   " </tr>"
    //                   " <tr style=\"height:152.05pt\">"
    //                   "  <td width=\"100%\" nowrap=\"\" colspan=\"6\" valign=\"top\" style=\"width:100.0%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:152.05pt\">"
    //                   "    <p class=\"MsoNormal\" style=\""
    //                   "    margin-top: 18px;"
    //                   "    margin-left: 10px;"
    //                   "\"><b><span style=\"font-size:12.0pt;font-family:����\">���赥λ������ǩ��</span></b></p>"
    //                   "  </td>"
    //                   " </tr>"
    //                   " <tr style=\"height:126.25pt\">"
    //                   "  <td width=\"100%\" nowrap=\"\" colspan=\"6\" valign=\"top\" style=\"width:100.0%;border:solid windowtext 1.0pt;"
    //                   "  border-top:none;"
    //                   "  padding:0cm 5.4pt 0cm 5.4pt;height:126.25pt\">"
    //                   "    <p class=\"MsoNormal\" style=\""
    //                   "    margin-top: 18px;"
    //                   "    margin-left: 10px;"
    //                   "\"><b><span style=\"font-size:12.0pt;font-family:����\">��ע</span></b></p>"
    //                   "  <p class=\"MsoNormal\"><b><span style=\"font-size:12.0pt;"
    //                   "  font-family:����\"><span>&nbsp;&nbsp;&nbsp; </span>A</span></b><b><span style=\"font-size:12.0pt;font-family:����\">���ܲμ�</span></b></p>"
    //                   "  </td>"
    //                   " </tr>"
    //                   " </tbody></table>"
    //                   "<p class=\"MsoNormal\">&nbsp;</p>"
    //                   "</td></tr></tbody></table></td></tr></tbody></table>"
    //                   "</div>"
    //                   "</body></html>").arg(id,name,form,time));
    result->clear();
    result->setTable("result");
    result->select();
    //    doc.setHtml(print);
    //    doc.print(printer);
    webView.setHtml(print);
    webView.print(printer);
}









void Window::on_changepassword_triggered()
{
    chgpassword.show();
}



void Window::on_exit_triggered()
{
    close();
}

void Window::on_printers_triggered()
{
    Print();
}

void Window::on_delete_button_clicked()
{
    QModelIndexList selected = ui->tableView->selectionModel()->selectedIndexes(); //��������ѡ����ģ����Ŀ�����б�
    int ok = QMessageBox::warning(this,tr("ɾ��ѡ�е���!"),tr("��ȷ��ɾ����ǰѡȡ�е�����?"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::Yes)
    {
        QMap<int, int> rowMap;
        foreach (QModelIndex index, selected)
        {
            rowMap.insert(index.row(), 0);
        }
        int rowToDel;
        QMapIterator<int, int> rowMapIterator(rowMap);
        rowMapIterator.toBack();
        while (rowMapIterator.hasPrevious())
        {
            rowMapIterator.previous();
            rowToDel = rowMapIterator.key();
            expert1->removeRow(rowToDel);
        }

        //        expert1->submitAll();
    }

}

void Window::on_add_button_clicked()
{
    int rowNum = expert1->rowCount(); //��ñ������
    QSqlQuery query(db);
    int id = 0;
    query.exec(QObject::tr("select max(���) from expert"));
    query.first();
    id = query.value(0).toInt()+1;
    expert1->select();
    //        qDebug("%d\n",id);


    //    model->setQuery(query);
    //    model->setSort(0,Qt::AscendingOrder); //id���ԣ�����0�У���������

    //    model->select();
    //    int id = model->record(rowNum-1).value(0).toInt()+1;
    expert1->insertRow(0); //���һ��
    //    ui->treeView_3->scrollToBottom();
    expert1->setData(expert1->index(0,0),id);
    expert1->setData(expert1->index(0,7),"1");
    //    expert1->sort(0,Qt::DescendingOrder);
    //    expert1->select();

    expert1->submitAll(); //����ֱ���ύ
}


void Window::on_begin_button_clicked()
{
    ui->label->setText(tr("���������С�����"));
    ui->label->repaint();
    Search();
}

void Window::on_continue_button_clicked()
{
    ui->label->setText(tr("���������С�����"));
    ui->label->repaint();
    Continue();
}

void Window::on_save_button_2_clicked()
{
    Save();
}

void Window::on_print_button_clicked()
{
    Print();
}

void Window::on_save_button_clicked()
{
    expert1->submitAll();
    AddItem();
}

void Window::on_pushButton_clicked()
{
    expert1->revertAll();
}

void Window::on_action_triggered()
{
    about.exec();
}

void Window::findCompany()
{
    QString find;
    company->setFilter(tr("������λ like '%%1%'").arg(ui->findCompany->text()));
    company->select();

}

void Window::on_findCompany_textChanged(const QString &arg1)
{
    findCompany();
}
