#include "perso.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QVariant>
#include <QPdfWriter>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QColor>
#include <QPrinter>
#include <QCalendarWidget>
#include <QDate>
#include <QBrush>
#include <QColor>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QSqlRecord>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
perso::perso(){
    id_p=0;
    nom="";
    prenom="";
    email="";
    mdp="";
    poste="";
    sexe="";
    salaire=0.0;
    date_n = QDate();



}
perso::perso(int id_p, QString nom, QString prenom, QString email, QString poste, QString mdp, QString sexe, float salaire, QDate date_n)
{
    this->id_p = id_p;
    this->nom = nom;
    this->prenom = prenom;
    this->email =email;
    this->poste =poste;
    this->mdp=mdp;
    this->sexe=sexe;
    this->salaire=salaire;
    this->date_n=date_n;



}
int perso::generateIdentifiant() {
    int identifiant = 0;

    // Generate 4 random numbers and build the integer
    for (int i = 0; i < 4; i++) {
        int num = QRandomGenerator::global()->bounded(10); // Random number between 0 and 9
        identifiant = identifiant * 10 + num; // Shift left by one digit and add the random number
    }

    return identifiant; // Return the generated integer
}

QString perso::generateNumericPassword() {
    QString password;
    for (int i = 0; i < 8; ++i) {
        int digit = rand() % 10; //
        password.append(QString::number(digit));
    }
    return password;
}


bool perso::ajout() {
    int id_p = generateIdentifiant(); // Generate the identifier
    QString mdp = generateNumericPassword(); // Generate a numeric password

    QSqlQuery query;
    query.prepare("INSERT INTO personnel (id_p, nom, prenom, email, date_n, salaire, poste, mdp, sexe) "
                  "VALUES (:id_p, :nom, :prenom, :email, :date_n, :salaire, :poste, :mdp, :sexe)");

    // Bind values to placeholders
    query.bindValue(":id_p", id_p);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":mdp", mdp);
    query.bindValue(":sexe", sexe);
    query.bindValue(":date_n", date_n.toString("yyyy-MM-dd"));
    query.bindValue(":salaire", QVariant::fromValue(salaire));

    // Display all attributes in qDebug for verification
    qDebug() << "Adding Employee with the following details:";
    qDebug() << "ID:" << id_p;
    qDebug() << "Name:" << nom;
    qDebug() << "Surname:" << prenom;
    qDebug() << "Email:" << email;
    qDebug() << "Position:" << poste;
    qDebug() << "Password:" << mdp;
    qDebug() << "Gender:" << sexe;
    qDebug() << "Hire Date:" << date_n.toString("yyyy-MM-dd");
    qDebug() << "Salary:" << salaire;

    // Execute the query and check for success
    if (!query.exec()) {
        qDebug() << "Error adding employee:" << query.lastError().text();
        return false;
    }

    return true;
}

bool perso::idExists(QString id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM personnel WHERE id_p = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.next()) {
            // Return true if the count is greater than 0, meaning the ID exists
            return query.value(0).toInt() > 0;
        }
    } else {
        qDebug() << "ID existence check query error:" << query.lastError().text();
    }

    // Return false if the query fails or the ID does not exist
    return false;
}

bool perso::supprimer(QString id_p) {
    if (!idExists(id_p)) {
        qDebug() << "ID does not exist, cannot delete.";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM personnel WHERE id_p = :id_p");
    query.bindValue(":id_p", id_p); // Bind the value correctly

    if (!query.exec()) {
        qDebug() << "Error deleting record:" << query.lastError().text();
        return false;
    }

    qDebug() << "Record with ID" << id_p << "deleted successfully.";
    return true;
}

QSqlQueryModel *perso::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * from personnel");


    // Set header data for each column correctly
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_Personnel"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("date d'emboche"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("mdp"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("sexe"));



    return model;
}

bool perso::modifier(QString nom, QString prenom, QString sexe, QString email, float salaire, QString poste, QDate date_n, QString id)
{
    QStringList updateFields;

    // Dynamically construct the fields to be updated
    if (!nom.isEmpty()) {
        updateFields << "nom = :nom";
    }
    if (!prenom.isEmpty()) {
        updateFields << "prenom = :prenom";
    }
    if (!sexe.isEmpty()) {
        updateFields << "sexe = :sexe";
    }
    if (!email.isEmpty()) {
        updateFields << "email = :email";
    }
    if (salaire > 0) {
        updateFields << "salaire = :salaire";
    }
    if (!poste.isEmpty()) {
        updateFields << "poste = :poste";
    }
    if (date_n.isValid()) {
        updateFields << "date_n = :date_n";  // Assuming "date_n" is the correct column name
    }

    // Check if there are fields to update
    if (updateFields.isEmpty()) {
        qDebug() << "No fields to update.";
        return false; // Nothing to update
    }

    // Join all fields with a comma to form the final query
    QString queryString = "UPDATE personnel SET " + updateFields.join(", ") + " WHERE id_p = :id";

    // Prepare the query
    QSqlQuery query;
    query.prepare(queryString);

    // Bind only the non-empty values
    if (!nom.isEmpty()) query.bindValue(":nom", nom);
    if (!prenom.isEmpty()) query.bindValue(":prenom", prenom);
    if (!sexe.isEmpty()) query.bindValue(":sexe", sexe);
    if (!email.isEmpty()) query.bindValue(":email", email);
    if (salaire > 0) query.bindValue(":salaire", salaire);
    if (!poste.isEmpty()) query.bindValue(":poste", poste);
    if (date_n.isValid()) query.bindValue(":date_n", date_n.toString("yyyy-MM-dd"));
    query.bindValue(":id",
                    id); // Bind 'id' as the employee identifier

    // Execute and check for success
    if (!query.exec()) {
        qDebug() << "Update failed: " << query.lastError().text();
        qDebug() << "Executed query:" << queryString;
        return false;
    }

    return true;
}

QSqlQueryModel* perso::rechercher(QString id) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM personnel WHERE id_p = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        model->setQuery(std::move(query));
    } else {
        qDebug() << "Search query error:" << query.lastError();
    }

    return model;
}







QSqlQueryModel* perso::tri(int index)
{
    QString query;

    // Déterminer la requête SQL en fonction de l'index sélectionné
    switch (index) {
        case 0: // ID Croissant
            query = "SELECT * FROM personnel ORDER BY id_p ASC";
            break;
        case 1: // ID Décroissant
            query = "SELECT * FROM personnel ORDER BY id_p DESC";
            break;
        case 2: // Salaire Croissant
            query = "SELECT * FROM personnel ORDER BY salaire ASC";
            break;
        case 3: // Salaire Décroissant
            query = "SELECT * FROM personnel ORDER BY salaire DESC";
            break;
        case 4: // Date d'embauche Croissant
            query = "SELECT * FROM personnel ORDER BY TO_DATE(date_n, 'DD-MM-YYYY') ASC";
            break;
        case 5: // Date d'embauche Décroissant
            query = "SELECT * FROM personnel ORDER BY TO_DATE(date_n, 'DD-MM-YYYY') DESC";
            break;
        default:
            query = "SELECT * FROM personnel";
            break;
    }

    // Créez un modèle de requête et exécutez la requête
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    // Si la requête échoue, afficher une erreur
    if (model->lastError().isValid()) {
        qDebug() << "Erreur de requête: " << model->lastError().text();
    }

    return model;
}




bool perso::exporterPDF(int id_p, const QString &filePath)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM personnel WHERE id_p = :id_p");
    query.bindValue(":id_p", id_p);

    if (!query.exec() || !query.next()) {
        return false; // Échec si les données ne sont pas trouvées
    }

    // Récupérer les données du personnel
    QString nom = query.value("nom").toString();
    QString prenom = query.value("prenom").toString();
    QString email = query.value("email").toString();
    QString date_n = query.value("date_n").toString();
    QString salaire = query.value("salaire").toString();
    QString poste = query.value("poste").toString();
    QString sexe = query.value("sexe").toString();

    // Configurer l'impression
    QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filePath);
    // Créer le contenu du PDF
    QTextDocument document;
    QString pdfContent = QString(
        "<h1 style='color:blue;'>Informations du Personnel</h1>"
        "<p><strong>ID :</strong> %1</p>"
        "<p><strong>Nom :</strong> %2</p>"
        "<p><strong>Prénom :</strong> %3</p>"
        "<p><strong>Email :</strong> %4</p>"
        "<p><strong>Date d'embauche :</strong> %5</p>"
        "<p><strong>Salaire :</strong> %6</p>"
        "<p><strong>Poste :</strong> %7</p>"
        "<p><strong>Sexe :</strong> %8</p>"
    ).arg(id_p).arg(nom).arg(prenom).arg(email).arg(date_n).arg(salaire).arg(poste).arg(sexe);

    document.setHtml(pdfContent);

    // Générer le PDF
    document.print(&printer);

    return true; // Succès
}






bool perso::getemployeemailpassword(const QString &id_p, const QString &mdp)
{
    QSqlQuery query;

    // Prepare the SQL query to select email and password based on login and password
    query.prepare("SELECT COUNT(*) FROM personnel WHERE id_p = :id_p AND MDP = :mdp;");
    query.bindValue(":id_p", id_p);
    query.bindValue(":mdp", mdp);

    // Execute the query
    if (query.exec()) {
        // If the query succeeds, check if any record is found
        if (query.next() && query.value(0).toInt() > 0) {
            return true;  // Successfully retrieved email and password
        } else {
            return false;  // No matching record found
        }
    } else {
        // If the query fails, display the error
        qDebug() << "Query failed:" << query.lastError().text();
        return false;  // Query execution failed
    }
}


bool perso::getEmployeeDetailsById(const QString &id_p, QString &nom, QString &prenom, QString &email, QString &poste)
{
    // Vérifiez que la connexion à la base de données est valide
    QSqlQuery query;

    // Préparer la requête SQL pour récupérer les détails de l'employé
    query.prepare("SELECT NOM, PRENOM, EMAIL, POSTE FROM personnel WHERE ID_P = :id_p");

    // Lier le paramètre :id_emp
    query.bindValue(":id_p", id_p);

    // Exécuter la requête
    if (query.exec()) {
        if (query.next()) {
            // Si un résultat est trouvé, affectez les valeurs aux références passées
            nom = query.value("NOM").toString();       // Correspond à la colonne NOM
            prenom = query.value("PRENOM").toString(); // Correspond à la colonne PRENOM
            email = query.value("EMAIL").toString();   // Correspond à la colonne EMAIL
            poste = query.value("POSTE").toString();   // Correspond à la colonne POSTE

            return true; // Succès
        } else {
            // Aucun employé trouvé pour l'ID donné
            qDebug() << "Aucun employé trouvé avec l'ID :" << id_p;
            return false;
        }
    } else {
        // La requête a échoué
        qDebug() << "Échec de la requête :" << query.lastError().text();
        return false;
    }
}
int perso::generateId() {
    // Générer un nombre aléatoire à 6 chiffres entre 100000 et 999999
    int randomId = QRandomGenerator::global()->bounded(100000, 1000000);
    return randomId; // Retourner l'identifiant généré
}



// Fonction pour ajouter un congé et colorier les dates dans le calendrier
bool perso::ajouterconge(int id_p, const QString &nom, const QString &prenom,
                         const QString &email, const QString &raison, const QString &poste,
                         const QDate &date_d, const QDate &date_f) {
    int id_conge = generateId(); // Générer un ID unique pour le congé
    QString conseilMessage = generateLeaveDecision(); // Générer une consigne pour le congé

    // Créer une instance de QSqlQuery
    QSqlQuery query;

    // Préparer la requête SQL pour insérer un congé
    query.prepare("INSERT INTO conge (ID_CONGE, ID_P, NOM, PRENOM, EMAIL, RAISON, POSTE, DATE_D, DATE_F, CONSIGNE, TYPE_CONGE, ETAT) "
                  "VALUES (:id_conge, :id_p, :nom, :prenom, :email, :raison, :poste, :date_d, :date_f, :consigne, 'Congé annuel', 'En attente')");

    // Lier les valeurs aux placeholders
    query.bindValue(":id_conge", id_conge);
    query.bindValue(":id_p", id_p);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":raison", raison);
    query.bindValue(":poste", poste);
    query.bindValue(":date_d", date_d);
    query.bindValue(":date_f", date_f);
    query.bindValue(":consigne", conseilMessage);

    // Exécuter la requête
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du congé :" << query.lastError().text();
        return false; // Échec de l'insertion
    }


    return true; // Insertion réussie
}
/*
void perso::colorerDatesConges(QCalendarWidget* calendarWidget, QDate date_d, QDate date_f) {
    if (calendarWidget) {
        QTextCharFormat format;
        format.setBackground(Qt::yellow); // Change la couleur de fond des dates (ici en jaune)

        // Parcourir les dates de la plage
        for (QDate date = date_d; date <= date_f; date = date.addDays(1)) {
            calendarWidget->setDateTextFormat(date, format); // Appliquer le format à chaque date
        }

        qDebug() << "Les dates ont été colorées!";
    } else {
        qDebug() << "Le calendrier n'est pas disponible!";
    }
}

*/


QString perso::generateLeaveDecision() {
    // Variables pour stocker le nombre d'employés totaux et d'employés non en congé
    int totalEmployees = 0;
    int employeesNotOnLeave = 0;

    // Requête pour obtenir le nombre total d'employés
    QSqlQuery query;

    // Compter le nombre total d'employés dans PERSONNELS
    query.prepare("SELECT COUNT(*) FROM PERSONNEL");
    if (query.exec() && query.next()) {
        totalEmployees = query.value(0).toInt();
    } else {
        qDebug() << "Erreur lors de la récupération du nombre d'employés:" << query.lastError().text();
        return "Erreur de calcul des employés.";
    }
    qDebug() << "totaleemploye:" <<totalEmployees;

    // Compter les employés qui ne sont pas en congé dans CONGE
    // Supposons que ETAT est le champ dans la table CONGE qui indique l'état du congé (ex. 'Non' pour non en congé)
    query.prepare("SELECT COUNT(*) FROM CONGE WHERE ETAT != 'En congé'"); // Modifiez cette condition selon vos données
    if (query.exec() && query.next()) {
        employeesNotOnLeave = query.value(0).toInt();
    } else {
        qDebug() << "Erreur lors de la récupération des employés non en congé:" << query.lastError().text();
        return "Erreur de calcul des employés non en congé.";
    }
    qDebug() << "employe on leave:" << employeesNotOnLeave;

    // Calcul du pourcentage d'employés non en congé
    if (totalEmployees == 0) {
        return "Aucun employé disponible pour calculer.";
    }

    float percentageNotOnLeave = (float)employeesNotOnLeave / totalEmployees * 100;
    float percentagework= 100-(percentageNotOnLeave);
    qDebug() << "percentage:" << percentageNotOnLeave;
    qDebug() << "percetagework:" << percentagework;

    // Décision selon le seuil de 70%
    if (percentagework >= 70) {
        return "Tu peux accepter le congé.";
    } else {
        return "N'accepte pas le congé.";
    }
}


QSqlQueryModel* perso::afficherconge() {
    // Création d'une nouvelle instance de QSqlQueryModel
    QSqlQueryModel* model = new QSqlQueryModel();

    // Requête SQL pour récupérer les congés en attente
    QString queryText = R"(
        SELECT ID_CONGE, NOM, PRENOM, EMAIL, POSTE, RAISON, DATE_D, DATE_F, TYPE_CONGE, CONSIGNE, ETAT
        FROM conge
        WHERE ETAT = 'En attente'
    )";

    // Exécution de la requête
    model->setQuery(queryText);

    // Vérification des erreurs de la requête
    if (model->lastError().isValid()) {
        qDebug() << "Erreur d'exécution de la requête SQL :" << model->lastError().text();
        delete model;  // Libération mémoire
        return nullptr;  // Retourner un pointeur nul pour signaler une erreur
    }

    // Configuration des en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Congé"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Raison"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Type Congé"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Consigne"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("État"));

    // Retourne le modèle rempli pour l'affichage
    return model;
}


bool perso::hasOngoingLeave(int id_emp) {
    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM conge WHERE id_p = :id_emp AND (etat = 'accepter' or etat ='En attente' ");
    query.bindValue(":id_emp", id_emp);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification du congé en cours :" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        qDebug() << "Nombre de congés en cours pour l'employé" << id_emp << ":" << count;
        return count > 0;
    }

    qDebug() << "Impossible de récupérer les résultats de la requête pour l'employé" << id_emp;
    return false;
}



bool perso::updateCongeStatus(const QString &id_conge, const QString &etat)
{
    // Prepare the SQL query to update only the 'etat' column
    QSqlQuery query;
    query.prepare("UPDATE conge SET etat = :etat WHERE ID_CONGE = :id_conge");

    // Bind the values to the query
    query.bindValue(":etat", etat);
    query.bindValue(":id_conge", id_conge);

    // Log the query for debugging
    qDebug() << "Executing query: UPDATE conge SET etat = " << etat
             << " WHERE ID_CONGE = " << id_conge;

    // Execute the query and check for success
    if (query.exec()) {
        qDebug() << "Record updated successfully.";
        return true;
    } else {
        qDebug() << "Error updating record: " << query.lastError().text();
        return false;
    }
}

void perso::updateTypeConge() {
    // Get today's date in the format YYYY-MM-DD
    QString today = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query;

    // Step 1: Update `type_conge` to 'fin' for records in the `conge` table where DATE_F matches today
    query.prepare("UPDATE conge "
                  "SET etat = 'fin' "
                  "WHERE DATE_F = TO_DATE(:today, 'YYYY-MM-DD')");
    query.bindValue(":today", today);

    if (!query.exec()) {
        qDebug() << "Error updating type_conge:" << query.lastError().text();
        return;
    }
    qDebug() << "type_conge updated successfully for records with DATE_F = " << today;

}



QString perso::getPosteById(int id_p) {
    // Créer une instance de QSqlQuery
    QSqlQuery query;

    // Préparer la requête SQL pour obtenir le poste de l'employé
    query.prepare("SELECT POSTE FROM PERSONNEL WHERE ID_P = :id_p");

    // Lier la valeur de l'ID employé au paramètre de la requête
    query.bindValue(":id_p", id_p);

    // Exécuter la requête
    if (query.exec()) {
        // Vérifier si la requête retourne un résultat
        if (query.next()) {
            // Retourner le poste de l'employé
            return query.value("POSTE").toString();
        } else {
            qDebug() << "Aucun employé trouvé avec l'ID:" << id_p;
            return ""; // Aucun employé trouvé
        }
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();
        return ""; // Erreur lors de la requête
    }
}


