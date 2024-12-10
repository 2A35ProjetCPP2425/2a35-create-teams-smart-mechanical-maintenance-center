#ifndef MATIERES_H
#define MATIERES_H
#include <QString>
#include<QsqlQuery>
#include<QsqlQueryModel>
class matieres
{
private:
    int Id_matieres,Quantite_M;
    QString nature_M,type_m ;

public:
    matieres();

    matieres(int, QString, int, QString);
    int getId_matieres();
    QString getnature_M();
    int getQuantite_M();
    QString gettype_m();
    void setnature_M(QString);
    void setQuantite_M(int);
    void settype_m(QString);

    bool ajouter();
  bool supprimer(int id);
    bool modifier(int);
    QSqlQueryModel * afficher();
    QSqlQueryModel * trier(bool assent);


};
#endif // MATIERES_H
