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
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QVariantAnimation>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    flameStatus(0),
    ui(new Ui::MainWindow),  // Initialize proxy model
    proxyModel(new QSortFilterProxyModel(this)), scene(new QGraphicsScene(this)),
    timelineScene(new QGraphicsScene(this)),
    arduino(new QSerialPort(this))
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



    setupArduino();


    // Connect other buttons to their respective slots
    connect(ui->ajouterButton, &QPushButton::clicked, this, &MainWindow::on_ajouterButton_clicked);
    connect(ui->supprimerButton, &QPushButton::clicked, this, &MainWindow::on_supprimerButton_clicked);
    connect(ui->modifierButton, &QPushButton::clicked, this, &MainWindow::on_modifierButton_clicked);
    connect(ui->trierButton, &QPushButton::clicked, this, &MainWindow::on_trierButton_clicked);
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &MainWindow::on_exportPdfButton_clicked);
    connect(ui->buttonOrdersPerMonth, &QPushButton::clicked, this, &MainWindow::displayOrdersPerMonthStatistics);
    connect(ui->buttonOrdersStatus, &QPushButton::clicked, this, &MainWindow::displayOrderStatusStatistics);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::on_loadHistoriqueButton_clicked);
    connect(ui->suiviButton, &QPushButton::clicked, this, &MainWindow::on_suiviButton_clicked);


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

void MainWindow::setupTimeline(const QStringList &labels, const QStringList &dates, int activeStep)
{
    // Nettoyer la scène existante
    timelineScene->clear();

    // Dimensions de la scène
    int sceneWidth = 800;
    int sceneHeight = 200;
    timelineScene->setSceneRect(0, 0, sceneWidth, sceneHeight);

    // Variables pour les cercles et les lignes
    int stepCount = labels.size();
    int circleRadius = 20;
    int yPosition = sceneHeight / 2;
    int stepSpacing = stepCount > 1 ? sceneWidth / (stepCount + 1) : sceneWidth / 2;  // Ajustement dynamique de l'espacement des étapes

    for (int i = 0; i < stepCount; ++i) {
        int xPosition = (i + 1) * stepSpacing;

        // Cercle
        QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(xPosition - circleRadius, yPosition - circleRadius, 2 * circleRadius, 2 * circleRadius);
        circle->setBrush(i <= activeStep ? Qt::green : Qt::gray); // Les étapes actives en vert
        circle->setPen(QPen(Qt::black));
        timelineScene->addItem(circle);

        // Assurez-vous que le format de la date est correct
        QString dateWithoutTime = dates[i].split('T').first();  // Récupérer uniquement la date sans l'heure

        // Parse the date string, fallback to current date if invalid
        QDate parsedDate = QDate::fromString(dateWithoutTime, "yyyy-MM-dd");
        if (!parsedDate.isValid()) {
            qDebug() << "Date invalide:" << dates[i];
            parsedDate = QDate::currentDate();  // Utiliser la date actuelle si la date est invalide
        }

        QString formattedDate = parsedDate.toString("dd MMM yyyy");

        // Créer la chaîne HTML formatée
        QString formattedText = QString("<b>%1</b><br><small>%2</small>").arg(labels[i], formattedDate);

        // Créer un QGraphicsTextItem et définir le contenu HTML
        QGraphicsTextItem *textItem = new QGraphicsTextItem();
        textItem->setHtml(formattedText);  // Utiliser setHtml pour permettre le formatage HTML
        textItem->setDefaultTextColor(Qt::black);
        textItem->setTextWidth(100);
        textItem->setFont(QFont("Arial", 10));
        textItem->setPos(xPosition - 50, yPosition + circleRadius + 10); // Position du texte sous le cercle
        timelineScene->addItem(textItem);

        // Ligne (entre les étapes)
        if (i < stepCount - 1) {
            QGraphicsLineItem *line = new QGraphicsLineItem(xPosition + circleRadius, yPosition, xPosition + stepSpacing - circleRadius, yPosition);
            QPen pen(Qt::DashLine);
            pen.setWidth(2);
            pen.setColor(Qt::black);
            line->setPen(pen);
            timelineScene->addItem(line);
        }
    }
}

void MainWindow::on_suiviButton_clicked()
{
    QString commandId = ui->idCommandeInput->text();

    if (commandId.isEmpty()) {
        qDebug() << "Veuillez entrer un ID de commande.";
        return;
    }

    // Récupérer les informations associées à l'ID de commande depuis la base de données
    QSqlQuery query;
    query.prepare(R"(
        SELECT c."ETATCOMMANDE", s."ETAPE", s."DESCRIPTION", s."DATE_ACTION"
        FROM "COMMANDE" c
        JOIN "SUIVI_COMMANDES" s ON c."IDCOMMANDE" = s."ID_COMMANDE"
        WHERE c."IDCOMMANDE" = :id_commande
        ORDER BY s."ETAPE" ASC
    )");
    query.bindValue(":id_commande", commandId);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des données:" << query.lastError().text();
        return;
    }

    QStringList labels;
    QStringList dates;
    int maxCompletedStep = -1;
    QString state; // Variable pour l'état de la commande

    // Récupérer les données et trier par 'etape'
    while (query.next()) {
        // Récupérer l'état de la commande
        state = query.value("ETATCOMMANDE").toString();

        // Récupérer les informations de suivi
        int step = query.value("ETAPE").toInt();
        QString description = query.value("DESCRIPTION").toString();
        QString dateAction = query.value("DATE_ACTION").toString();

        // Ajouter la description et la date dans les listes
        labels.append(description);
        dates.append(dateAction);

        // Calculer la dernière étape complète
        if (step > maxCompletedStep) {
            maxCompletedStep = step;
        }
    }

    if (labels.isEmpty()) {
        qDebug() << "Aucune donnée trouvée pour l'ID de commande :" << commandId;
        setupTimeline({}, {}, -1); // Réinitialiser la timeline si aucune donnée n'est trouvée
        return;
    }

    // Afficher l'état de la commande (par exemple, dans un QLabel)
    ui->label_4->setText("État de la commande: " + state);

    // Mettre à jour la timeline avec les données récupérées
    setupTimeline(labels, dates, maxCompletedStep);
}


void MainWindow::setupArduino() {
    QString portName = "COM8";  // Change this to your actual COM port
    arduino->setPortName(portName);
    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);

    if (arduino->open(QIODevice::ReadWrite)) {  // Change ReadOnly to ReadWrite
        qDebug() << "Connection with Arduino successful on port:" << portName;
        QObject::connect(arduino, &QSerialPort::readyRead, this, &MainWindow::readFromArduino);
    } else {
        qDebug() << "Error: Unable to connect to Arduino on port:" << portName;
        QMessageBox::critical(this, "Error", "Unable to connect to Arduino!");
    }
}


// Slot to handle data received from Arduino
void MainWindow::readFromArduino() {
    QByteArray data = arduino->readAll();  // Read all available data
    QString value = QString(data).trimmed();  // Convert to string and trim spaces

    qDebug() << "Received data from Arduino:" << value;  // Log the raw data

    bool ok;
    int receivedValue = value.toInt(&ok);  // Convert to integer

    if (ok) {
        flameStatus = receivedValue;  // Update the flame status
        qDebug() << "Flame Status:" << flameStatus;
        updateFlammeTable(flameStatus);  // Update the database
        controlBuzzer(flameStatus);  // Control the buzzer based on flame status
        setupTimer();
    } else {
        qDebug() << "Invalid data received:" << value;
    }
}


void MainWindow::updateFlammeTable(int flameStatus)
{
    // Validate flameStatus before updating
    if (flameStatus < 0 || flameStatus > 3) {
        qDebug() << "Invalid flame status:" << flameStatus;
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE EQUIPEMENT SET flameStatus = :flameStatus");

    query.bindValue(":flameStatus", flameStatus);

    if (query.exec()) {
        qDebug() << "Flame status updated in the database to:" << flameStatus;
    } else {
        qDebug() << "Failed to update flame status in the database:" << query.lastError().text();
    }
}
int MainWindow::readFlammeStatusFromDB()
{
    QSqlQuery query;
    int flameStatus = -1; // Default value in case of an error

    if (query.exec("SELECT flameStatus FROM EQUIPEMENT")) {
        if (query.next()) {
            flameStatus = query.value(0).toInt(); // Get the flameStatus value
            qDebug() << "Flame status read from database:" << flameStatus;
        } else {
            qDebug() << "No data found in Flamme table.";
        }
    } else {
        qDebug() << "Failed to read flame status from database:" << query.lastError().text();
    }

    return flameStatus;
}
#define BUZZER_PIN 13
#define LOW  0
#define HIGH 1

void MainWindow::controlBuzzer(int flameStatus) {
    QByteArray command;

    switch (flameStatus) {
    case 0:  // No flame detected
        command = "BUZZER_OFF\n";
        break;
    case 1:  // Low-intensity flame
        command = "BUZZER_LOW\n";
        break;
    case 2:  // Medium-intensity flame
        command = "BUZZER_MEDIUM\n";
        break;
    case 3:  // High-intensity flame
        command = "BUZZER_HIGH\n";
        break;
    default:
        qDebug() << "Invalid flame status.";
        return;
    }

    arduino->write(command);  // Send the command to the Arduino
    qDebug() << "Command sent:" << command;  // Log the command sent
}


void MainWindow::updateBuzzerStatus()
{
    int flameStatus = readFlammeStatusFromDB(); // Read flame status from SQL
    if (flameStatus != -1) { // Check if a valid status was retrieved
        controlBuzzer(flameStatus); // Control the buzzer based on the status
    } else {
        qDebug() << "Failed to retrieve flame status from database.";
    }
}
void MainWindow::setupTimer()
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateBuzzerStatus);
    timer->start(1000); // Check every 1 second
}
