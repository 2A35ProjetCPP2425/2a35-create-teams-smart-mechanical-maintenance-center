#include "service.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Constructeur par défaut
Service::Service() {
    // Initialisation des attributs avec des valeurs par défaut
    ID = 0;
    PRIX_SERVICE =0;  // Initialisation en chaîne vide pour le prix
    NOM_SERVICE = "";
    TYPE_PANNE = "";
    MODEL_PANNE = "";
}

// Constructeur avec paramètres
Service::Service(int id, const QString& nom_service, const QString& type_panne, int prix_service, const QString& model_panne)
    : ID(id), NOM_SERVICE(nom_service), TYPE_PANNE(type_panne),PRIX_SERVICE(prix_service), MODEL_PANNE(model_panne) {}

// Getters
int Service::getId() const { return ID; }
QString Service::getNomService() const { return NOM_SERVICE; }
QString Service::getTypePanne() const { return TYPE_PANNE; }
QString Service::getModelPanne() const { return MODEL_PANNE; }
int Service::getPrixService() const { return PRIX_SERVICE; }

// Setters
void Service::setId(int id) { ID = id; }
void Service::setNomService(const QString& nom_service) { NOM_SERVICE = nom_service; }
void Service::setTypePanne(const QString& type_panne) { TYPE_PANNE = type_panne; }
void Service::setModelPanne(const QString& model_panne) { MODEL_PANNE = model_panne; }
void Service::setPrixService(int prix_service) { PRIX_SERVICE = prix_service; }

// Méthode pour ajouter un service
/*bool Service::ajouter() {
    QSqlQuery query;

    // Préparation de la requête d'insertion
    query.prepare("INSERT INTO AZIZ.SERVICE (ID, nom_service, type_panne, model_panne, prix_service) "
                  "VALUES (:id, :nom_service, :type_panne, :model_panne, :prix_service)");

    // Liaison des valeurs

    query.bindValue(":id", ID);
    query.bindValue(":nom_service", NOM_SERVICE);
    query.bindValue(":type_panne", TYPE_PANNE);
    query.bindValue(":model_panne", MODEL_PANNE);
    query.bindValue(":prix_service", PRIX_SERVICE);

    // Exécution de la requête et vérification d'erreur
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du service:" << query.lastError().text();
        return false;
    }
    return true;
}*/
bool Service::ajouter() {
    // Contrôles de saisie
    if (NOM_SERVICE.isEmpty()) {
        qDebug() << "Erreur : Le nom du service ne peut pas être vide.";
        return false;
    }
    if (TYPE_PANNE.isEmpty()) {
        qDebug() << "Erreur : Le type de panne ne peut pas être vide.";
        return false;
    }
    if (MODEL_PANNE.isEmpty()) {
        qDebug() << "Erreur : Le modèle de panne ne peut pas être vide.";
        return false;
    }
    if (PRIX_SERVICE <= 0) {
        qDebug() << "Erreur : Le prix du service doit être supérieur à zéro.";
        return false;
    }

    QSqlQuery query;

    // Préparation de la requête d'insertion
    query.prepare("INSERT INTO SERVICE (ID, nom_service, type_panne, model_panne, prix_service) "
                  "VALUES (:id, :nom_service, :type_panne, :model_panne, :prix_service)");

    // Liaison des valeurs
    query.bindValue(":id", ID);
    query.bindValue(":nom_service", NOM_SERVICE);
    query.bindValue(":type_panne", TYPE_PANNE);
    query.bindValue(":model_panne", MODEL_PANNE);
    query.bindValue(":prix_service", PRIX_SERVICE);

    // Exécution de la requête et vérification d'erreur
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du service:" << query.lastError().text();
        return false;
    }

    qDebug() << "Service ajouté avec succès!";
    return true;
}


// Méthode pour afficher les services
QSqlQueryModel* Service::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM AZIZ.SERVICE");

    // Définir les en-têtes de colonnes pour le modèle
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Service"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type Panne"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Modèle voiture"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Prix Service"));

    return model;
}
/*
// Méthode pour modifier un service
bool Service::modifier(int id, const QString& nom_service, const QString& type_panne,int prix_service, const QString& model_panne) {
    QSqlQuery query;

    // Préparation de la requête de mise à jour
    query.prepare("UPDATE AZIZ.SERVICE SET nom_service = :nom_service, type_panne = :type_panne, "
                  "model_panne = :model_panne, prix_service = :prix_service "
                  "WHERE id = :id");

    // Liaison des valeurs
    query.bindValue(":id", id);
    query.bindValue(":nom_service", nom_service);
    query.bindValue(":type_panne", type_panne);
    query.bindValue(":model_panne", model_panne);
    query.bindValue(":prix_service", prix_service);

    // Exécution de la requête et vérification d'erreur
    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification du service:" << query.lastError().text();
        return false;
    }
    return true;
}*/
bool Service::modifier(int id, const QString& nom_service, const QString& type_panne, int prix_service, const QString& model_panne) {
    // Contrôles de saisie
    if (nom_service.isEmpty()) {
        qDebug() << "Erreur : Le nom du service ne peut pas être vide.";
        return false;
    }
    if (type_panne.isEmpty()) {
        qDebug() << "Erreur : Le type de panne ne peut pas être vide.";
        return false;
    }
    if (model_panne.isEmpty()) {
        qDebug() << "Erreur : Le modèle de panne ne peut pas être vide.";
        return false;
    }
    if (prix_service <= 0) {
        qDebug() << "Erreur : Le prix du service doit être supérieur à zéro.";
        return false;
    }

    QSqlQuery query;

    // Préparation de la requête de mise à jour
    qDebug() << "Préparation de la requête de mise à jour dans la table 'SERVICE'...";
    query.prepare("UPDATE AZIZ.SERVICE SET nom_service = :nom_service, type_panne = :type_panne, "
                  "model_panne = :model_panne, prix_service = :prix_service "
                  "WHERE id = :id");

    // Liaison des valeurs
    query.bindValue(":id", id);
    query.bindValue(":nom_service", nom_service);
    query.bindValue(":type_panne", type_panne);
    query.bindValue(":model_panne", model_panne);
    query.bindValue(":prix_service", prix_service);

    // Exécution de la requête et vérification d'erreur
    qDebug() << "Exécution de la requête de mise à jour...";
    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification du service:" << query.lastError().text();
        return false;
    }

    qDebug() << "Service modifié avec succès!";
    return true;
}

/*
// Méthode pour supprimer un service
bool Service::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM AZIZ.SERVICE WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du service:" << query.lastError().text();
        return false;
    }
    return true;
}

*/
bool Service::supprimer(int id) {
    // Contrôle de saisie : vérifier que l'ID est valide
    if (id <= 0) {
        qDebug() << "Erreur : L'ID du service doit être un entier positif.";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM AZIZ.SERVICE WHERE id = :id");
    query.bindValue(":id", id);

    // Exécution de la requête et vérification d'erreur
    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du service:" << query.lastError().text();
        return false;
    }

    qDebug() << "Service supprimé avec succès!";
    return true;
}
QSqlQueryModel* Service::afficher2() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID,MODEL_PANNE FROM SERVICE");

    // Définir les en-têtes de colonnes pour le modèle
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("model"));

    return model;
}
QMap<QString, QString> Service::getPiecePourPanne() const {
    QMap<QString, QString> pannePieces = {
        {"Crevaison", "Pneus"},
        {"Freins defectueux", "Plaquettes de frein"},
        {"Probleme d'huile", "Filtres à huile"},
        {"Probleme de moteur", "Filtres à air"},
        {"Probleme d'injection", "Filtres à carburant"},
        {"Courroie cassee", "Courroies"},
        {"Probleme d'alternateur", "Alternateurs"},
        {"Surchauffe moteur", "Pompes à eau"},
        {"Probleme electrique", "Batterie"},
        {"Demarrage difficile", "Bougies d'allumage"}
    };

    return pannePieces;
}
QSqlQueryModel* Service::afficher3() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID,TYPE_PANNE,MODEL_PANNE FROM SERVICE");

    // Définir les en-têtes de colonnes pour le modèle
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("panne"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("model"));

    return model;
}
QMap<QString, int> Service::obtenirStatistiquesModeles() {
    QMap<QString, int> stats;

    QSqlQuery query;
    query.prepare("SELECT MODEL_PANNE, COUNT(*) AS count FROM SERVICE GROUP BY MODEL_PANNE");

    if (query.exec()) {
        while (query.next()) {
            QString modele = query.value(0).toString();
            int count = query.value(1).toInt();
            stats[modele] = count;
        }
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
    }

    return stats;
}
