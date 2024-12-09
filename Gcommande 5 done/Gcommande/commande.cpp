#include "commande.h"
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QListView>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>



// Constructeur par défaut
Commande::Commande() {
    // Initialiser les attributs avec des valeurs par défaut
    idCommande = 0;
    montantTotal = 0;


}

// Constructeur avec paramètres
Commande::Commande(const QDate& dateCommande, const QString& etatCommande,
                   int montantTotal, const QDate& dateLivraison)
    : dateCommande(dateCommande), etatCommande(etatCommande),
      montantTotal(montantTotal), dateLivraison(dateLivraison) {}

// Getters
int Commande::getIdCommande() const { return idCommande; }
QDate Commande::getDateCommande() const { return dateCommande; }
QString Commande::getEtatCommande() const { return etatCommande; }
int Commande::getMontantTotal() const { return montantTotal; }
QDate Commande::getDateLivraison() const { return dateLivraison; }

// Setters
void Commande::setDateCommande(const QDate& dateCommande) { this->dateCommande = dateCommande; }
void Commande::setEtatCommande(const QString& etatCommande) { this->etatCommande = etatCommande; }
void Commande::setMontantTotal(int montantTotal) { this->montantTotal = montantTotal; }
void Commande::setDateLivraison(const QDate& dateLivraison) { this->dateLivraison = dateLivraison; }

void Commande::setIdCommande(int id) {
    idCommande = id;
}

bool Commande::idExists(int idCommande) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM commande WHERE idCommande = :idCommande");
    query.bindValue(":idCommande", idCommande);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

// Fonction pour générer un idCommande unique
int Commande::genererIdCommande() {
    // Initialiser le générateur de nombres aléatoires
    std::srand(std::time(nullptr));

    int id;
    QSqlQuery query;

    do {
        // Générer un ID aléatoire entre 1000000 et 9999999
        id = 10000 + std::rand() % 90000;

        // Vérifier si l'ID existe déjà dans la base
        query.prepare("SELECT COUNT(*) FROM commande WHERE idCommande = :id");
        query.bindValue(":id", id);

        if (!query.exec()) {
            qDebug() << "Erreur lors de la vérification de l'idCommande:" << query.lastError().text();
            return -1; // Retourner -1 en cas d'erreur
        }

        query.next();
    } while (query.value(0).toInt() > 0); // Répéter si l'ID existe déjà

    return id;
}

bool Commande::ajouter() {
    QSqlQuery query;

    // Préparation de la requête pour insérer dans la table commande
    query.prepare("INSERT INTO commande (idcommande, datecommande, etatcommande, montantTotal, datelivraison) "
                  "VALUES (:idcommande, TO_DATE(:dateCommande, 'YYYY-MM-DD'), :etatCommande, :montantTotal, TO_DATE(:dateLivraison, 'YYYY-MM-DD'))");

    // Liaison des valeurs
    int idcommande = genererIdCommande();
    query.bindValue(":idcommande", idcommande);
    query.bindValue(":dateCommande", dateCommande.toString("yyyy-MM-dd"));
    query.bindValue(":etatCommande", etatCommande);
    query.bindValue(":montantTotal", montantTotal);
    query.bindValue(":dateLivraison", dateLivraison.toString("yyyy-MM-dd"));

    // Exécution de la requête et vérification d'erreur
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de la commande:" << query.lastError().text();
        return false;
    }

    // Ajout de l'historique après une insertion réussie
    int idPerso = 1;  // Exemple, remplacez par le bon identifiant de l'employé
    QString action = "ajout commande";

    if (!ajouterHistorique(idcommande, idPerso, action)) {
        qDebug() << "Erreur lors de l'ajout à l'historique.";
        return false;
    }

    // Ajouter un suivi pour cette commande dans la table suivi_commandes
    QSqlQuery suiviQuery;
    suiviQuery.prepare("INSERT INTO suivi_commandes (id_suivi, id_commande, etape, description, date_action) "
                           "VALUES (SUIVI_COMMANDES_SEQ.NEXTVAL, :id_commande, :etape, :description, SYSDATE)");
    suiviQuery.bindValue(":id_commande", idcommande);

    // Etape par défaut : Commande passée (0) lors de l'ajout
    int etape = 0;  // Commande passée
    QString description = "Commande ajoutée";

    // Si l'état est "En préparation", nous mettons à jour l'étape
    if (etatCommande == "En préparation") {
        etape = 1;
        description = "En préparation";
    } else if (etatCommande == "En livraison") {
        etape = 2;
        description = "En livraison";
    }

    suiviQuery.bindValue(":etape", etape);
    suiviQuery.bindValue(":description", description);

    if (!suiviQuery.exec()) {
        qDebug() << "Erreur lors de l'ajout dans suivi_commandes:" << suiviQuery.lastError().text();
        return false;
    }

    qDebug() << "Commande ajoutée, suivi ajouté et historique mis à jour avec succès.";
    return true;
}



// Méthode pour afficher les commandes
QSqlQueryModel* Commande::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM commande"); // Assurez-vous que le nom de la table est correct

    // Définir les en-têtes de colonnes pour le modèle
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("État Commande"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Montant Total"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Livraison"));




    return model;
}

bool Commande::modifier(const QDate& dateCommande, const QString& etatCommande,
                        int montantTotal, const QDate& dateLivraison) {
    // Vérifier que l'idCommande est valide (non nul ou négatif)
    if (idCommande <= 0) {
        qDebug() << "idCommande non valide.";
        return false;
    }

    QStringList updateFields;

    // Dynamically construct the fields to be updated
    if (dateCommande.isValid()) {
        updateFields << "dateCommande = TO_DATE(:dateCommande, 'YYYY-MM-DD')";
    }
    if (!etatCommande.isEmpty()) {
        updateFields << "etatCommande = :etatCommande";
    }
    if (montantTotal > 0) { // Assuming montantTotal should be positive
        updateFields << "montantTotal = :montantTotal";
    }
    if (dateLivraison.isValid()) {
        updateFields << "dateLivraison = TO_DATE(:dateLivraison, 'YYYY-MM-DD')";
    }

    // Check if there are fields to update
    if (updateFields.isEmpty()) {
        qDebug() << "No fields to update.";
        return false; // Nothing to update
    }

    // Construct the update query
    QString queryString = "UPDATE commande SET " + updateFields.join(", ") + " WHERE idCommande = :idCommande";

    // Prepare the query
    QSqlQuery query;
    query.prepare(queryString);

    // Bind values only for the fields that are not empty or default
    if (dateCommande.isValid()) query.bindValue(":dateCommande", dateCommande.toString("yyyy-MM-dd"));
    if (!etatCommande.isEmpty()) query.bindValue(":etatCommande", etatCommande);
    if (montantTotal > 0) query.bindValue(":montantTotal", montantTotal);
    if (dateLivraison.isValid()) query.bindValue(":dateLivraison", dateLivraison.toString("yyyy-MM-dd"));

    // Bind the idCommande value from the object itself
    query.bindValue(":idCommande", idCommande);

    // Execute the query and check for success
    if (!query.exec()) {
        qDebug() << "Failed to update Commande:" << query.lastError().text();
        return false;
    }

    // After successful modification, log the action in the history
    int idPerso = 1;  // Example: Replace with actual logic to fetch the employee's ID
    QString action = "modification commande";

    if (!ajouterHistorique(idCommande, idPerso, action)) {
        qDebug() << "Erreur lors de l'ajout à l'historique.";
        return false;  // You may choose to continue or fail here depending on the importance of logging
    }

    // Now update or insert into suivi_commandes to reflect the changes
    // For this example, let's assume you need to create a new step in suivi_commandes
    QSqlQuery suiviQuery;
    suiviQuery.prepare("INSERT INTO suivi_commandes (id_suivi, id_commande, etape, description, date_action) "
                           "VALUES (SUIVI_COMMANDES_SEQ.NEXTVAL, :id_commande, :etape, :description, SYSDATE)");
    suiviQuery.bindValue(":id_commande", idCommande);

    // Set the step based on the new state
    int etape = 0;  // Default to "Commande passée"
    QString description = "Commande modifiée";

    if (etatCommande == "En préparation") {
        etape = 1;
        description = "En préparation";
    } else if (etatCommande == "En livraison") {
        etape = 2;
        description = "En livraison";
    }

    suiviQuery.bindValue(":etape", etape);
    suiviQuery.bindValue(":description", description);

    if (!suiviQuery.exec()) {
        qDebug() << "Failed to update suivi_commandes:" << suiviQuery.lastError().text();
        return false;
    }

    qDebug() << "Commande modifiée et historique mis à jour avec succès.";
    return true;
}




// Méthode pour supprimer une commande
// Méthode pour supprimer une commande
bool Commande::supprimer(int idCommande) {
    // Vérifier que l'idCommande est valide (non nul ou négatif)
    if (idCommande <= 0) {
        qDebug() << "idCommande non valide.";
        return false;
    }

    // Suppression de la commande
    QSqlQuery query;
    query.prepare("DELETE FROM commande WHERE idCommande = :idCommande");
    query.bindValue(":idCommande", idCommande);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression de la commande:" << query.lastError().text();
        return false;
    }

    // Ajouter un suivi pour cette suppression dans la table suivi_commandes
    QSqlQuery suiviQuery;
    suiviQuery.prepare("INSERT INTO suivi_commandes (id_suivi, id_commande, etape, description, date_action) "
                           "VALUES (SUIVI_COMMANDES_SEQ.NEXTVAL, :id_commande, :etape, :description, SYSDATE)");
    suiviQuery.bindValue(":id_commande", idCommande);

    // Etape pour suppression : valeur -1 pour indiquer la suppression
    int etape = -1;  // Suppression de la commande
    QString description = "Commande supprimée";

    suiviQuery.bindValue(":etape", etape);
    suiviQuery.bindValue(":description", description);

    if (!suiviQuery.exec()) {
        qDebug() << "Erreur lors de l'ajout dans suivi_commandes:" << suiviQuery.lastError().text();
        return false;
    }

    // Après la suppression, ajouter l'historique de l'action
    int idPerso = 1;  // Remplacer par la logique appropriée pour obtenir l'ID de l'employé
    QString action = "suppression commande";

    if (!ajouterHistorique(idCommande, idPerso, action)) {
        qDebug() << "Erreur lors de l'ajout à l'historique.";
        return false;  // Vous pouvez choisir de retourner false ou true ici selon le comportement souhaité
    }

    qDebug() << "Commande supprimée, suivi supprimé et historique mis à jour avec succès.";
    return true;
}




void Commande::telechargerPDF() {
    QPdfWriter pdf("C:/Users/omark/OneDrive/Desktop/PDF/CommandesList.pdf");
    pdf.setPageSize(QPageSize(QPageSize::A3));
    pdf.setPageOrientation(QPageLayout::Portrait);
    QPainter painter(&pdf);

    // Background color
    painter.fillRect(QRect(0, 0, pdf.width(), pdf.height()), QColor("#B6E5E9"));

    // Title
    QFont titleFont("Arial", 30, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(3800, 1400, "LISTE DES COMMANDES");

    // Set up fonts for header and content
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Times New Roman", 11);
    painter.setFont(contentFont);

    // Execute query to get all orders
    QSqlQuery query;
    query.prepare("SELECT idCommande, dateCommande, etatCommande, montantTotal, dateLivraison FROM commande");
    query.exec();

    // Set up table positioning
    int initialYPosition = 2000;
    int currentYPosition = initialYPosition;
    int cellHeight = 500;
    int tableMargin = 300;

    // Define custom column widths based on expected content length
    int columnWidths[] = {1500, 2000, 1500, 1500, 2000};  // Adjust widths as needed

    // Column headers
    QStringList headers = {"ID Commande", "Date Commande", "État", "Montant Total", "Date Livraison"};

    // Draw header row
    painter.setFont(headerFont);
    painter.setPen(Qt::black);
    int xPosition = tableMargin;
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawRect(xPosition, currentYPosition, columnWidths[i], cellHeight);
        painter.drawText(xPosition + 10, currentYPosition + cellHeight / 2 + 5, headers[i]);
        xPosition += columnWidths[i];
    }
    currentYPosition += cellHeight;  // Move to the next row

    // Set alternating row colors
    QColor rowColor1(224, 247, 250);  // #E0F7FA
    QColor rowColor2(255, 255, 255);  // #FFFFFF

    // Draw each order's data in a table row
    painter.setFont(contentFont);
    bool alternate = false;
    while (query.next()) {
        xPosition = tableMargin;

        // Set row background color
        QColor rowColor = alternate ? rowColor1 : rowColor2;
        painter.fillRect(xPosition, currentYPosition, pdf.width() - 2 * tableMargin, cellHeight, rowColor);
        alternate = !alternate;

        // Draw each cell in the current row
        QStringList rowData = {
            query.value("idCommande").toString(),
            query.value("dateCommande").toDate().toString("dd/MM/yyyy"),
            query.value("etatCommande").toString(),
            query.value("montantTotal").toString(),
            query.value("dateLivraison").toDate().toString("dd/MM/yyyy")
        };

        for (int i = 0; i < rowData.size(); ++i) {
            painter.drawRect(xPosition, currentYPosition, columnWidths[i], cellHeight);

            // Draw text within the cell
            painter.drawText(xPosition + 10, currentYPosition + cellHeight / 2 + 5, rowData[i]);
            xPosition += columnWidths[i];
        }

        currentYPosition += cellHeight;  // Move down for the next row

        // Check if we need a new page
        if (currentYPosition > pdf.height() - 2000) {
            pdf.newPage();
            currentYPosition = initialYPosition;  // Reset to top of the new page

            // Re-draw header row on the new page
            xPosition = tableMargin;
            painter.setFont(headerFont);
            for (int i = 0; i < headers.size(); ++i) {
                painter.drawRect(xPosition, currentYPosition, columnWidths[i], cellHeight);
                painter.drawText(xPosition + 10, currentYPosition + cellHeight / 2 + 5, headers[i]);
                xPosition += columnWidths[i];
            }
            currentYPosition += cellHeight;  // Move to the next row after header
        }
    }

    painter.end();
}

bool Commande::ajouterHistorique(int idCommande, int idPerso, const QString& action) {
    // Vérifier que les paramètres sont valides
    if (idCommande <= 0 || idPerso <= 0 || action.isEmpty()) {
        qDebug() << "Paramètres invalides pour ajouter à l'historique.";
        return false;
    }

    // Obtenir l'heure actuelle
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timestamp = currentTime.toString("yyyy-MM-dd HH:mm:ss");

    QSqlQuery query;
    query.prepare("INSERT INTO historique_commandes (ID_Commande, ID_Perso, Action, Date_Action) "
                  "VALUES (:idCommande, :idPerso, :action, TO_DATE(:dateAction, 'YYYY-MM-DD HH24:MI:SS'))");
    query.bindValue(":idCommande", idCommande);
    query.bindValue(":idPerso", idPerso);
    query.bindValue(":action", action);
    query.bindValue(":dateAction", timestamp); // Ajout manuel du timestamp

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout à l'historique:" << query.lastError().text();
        return false;
    }

    qDebug() << "Historique ajouté avec succès :"
             << "ID_Commande:" << idCommande
             << "ID_Perso:" << idPerso
             << "Action:" << action
             << "Date_Action:" << timestamp;
    return true;
}



void Commande::loadHistorique(QListView *historiqueListView) {
    // Préparer la requête pour récupérer l'historique
    QSqlQuery query;
    if (!query.exec("SELECT * FROM historique_commandes")) {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        return;
    }

    // Créer un modèle de données pour le QListView
    QStandardItemModel *model = new QStandardItemModel();

    // Récupérer les résultats de la requête
    while (query.next()) {
        // Créer un item pour chaque ligne
        QString itemText = "ID_Commande: " + query.value("ID_Commande").toString() +
                           " | ID_Perso: " + query.value("ID_Perso").toString() +
                           " | Action: " + query.value("Action").toString() +
                           " | Date_Action: " + query.value("Date_Action").toDateTime().toString("yyyy-MM-dd HH:mm:ss");

        QStandardItem *item = new QStandardItem(itemText);
        model->appendRow(item);
    }

    // Associer le modèle au QListView
    historiqueListView->setModel(model);
}

