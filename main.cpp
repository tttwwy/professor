#include <window.h>

#include <QApplication>
#include <QTextCodec>
#include "loginto.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *code = QTextCodec::codecForName("system");
    QTextCodec::setCodecForTr(code);
    QTextCodec::setCodecForLocale(code);
    QTextCodec::setCodecForCStrings(code);

    Window w;
    Loginto login;
    if (login.exec()==QDialog::Accepted)
    {
        w.chgpassword.username = login.username;
        //        w.setWindowFlags(w.windowFlags() & ~Qt::WindowMaximizeButtonHint &  ~Qt::WindowMinimizeButtonHint);
        w.setWindowState(Qt::WindowMaximized);
        w.showMaximized();

        //        w.show();
        //        splash.finish(&w);
        return a.exec();
    }
    else
        return 0;

}
