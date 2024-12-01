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
    db.setDatabaseName("projet_c++_2A35"); // Insert the data source name (DSN)
    db.setUserName("islem");          // Insert username
    db.setPassword("zribi");          // Insert password

    if (!db.open()) {  // Explicitly try to open the database
        qDebug() << "Database connection error:" << db.lastError().text();
    } else {
        qDebug() << "Connection successful!";
        test = true;
    }

    return test;
}
