#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    bool test=c.createconnection();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Gcommande_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    if(test)
    {
        w.show();

        QMessageBox::information(nullptr,QObject::tr("database is open"),
        QObject::tr("connection succesful.\n"
                    "click cancel to exit."), QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                              QObject::tr("connection failed.\n"
                                          "click Cancel to exit."), QMessageBox::Cancel);
    w.show();
    return a.exec();
}
