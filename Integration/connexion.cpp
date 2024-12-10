#include "connexion.h"
#include <QDebug>
#include <QSqlError>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Integration"); // Insert the data source name (DSN)
    db.setUserName("Integ");          // Insert username
    db.setPassword("Integ");           // Insert password

    if (!db.open()) {  // Explicitly try to open the database
        qDebug() << "Database connection error:" << db.lastError().text();
    } else {
        qDebug() << "Connection successful!";
        test = true;
    }

    return test;
}
