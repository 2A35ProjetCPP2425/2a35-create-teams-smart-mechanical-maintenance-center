#ifndef PIECE_H
#define PIECE_H

#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSlice>
#include <QPieSeries>
//using namespace  QtCharts;

class piece
{
public:

    int  id , reff , quant , prix ;
    QString etat;
    QString image;




public:

    piece();
    piece(int id, int reff, int quant, int prix, const QString &etat, const QString &image);



    int getId() const { return id; }
    int getReff() const { return reff; }
    int getQuant() const { return quant; }
    int getPrix() const { return prix; }
    QString getEat()  { return etat; }


    // Setters*
    void setId(int id_m) { id = id_m; }
    void setReff(int reff_m) { reff = reff_m; }
     void setQuant(int quant_m) { quant = quant_m; }
     void setPrix(int prix_m) { prix = prix_m; }
     void setEtat(QString etat_m) {etat = etat_m;}
     void setImage(const QString &encodedImage) { image = encodedImage; }


    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier();

    // metiers simple
    QSqlQueryModel* rechercher(QString critere, QString valeur);
    QSqlQueryModel* trier(QString critere);
    QString genererContenuPDF();

    void genererPDF(const QString& fichierPDF);
    QPieSeries* statistiques();

    QString getImageById(int id);


};

#endif // PIECE_H
