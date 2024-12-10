#ifndef COMMANDE_H
#define COMMANDE_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QDate>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QListView>



class Commande
{
      int idCommande; // généré automatiquement
      QDate dateCommande;
      QString etatCommande;
      int montantTotal;
      QDate dateLivraison;
      int genererIdCommande();
public:
      // Constructeur
          Commande();
          Commande(const QDate& dateCommande, const QString& etatCommande,
                   int montantTotal, const QDate& dateLivraison);


          // Getters et setters
          int getIdCommande() const; // Facultatif
          QDate getDateCommande() const;
          QString getEtatCommande() const;
          int getMontantTotal() const;
          QDate getDateLivraison() const;

          void setDateCommande(const QDate& dateCommande);
          void setEtatCommande(const QString& etatCommande);
          void setMontantTotal(int montantTotal);
          void setDateLivraison(const QDate& dateLivraison);

          void setIdCommande(int id);

          bool idExists(int idCommande);
          //Crud Methodes
          bool ajouter();
          QSqlQueryModel* afficher();
          bool modifier( const QDate& dateCommande, const QString& etatCommande,
                                  int montantTotal, const QDate& dateLivraison);
          bool supprimer(int idCommande);
          void telechargerPDF();
          // Dans le fichier commande.h
          bool ajouterHistorique(int idCommande, int idPerso, const QString& action);
          void loadHistorique(QListView *historiqueListView);






};

#endif // COMMANDE_H
