#include "mainwindow.h"
#include <QApplication>
#include "connexion.h"
#include <QMessageBox>
#include "dialog.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Connection c;
    bool test = c.createconnect();

    if (!test) {
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                              QObject::tr("Connection failed.\nClick Cancel to exit."),
                              QMessageBox::Cancel);
        return -1; // Exit the application if database connection fails
    }

    QMessageBox::information(nullptr, QObject::tr("Database is open"),
                             QObject::tr("Connection successful.\nClick OK to continue."),
                             QMessageBox::Ok);

    // Show the login dialog
    Dialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        int email = dialog.getEmail();  // Call the getter functio
        qDebug() << "Email passed to MainWindow:" << email;
        MainWindow w(email);
        w.show();
        return a.exec(); // Start the application event loop
    } else {
        return 0; // Exit if login fails
    }

}
