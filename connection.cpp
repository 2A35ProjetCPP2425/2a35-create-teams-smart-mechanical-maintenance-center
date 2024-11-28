#include "connection.h"

Connection::Connection(){}

bool Connection::createconnection()
{
    db= QSqlDatabase::addDatabase("QODBC");
    bool test=false;
    db.setDatabaseName("test");
    db.setUserName("omar");
    db.setPassword("omar");

    if (db.open()) test=true;

    return test;

}
void Connection::closeConnection(){db.close();}
