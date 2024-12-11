#include "connexion.h"

Connection::Connection()
{

}

bool Connection::createconnection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    bool test=false;
    db.setDatabaseName("CPP_PROJECT");
    db.setUserName("YASSINE");
    db.setPassword("projetcpp2a35");

    if (db.open())
        test=true;
    return  test;
}
void Connection::closeConnection(){
    db.close();
}
