#ifndef CLIENT_H
#define CLIENT_H
#include <QGraphicsView>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidget>
#include "qgraphicsview.h"
#include<QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QTableWidgetItem>
class client
{
    QString nom, prenom,adresse,pays,mail;
    int code_client,cin,tel;
public:
    client();
    client(int,QString,QString,int,QString,QString,int,QString);

    int getcode_client(){return code_client;}
    QString getNom(){return nom;}
    QString getPrenom(){return prenom;}
    int getCin(){return cin;}
    QString getAdresse(){return adresse;}
    QString getPays(){return pays;}
    int getTel(){return tel;}
    QString getMail(){return mail;}

    void setCode_client(int code_client){this->code_client=code_client;}
    void setNom(QString n){nom=n;}
    void setPrenom(QString pr){prenom=pr;}
    void setCin(int cin){this->cin=cin;}
    void setAdresse(QString ad){adresse=ad;}
    void setPays(QString pa){pays=pa;}
    void setTel(int tel){this->tel=tel;}
    void setMail(QString email){mail=email;}

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier();
    int getNextCodeClient();
    bool exists(int);
    QSqlQueryModel * rechercher(QString rech);
    bool existsByCin(const QString &cinStr);
    bool existsByTel(const QString &telStr);
    QSqlQueryModel * tri(const QString &colonne, const QString &ordre);
    void displayRegionStatistics(QGraphicsView *pieView);
    void displayCityStatistics(QTableWidget *tableView);
    void displayEmailDomainStatistics(QGraphicsView *domainBarView);
};


#endif // CLIENT_H
