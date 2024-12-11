#include "piece.h"
#include <QSqlQuery>
#include <QSqlQueryModel>

#include <QVariant>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QFile>
#include <QTextDocument>
#include <QDate>
#include <QSqlError>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSlice>
#include <QPieSeries>
#include <QDebug>
piece::piece() {}

piece::piece(int id, int reff, int quant, int prix, const QString &etat, const QString &image)
    : id(id), reff(reff), quant(quant), prix(prix), etat(etat), image(image) {}

bool piece::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO piece (id, reff, quant, prix, etat, image) "
                  "VALUES (:id, :reff, :quant, :prix, :etat, :image)");
    query.bindValue(":id", id);
    query.bindValue(":reff", reff);
    query.bindValue(":quant", quant);
    query.bindValue(":prix", prix);
    query.bindValue(":etat", etat);
    query.bindValue(":image", image);

    return query.exec();
}

bool piece::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE PIECE SET REFF=:reff, QUANT=:quant, PRIX=:prix WHERE ID=:id");
    query.bindValue(":id", id);
    query.bindValue(":reff", reff);
    query.bindValue(":quant", quant);
    query.bindValue(":prix", prix);

    return query.exec();
}



QSqlQueryModel* piece::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PIECE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Reference"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Quantity"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Etat"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Image (Base64)"));



    return model;
}

bool piece::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PIECE WHERE ID=:id");
    query.bindValue(":id", id);

    return query.exec();
}



// Recherche par critère
QSqlQueryModel* piece::rechercher(QString critere, QString valeur) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = QString("SELECT * FROM piece WHERE %1 LIKE '%%2%'").arg(critere, valeur);
    model->setQuery(queryStr); // envoye requete
    return model;
}

// Trier par critère
QSqlQueryModel* piece::trier(QString critere) {
    QSqlQueryModel* model = new QSqlQueryModel();//pour afficher les résultats d'une requête SQL


    QString queryStr = QString("SELECT * FROM piece ORDER BY %1").arg(critere);
    model->setQuery(queryStr);
    return model;//renvoie un modèle
}
// Générer le modèle HTML pour le PDF
QString piece::genererContenuPDF() {
    // Définir un modèle HTML avec du CSS pour la mise en page
    QString html = R"(
    <html>
    <head>
        <style>
            body {
                font-family: Arial, sans-serif;
                margin: 0;
                padding: 0;
            }
            h1 {
                text-align: center;
                color: #333;
                margin-bottom: 20px;
            }
            table {
                width: 100%;
                border-collapse: collapse;
                margin: 0;
            }
            th, td {
                border: 1px solid #dddddd;
                padding: 10px;
                text-align: left;
                font-size: 12px;
            }
            th {
                background-color: #f2f2f2;
                font-weight: bold;
            }
            .footer {
                text-align: center;
                font-size: 12px;
                color: #666;
                margin-top: 30px;
            }
        </style>
    </head>
    <body>
        <h1>Rapport des Pieces</h1>
        <table>
            <tr>
                <th>ID piece</th>
                <th>Reference</th>
                <th>quantite</th>
                <th>prix</th>
                <th>etat</th>
            </tr>
    )";

    // Ajouter les pieces à la table dans la boucle
    QSqlQuery query("SELECT * FROM piece");
    while (query.next()) {
        int id = query.value("id").toInt();
        int reff = query.value("reff").toInt();
        int quantite = query.value("quant").toInt();
        int prix = query.value("prix").toInt();
        QString etat = query.value("etat").toString();


        html += QString(
                    "<tr>"
                    "<td>%1</td>"
                    "<td>%2</td>"
                    "<td>%3</td>"
                    "<td>%4</td>"
                    "<td>%5</td>"

                    "</tr>"
                    )
                    .arg(id)
                    .arg(reff)
                    .arg(quantite)
                    .arg(prix)
                    .arg(etat);

    }

    html += QString(R"(
        </table>
        <div class='footer'>
            <p>Ce rapport a été généré automatiquement le %1.</p>
        </div>
    </body>
    </html>
    )").arg(QDate::currentDate().toString("dd/MM/yyyy"));

    return html;
}

// Gérer la génération du PDF
void piece::genererPDF(const QString& fichierPDF) {
    QString contenuHTML = genererContenuPDF(); // Récupérer le contenu HTML généré

    QTextDocument document;
    document.setHtml(contenuHTML);

    // Configurer le QPrinter pour créer un PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fichierPDF);

    // Définir les marges de la page
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    // Imprimer le document dans le fichier PDF
    document.print(&printer);
}
QPieSeries* piece::statistiques() {
    QPieSeries* series = new QPieSeries();

    // Requête SQL pour compter les piece par etat
    QSqlQuery query;
    query.prepare("SELECT etat, COUNT(*) FROM piece GROUP BY etat");

    if (query.exec()) {
        while (query.next()) {
            QString etat = query.value(0).toString();
            int count = query.value(1).toInt();
            series->append(etat, count);
        }
    } else {
        qDebug() << "Erreur lors de la récupération des statistiques par etat : " << query.lastError().text();
    }

    return series;
}
QString piece::getImageById(int id) {
    QSqlQuery query;
    query.prepare("SELECT image FROM piece WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return query.value(0).toString(); // Retourne la chaîne Base64
    }

    return QString(); // Retourne une chaîne vide si l'ID est introuvable
}


