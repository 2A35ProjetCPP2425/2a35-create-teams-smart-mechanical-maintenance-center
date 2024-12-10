#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMap>

class Service
{
    int ID;                   // Identifiant du service
    QString NOM_SERVICE;      // Nom du service
    QString TYPE_PANNE;       // Type de panne
    int PRIX_SERVICE;      // Prix du service (modifié en double)
    QString MODEL_PANNE;      // Modèle de panne (modifié ici)

public:
    // Constructeurs
    Service();
    Service(int id, const QString& nom_service, const QString& type_panne, int prix_service, const QString& model_panne);

    // Getters
    int getId() const;
    QString getNomService() const;
    QString getTypePanne() const;
    int getPrixService() const;  // Modification pour le prix
    QString getModelPanne() const;  // Modification ici
    QMap<QString, QString> getPiecePourPanne() const;

    // Setters
    void setId(int id);
    void setNomService(const QString& nom_service);
    void setTypePanne(const QString& type_panne);
    void setPrixService(int prix_service);  // Modification pour le prix
    void setModelPanne(const QString& model_panne);  // Modification ici

    // Méthodes CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    QSqlQueryModel* afficher2();
    QSqlQueryModel* afficher3();
    bool modifier(int id, const QString& nom_service, const QString& type_panne, int prix_service, const QString& model_panne);
    bool supprimer(int id);
    QMap<QString, int> obtenirStatistiquesModeles();

};

#endif // SERVICE_H
