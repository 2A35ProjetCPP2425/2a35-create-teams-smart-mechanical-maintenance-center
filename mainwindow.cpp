#include "mainwindow.h"
#include "qscatterseries.h"
#include "ui_mainwindow.h"
#include "commande.h"  // Assurez-vous d'inclure la classe Commande
#include <QMessageBox> // Pour afficher des messages
#include <QSqlQueryModel> // Pour afficher les données
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QSqlError>
#include <QtCharts/QLineSeries>
#include <QDateTime>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    proxyModel(new QSortFilterProxyModel(this)),  // Initialize proxy model
    scene(new QGraphicsScene(this)), timelineScene(new QGraphicsScene(this))
{
    ui->setupUi(this);
        ui->graphicsView_2->setScene(timelineScene);
        ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
        ui->graphicsView_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Initialize and set the proxy model
    Commande commande; // Create an instance of Commande to fetch data
    QSqlQueryModel* model = commande.afficher(); // Fetch the model from the database
    proxyModel->setSourceModel(model); // Set the source model for the proxy
    ui->tableView->setModel(proxyModel); // Attach the proxy model to the table view

    // Connect the lineEdit for dynamic searching
    connect(ui->lineEditRecherche, &QLineEdit::textChanged, this, [this](const QString &text) {
        proxyModel->setFilterKeyColumn(-1); // Enable search across all columns
        proxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption)); // Use case-insensitive search
    });

    // Connect other buttons to their respective slots
    connect(ui->ajouterButton, &QPushButton::clicked, this, &MainWindow::on_ajouterButton_clicked);
    connect(ui->supprimerButton, &QPushButton::clicked, this, &MainWindow::on_supprimerButton_clicked);
    connect(ui->modifierButton, &QPushButton::clicked, this, &MainWindow::on_modifierButton_clicked);
    connect(ui->trierButton, &QPushButton::clicked, this, &MainWindow::on_trierButton_clicked);
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &MainWindow::on_exportPdfButton_clicked);
    connect(ui->buttonOrdersPerMonth, &QPushButton::clicked, this, &MainWindow::displayOrdersPerMonthStatistics);
    connect(ui->buttonOrdersStatus, &QPushButton::clicked, this, &MainWindow::displayOrderStatusStatistics);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::on_loadHistoriqueButton_clicked);
    connect(ui->suiviButton, &QPushButton::clicked, this, &MainWindow::onClickSuivi);


}

MainWindow::~MainWindow() {
    delete ui;
}



void MainWindow::reinitialiserChamps() {
    ui->lineEdit_2->clear();      // Réinitialise l'état de la commande
    ui->lineEdit_3->clear();      // Réinitialise le montant total
    ui->lineEdit_6->clear();      // Réinitialise l'ID de commande pour suppression/modification
    ui->dateEdit->clear();        // Réinitialise la date de commande
    ui->dateEdit_2->clear();      // Réinitialise la date de livraison
}

// Fonction pour ajouter une commande
void MainWindow::on_ajouterButton_clicked() {
    QDate dateCommande = ui->dateEdit->date();
    QString etatCommande = ui->lineEdit_2->text();

    // Contrôle du montant total
    bool montantOk;
    int montantTotal = ui->lineEdit_3->text().toInt(&montantOk);
    QDate dateLivraison = ui->dateEdit_2->date();

    // Liste des états valides
        QStringList etatsValides = {"Commande passee", "En cours", "En livraison", "Livree"};

        // Validation de l'état de commande
        if (!etatsValides.contains(etatCommande, Qt::CaseInsensitive)) {
            QMessageBox::warning(this, "Erreur de saisie",
                                 QString("L'état de la commande est invalide. Les états valides sont : %1.")
                                 .arg(etatsValides.join(", ")));
            return;
        }


    // Validation du montant total
    if (!montantOk || montantTotal <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le montant total est vide ou invalide !");
        return;
    }

    // Vérification des dates
    if (!dateCommande.isValid()) {
        QMessageBox::warning(this, "Erreur de saisie", "La date de commande est invalide.");
        return;
    }
    if (!dateLivraison.isValid()) {
        QMessageBox::warning(this, "Erreur de saisie", "La date de livraison est invalide.");
        return;
    }
    if (dateLivraison < dateCommande) {
        QMessageBox::warning(this, "Erreur de saisie", "La date de livraison ne peut pas être antérieure à la date de commande.");
        return;
    }

    // Si toutes les validations sont réussies
    QMessageBox::information(this, "Succès", "Les informations de la commande sont valides.");

    // Procéder à l'ajout de la commande
    Commande commande(dateCommande, etatCommande, montantTotal, dateLivraison);

    if (commande.ajouter()) {
        QMessageBox::information(this, "Ajout réussi", "La commande a été ajoutée avec succès.");
        ui->tableView->setModel(commande.afficher()); // Actualiser l'affichage des commandes
        reinitialiserChamps();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la commande.");
    }
    updateModel();

}

// Fonction pour supprimer une commande
void MainWindow::on_supprimerButton_clicked() {
    int idCommande = ui->lineEdit_6->text().toInt(); // Récupérer l'ID de commande à supprimer

    Commande commande; // Utilise le constructeur par défaut

    // Appeler la fonction supprimer
    if (commande.supprimer(idCommande)) {
        QMessageBox::information(this, "Suppression réussie", "La commande a été supprimée avec succès.");
        ui->tableView->setModel(commande.afficher()); // Actualiser l'affichage des commandes
        reinitialiserChamps();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de la commande.");
    }
    updateModel();

}
void MainWindow::on_modifierButton_clicked() {
    // Récupérer l'ID de commande
    bool idOk;
    int idCommande = ui->lineEdit_6->text().toInt(&idOk);

    if (!idOk || idCommande <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID de commande est invalide (il doit être un entier positif).");
        return;
    }

    // Récupérer les autres champs
    QString etatCommande = ui->lineEdit_2->text();
    bool montantOk;
    double montantTotal = ui->lineEdit_3->text().toDouble(&montantOk);

    QDate dateCommande = ui->dateEdit->date();
    QDate dateLivraison = ui->dateEdit_2->date();

    // Liste des états valides
    QStringList etatsValides = {"Commande passee", "En cours", "En livraison", "Livree"};

    // Validation de l'état de commande
    if (!etatsValides.contains(etatCommande, Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             QString("L'état de la commande est invalide. Les états valides sont : %1.")
                             .arg(etatsValides.join(", ")));
        return;
    }

    // Validation du montant total
    if ( montantTotal < 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le montant total est vide ou invalide (il doit être un nombre positif).");
        return;
    }

    // Vérifier si l'ID existe dans la base
    Commande commande;
    commande.setIdCommande(idCommande);

    if (!commande.idExists(idCommande)) {
        QMessageBox::warning(this, "Erreur", "L'ID de commande n'existe pas dans la base de données.");
        return;
    }

    // Modification après validation des champs
    if (commande.modifier(dateCommande, etatCommande, montantTotal, dateLivraison)) {
        QMessageBox::information(this, "Succès", "Les informations de la commande ont été modifiées avec succès.");
        ui->tableView->setModel(commande.afficher());
        ui->tableView->resizeColumnsToContents();
        reinitialiserChamps();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification des informations de la commande.");
    }
    updateModel();
}

void MainWindow::on_trierButton_clicked() {
    // Retrieve the selected criterion and sort order
    QString critere = ui->comboBoxTri->currentText();
    QString ordre = ui->comboBoxOrdre->currentText();

    qDebug() << "Selected criterion:" << critere;
    qDebug() << "Selected order:" << ordre;

    // Determine the sort order (ascending or descending)
    Qt::SortOrder sortOrder;
    if (ordre == "Croissant") {
        sortOrder = Qt::AscendingOrder;
    } else if (ordre == "Décroissant") {
        sortOrder = Qt::DescendingOrder;
    } else {
        QMessageBox::warning(this, "Erreur", "Ordre de tri invalide.");
        return;
    }

    qDebug() << "SortOrder value:" << (sortOrder == Qt::AscendingOrder ? "Ascending" : "Descending");

    // Map criterion to column index
    int sortColumnIndex = -1;
    if (critere == "ID Commande") {
        sortColumnIndex = 0;
    } else if (critere == "Date Commande") {
        sortColumnIndex = 1;
    } else if (critere == "Date Livraison") {
        sortColumnIndex = 4;
    } else {
        QMessageBox::warning(this, "Erreur", "Critère de tri invalide.");
        return;
    }

    qDebug() << "Sorting column:" << sortColumnIndex;

    // Ensure proxy model is created and reused
    static QSortFilterProxyModel* proxyModel = nullptr;
    if (!proxyModel) {
        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(ui->tableView->model());
        ui->tableView->setModel(proxyModel);
    }

    // Apply sorting
    if (sortColumnIndex != -1) {
        proxyModel->sort(sortColumnIndex, sortOrder);
    }
}

void MainWindow::on_exportPdfButton_clicked() {
    Commande commande;
    commande.telechargerPDF();
    QMessageBox::information(this, "Exportation PDF", "Le fichier PDF des commandes a été généré avec succès.");
}

void MainWindow::displayOrdersPerMonthStatistics() {
    // Initialize month names for X-axis categories
    QStringList months = {"January", "February", "March", "April", "May", "June",
                          "July", "August", "September", "October", "November", "December"};
    QMap<QString, int> monthCounts; // Map to store counts for each month-year
    for (const QString &month : months) {
        monthCounts[month] = 0; // Initialize counts to zero
    }

    // Query to count orders grouped by month and year
    QSqlQuery query;
    query.prepare(
        "SELECT TO_CHAR(dateCommande, 'MM') AS month, "
        "TO_CHAR(dateCommande, 'YYYY') AS year, "
        "COUNT(*) AS count "
        "FROM commande "
        "GROUP BY TO_CHAR(dateCommande, 'MM'), TO_CHAR(dateCommande, 'YYYY') "
        "ORDER BY TO_CHAR(dateCommande, 'YYYY'), TO_CHAR(dateCommande, 'MM')"
    );

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Prepare data for the chart
    QBarSet *set = new QBarSet("Number of Orders"); // Data set for bar chart
    QStringList categories; // X-axis labels (month-year)
    QMap<QString, int> dataMap; // Store month-year and corresponding count

    while (query.next()) {
        QString month = query.value("month").toString();
        QString year = query.value("year").toString();
        int count = query.value("count").toInt();

        // Create label for the X-axis in "Month-Year" format
        QString label = months[month.toInt() - 1] + " " + year; // e.g., "January 2024"
        dataMap[label] = count; // Store the count for this month-year
    }

    // Populate the QBarSet and categories for the chart
    for (auto it = dataMap.constBegin(); it != dataMap.constEnd(); ++it) {
        *set << it.value();    // Ajoute la valeur au dataset
        categories << it.key(); // Ajoute la clé aux catégories
    }


    // Create the bar series and add the dataset
    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Create the chart and set its properties
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Monthly Order Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Set up the X-axis (categories)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories); // Add month-year labels
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Set up the Y-axis (values)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Number of Orders");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create a chart view and set antialiasing
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set a fixed size for the chart view to make it smaller
    chartView->setFixedSize(600, 400); // Adjust the size as needed

    // Set up the QGraphicsView
    ui->graphicsView->setScene(new QGraphicsScene());

    // Add the chart view to the scene
    ui->graphicsView->scene()->addWidget(chartView);

    // Adjust the view to fit the scene
    ui->graphicsView->fitInView(ui->graphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);

    // Refresh the scene and display the chart
    ui->graphicsView->scene()->update();
    ui->graphicsView->show();
}

void MainWindow::displayOrderStatusStatistics() {
    // Query to get the count of orders by status
    QSqlQuery query;
    query.prepare(
        "SELECT etatCommande, COUNT(*) as count "
        "FROM commande "
        "GROUP BY etatCommande"
    );

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Declare totalCount variable and initialize it to 0
    int totalCount = 0;

    // First pass through the query results to calculate the total count of orders
    while (query.next()) {
        totalCount += query.value("count").toInt(); // Sum up the counts
    }

    // Reset the query to execute again for chart creation
    query.exec();

    // Create a Pie Chart Series
    QPieSeries *series = new QPieSeries();

    // Process the query results and populate the pie chart series
    while (query.next()) {
        QString status = query.value("etatCommande").toString();
        int count = query.value("count").toInt();

        // Add each status and its count as a slice in the pie chart
        QPieSlice *slice = series->append(status, count);

        // Optionally, customize the slice (e.g., show labels and percentages)
        slice->setLabelVisible(true);
        if (totalCount > 0) {
            slice->setLabel(QString("%1: %2 orders (%3%)")
                            .arg(status)
                            .arg(count)
                            .arg((count / (float)totalCount) * 100, 0, 'f', 1)); // Correct percentage calculation
        }
    }

    // Create the chart and add the pie series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Order Status Distribution");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create a chart view and set antialiasing for better visuals
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set a fixed size for the chart view to make it smaller
    chartView->setFixedSize(600, 400); // Adjust the size as needed

    // Set up the QGraphicsView
    ui->graphicsView->setScene(new QGraphicsScene());

    // Add the chart view to the scene
    ui->graphicsView->scene()->addWidget(chartView);

    // Use fitInView to make sure the chart fits within the view's scene, but keep the aspect ratio
    ui->graphicsView->fitInView(ui->graphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);

    // Refresh the scene and display the chart
    ui->graphicsView->scene()->update();
    ui->graphicsView->show();
}
void MainWindow::on_loadHistoriqueButton_clicked()
{
    // Récupérer le QListView qui se trouve dans l'onglet "Historique"
    QListView *historiqueListView = ui->tabWidget->findChild<QListView*>("historiqueListView");

    // Charger et afficher l'historique dans le QListView
    cmp.loadHistorique(historiqueListView);
}

void MainWindow::updateModel() {
    Commande commande; // Create an instance of Commande to fetch data
    QSqlQueryModel* model = commande.afficher(); // Fetch the model from the database
    proxyModel->setSourceModel(model); // Set the source model for the proxy
    ui->tableView->setModel(proxyModel); // Attach the proxy model to the table view
}

void MainWindow::updateTimeline(const QList<QPair<QString, QDateTime>> &steps, int currentStep)
{
    // Effacer la scène existante
    timelineScene->clear();

    // Configurations
    int stepCount = steps.size();
    int sceneWidth = 600;  // Largeur de la timeline
    int sceneHeight = 100; // Hauteur de la timeline
    int stepSpacing = sceneWidth / stepCount;

    timelineScene->setSceneRect(0, 0, sceneWidth, sceneHeight);

    // Dessiner la ligne de la timeline
    QPen pen(Qt::black, 2);
    timelineScene->addLine(50, sceneHeight / 2, sceneWidth - 50, sceneHeight / 2, pen);

    for (int i = 0; i < stepCount; ++i)
    {
        int x = 50 + i * stepSpacing; // Position X du point

        // Dessiner un cercle pour chaque étape
        QBrush brush(i == currentStep ? Qt::blue : Qt::gray); // Couleur selon l'étape actuelle
        QGraphicsEllipseItem *stepCircle = timelineScene->addEllipse(x - 10, (sceneHeight / 2) - 10, 20, 20, pen, brush);

        // Ajouter le texte pour chaque étape (étape + date)
        QString stepText = steps[i].first + "\n" + steps[i].second.toString("yyyy-MM-dd hh:mm:ss");
        QGraphicsTextItem *textItem = timelineScene->addText(stepText);
        textItem->setDefaultTextColor(Qt::black);
        textItem->setFont(QFont("Arial", 10));
        textItem->setPos(x - textItem->boundingRect().width() / 2, (sceneHeight / 2) + 15);
    }
}

void MainWindow::onClickSuivi()
{
    // Lire l'ID de commande depuis un champ (par exemple, une QLineEdit nommée ui->idCommandeInput)
    int idCommande = ui->idCommandeInput->text().toInt();

    if (idCommande == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de commande valide.");
        return;
    }

    // Récupérer l'état de la commande depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT etatCommande, dateCommande FROM Commande WHERE idCommande = :idCommande");
    query.bindValue(":idCommande", idCommande);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les données : " + query.lastError().text());
        return;
    }

    if (query.next()) {
        QString etatCommande = query.value("etatCommande").toString();
        QDateTime dateCommande = query.value("dateCommande").toDateTime();

        // Étapes possibles pour la timeline
        QList<QPair<QString, QDateTime>> suiviSteps = {
            qMakePair("Commande passée", dateCommande.addDays(-3)),  // Ex: 3 jours avant
            qMakePair("En cours", dateCommande.addDays(-2)),         // 2 jours avant
            qMakePair("En livraison", dateCommande.addDays(-1)),     // 1 jour avant
            qMakePair("Livrée", dateCommande)                        // Date actuelle
        };

        int currentStep = -1;

        // Déterminer l'étape actuelle en fonction de l'état de la commande
        if (etatCommande == "Commande passee") {
            currentStep = 0;
        } else if (etatCommande == "En cours") {
            currentStep = 1;
        } else if (etatCommande == "En livraison") {
            currentStep = 2;
        } else if (etatCommande == "Livree") {
            currentStep = 3;
        } else {
            QMessageBox::warning(this, "Erreur", "État de commande inconnu : " + etatCommande);
            return;
        }

        // Mettre à jour la timeline avec les étapes et les dates
        updateTimeline(suiviSteps, currentStep);
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune commande trouvée avec l'ID fourni.");
    }
}
