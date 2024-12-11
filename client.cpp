#include "client.h"
#include <QDebug>
#include <QSqlError>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QGraphicsScene>
#include <QSqlQuery>
#include <QTableWidget>
#include <QString>
#include <QSizePolicy>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QChart>
//using namespace  QtCharts;
client::client() : code_client(0), cin(0), tel(0) {}

client::client(int code_client, QString nom, QString prenom, int cin, QString adresse, QString pays, int tel, QString mail) {
    this->code_client = code_client;
    this->nom = nom;
    this->prenom = prenom;
    this->cin = cin;
    this->adresse = adresse;
    this->pays = pays;
    this->tel = tel;
    this->mail = mail;
}

bool client::ajouter() {
    QSqlQuery query;
    QString res = QString::number(code_client);
    query.prepare("INSERT INTO client (code_client, nom, prenom, cin, adresse, pays, tel, mail) "
                  "VALUES (:code_client, :nom, :prenom, :cin, :adresse, :pays, :tel, :mail)");
    query.bindValue(":code_client", res);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    QString re = QString::number(cin);
    query.bindValue(":cin", re);
    query.bindValue(":adresse", adresse);
    query.bindValue(":pays", pays);
    QString rr = QString::number(tel);
    query.bindValue(":tel", rr);
    query.bindValue(":mail", mail);
    return query.exec();
}

QSqlQueryModel* client::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM client");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("code_client"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("adresse"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("pays"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("tel"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("mail"));

    return model;
}

bool client::supprimer(int code_client) {
    QSqlQuery query;
    QString res = QString::number(code_client);
    query.prepare("DELETE FROM client WHERE code_client = :code_client");
    query.bindValue(":code_client", res);
    return query.exec();
}
bool client::modifier() {
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database connection is not open!";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE client SET nom = :nom, prenom = :prenom, cin = :cin, adresse = :adresse, pays = :pays, tel = :tel, mail = :mail WHERE code_client = :code_client");
    query.bindValue(":code_client", code_client);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":cin", cin);
    query.bindValue(":adresse", adresse);
    query.bindValue(":pays", pays);
    query.bindValue(":tel", tel);
    query.bindValue(":mail", mail);

    if (query.exec()) {
        qDebug() << "Client modified successfully";
        return true;
    } else {
        qDebug() << "Modification error: " << query.lastError().text(); // Provide more detailed error information
        return false;
    }
}
int client::getNextCodeClient() {
    QSqlQuery query;
    query.exec("SELECT MAX(code_client) FROM client");
    if (query.next()) {
        int maxCode = query.value(0).toInt();
        // Return the next available code
        return maxCode < 9999 ? maxCode + 1 : -1;
    }
    return 1; // If the table is empty, start from 1
}
bool client::exists(int code_client) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM client WHERE code_client = :code_client");
    query.bindValue(":code_client", QString::number(code_client));
    query.exec();

    if (query.next()) {
        return query.value(0).toInt() > 0; // Returns true if client exists
    }
    return false; // Return false if the query fails or client does not exist
}
bool client::existsByCin(const QString &cinStr) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM client WHERE cin = :cin");
    query.bindValue(":cin", cinStr);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    } else {
        qDebug() << "Erreur lors de la vérification du CIN:" << query.lastError().text();
        return false;
    }
}
bool client::existsByTel(const QString &telStr) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM client WHERE tel = :tel");
    query.bindValue(":tel", telStr);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    } else {
        qDebug() << "Erreur lors de la vérification du numéro de téléphone:" << query.lastError().text();
        return false;
    }
}
QSqlQueryModel* client::rechercher(QString rech) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString recher = "select * from client where nom like '%" + rech + "%' or prenom like '%" + rech + "%' or cin like '%" + rech + "%' or adresse like '%" + rech + "%' or pays like '%" + rech + "%' or code_client like '%" + rech + "%' or tel like '%" + rech + "%' or mail like '%" + rech + "%'";
    model->setQuery(recher);
    return model;
}
QSqlQueryModel * client::tri(const QString &colonne, const QString &ordre)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query = QString("SELECT * FROM client ORDER BY %1 %2").arg(colonne, ordre);
    model->setQuery(query);
    return model;
}
/*void client::displayRegionStatistics(QGraphicsView *pieView) {
    int europe = 0, asie = 0, amerique = 0;

    // Querying the database and retrieving based on 'pays' column.
    QSqlQuery query;

    // Count clients from Europe (Tunisie, France)
    if (query.exec("SELECT COUNT(*) FROM client WHERE pays IN ('Tunisie', 'France')")) {
        if (query.next()) {
            europe = query.value(0).toInt(); // Retrieving count directly
        }
    } else {
        qDebug() << "Erreur lors de la récupération des données pour l'Europe:" << query.lastError().text();
    }

    // Count clients from Asia (Espagne, Allemagne, Italie)
    if (query.exec("SELECT COUNT(*) FROM client WHERE pays IN ('Espagne', 'Allemagne', 'Italie')")) {
        if (query.next()) {
            asie = query.value(0).toInt(); // Retrieving count directly
        }
    } else {
        qDebug() << "Erreur lors de la récupération des données pour l'Asie:" << query.lastError().text();
    }

    // Count clients from America (US, UK, Canada, Australia, Emirates)
    if (query.exec("SELECT COUNT(*) FROM client WHERE pays IN ('US', 'UK', 'Canada', 'Australia', 'Emirates')")) {
        if (query.next()) {
            amerique = query.value(0).toInt(); // Retrieving count directly
        }
    } else {
        qDebug() << "Erreur lors de la récupération des données pour l'Amérique:" << query.lastError().text();
    }

    // Proceed to create the chart as before.
    QPieSeries *series = new QPieSeries();
    if (europe > 0) series->append("Europe", europe);
    if (asie > 0) series->append("Asie", asie);
    if (amerique > 0) series->append("Amérique", amerique);
    if (series->slices().isEmpty()) {
        series->append("Aucune donnée", 1);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribution des clients par région");

    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set up the chart view in the provided pie view widget
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addWidget(chartView);
    pieView->setScene(scene);
    pieView->show();
}*/

void client::displayRegionStatistics(QGraphicsView *pieView) {
    QPieSeries *series = new QPieSeries();
    // Add your data to series here...
    QChart *chart = new QChart();
    chart->addSeries(series);

    // Display chart in QGraphicsView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    pieView->setScene(chartView->scene());
}
/*void client::displayCityStatistics(QGraphicsView *barView) {
    QMap<QString, int> cityCounts;

    QSqlQuery query;

    // Query to retrieve cities (addresses) and count clients in each city
    if (query.exec("SELECT adresse FROM client")) {
        while (query.next()) {
            QString city = query.value(0).toString();  // The address (city)
            cityCounts[city]++;  // Increment the counter for this city
        }
    } else {
        qDebug() << "Error retrieving data for cities:" << query.lastError().text();
        return;
    }

    // Create QBarSet and populate it with city data
    QBarSet *barSet = new QBarSet("Number of Clients");
    QStringList categories;
    for (auto it = cityCounts.constBegin(); it != cityCounts.constEnd(); ++it) {
        categories << it.key();  // Add city names to categories
        *barSet << it.value();   // Add city counts to bar set
    }

    // Create a bar series and add the bar set to it
    QBarSeries *series = new QBarSeries();
    series->append(barSet);

    // Create the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Client Distribution by City");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Customize the axis
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Number of Clients");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create the chart view and display it in QGraphicsView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(barView->size());
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Display the chart in the QGraphicsView widget
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addWidget(chartView);
    barView->setScene(scene);
    barView->show();
}*/
void client::displayCityStatistics(QTableWidget *tableView) {
    QMap<QString, int> cityCounts;
    QSqlQuery query;

    // Query to retrieve cities (addresses) and count clients in each city
    if (query.exec("SELECT adresse FROM client")) {
        while (query.next()) {
            QString city = query.value(0).toString();  // The address (city)
            cityCounts[city]++;  // Increment the counter for this city
        }
    } else {
        qDebug() << "Error retrieving data for cities:" << query.lastError().text();
        return;
    }

    // Set up table headers
    tableView->setRowCount(cityCounts.size());
    tableView->setColumnCount(2);
    tableView->setHorizontalHeaderLabels(QStringList() << "City" << "Number of Clients");

    // Populate table with city data
    int row = 0;
    for (auto it = cityCounts.constBegin(); it != cityCounts.constEnd(); ++it) {
        QTableWidgetItem *cityItem = new QTableWidgetItem(it.key());
        QTableWidgetItem *countItem = new QTableWidgetItem(QString::number(it.value()));
        tableView->setItem(row, 0, cityItem);
        tableView->setItem(row, 1, countItem);
        row++;
    }

    // Optional: Adjust table view properties for better appearance
    tableView->resizeColumnsToContents();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}
/*void client::displayEmailDomainStatistics(QTableWidget *tableWidget) {
    QMap<QString, int> domainCounts;
    QSqlQuery query;

    // Query to retrieve email addresses
    if (query.exec("SELECT mail FROM client")) {
        while (query.next()) {
            QString email = query.value(0).toString();
            QString domain = email.section('@', -1); // Extract the domain part
            domainCounts[domain]++;
        }
    } else {
        qDebug() << "Error retrieving email data:" << query.lastError().text();
        return;
    }

    // Set up the table widget for displaying results
    tableWidget->clear();
    tableWidget->setRowCount(domainCounts.size());
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels({"Email Domain", "Client Count"});

    int row = 0;
    for (auto it = domainCounts.constBegin(); it != domainCounts.constEnd(); ++it) {
        // Add each domain and count to the table
        QTableWidgetItem *domainItem = new QTableWidgetItem(it.key());
        QTableWidgetItem *countItem = new QTableWidgetItem(QString::number(it.value()));

        tableWidget->setItem(row, 0, domainItem);
        tableWidget->setItem(row, 1, countItem);

        row++;
    }

    // Customize the table view appearance
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    tableWidget->setSelectionMode(QTableWidget::NoSelection);
    tableWidget->setStyleSheet("QTableWidget { border: none; }");
}
*/
void client::displayEmailDomainStatistics(QGraphicsView *domainBarView) {
    QMap<QString, int> domainCounts;
    QSqlQuery query;

    // Query to retrieve email addresses and parse domains
    if (query.exec("SELECT mail FROM client")) {
        while (query.next()) {
            QString email = query.value(0).toString();
            QString domain = email.section('@', -1); // Extract domain part after '@'
            domain = domain.section('.', 0, 0); // Extract the part before the first '.' (service provider)

            // Increment the count for this domain
            domainCounts[domain]++;
        }
    } else {
        qDebug() << "Erreur lors de la récupération des emails:" << query.lastError().text();
        return;
    }

    // Create QBarSet and populate it with domain data
    QBarSet *barSet = new QBarSet("Nombre de clients");
    QStringList categories;
    for (auto it = domainCounts.constBegin(); it != domainCounts.constEnd(); ++it) {
        categories << it.key();  // Add domain names to categories
        *barSet << it.value();   // Add client counts to bar set
    }

    // Create a bar series and add the bar set to it
    QBarSeries *series = new QBarSeries();
    series->append(barSet);

    // Create the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Fréquence des domaines d'email parmi les clients");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Customize the X-axis for domains
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Customize the Y-axis to show integer values
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de clients");
    axisY->setLabelFormat("%i");  // Ensure the labels are displayed as integers
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create the chart view and display it in QGraphicsView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(domainBarView->size());
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Display the chart in the QGraphicsView widget
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addWidget(chartView);
    domainBarView->setScene(scene);
    domainBarView->show();
}
