#include "matieres.h"
#include "QDebug"
#include "QSqlError"
matieres::matieres(){};
matieres::matieres(int Id_matieres, QString nature_M, int Quantite_M, QString type_m)
{
    this->nature_M = nature_M;
    this->Quantite_M = Quantite_M;
    this->type_m = type_m;
    this->Id_matieres = Id_matieres; // Initialize Id_matieres to an invalid value
}
int matieres::getId_matieres() {
    return Id_matieres;
}

QString matieres::getnature_M() {
    return nature_M;
}

int matieres::getQuantite_M() {
    return Quantite_M;
}

QString matieres::gettype_m() {
    return type_m;
}

void matieres::setnature_M(QString nature_M) {
    this->nature_M = nature_M;
}

void matieres::setQuantite_M(int Quantite_M) {
    this->Quantite_M = Quantite_M;
}

void matieres::settype_m(QString type_m) {
    this->type_m = type_m;
}
bool matieres::supprimer(int id) {
    QSqlQuery query;

    // Prepare the DELETE statement
    query.prepare("DELETE FROM matieres WHERE Id_matieres = :Id_matieres");
    query.bindValue(":Id_matieres", id);

    // Execute the query
    if (query.exec()) {
        return true;  // Success
    } else {
        qDebug() << "Error deleting record: " << query.lastError().text();
        return false;  // Failure
    }
}

bool matieres::ajouter() {
    QSqlQuery query;



    query.prepare("INSERT INTO matieres (Id_matieres, nature_M, Quantite_M, type_m) "
                  "VALUES (:Id_matieres, :nature_M, :Quantite_M, :type_m)");

    query.bindValue(":Id_matieres", Id_matieres);  // Bind the manual ID
    query.bindValue(":nature_M", nature_M);
    query.bindValue(":Quantite_M", Quantite_M);
    query.bindValue(":type_m", type_m);


    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error inserting record: " << query.lastError().text();
        return false;
    }
}
bool matieres::modifier(int idr) {
    QSqlQuery query;

    // Update the matieres table based on Id_matieres
    query.prepare("UPDATE matieres SET nature_M = :nature_M, Quantite_M = :Quantite_M, type_m = :type_m WHERE Id_matieres = :Id_matieres");

    query.bindValue(":Id_matieres", idr);  // Update based on the provided idr
    query.bindValue(":nature_M", nature_M);
    query.bindValue(":Quantite_M", Quantite_M);
    query.bindValue(":type_m", type_m);


    return query.exec();  // Execute the update query
}

QSqlQueryModel * matieres ::afficher(){
    QSqlQueryModel * model=new QSqlQueryModel();
    model->setQuery("select * from matieres");
    model->setHeaderData (0,Qt::Horizontal, QObject::tr("Id_matieres"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("nature_M"));
    model->setHeaderData (2,Qt::Horizontal, QObject::tr("Quantite_M"));
    model->setHeaderData (3,Qt::Horizontal, QObject::tr("type_m"));

    return model;

};
QSqlQueryModel* matieres::trier( bool ascent) {
    QSqlQueryModel* model = new QSqlQueryModel();


        if (ascent) {
            model->setQuery("SELECT * FROM MATIERES ORDER BY QUANTITE_M");
        } else {
            model->setQuery("SELECT * FROM MATIERES ORDER BY QUANTITE_M DESC");
        }


    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_MATIERES"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NATURE_M"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("QUANTITE_M"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TYPE_M"));



    return model;
}
