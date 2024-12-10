#include "mainwindow.h"
#include "service.h"
#include "ui_mainwindow.h"
#include "ui_dialog.h"
#include <QSqlError>
#include <QMessageBox>
#include "perso.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSortFilterProxyModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QApplication>
#include <QDebug>
#include "client.cpp"
#include "client.h"
#include <QModelIndex>
#include <QTextStream>
#include <QTcpSocket>
#include <QSslSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QTextDocument>
#include <QString>
#include <QSqlQuery>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlError>
#include <fstream>
#include<matieres.cpp>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include "generateurpdf.h"
#include"matieres.h"
#include"ihmgraphique.h"
#include "qrcode.h"
#include <QTimer>

#include "piece.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSlice>
#include <QPieSeries>
#include <QFileDialog>
#include<fstream>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QSystemTrayIcon> // hedhiii notification
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QIcon>
#include"qrcode.h"
#include <QTimer>
#include <QFile>
#include<QSqlError>
#include <QDebug>


#include "qscatterseries.h"
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




using qrcodegen::QrCode;
using qrcodegen::QrSegment;
//using namespace QtCharts;
MainWindow::MainWindow(int email, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    userEmail(email),
    flameStatus(0),proxyModel(new QSortFilterProxyModel(this)), scene(new QGraphicsScene(this)),
    timelineScene(new QGraphicsScene(this)),
    arduino(new QSerialPort(this))

{
    ui->setupUi(this);

    ui->graphicsView_3->setScene(timelineScene);
         ui->graphicsView_3->setRenderHint(QPainter::Antialiasing);
         ui->graphicsView_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
         ui->graphicsView_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

     // Initialize and set the proxy model
     Commande commande; // Create an instance of Commande to fetch data
     QSqlQueryModel* model2 = commande.afficher(); // Fetch the model from the database
     proxyModel->setSourceModel(model2); // Set the source model for the proxy
     ui->tableView_3->setModel(proxyModel); // Attach the proxy model to the table view

     // Connect the lineEdit for dynamic searching
     connect(ui->lineEditRecherche, &QLineEdit::textChanged, this, [this](const QString &text) {
         proxyModel->setFilterKeyColumn(-1); // Enable search across all columns
         proxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption)); // Use case-insensitive search
     });



     setupArduino();

     // Connect other buttons to their respective slots
        connect(ui->ajouterButton_2, &QPushButton::clicked, this, &MainWindow::on_ajouterButton2_clicked);
        connect(ui->supprimerButton, &QPushButton::clicked, this, &MainWindow::on_supprimerButton_clicked);
        connect(ui->modifierButton, &QPushButton::clicked, this, &MainWindow::on_modifierButton_clicked);
        connect(ui->trierButton, &QPushButton::clicked, this, &MainWindow::on_trierButton_clicked);
        connect(ui->exportPdfButton, &QPushButton::clicked, this, &MainWindow::on_exportPdfButton_clicked);
        connect(ui->buttonOrdersPerMonth, &QPushButton::clicked, this, &MainWindow::displayOrdersPerMonthStatistics);
        connect(ui->buttonOrdersStatus, &QPushButton::clicked, this, &MainWindow::displayOrderStatusStatistics);
        connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::on_loadHistoriqueButton_clicked);
        connect(ui->suiviButton, &QPushButton::clicked, this, &MainWindow::on_suiviButton_clicked);





    matieres matieree;

    ui->tab_m->setModel(matieree.afficher());
   connect(ui->tab_m, &QTableWidget::clicked, this, &MainWindow::on_tab_m_clicked);

    ui->tableView->setModel(pro.afficher());
    ui->tableView->resizeColumnsToContents();
    displayGenderStatistics();
    displayHiringPeriodStatistics();
    onUpdateTypeCongeClicked();
    privilage();

    ui->tableView->setModel(pro.afficher());
    ui->tableView->resizeColumnsToContents();

    QSqlQueryModel *model = pro.afficherconge();
    ui->afficher_conge_2->setModel(model);
    ui->afficher_conge_2->resizeColumnsToContents();

    connect(ui->ajouterServiceButton, &QPushButton::clicked, this, &MainWindow::on_ajouterServiceButton_clicked);
    connect(ui->supprimerServiceButton, &QPushButton::clicked, this, &::MainWindow::on_ajouterServiceButton_clicked);
    connect(ui->modifierServiceButton, &QPushButton::clicked, this, &::MainWindow::on_ajouterServiceButton_clicked);
    connect(ui->comboBoxTri, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &::MainWindow::on_comboBox_tri_changed);
    ui->comboBoxTri->addItem("Trier par");
    ui->comboBoxTri->addItem("Trier par nom du service");  // index 1
    ui->comboBoxTri->addItem("Trier par prix du service"); // index 2
    ui->comboBoxTri->addItem("Trier par type de panne");   // index 3
    ui->comboBoxTri->addItem("Trier par modèle");          // index 4

    searchBar = new QLineEdit(this);  // Crée le widget QLineEdit
    searchBar->setPlaceholderText("Rechercher un service...");  // Texte d'exemple
    // Assure-toi qu'il y a un QVBoxLayout dans ton UI, sinon tu devras l'ajouter
    ui->verticalLayout->insertWidget(0, searchBar);  // Ajoute le QLineEdit au layout (index 0 pour le mettre en haut)
    // Connecte le signal de changement de texte au slot de filtrage
    connect(searchBar, &QLineEdit::textChanged, this, &MainWindow::filterServices);




       //Service service(0, "", "", 0, "");
       ui->serviceTableView->setModel(service.afficher());




       connect(ui->historique, &QPushButton::clicked, this, &MainWindow::on_historique_clicked);
          connect(ui->radioButtonid, &QRadioButton::clicked, this, &MainWindow::on_radioButtonid_clicked);
          connect(ui->lineEdit, &QLineEdit::cursorPositionChanged, this, &MainWindow::on_lineEdit_cursorPositionChanged);

    connect(ui->ajouter, &QPushButton::clicked, this, &MainWindow::on_addEmployeeButton_clicked);
    connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::on_supprimerEmploye_clicked);
    connect(ui->modifier, &QPushButton::clicked, this, &MainWindow::on_modifyEmployeeButton_clicked);
    connect(ui->chercher, &QPushButton::clicked, this, &MainWindow::on_searchButton_clicked);
    connect(ui->comboBox_tri, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_tri_changed(int)));
    connect(ui->id_p, &QLineEdit::textChanged, this, &::MainWindow::onIdChanged);
    connect(ui->id_p, &QLineEdit::editingFinished, this, &MainWindow::on_idEmpChanged);
   connect(ui->pushButton_refresh, &QPushButton::clicked, this, &MainWindow::refreshStatistics);
   connect(ui->pushButton_genre, &QPushButton::clicked, this, &::MainWindow::refreshGenderStatistics);
    ui->tableView->setModel(pro.afficher());
        ui->tableView->resizeColumnsToContents();
        connect(ui->btn_export_pdf, &QPushButton::clicked, this, &MainWindow::on_btn_export_pdf_clicked);


        connect(ui->soumettre_3, &QPushButton::clicked, this, &MainWindow::on_soumettreButton_clicked);
         connect(ui->acceptation_2, &QPushButton::clicked, this, &MainWindow::on_acceptation_clicked);

         connect(ui->afficher_conge_2->selectionModel(), &QItemSelectionModel::selectionChanged,
                 this, &MainWindow::onSelectionChanged);
         connect(ui->acceptation_2, &QPushButton::clicked, this, &MainWindow::on_acceptation_clicked);
         connect(ui->refuser_2, &QPushButton::clicked, this, &MainWindow::on_refuser_clicked);
        // connect(ui->annule_conge_2, &QPushButton::clicked, this, &MainWindow::onclearcongebutton);
         //connect(ui->recherche_conge_2, &QPushButton::clicked, this, &MainWindow::on_searchcongeButtonhistorique_clicked);

         connect(ui->pb_ajouter, &QPushButton::clicked, this, &MainWindow::on_pb_ajouter_clicked);
         connect(ui->pb_ok, &QPushButton::clicked, this, &MainWindow::on_pb_ok_clicked);
         connect(ui->pb_supprimer, &QPushButton::clicked, this, &MainWindow::on_pb_supprimer_clicked);
         connect(ui->tab_m, &QTableView::clicked, this, &MainWindow::on_tab_m_clicked);
         connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
         connect(ui->pushBstat, &QPushButton::clicked, this, &MainWindow::on_pushBstat_clicked);
         connect(ui->radioButton, &QRadioButton::clicked, this, &MainWindow::on_radioButton_clicked);
         connect(ui->radioButton_2, &QRadioButton::clicked, this, &MainWindow::on_radioButton_2_clicked);
         connect(ui->pushBhm, &QPushButton::clicked, this, &MainWindow::on_pushBhm_clicked);
         connect(ui->pb_ajouterr, &QPushButton::clicked, this, &MainWindow::on_pb_ajouterr_clicked);
         connect(ui->pb_modifierr_2, &QPushButton::clicked, this, &MainWindow::on_pb_modifierr_2_clicked);
         connect(ui->pb_supprimerr, &QPushButton::clicked, this, &MainWindow::on_pb_supprimerr_clicked);
         connect(ui->pushButton_10, &QPushButton::clicked, this, &MainWindow::on_pushButton_10_clicked);


        ui->nom_conge_3->setEnabled(false);
        ui->prenom_conge_3->setEnabled(false);
        ui->email_conge_3->setEnabled(false);
        ui->poste_conge_3->setEnabled(false);
        ui->raison_conge_3->setEnabled(false);
        ui->date_debut_3->setEnabled(false);
        ui->date_fin_3->setEnabled(false);
        ui->soumettre_3->setEnabled(false);
        ui->acceptation_2->setEnabled(false);
        ui->refuser_2->setEnabled(false);

        client tempClient;
        QTableView* tableView = ui->affclient;

        if (tableView) {
            tableView->setModel(tempClient.afficher());
        } else {
            qDebug() << "QTableView 'affclient' not found!";
        }
        if (tableView) {
            tableView->setSortingEnabled(true);

            connect(tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, [this](int column) mutable {
                QString columnName;
                switch(column) {
                case 0: columnName = "code_client"; break;
                case 1: columnName = "nom"; break;
                case 2: columnName = "prenom"; break;
                case 3: columnName = "cin"; break;
                case 4: columnName = "adresse"; break;
                case 5: columnName = "pays"; break;
                case 6: columnName = "tel"; break;
                case 7: columnName = "mail"; break;
                default: columnName = "code_client"; break;
                }

                static bool ascendant = true;
                QString ordre = ascendant ? "ASC" : "DESC";
                ascendant = !ascendant;

                // Appeler la méthode appliquerTri avec les paramètres de tri
                appliquerTri(columnName, ordre);
            });
        } else {
            qDebug() << "Le QTableView 'affclient' n'a pas été trouvé.";
        }

        QComboBox* paysComboBox = ui->paysc;
        if (paysComboBox) {
            paysComboBox->addItem("Tunisie");
            paysComboBox->addItem("France");
            paysComboBox->addItem("Espagne");
            paysComboBox->addItem("Allemagne");
            paysComboBox->addItem("Italie");
            paysComboBox->addItem("US");
            paysComboBox->addItem("UK");
            paysComboBox->addItem("Canada");
            paysComboBox->addItem("Australia");
            paysComboBox->addItem("Emirates");
        }
        QComboBox* payscComboBox = ui->payscnouv;
        if (payscComboBox) {
            payscComboBox->addItem("Tunisie");
            payscComboBox->addItem("France");
            payscComboBox->addItem("Espagne");
            payscComboBox->addItem("Allemagne");
            payscComboBox->addItem("Italie");
            payscComboBox->addItem("US");
            payscComboBox->addItem("UK");
            payscComboBox->addItem("Canada");
            payscComboBox->addItem("Australia");
            payscComboBox->addItem("Emirates");
        }


//piece

        ui->tableView_2->setModel(p.afficher());
        afficherStatistiques();

        setupTrayIcon();


        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::checkQuantities);

        // Démarrer le timer pour une exécution toutes les 2 heures
        timer->start(7200000); // 2 heures en millisecondes
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*
void MainWindow::on_acceptation_clicked()
{
    // Get the selected index from the table view
    QModelIndexList selectedIndexes = ui->afficher_conge->selectionModel()->selectedRows();

    // Check if there is a selection
    if (!selectedIndexes.isEmpty()) {
        // Get the first selected row (we assume it's a single row selection)
        QModelIndex index = selectedIndexes.first();

        // Get the 'id_conge' from the selected row (assuming it's in column 0)
        QString id_conge = ui->afficher_conge->model()->data(index.sibling(index.row(), 0)).toString();

        // Call the method in Employe.cpp to update the database
        Employe employe;
        if (employe.updateCongeStatus(id_conge, "yes", "en cour")) {
            qDebug() << "Record updated successfully!";
        } else {
            qDebug() << "Error updating record!";
        }
    }
    QSqlQueryModel *model = emp.afficherconge();

    // Set the model to the table view
    ui->afficher_conge->setModel(model);

    // Optionally, resize the column to fit the content
    ui->afficher_conge->resizeColumnsToContents();
    ui->id_emp_3->clear();

    // Disable the buttons when no row is selected
    ui->acceptation->setEnabled(false);
    ui->refuser->setEnabled(false);
    //refreshTableView();
    //afficherCongeEmploye();

}
void MainWindow::on_refuser_clicked()
{
    // Get the selected index from the table view
    QModelIndexList selectedIndexes = ui->afficher_conge->selectionModel()->selectedRows();

    // Check if there is a selection
    if (!selectedIndexes.isEmpty()) {
        // Get the first selected row (we assume it's a single row selection)
        QModelIndex index = selectedIndexes.first();

        // Get the 'id_conge' from the selected row (assuming it's in column 0)
        QString id_conge = ui->afficher_conge->model()->data(index.sibling(index.row(), 0)).toString();

        // Set the 'id_conge' value in the QLineEdit (id_emp_3)
        ui->id_emp_3->setText(id_conge);

        // Enable the buttons
        ui->acceptation->setEnabled(true);
        ui->refuser->setEnabled(true);

        // Call the method in Employe.cpp to update the database
        Employe employe;
        if (employe.updateCongeStatus(id_conge, "no", "refuser")) {
            qDebug() << "Record updated successfully!";
        } else {
            qDebug() << "Error updating record!";
        }
    } else {
        // If no row is selected (i.e., selection is cleared)
        ui->id_emp_3->clear();

        // Disable the buttons
        ui->acceptation->setEnabled(false);
        ui->refuser->setEnabled(false);
    }

    // Refresh the table view to reflect database changes
    QSqlQueryModel *model = emp.afficherconge();
    ui->afficher_conge->setModel(model);

    // Optionally, resize the column to fit the content
    ui->afficher_conge->resizeColumnsToContents();

    // Clear the selection after update
    ui->afficher_conge->selectionModel()->clearSelection();
    ui->id_emp_3->clear();

    // Disable the buttons when no row is selected
    ui->acceptation->setEnabled(false);
    ui->refuser->setEnabled(false);
    refreshTableView();
    afficherCongeEmploye();


}
*/


void MainWindow::afficherStatistiques() {

    // Récupérer la série de statistiques (pie chart)
    QPieSeries* series = p.statistiques();

    // Créer un objet QChart pour afficher le graphique
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des pieces par  quantite");

    // Créer un QChartView pour intégrer le QChart
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);  // Améliorer le rendu

    // Créer une QGraphicsScene et ajouter le QChartView
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->addWidget(chartView);

    // Ajouter la scène dans le QGraphicsView (stat_e)
    ui->aff_stat->setScene(scene);

    // Redimensionner la scène pour s'adapter au QGraphicsView
    chartView->resize(ui->aff_stat->size());  // Ajuste la taille du chartView à celle de stat_e
}

void MainWindow::setupTrayIcon() {
    // Créer un menu contextuel pour l'icône du tray
    trayIconMenu = new QMenu(this);

    // Créer une action pour quitter l'application
    QAction *quitAction = new QAction("Quitter", this);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);
    trayIconMenu->addAction(quitAction);

    // Créer l'icône du tray
    trayIcon = new QSystemTrayIcon(this);

    // Définir l'icône (utiliser le chemin relatif à partir des ressources Qt)
    trayIcon->setIcon(QIcon("notif.png"));
    trayIcon->setContextMenu(trayIconMenu);

    // Afficher l'icône dans la barre des tâches
    trayIcon->show();
}
void MainWindow::showNotification(const QString &title, const QString &message)
{
    // Vérifier si l'icône du tray est activée et afficher la notification
    if (trayIcon->isVisible()) {
        trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 3000);  // Durée en ms
    }
}
void MainWindow::checkQuantities()
{
    QSqlQuery query;
    query.prepare("SELECT id, quant FROM piece WHERE quant < 30");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification des quantités:" << query.lastError().text();
        return;
    }

    QString notificationMessage;
    while (query.next()) {
        int id = query.value("id").toInt();
        int quant = query.value("quant").toInt();
        notificationMessage += QString("ID: %1 (Quantité restante: %2)\n").arg(id).arg(quant);
    }

    if (!notificationMessage.isEmpty()) {
        showNotification(
            "Produits avec faible quantité",
            notificationMessage
            );
    }
}

void MainWindow::privilage() {
    // Créer une instance de la classe perso
    perso pro;

    // Obtenir le rôle de l'utilisateur en appelant la fonction
    QString role = pro.getPosteById(userEmail);
    qDebug() << "Role déterminé pour l'utilisateur:" << role;

    // Désactiver tous les onglets par défaut
    for (int i = 0; i < ui->tabWidget_6->count(); ++i) {
        ui->tabWidget_6->setTabEnabled(i, false);
    }

    // Déterminer l'index de l'onglet à activer en fonction du rôle
    QMap<QString, int> roleToTabIndex = {
        {"personel", 0},
        {"client", 1},
        {"service", 2},
        {"matiere", 3},
        {"piece", 4},
        {"commande", 5}
    };

    if (roleToTabIndex.contains(role)) {
        int tabIndex = roleToTabIndex[role];
        ui->tabWidget_6->setTabEnabled(tabIndex, true);
        ui->tabWidget_6->setCurrentIndex(tabIndex);
    } else {
        qDebug() << "Rôle inconnu ou non géré:" << role;
    }
}


void MainWindow::refreshTableView()
{
    // Refresh the table model
    QSqlQueryModel *model = pro.afficherconge();
    ui->afficher_conge_2->setModel(model);

    // Resize the columns to fit content
    ui->afficher_conge_2->resizeColumnsToContents();

    // Reconnect the selection model to the onSelectionChanged slot
    connect(ui->afficher_conge_2->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::onSelectionChanged);
}


void MainWindow::on_addEmployeeButton_clicked()
{
    // Validate 'nom'
    if (ui->nom_3->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Nom vide !!");
        return;
    }
    QRegularExpression nameRegex("^[A-Za-z ]*$"); // Regex for only letters and spaces
        QRegularExpressionValidator nameValidator(nameRegex);

        // Validate 'nom'
        QString nom_3 = ui->nom_3->text();
        int pos = 0;
        if (nom_3.isEmpty() || nameValidator.validate(nom_3, pos) != QValidator::Acceptable) {
            QMessageBox::warning(this, "Input Error", "Nom can only contain letters and spaces.");
            return;
        }


    // Validate 'prenom'
    if (ui->prenom_3->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Prenom vide !!");
        return;
    }
    QRegularExpression prenomRegex("^[A-Za-z ]*$"); // Regex for only letters and spaces
        QRegularExpressionValidator prenomValidator(nameRegex);

        // Validate 'nom'
        QString prenom_3 = ui->prenom_3->text();
        int pos1 = 0;
        if (prenom_3.isEmpty() || nameValidator.validate(prenom_3, pos) != QValidator::Acceptable) {
            QMessageBox::warning(this, "Input Error", "prenom can only contain letters and spaces.");
            return;
        }

    // Validate 'email' format using regex
    QString email = ui->email->text();
    QRegularExpression emailRegex(R"((\w+)(\.?[\w-]+)*@([\w-]+\.)+[\w-]{2,})");
    QRegularExpressionValidator emailValidator(emailRegex);
    int emailPos = 0;
    if (ui->email->text().isEmpty() || emailValidator.validate(email, emailPos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Input Error", "Email vide ou invalide !!");
        return;
    }

    // Validate 'salaire' input is numeric
    bool salaireOk;
    double salaire = ui->salaire->text().toDouble(&salaireOk);
    if (!salaireOk || salaire <= 0) {
        QMessageBox::warning(this, "Input Error", "Salaire vide ou invalide !!");
        return;
    }

    // Validate 'poste' selection
    if (ui->poste->currentText() == "choisir poste") {
        QMessageBox::warning(this, "Input Error", "Choisir un poste valide.");
        return;
    }

    // Validate 'sexe' selection
    if (!ui->homme->isChecked() && !ui->femme->isChecked()) {
        QMessageBox::warning(this, "Input Error", "Choisir votre genre.");
        return;
    }

    // Retrieve values from UI elements and set employee attributes
    QString nom = ui->nom_3->text();
    QString prenom = ui->prenom_3->text();
    QString sexe = ui->homme->isChecked() ? "M" : "F"; // Determine gender from radio buttons
    QString poste = ui->poste->currentText();
    QDate dateNaissance = ui->date_n->date(); // Retrieve date from QDateEdit widget

    // Set employee object attributes
    pro.setnom(nom);
    pro.setprenom(prenom);
    pro.setemail(email);
    pro.setsalaire(salaire);
    pro.setposte(poste);
    pro.setsexe(sexe);
    pro.date_n = dateNaissance; // Ensure date is directly set in the Personnel class

    // Attempt to add employee
    bool test = pro.ajout();
    if (test) {
        QMessageBox::information(this, "Info", "Employee added with success.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to add employee.");
        return;
    }

    // Check confirmation and reset fields if addition was successful
    if (ui->confirmation->isChecked()) {
        if (test) {
            QMessageBox::information(this, "Success", "Employee added successfully: " + nom + " " + prenom);

            // Reset UI fields after successful addition
            ui->nom_3->clear();
            ui->prenom_3->clear();
            ui->email->clear();
            ui->salaire->clear();
            ui->poste->setCurrentIndex(0);           // Reset to default "choose poste"
            ui->homme->setChecked(false);            // Uncheck gender selection
            ui->femme->setChecked(false);
            ui->confirmation->setChecked(false);     // Uncheck confirmation checkbox
            ui->date_n->setDate(QDate::currentDate()); // Reset date to today
        }
    } else {
        QMessageBox::warning(this, "Error", "You must confirm the addition of the employee.");
    }
    ui->tableView->setModel(pro.afficher());
    ui->tableView->resizeColumnsToContents();


}
void MainWindow::on_supprimerEmploye_clicked() {
    // Get the ID from the input field

    QString id = ui->id_sup->text();

    // Check if the input field is empty
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "L'ID pour supprimer est vide.");
        return;
    }


    bool exists = pro.idExists(id);
    bool test = pro.supprimer(id);
    if (exists) {
        if (test) {
            QMessageBox::information(this, "Success", "Employé supprimé avec succès.");
            ui->id_sup->clear();
            ui->tableView->setModel(pro.afficher());
            ui->tableView->resizeColumnsToContents();


        } else {
            QMessageBox::warning(this, "Error", "Échec de la suppression de l'employé.");
        }
    } else {
        QMessageBox::warning(this, "Error", "L'ID n'existe pas dans la base de données.");
        return;
    }

}
void MainWindow::on_searchButton_clicked() {
    QString id = ui->id_rech->text();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid ID.");
        return;
    }

    // Get the model currently set in the table view
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());

    if (!model) {
        QMessageBox::warning(this, "Model Error", "No model found for the table view.");
        return;
    }

    // Select the matching row in the table view
    QModelIndex matchingIndex;
    for (int row = 0; row < model->rowCount(); ++row) {
        // Check if the ID in the first column matches the entered ID
        if (model->index(row, 0).data().toString() == id) {
            matchingIndex = model->index(row, 0); // Select the first column in the matching row
            break;
        }
    }

    if (matchingIndex.isValid()) {
        // Select the matching row
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        QItemSelectionModel* selectionModel = ui->tableView->selectionModel();
        selectionModel->select(matchingIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);

        // Scroll to the selected row to ensure it is visible
        ui->tableView->scrollTo(matchingIndex);

        // Highlight the selected row (Blue by default in most styles)
    } else {
        QMessageBox::information(this, "Search Result", "No matching employee found.");
    }
}
void MainWindow::on_modifyEmployeeButton_clicked()
{
    // Récupérer l'ID de l'employé à partir d'un champ de saisie
    QString id = ui->id_sup_2->text();

    // Vérifier si l'ID est vide
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID pour modifier est vide.");
        return;
    }

    // Récupérer les valeurs de l'interface utilisateur pour chaque champ
    QString nom = ui->nom_3->text();
    QString prenom = ui->prenom_3->text();
    QString sexe = ui->homme->isChecked() ? "M" : (ui->femme->isChecked() ? "F" : "");
    QString email = ui->email->text();
    float salaire = ui->salaire->text().toFloat();

    // Vérification de la récupération de la valeur du poste depuis le comboBox
    QString poste = ui->poste->currentText();
    QDate date_n = ui->date_n->date();

    // Vérifier si l'ID existe avant de modifier
    if (!pro.idExists(id)) {
        QMessageBox::information(this, "Échec", "L'ID pour modifier n'existe pas.");
        return;
    }

    // Vérifier si l'utilisateur a confirmé la modification
    if (!ui->confirmation->isChecked()) {
        QMessageBox::warning(this, "Échec", "Veuillez confirmer la modification !");
        return;
    }

    // Appeler la fonction de modification avec les valeurs collectées
    bool success = pro.modifier(nom, prenom, sexe, email, salaire, poste, date_n, id);

    // Afficher un message de succès ou d'échec
    if (success) {
        QMessageBox::information(this, "Succès", "Les informations de l'employé ont été modifiées avec succès.");
        ui->id_sup->clear();
        ui->tableView->setModel(pro.afficher());
        ui->tableView->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, "Échec", "Échec de la modification des informations de l'employé.");
    }
}


void MainWindow::on_comboBox_tri_changed(int index)
{
    // Créer un objet de la classe perso
    perso p;

    // Récupérer le modèle trié
    QSqlQueryModel *model = p.tri(index);

    // Mettre à jour la vue du tableau avec le modèle trié
    ui->tableView->setModel(model);

    // Ajuster la largeur des colonnes pour s'adapter au contenu
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_btn_export_pdf_clicked()
{
    // Récupérer l'identifiant du personnel sélectionné
    int row = ui->tableView->currentIndex().row();
    if (row == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un personnel à exporter.");
        return;
    }

    QModelIndex index = ui->tableView->model()->index(row, 0); // Supposons que la première colonne est l'ID_P
    int id_p = ui->tableView->model()->data(index).toInt();

    // Demander le chemin pour sauvegarder le fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter PDF", QString(), "*.pdf");
    if (filePath.isEmpty()) {
        return; // Si l'utilisateur annule l'enregistrement
    }

    if (QFileInfo(filePath).suffix().isEmpty()) {
        filePath.append(".pdf");
    }

    // Créer une instance de la classe perso
    perso p;

    // Appeler la méthode exporterPDF
    if (p.exporterPDF(id_p, filePath)) {
        QMessageBox::information(this, "Succès", "Le fichier PDF a été exporté avec succès !");
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'exportation du fichier PDF.");
    }
}
void MainWindow::refreshGenderStatistics()
{
    displayGenderStatistics(); // Rafraîchit les statistiques de genre
}

void MainWindow::displayGenderStatistics()
{
    int maleCount = 0;
    int femaleCount = 0;

    // Requête SQL pour obtenir les comptages des genres
    QSqlQuery query;
    query.prepare("SELECT sexe, COUNT(*) as count FROM PERSONNEL GROUP BY sexe");
    query.exec();

    // Traiter les résultats de la requête pour affecter les valeurs à maleCount et femaleCount
    while (query.next()) {
        QString gender = query.value(0).toString().toLower(); // Obtenir la valeur de sexe depuis la base de données
        int count = query.value(1).toInt(); // Obtenir le nombre pour chaque sexe

        if (gender == "m") {
            maleCount = count;
        } else if (gender == "f") {
            femaleCount = count;
        }
    }

    // Créer la série de graphique circulaire (Pie Chart)
    QPieSeries *series = new QPieSeries();
    series->append("Male", maleCount);
    series->append("Female", femaleCount);

    // Créer un graphique et ajouter la série
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Gender Distribution");

    // Personnaliser les tranches du graphique pour ajouter des étiquettes
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1) + "%");
    }

    // Créer une vue pour afficher le graphique circulaire
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Définir la taille de la vue pour qu'elle s'adapte dans le QGraphicsView
    chartView->setFixedSize(ui->pie->size());

    // Effacer la scène précédente et définir la nouvelle scène avec la vue du graphique
    ui->pie->setScene(new QGraphicsScene());
    ui->pie->scene()->addWidget(chartView);  // Ajouter la vue du graphique à la scène

    // Rafraîchir la scène du QGraphicsView
    ui->pie->scene()->update();
    ui->pie->show();  // Assurez-vous que le graphique est affiché
}



// Méthode appelée lors du clic sur le bouton
void MainWindow::refreshStatistics() {
    displayHiringPeriodStatistics(); // Rafraîchit le graphique
}

// Fonction d'affichage des statistiques (que vous avez déjà)
void MainWindow::displayHiringPeriodStatistics() {
    // Requête SQL pour regrouper les embauches par mois et année
    QSqlQuery query;
    query.prepare(
        "SELECT "
        "  EXTRACT(YEAR FROM TO_DATE(DATE_N, 'YYYY-MM-DD')) AS year, "
        "  EXTRACT(MONTH FROM TO_DATE(DATE_N, 'YYYY-MM-DD')) AS month, "
        "  COUNT(*) AS count "
        "FROM personnel "
        "GROUP BY EXTRACT(YEAR FROM TO_DATE(DATE_N, 'YYYY-MM-DD')), "
        "         EXTRACT(MONTH FROM TO_DATE(DATE_N, 'YYYY-MM-DD')) "
        "ORDER BY year, month"
    );

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
        return;
    }

    // Préparer les données pour le graphique
    QBarSet *set = new QBarSet("Nombre d'embauches");
    QStringList categories; // Labels pour l'axe X

    while (query.next()) {
        QString year = query.value("year").toString();
        QString month = query.value("month").toString();
        int count = query.value("count").toInt();

        // Format de la catégorie : "Année-Mois"
        QString category = QString("%1-%2").arg(year).arg(month.toInt(), 2, 10, QChar('0')); // Format "YYYY-MM"

        *set << count;         // Ajouter le nombre d'embauches à l'ensemble
        categories << category; // Ajouter la catégorie (année-mois) aux catégories
    }

    // Créer la série de barres
    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Créer le graphique et ajouter la série
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des périodes d'embauche (par mois et année)");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Configurer l'axe X
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Mois et Année");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Configurer l'axe Y
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre d'embauches");
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Ajuster la taille des polices
    QFont titleFont = chart->titleFont();
    titleFont.setPointSize(14); // Augmente la taille du titre
    chart->setTitleFont(titleFont);

    QFont axisFont = axisX->labelsFont();
    axisFont.setPointSize(10); // Ajuste la taille des labels des axes
    axisX->setLabelsFont(axisFont);
    axisY->setLabelsFont(axisFont);

    // Créer une vue pour le graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Ajuster la taille de la vue pour rendre le graphique plus clair
    chartView->setMinimumSize(800, 600); // Augmente la taille

    // Configurer la QGraphicsView
    ui->graphicsView->setScene(new QGraphicsScene());
    ui->graphicsView->scene()->addWidget(chartView);

    // Adapter la vue pour que le graphique s'affiche bien
    ui->graphicsView->fitInView(ui->graphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->scene()->update();
}


void MainWindow::populateEmployeeInfo()
{

    // Créer une instance de la classe Personel
    perso pro;
    QString id_p = ui->id_p->text().trimmed(); // Récupérer et nettoyer l'entrée utilisateur

    // Vérifier si l'identifiant est vide
    if (id_p.isEmpty()) {

        return;
    }

    // Variables pour stocker les détails de l'employé
    QString nom, prenom, email, poste;

    // Tenter de récupérer les détails de l'employé via l'ID
    bool employeeFound = pro.getEmployeeDetailsById(id_p, nom, prenom, email, poste);

    // Afficher les détails pour vérification
    qDebug() << "ID:" << id_p << ", Nom:" << nom << ", Prénom:" << prenom
             << ", Email:" << email << ", Poste:" << poste;

    if (employeeFound) {
        // Si l'employé est trouvé, mettre à jour les champs de l'interface utilisateur
        ui->nom_conge_3->setText(nom);
        ui->prenom_conge_3->setText(prenom);
        ui->email_conge_3->setText(email);
        ui->date_debut_3->setDate(QDate::currentDate());
        ui->date_fin_3->setDate(QDate::currentDate());
        ui->poste_conge_3->setText(poste);

    } else {
        // Si l'employé n'est pas trouvé, afficher un message d'erreur
        QMessageBox::warning(this, "Erreur", "Aucun employé trouvé avec l'ID " + id_p + ".");
        ui->id_p->clear(); // Effacer l'entrée de l'utilisateur
    }
}



void MainWindow::on_idEmpChanged() {
    populateEmployeeInfo();  // Call the function when the ID is changed
}
void MainWindow::on_soumettreButton_clicked()
{
    // Créer une instance de la classe Personel
    perso pro;

    // Récupérer les valeurs des champs du formulaire
    QString id_p = ui->id_p->text().trimmed();
    QString nom = ui->nom_conge_3->text().trimmed();
    QString prenom = ui->prenom_conge_3->text().trimmed();
    QString email = ui->email_conge_3->text().trimmed();
    QString raison = ui->raison_conge_3->text().trimmed();
    QString poste = ui->poste_conge_3->text().trimmed();
    QDate date_d = ui->date_debut_3->date();
    QDate date_f = ui->date_fin_3->date();

    // Conversion de l'ID en entier
    int id_emp = id_p.toInt();

    // Vérification des champs obligatoires
    if (id_p.isEmpty() || nom.isEmpty() || prenom.isEmpty() || email.isEmpty() ||
        raison.isEmpty() || poste.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs avant de soumettre.");
        return;
    }

    // Validation de l'email (expression régulière)
    QRegularExpression emailRegex(
        R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir une adresse email valide.");
        return;
    }

    // Vérification de la logique des dates
    if (date_d > date_f) {
        QMessageBox::warning(this, "Erreur", "La date de début doit être antérieure à la date de fin.");
        return;
    }

    // Vérifier si l'employé a déjà un congé en cours

    if (pro.hasOngoingLeave(id_emp)) {
        QMessageBox::warning(this, "Erreur", "Cet employé a déjà un congé en cours.");
        return;
    }

    // Appeler la méthode ajouterconge pour insérer le congé
    if (pro.ajouterconge(id_emp, nom, prenom, email, raison, poste, date_d, date_f)) {
        QMessageBox::information(this, "Succès", "Le congé a été ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du congé. Veuillez réessayer.");
    }

    QSqlQueryModel *model = pro.afficherconge();
    ui->afficher_conge_2->setModel(model);
    ui->afficher_conge_2->resizeColumnsToContents();

    refreshTableView();


}

void MainWindow::onSelectionChanged(const QItemSelection &selected)
{
    // Check if something is selected
    if (!selected.isEmpty()) {
        // Get the first selected index (row, column)
        QModelIndex index = selected.indexes().first();

        // Check if the selected column is the one containing the 'id_conge' (assuming column 0)
        if (index.column() == 0) {
            // Retrieve the 'id_conge' from the model (assuming it's the first column)
            QString id_conge = ui->afficher_conge_2->model()->data(index).toString();
            // Set the 'id_conge' value in the QLineEdit (id_emp_3)
            ui->id_emp_7->setText(id_conge);

            // Enable the buttons (assuming buttons are named acceptation and refuser)
            ui->acceptation_2->setEnabled(true);
            ui->refuser_2->setEnabled(true);


        }
    }
    else {
        // If no row is selected (i.e., selection is cleared), clear the QLineEdit
        ui->id_emp_7->clear();

        // Disable the buttons when no row is selected
        ui->acceptation_2->setEnabled(false);
        ui->refuser_2->setEnabled(false);
    }
}
void MainWindow::onIdChanged()
{
    // Enable all other fields if the ID field is not empty
    QString id_p=ui->id_p->text();
    if (id_p.isEmpty()){
        ui->nom_conge_3->clear();
        ui->prenom_conge_3->clear();
        ui->email_conge_3->clear();
       // ui->poste_conge_3->setCurrentIndex(0);
        ui->date_debut_3->setDate(QDate::currentDate());
        ui->date_fin_3->setDate(QDate::currentDate());
        ui->nom_conge_3->setEnabled(false);
        ui->prenom_conge_3->setEnabled(false);
        ui->email_conge_3->setEnabled(false);
        ui->poste_conge_3->setEnabled(false);

        ui->raison_conge_3->setEnabled(false);
        ui->date_debut_3->setEnabled(false);
        ui->date_fin_3->setEnabled(false);
        ui->soumettre_3->setEnabled(false);
    }else{
        ;
        ui->raison_conge_3->setEnabled(true);
        ui->date_debut_3->setEnabled(true);
        ui->date_fin_3->setEnabled(true);
        ui->soumettre_3->setEnabled(true);
    }


}
void MainWindow::on_acceptation_clicked()
{
    // Get the selected index from the table view
    QModelIndexList selectedIndexes = ui->afficher_conge_2->selectionModel()->selectedRows();

    // Check if there is a selection
    if (!selectedIndexes.isEmpty()) {
        // Get the first selected row (we assume it's a single row selection)
        QModelIndex index = selectedIndexes.first();

        // Get the 'id_conge' from the selected row (assuming it's in column 0)
        QString id_conge = ui->afficher_conge_2->model()->data(index.sibling(index.row(), 0)).toString();

        // Call the method in Personel.cpp to update the 'etat' column
        perso pro;
        if (pro.updateCongeStatus(id_conge, "accepter")) {
            qDebug() << "Record updated successfully!";
        } else {
            qDebug() << "Error updating record!";
        }
    }

    // Refresh the table view
    QSqlQueryModel *model = pro.afficherconge();
    ui->afficher_conge_2->setModel(model);

    // Optionally, resize the columns to fit the content
    ui->afficher_conge_2->resizeColumnsToContents();
    ui->acceptation_2->setEnabled(false);
    ui->refuser_2->setEnabled(false);
    // Clear any input fields (if applicable)
    ui->id_p->clear();
    refreshTableView();
}
void MainWindow::on_refuser_clicked()
{
    // Get the selected index from the table view
    QModelIndexList selectedIndexes = ui->afficher_conge_2->selectionModel()->selectedRows();

    // Check if there is a selection
    if (!selectedIndexes.isEmpty()) {
        // Get the first selected row (we assume it's a single row selection)
        QModelIndex index = selectedIndexes.first();

        // Get the 'id_conge' from the selected row (assuming it's in column 0)
        QString id_conge = ui->afficher_conge_2->model()->data(index.sibling(index.row(), 0)).toString();

        // Call the method in Personel.cpp to update the database
        perso employe;
        if (employe.updateCongeStatus(id_conge, "refuser")) {
            qDebug() << "Record updated successfully!";
        } else {
            qDebug() << "Error updating record!";
        }
    }

    // Refresh the table view to reflect database changes
    QSqlQueryModel *model = pro.afficherconge();
    ui->afficher_conge_2->setModel(model);

    // Optionally, resize the column to fit the content
    ui->afficher_conge_2->resizeColumnsToContents();

    // Clear the selection after update
    ui->afficher_conge_2->selectionModel()->clearSelection();
    // Disable the buttons when no row is selected
    ui->acceptation_2->setEnabled(false);
    ui->refuser_2->setEnabled(false);
    ui->id_p->clear();
    refreshTableView();

}



void MainWindow::onUpdateTypeCongeClicked() {
    perso pro;
    pro.updateTypeConge();
}
//-------------------------
//client
void MainWindow::on_pushButton_ajouter_clicked() {
    qDebug() << "Le bouton Ajouter a été cliqué.";

    QString nom = ui->nom->text();

    QString prenom = ui->prenom->text();

    QString cinStr = ui->cin->text();

    QString adresse = ui->adresse->text();

    QString pays = ui->paysc->currentText();

    QString telStr = ui->tel->text();

    QString email = ui->mail->text();

    // Validate inputs
    if (nom.isEmpty() || prenom.isEmpty() || cinStr.isEmpty() || adresse.isEmpty() ||
        pays.isEmpty() || telStr.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Input Error"),
                             QObject::tr("Tous les champs sont obligatoires.\n"
                                         "Veuillez remplir tous les champs."),
                             QMessageBox::Ok);
        return;
    }
    if (!((nom >= 'a' && nom <= 'z')) || ((nom >= 'A' && nom <= 'Z'))) {
        QMessageBox::warning(nullptr, QObject::tr("Input Error"),
                             QObject::tr("Nom invalide Veuillez entrer une chaine de characters."),
                             QMessageBox::Ok);
        return;
    }
    if (!((prenom >= 'a' && prenom <= 'z')) || ((prenom >= 'A' && prenom <= 'Z'))) {
        QMessageBox::warning(nullptr, QObject::tr("Input Error"),
                             QObject::tr("Prenom invalide Veuillez entrer une chaine de characters."),
                             QMessageBox::Ok);
        return;
    }
    if (!email.contains("@")) {
        QMessageBox::warning(nullptr, QObject::tr("Input Error"),
                             QObject::tr("Veuillez entrer une adresse e-mail valide contenant '@'."),
                             QMessageBox::Ok);
        return; // Exit if the email is not valid
    }

    bool okCin;
    int cin = cinStr.toInt(&okCin);
    if (!okCin || cinStr.length() != 8) {
        QMessageBox::warning(nullptr, QObject::tr("Input Error"),
                             QObject::tr("Le CIN doit contenir exactement 8 chiffres."),
                             QMessageBox::Ok);
        return;
    }
    bool okTel;
    int tel = telStr.toInt(&okTel);
    if (!okTel || telStr.length() != 8) {
        QMessageBox::warning(nullptr, QObject::tr("Input Error"),
                             QObject::tr("Le numéro de téléphone doit contenir exactement 8 chiffres."),
                             QMessageBox::Ok);
        return;
    }
    client tempClient;
    if (tempClient.existsByCin(cinStr)) {
        QMessageBox::warning(nullptr, QObject::tr("Input Error"), QObject::tr("Le CIN est déjà utilisé."), QMessageBox::Ok);
        return ;
    }
    if (tempClient.existsByTel(telStr)) {
        QMessageBox::warning(nullptr, QObject::tr("Input Error"), QObject::tr("Le numéro de téléphone est déjà utilisé."), QMessageBox::Ok);
        return ;
    }
    int code_client = tempClient.getNextCodeClient();
    if (code_client == -1) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Nombre maximum de clients atteint.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return;
    }

    // Set the generated code_client to the QLineEdit
    QLineEdit* codeLineEdit = ui->codecl;

    codeLineEdit->setText(QString::number(code_client));
    codeLineEdit->setReadOnly(true);

    // Create the client object
    client C(code_client, nom, prenom, cin, adresse, pays, tel, email);
    bool test = C.ajouter();

    if (test) {
        QTableView* tableView = ui->affclient;
        tableView->setModel(tempClient.afficher());

        // Clear the input fields except for code_client
        ui->nom->clear();
        ui->prenom->clear();
        ui->cin->clear();
        ui->adresse->clear();
        ui->paysc->clear();
        ui->tel->clear();
        ui->mail->clear();

        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("ajout effectué.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                              QObject::tr("ajout non effectué.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_supprimer_clicked() {
    QString codeClientStr = ui->modid->text();

    // Validate the input
    if (codeClientStr.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Input Error"),
                             QObject::tr("Le champ du code client ne peut pas être vide.\n"
                                         "Veuillez entrer un code client valide."),
                             QMessageBox::Ok);
        return; // Exit the function if input is invalid
    }

    bool ok;
    int code_client = codeClientStr.toInt(&ok);

    if (!ok || code_client <= 0) {
        QMessageBox::warning(nullptr, QObject::tr("Input Error"),
                             QObject::tr("Veuillez entrer un code client valide supérieur à zéro."),
                             QMessageBox::Ok);
        return; // Exit the function if input is invalid
    }

    client tempClient; // Temporary client object for deletion

    // Check if the client exists before trying to delete
    if (!tempClient.exists(code_client)) {
        QMessageBox::warning(nullptr, QObject::tr("Error"),
                             QObject::tr("Le code client n'existe pas.\n"
                                         "Veuillez entrer un code client valide."),
                             QMessageBox::Ok);
        return; // Exit if the client does not exist
    }

    bool test = tempClient.supprimer(code_client);

    // Check if the deletion was successful
    if (test) {
        QTableView* tableView = ui->affclient;

        tableView->setModel(tempClient.afficher()); // Update the table view after deletion

        QMessageBox::information(nullptr, QObject::tr("Success"),
                                 QObject::tr("Suppression effectuée.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("Suppression non effectuée.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }
}
void MainWindow::on_pushButton_refresh_clicked() {
    client tempClient;
    ui->affclient->setModel(tempClient.afficher());  // Assurez-vous que cette méthode renvoie le modèle souhaité
}
void MainWindow::on_annulercl_clicked() {
    // Clear the input fields using the specified paths
    ui->nom->clear();

    ui->prenom->clear();

    ui->cin->clear();

    ui->adresse->clear();

    ui->paysc->setCurrentIndex(-1); // Reset selection

    ui->tel->clear();

    ui->mail->clear();

    ui->codecl->clear(); // Clear code_client
}
void MainWindow::on_affclient_clicked(const QModelIndex &index) {
    if (!index.isValid())
        return;

    // Get the selected row
    int row = index.row();
    QTableView* tableView = ui->affclient;

    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(tableView->model());

    if (model) {

        ui->code_clnouv->setText(model->data(model->index(row, 0)).toString());

        ui->nomnouv->setText(model->data(model->index(row, 1)).toString());

        ui->prenomnouv->setText(model->data(model->index(row, 2)).toString());

        ui->cinnouv->setText(model->data(model->index(row, 3)).toString());

        ui->adressenouv->setText(model->data(model->index(row, 4)).toString());

        ui->payscnouv->setCurrentText(model->data(model->index(row, 5)).toString());

        ui->telnouv->setText(model->data(model->index(row, 6)).toString());

        ui->mailnouv->setText(model->data(model->index(row, 7)).toString());
    }
}
void MainWindow::on_pushButton_modifierClient_clicked()
{
    // Retrieve the modified data from UI
    int code_client = ui->code_clnouv->text().toInt();

    QString nom = ui->nomnouv->text();

    QString prenom = ui->prenomnouv->text();

    int cin = ui->cinnouv->text().toInt();

    QString adresse = ui->adressenouv->text();

    QString pays = ui->payscnouv->currentText();

    int tel = ui->telnouv->text().toInt();

    QString mail = ui->mailnouv->text();

    // Create a client object with updated information
    client C(code_client, nom, prenom, cin, adresse, pays, tel, mail);

    // Call the modifier method
    if (C.modifier()) {
        QMessageBox::information(this, "Success", "Client modified successfully.");
        // Refresh the view after modification
        QTableView* tableView = ui->affclient;

        client tempClient;
        tableView->setModel(tempClient.afficher());
    } else {
        QMessageBox::critical(this, "Error", "Failed to modify client.");
    }
}
void MainWindow::on_annulermodif_clicked() {
    ui->code_clnouv->clear();

    ui->nomnouv->clear();

    ui->prenomnouv->clear();

    ui->cinnouv->clear();

    ui->adressenouv->clear();

    ui->payscnouv->setCurrentIndex(-1);

    ui->telnouv->clear();

    ui->mailnouv->clear();
}
void MainWindow::on_fichiersButton_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Sélectionner des fichiers à joindre");

    // Vérifiez si des fichiers ont été sélectionnés
    if (files.isEmpty()) {
        QMessageBox::warning(this, "Aucun fichier sélectionné", "Veuillez sélectionner au moins un fichier.");
        return;
    }

    // Ajouter les fichiers à la liste des pièces jointes
    attachments.append(files);  // Stockez les chemins des fichiers pour les envoyer plus tard

    // Récupérer le QListWidget pour afficher les pièces jointes
    QListWidget *piecesList = ui->pieces;

    // Vérifiez que le QListWidget a été récupéré avec succès
    if (!piecesList) {
        qDebug() << "Erreur : le QListWidget 'pieces' est introuvable.";
        return;
    }

    piecesList->clear();  // Effacer les éléments précédents

    // Afficher les fichiers sélectionnés dans le QListWidget
    foreach (const QString &file, files) {
        qDebug() << "Ajout du fichier:" << file;  // Message de débogage
        piecesList->addItem(file);  // Ajouter chaque fichier à la liste
    }
}
void MainWindow::on_envoyermailButton_clicked() {
    QString destinataire = ui->dest->text();

    QString objet = ui->obj->text();

    QString corps = ui->mess->toPlainText();

    // Vérification de la validité des champs
    if (destinataire.isEmpty() || objet.isEmpty() || corps.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis avant d'envoyer l'email.");
        return;
    }

    // Appel de la fonction d'envoi d'email avec pièces jointes
    int result = mailer::sendEmail(destinataire, objet, corps, attachments);

    if (result == 0) {
        QMessageBox::information(this, "Succès", "Email envoyé avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'envoi de l'email.");
    }

    // Effacer les champs
    ui->dest->clear();

    ui->obj->clear();

    ui->mess->clear();

    // Effacer les pièces jointes
    attachments.clear();
    QListWidget *piecesList = ui->pieces;
    if (piecesList) {
        piecesList->clear();
    }
}
void MainWindow::on_effmail_clicked() {
    ui->dest->clear();

    ui->obj->clear();

    ui->mess->clear();

    ui->pieces->clear();
}
void MainWindow::sendSMS(const QString &toNumber, const QString &message) {
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

    QString accountSID = "AC50bfdab6d7e7549746cca999deada0b7";
    QString authToken = "4250f147a77ac347066cffc52000cfdf";
    QString fromNumber = "+13608898732";

    QString url = "https://api.twilio.com/2010-04-01/Accounts/" + accountSID + "/Messages.json";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString concatenated = accountSID + ":" + authToken;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    QUrlQuery params;
    params.addQueryItem("To", toNumber);
    params.addQueryItem("From", fromNumber);
    params.addQueryItem("Body", message);

    qDebug() << "Requête envoyée : " << params.toString(QUrl::FullyEncoded).toUtf8();

    networkManager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());

    QObject::connect(networkManager, &QNetworkAccessManager::finished, this, [networkManager](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "SMS envoyé avec succès!";
        } else {
            qDebug() << "Erreur d'envoi de SMS:" << reply->errorString();
        }
        reply->deleteLater();
        networkManager->deleteLater();
    });
}
void MainWindow::on_envoyersms_clicked() {
    QString toNumber = ui->desttel->text();  // Récupère le numéro du destinataire
    QString message = ui->smms->text();      // Récupère le contenu du SMS

    // Appel de la fonction d'envoi de SMS
    sendSMS(toNumber, message);
    if (toNumber.isEmpty() || message.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis avant d'envoyer l'sms.");
        return;
    }
}
void MainWindow::on_pushButton_recherche_clicked() {
    QString searchText = ui->recher->text();

    QTableView* tableView = ui->affclient;
    if (tableView) {
        client tempClient;
        tableView->setModel(tempClient.rechercher(searchText));
    } else {
        qDebug() << "tableView not found!";
    }
}
void MainWindow::appliquerTri(const QString &columnName, const QString &ordre) {
    // Récupérer le QTableView
    client clientModel;
    QTableView *tableView = ui->affclient;

    // Vérifier que tableView n'est pas nul
    if (tableView) {
        // Appliquer le modèle de tri
        tableView->setModel(clientModel.tri(columnName, ordre));
        tableView->setSortingEnabled(true);  // Activer le tri si nécessaire
    } else {
        qDebug() << "Le QTableView 'affclient' n'a pas été trouvé.";
    }
}
void MainWindow::on_pdf_clicked()
{
    // Spécifiez le chemin de la tableView
    QTableView *tableView = ui->affclient;

    if (!tableView) {
        qWarning() << "La QTableView n'a pas été trouvée.";
        return;
    }

    // Ouvrir une boîte de dialogue pour choisir le chemin du fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "*.pdf");
    if (filePath.isEmpty()) {
        return;
    }

    // Configurer le PDF
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(358);
    QPainter painter(&pdfWriter);

    int yPosition = 167; // Position de départ verticale
    int xPosition = 160; // Position de départ horizontale
    int rowHeight = 75;  // Hauteur de chaque ligne
    int columnWidth = 282; // Largeur de chaque colonne

    // Récupérer le modèle de données de la QTableView
    QAbstractItemModel *model = tableView->model();

    // Imprimer les en-têtes de colonnes
    for (int column = 0; column < model->columnCount(); ++column) {
        QString headerText = model->headerData(column, Qt::Horizontal).toString();
        painter.drawText(xPosition + column * columnWidth, yPosition, headerText);
    }
    yPosition += rowHeight;  // Avance à la ligne suivante

    // Imprimer les données de chaque cellule
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column) {
            QString cellText = model->data(model->index(row, column)).toString();
            painter.drawText(xPosition + column * columnWidth, yPosition, cellText);
        }
        yPosition += rowHeight;  // Avance à la ligne suivante
    }

    painter.end();
    qDebug() << "Le fichier PDF a été créé avec succès à l'emplacement :" << filePath;
}
void MainWindow::on_annuler_2_clicked() {
    ui->desttel->clear();
    ui->smms->clear();
}
void MainWindow::setupConnections() {
    QPushButton *statButton = ui->statis;

    if (statButton) {
        connect(statButton, &QPushButton::clicked, this, &MainWindow::on_statis_clicked);
    } else {
        qDebug() << "Erreur : le bouton 'statButton' n'a pas été trouvé.";
    }
    QPushButton *cityButton = ui->statis;//ville

    if (cityButton) {
        connect(cityButton, &QPushButton::clicked, this, &MainWindow::on_statis_clicked);
    } else {
        qDebug() << "Erreur : le bouton 'cityButton' n'a pas été trouvé.";
    }
    QPushButton *emailDomainButton = ui->statis;//domaine

    if (emailDomainButton) {
        connect(emailDomainButton, &QPushButton::clicked, this, &MainWindow::on_statis_clicked);
    } else {
        qDebug() << "Erreur : le bouton 'domaines' n'a pas été trouvé.";
    }
}

void MainWindow::on_statis_clicked() {
    // Accéder au widget QGraphicsView pour afficher les statistiques
    QGraphicsView *pieView = ui->pie;

    if (pieView) {
        // Appeler la fonction displayRegionStatistics() dans client.cpp
        client tempclient;
        tempclient.displayRegionStatistics(pieView);
    } else {
        qDebug() << "Erreur : le widget 'pie' n'a pas été trouvé.";
    }
    QTableWidget *cityTableView = ui->piee;

    if (cityTableView) {
        client tempClient;
        tempClient.displayCityStatistics(cityTableView);
    } else {
        qDebug() << "Error: 'cityTableView' widget not found.";
    }
    QGraphicsView *domainBarView = ui->dom;

    if (domainBarView) {
        client tempClient;
        tempClient.displayEmailDomainStatistics(domainBarView);
    } else {
        qDebug() << "Erreur : le widget 'domainBarView' n'a pas été trouvé.";
    }
}
void MainWindow::filterServices() {
    QString searchText = searchBar->text(); // Assurez-vous que searchBar est bien un QLineEdit défini dans votre interface

    QString queryStr;
    if (searchText.isEmpty()) {
        queryStr = "SELECT * FROM service";  // Si le champ est vide, montre tous les services
    } else {
        // Requête pour filtrer par nom_service, description ou prix_service
        queryStr = QString("SELECT * FROM service WHERE ID LIKE '%%1%' OR NOM_SERVICE LIKE '%%1%' OR MODEL_PANNE LIKE '%%1%'  ").arg(searchText);
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(queryStr);  // Définit la requête à exécuter

    // Vérifie si la requête a échoué
    if (model->lastError().isValid()) {
        QMessageBox::warning(this, "Erreur", model->lastError().text());
    } else {
        ui->serviceTableView->setModel(model); // Applique le modèle à la table
    }
}

// Fonction pour ajouter un service
void MainWindow::on_ajouterServiceButton_clicked() {
    int ID = ui->lineEdit_ID_Service->text().toInt();
    QString NOM_SERVICE = ui->lineEdit_NOM_SERVICE->text();
    QString TYPE_PANNE = ui->comboBox_4->currentText();
    QString MODEL_PANNE = ui->comboBox_3->currentText();
    int PRIX_SERVICE = ui->lineEdit_PRIX_SERVICE->text().toInt();

    Service service(ID, NOM_SERVICE, TYPE_PANNE,PRIX_SERVICE,MODEL_PANNE);

    if (service.ajouter()) {
        QMessageBox::information(this, "Ajout réussi", "Le service a été ajouté avec succès.");
        ui->serviceTableView->setModel(service.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du service.");
    }
}

// Fonction pour supprimer un service
void MainWindow::on_supprimerServiceButton_clicked() {
    int ID = ui->lineEdit_ID_Service1->text().toInt();
    Service service;

    if (service.supprimer(ID)) {
        QMessageBox::information(this, "Suppression réussie", "Le service a été supprimé avec succès.");
        ui->serviceTableView->setModel(service.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du service.");
    }
}

// Fonction pour modifier un service
void MainWindow::on_modifierServiceButton_clicked() {
    int ID = ui->lineEdit_ID_Service1->text().toInt();
    if (ID == 0) {
        QMessageBox::warning(this, "Erreur d'entrée", "L'ID du service pour modification est vide ou invalide.");
        return;
    }

    QString NOM_SERVICE = ui->lineEdit_NOM_SERVICE->text();
    QString TYPE_PANNE = ui->comboBox_4->currentText();
    QString MODEL_PANNE = ui->comboBox_3->currentText();
    int PRIX_SERVICE = ui->lineEdit_PRIX_SERVICE->text().toInt();

    Service service;
    if (service.modifier(ID, NOM_SERVICE, TYPE_PANNE,PRIX_SERVICE, MODEL_PANNE)) {
        QMessageBox::information(this, "Succès", "Les informations du service ont été modifiées avec succès.");
        ui->serviceTableView->setModel(service.afficher());
    } else {
        QMessageBox::warning(this, "Échec", "Échec de la modification du service.");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    // Récupérer l'index de la ligne sélectionnée dans tableView_2
    QModelIndexList selectedIndexes = ui->serviceTableView_2->selectionModel()->selectedRows();

    // Vérifier si une ligne est sélectionnée
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, tr("Aucune sélection"),
                             tr("Veuillez sélectionner une ligne pour vérifier les informations.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        return;
    }

    // Supposer que l'ID est dans la première colonne (index 0)
    QString id = selectedIndexes.first().data().toString();

    // Créer une requête pour récupérer le MODEL_PANNE correspondant à l'ID
    QSqlQuery query1;
    query1.prepare("SELECT TYPE_PANNE FROM SERVICE WHERE id = :id");
    query1.bindValue(":id", id);

    // Exécuter la requête et traiter le résultat
    if (query1.exec()) {
        if (query1.next()) {
            QString modelPanne = query1.value(0).toString(); // Récupérer le champ MODEL_PANNE
            ui->label_13->setText("Le panne de ce voiture est " + modelPanne);
             // Afficher le résultat dans le label
        } else {
            QMessageBox::warning(this, tr("Aucune donnée"),
                                 tr("Aucun enregistrement trouvé pour l'ID sélectionné."));
            ui->label_13->setText(""); // Réinitialiser le label en cas d'absence de résultat
        }
    } else {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("Échec de l'exécution de la requête :\n") + query1.lastError().text());
    }

}


void MainWindow::on_pushButton_3_clicked()
{
    ui->serviceTableView_2->setModel(service.afficher2());
}


void MainWindow::on_pushButton_7_clicked() {
    QModelIndexList selectedIndexes = ui->serviceTableView_5->selectionModel()->selectedRows();

    // Vérifier si une ligne est sélectionnée
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, tr("Aucune sélection"),
                             tr("Veuillez sélectionner une ligne pour vérifier les informations.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        return;
    }

    // Récupérer les données des colonnes de la ligne sélectionnée
    int row = selectedIndexes.first().row();
    QString TYPE_PANNE = ui->serviceTableView_5->model()->index(row, 1).data().toString(); // Première colonne
    QString MODEL_PANNE = ui->serviceTableView_5->model()->index(row, 2).data().toString(); // Deuxième colonne

    // Initialiser le service et les pièces associées
    Service service;
    QMap<QString, QString> pannePieces = service.getPiecePourPanne();

    // Vérifier si TYPE_PANNE existe dans la liste
    if (pannePieces.contains(TYPE_PANNE)) {
        QString piece = pannePieces.value(TYPE_PANNE);

        // Mettre à jour le QLabel avec la pièce associée et le modèle
        ui->label_27->setText("Modèle : " + MODEL_PANNE + "\n" +
                              "Type de panne : " + TYPE_PANNE + "\n" +
                              "Pièce associée : " + piece);
    } else {
        // Afficher un message d'erreur dans le QLabel
        ui->label_27->setText("Type de panne non reconnu.");
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->serviceTableView_5->setModel(service.afficher3());
}
void MainWindow::on_comboBoxTriChanged(int index) {
    QSqlQueryModel *model = new QSqlQueryModel;
    QString query;

    // Construire la requête SQL en fonction de l'index sélectionné
    switch (index) {
    case 1: // Trier par nom du service
        query = "SELECT * FROM SERVICE ORDER BY NOM_SERVICE ASC";
        break;
    case 2: // Trier par prix du service
        query = "SELECT * FROM SERVICE ORDER BY PRIX_SERVICE ASC";
        break;
    case 3: // Trier par type de panne
        query = "SELECT * FROM SERVICE ORDER BY TYPE_PANNE ASC";
        break;
    case 4: // Trier par modèle
        query = "SELECT * FROM SERVICE ORDER BY MODEL_PANNE ASC";
        break;
    default:
        QMessageBox::warning(this, "Erreur", "Option de tri invalide.");
        return;
    }

    // Exécuter la requête et mettre à jour le modèle
    model->setQuery(query);

    // Vérifier les erreurs de requête
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur SQL", model->lastError().text());
        return;
    }

    // Appliquer le modèle à la vue
    ui->serviceTableView->setModel(model);
}
void MainWindow::on_pushButton_generatePDF_clicked() {
    // Accéder à la tableView directement depuis l'UI
    QTableView *serviceTableView = ui->serviceTableView;

    QString fileName = QFileDialog::getSaveFileName(this, "Save as PDF", "", "PDF Files (.pdf);;All Files ()");
    if (fileName.isEmpty()) {
        return;
    }

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageOrientation(QPageLayout::Portrait);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);

    QAbstractItemModel *model = serviceTableView->model();

    int x = 10;   // Position x de départ
    int y = 30;   // Position y de départ
    int rowHeight = 100;
    int columnWidth = 500;

    // Définir la police pour le PDF avec une taille plus grande
    QFont font = painter.font();
    font.setPointSize(12);  // Augmenter la taille de la police à 12
    painter.setFont(font);

    // Dessiner les en-têtes de colonnes
    for (int column = 0; column < model->columnCount(); ++column) {
        QRect rect(x + column * columnWidth, y, columnWidth, rowHeight);
        painter.drawText(rect, Qt::AlignCenter, model->headerData(column, Qt::Horizontal).toString());
        painter.drawRect(rect);
    }

    // Dessiner les lignes
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column) {
            QRect rect(x + column * columnWidth, y + (row + 1) * rowHeight, columnWidth, rowHeight);
            painter.drawText(rect, Qt::AlignCenter, model->data(model->index(row, column)).toString());
            painter.drawRect(rect);
        }
    }

    painter.end();
    QMessageBox::information(this, "Export Complete", "Data has been exported to PDF successfully.");
}
void MainWindow::afficherStatistiquesDansMessageBox() {
    // Instanciez la classe Service pour obtenir les statistiques
    Service service;
    QMap<QString, int> stats = service.obtenirStatistiquesModeles();

    // Préparez le texte pour la QMessageBox
    QString message = "Statistiques des modèles de voitures :\n\n";
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        message += QString("Modèle : %1 - Nombre : %2\n").arg(it.key()).arg(it.value());
    }

    // Vérifiez si des statistiques sont disponibles
    if (stats.isEmpty()) {
        message = "Aucune donnée statistique disponible pour les modèles de voitures.";
    }

    // Affichez les statistiques dans une QMessageBox
    QMessageBox::information(this, "Statistiques des modèles", message);
}
void MainWindow::on_pushButton_afficherStats_clicked() {
    afficherStatistiquesDansMessageBox();
}




//matiere
void MainWindow::on_pb_ajouter_clicked()
{
    int Id_matieres = ui->id_mat->text().toInt();
    QString nature_M = ui->nature_m->text();
    QString type_m = ui->type->text();
    int Quantite_M = ui->qt->text().toInt();


    matieres matiere(Id_matieres, nature_M, Quantite_M, type_m);
    bool test = matiere.ajouter();
    QRegularExpression characterRegex("^[a-zA-Zéèêëàâäîïôöûüç -]*$");

       if (!characterRegex.match(nature_M).hasMatch()) {
           QMessageBox::warning(this, tr("Caractères invalides"),
                                tr("Veuillez utiliser uniquement des caractères alphabétiques pour le champ 'Nature'."), QMessageBox::Ok);
           return;
       }

       if (!characterRegex.match(type_m).hasMatch()) {
           QMessageBox::warning(this, tr("Caractères invalides"),
                                tr("Veuillez utiliser uniquement des caractères alphabétiques pour le champ 'Type'."), QMessageBox::Ok);
           return;
       }
    if (test) {
        ui->tab_m->setModel(matiere.afficher());

        // Log the action
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        QString logEntry = QString("[%1] Ajout: Id = %2, Nature = %3, Type = %4, Quantité = %5")
                            .arg(timestamp)
                            .arg(Id_matieres)
                            .arg(nature_M)
                            .arg(type_m)
                            .arg(Quantite_M);
        historique.append(logEntry);

        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Ajout effectué"), QMessageBox::Cancel);
    } else {
        QMessageBox::information(nullptr, QObject::tr("Not OK"), QObject::tr("Ajout non effectué"), QMessageBox::Cancel);
    }
}
void MainWindow::on_pb_ok_clicked()
{
    int id = ui->l_m1->text().toInt();
    QString nature_M = ui->l_m2->text();
    int Quantite_M = ui->l_m3->text().toInt();
    QString type_m = ui->l_m4->text();

    QRegularExpression characterRegex("^[a-zA-Zéèêëàâäîïôöûüç -]*$");

       if (!characterRegex.match(nature_M).hasMatch()) {
           QMessageBox::warning(this, tr("Caractères invalides"),
                                tr("Veuillez utiliser uniquement des caractères alphabétiques pour le champ 'Nature'."), QMessageBox::Ok);
           return;
       }

       if (!characterRegex.match(type_m).hasMatch()) {
           QMessageBox::warning(this, tr("Caractères invalides"),
                                tr("Veuillez utiliser uniquement des caractères alphabétiques pour le champ 'Type'."), QMessageBox::Ok);
           return;
       }
    matieres matier(id, nature_M, Quantite_M, type_m);
    bool test = matier.modifier(id);

    if (test) {
        ui->tab_m->setModel(matier.afficher());

        // Log the action
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        QString logEntry = QString("[%1] Modification: Id = %2, Nature = %3, Type = %4, Quantité = %5")
                            .arg(timestamp)
                            .arg(id)
                            .arg(nature_M)
                            .arg(type_m)
                            .arg(Quantite_M);
        historique.append(logEntry);

        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Modification effectuée"), QMessageBox::Cancel);
    } else {
        QMessageBox::information(nullptr, QObject::tr("Not OK"), QObject::tr("Modification non effectuée"), QMessageBox::Cancel);
    }
}

void MainWindow::on_pb_supprimer_clicked()
{
    matieres matier;
    int id = ui->s_id->text().toInt();
    bool test = matier.supprimer(id);

    if (test) {
        ui->tab_m->setModel(matier.afficher());

        // Log the action
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        QString logEntry = QString("[%1] Suppression: Id = %2")
                            .arg(timestamp)
                            .arg(id);
        historique.append(logEntry);

        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Suppression effectuée"), QMessageBox::Cancel);
    } else {
        QMessageBox::information(nullptr, QObject::tr("Not OK"), QObject::tr("Suppression non effectuée"), QMessageBox::Cancel);
    }
}


void MainWindow::on_tab_m_clicked(const QModelIndex &index)
{
    // Get the row of the clicked item using QModelIndex
    int row = index.row();

    // Populate the form fields based on the clicked row
    ui->id_mat->setText(ui->tab_m->model()->data(ui->tab_m->model()->index(row, 0)).toString());  // ID
    ui->nature_m->setText(ui->tab_m->model()->data(ui->tab_m->model()->index(row, 1)).toString());
    ui->qt->setText(ui->tab_m->model()->data(ui->tab_m->model()->index(row, 2)).toString());
    ui->type->setText(ui->tab_m->model()->data(ui->tab_m->model()->index(row, 3)).toString());

    // Quantity

    // Retrieve the data from the form fields (set after click)
    int Id_matieres = ui->id_mat->text().toInt();
    QString nature_M = ui->nature_m->text();
    QString type_m = ui->type->text();
    int Quantite_M = ui->qt->text().toInt();


    // Concatenate data into a single string (or use a suitable format)
    QString qrData = "ID Matière: " + QString::number(Id_matieres) + "\n"
                     + "Nature: " + nature_M + "\n"
            + "Quantité: " + QString::number(Quantite_M) + "\n"
                     + "Type: " + type_m + "\n";


    // Generate QR Code
    const QrCode qr = QrCode::encodeText(qrData.toStdString().c_str(), QrCode::Ecc::LOW);

    // Save the QR code to a file
    std::ofstream myfile;
    myfile.open("qrcode.svg");
    myfile << qr.toSvgString(1); // Save as SVG
    myfile.close();

    // Load the QR code image (can use any supported format like PNG)
    QPixmap pix("qrcode.svg");

    // Scale the image as needed
    QPixmap scaledPix = pix.scaled(200, 200);  // Resize if necessary

    // Display the QR code image on the label
    ui->QRCODE->setPixmap(scaledPix);
}
void MainWindow::on_pushButton_clicked()
{
    QString nomFichierPDF = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");

              if (!nomFichierPDF.isEmpty()) {
                  QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tab_m->model());

                  if (model) {
                      GenerateurPDF::genererPDF(model, nomFichierPDF);
                  } else {
                      QMessageBox::warning(this, "Erreur", "Le modèle n'est pas de type QSqlQueryModel.");
                  }
              }
}



void MainWindow::on_pushBstat_clicked()
{
    // Create a new dialog to display the charts
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(dialog);


    Stat();

    layout->addWidget(ui->lab_chart_bar);

    dialog->setWindowTitle("MATIERES Statistics");
    dialog->resize(800, 600); // Adjust size as needed

    // Show the dialog
    dialog->exec();
}
void MainWindow::Stat()  {
    // Debug statement to check if the function is being called
    qDebug() << "Stat function called.";

    // Clear the existing layout
    QLayout *layout = ui->lab_chart_bar->layout();
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    // Create the chart
    QChart *chart = new QChart();
    chart->setTitle("Répartition par Quantité");

    // Create the Pie Series
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);

    // Query to fetch quantities
    QSqlQuery query;
    query.prepare("SELECT quantite_m FROM MATIERES");
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return;
    }

    // Initialize counters for each quantity range
    int moreThan5 = 0;
    int lessThan5 = 0;
    int equalTo5 = 0;

    // Process each record's quantity
    while (query.next()) {
        int quantite = query.value("quantite_m").toInt();

        if (quantite > 5) {
            moreThan5++;
        } else if (quantite < 5) {
            lessThan5++;
        } else {
            equalTo5++;
        }
    }

    // Append slices for each category
    if (lessThan5 > 0) {
        QPieSlice *slice = series->append("Less than 5", lessThan5);
        slice->setLabelVisible(true);
        slice->setBrush(Qt::blue);  // Customize color
    }
    if (equalTo5 > 0) {
        QPieSlice *slice = series->append("Equal to 5", equalTo5);
        slice->setLabelVisible(true);
        slice->setBrush(Qt::green);  // Customize color
    }
    if (moreThan5 > 0) {
        QPieSlice *slice = series->append("More than 5", moreThan5);
        slice->setLabelVisible(true);
        slice->setBrush(Qt::red);  // Customize color
    }

    // Add series to the chart
    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignRight);

    // Create the chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Add the chart view to the existing layout
    layout = new QVBoxLayout(ui->lab_chart_bar);
    layout->addWidget(chartView);
    ui->lab_chart_bar->setLayout(layout);

    // Optional: Resize the chart view to match the size of the widget
    chartView->resize(ui->lab_chart_bar->size());

    // Debug statement to check if the chart is added to the layout
    qDebug() << "Chart added to layout.";
}
void MainWindow::on_radioButton_clicked()
{
    matieres m;
        QSqlQueryModel* sortedModel = m.trier(true);

        // Now you can use the 'sortedModel' as needed, for example, set it to a QTableView
        if (sortedModel) {
            ui->tab_m->setModel(sortedModel);
        } else {
            // Handle the case where an error occurred during sorting
            qDebug() << "Error sorting matieres data.";
        }
}

void MainWindow::on_radioButton_2_clicked()
{
    matieres m;
       QSqlQueryModel* sortedModel = m.trier(false);

       // Now you can use the 'sortedModel' as needed, for example, set it to a QTableView
       if (sortedModel) {
           ui->tab_m->setModel(sortedModel);
       } else {
           // Handle the case where an error occurred during sorting
           qDebug() << "Error sorting matieres data in descending order.";
       }
}


void MainWindow::on_pushBhm_clicked()
{

}

void MainWindow::on_pb_ajouterr_clicked()
{
       ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pb_modifierr_2_clicked()
{
       ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pb_supprimerr_clicked()
{
       ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButton_10_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_radioButtonid_clicked()
{
    QString idFilter = ui->lineEdit->text().trimmed();

    matieres matiereInstance; // Create an instance of the `matieres` class
    QSqlQueryModel *originalModel = matiereInstance.afficher();
    int numRows = originalModel->rowCount();
    int numCols = originalModel->columnCount();


    QStandardItemModel *filteredModel = new QStandardItemModel(this);

    // Set the horizontal header labels
    QStringList headerLabels;
    for (int col = 0; col < numCols; ++col) {
        headerLabels << originalModel->headerData(col, Qt::Horizontal).toString();
    }
    filteredModel->setHorizontalHeaderLabels(headerLabels);

    // Iterate over the rows of the original model to filter data
    for (int row = 0; row < numRows; ++row) {
        QModelIndex index = originalModel->index(row, 0); // Column 0 for 'id_e'
        QString data = originalModel->data(index).toString();
        if (data.contains(idFilter, Qt::CaseInsensitive)) { // Case-insensitive filter
            QList<QStandardItem *> items;
            for (int col = 0; col < numCols; ++col) {
                items.append(new QStandardItem(originalModel->data(originalModel->index(row, col)).toString()));
            }
            filteredModel->appendRow(items);
        }
    }

    // Set the filtered model to the table view
    ui->tab_m->setModel(filteredModel);
}

void MainWindow::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
    QString idFilter = ui->lineEdit->text().trimmed();

    matieres matiereInstance; // Create an instance of the `matieres` class
    QSqlQueryModel *originalModel = matiereInstance.afficher();
    int numRows = originalModel->rowCount();
    int numCols = originalModel->columnCount();

    // Create a new standard item model to hold filtered data
    QStandardItemModel *filteredModel = new QStandardItemModel(this);

    // Set the horizontal header labels
    QStringList headerLabels;
    for (int col = 0; col < numCols; ++col) {
        headerLabels << originalModel->headerData(col, Qt::Horizontal).toString();
    }
    filteredModel->setHorizontalHeaderLabels(headerLabels);


    for (int row = 0; row < numRows; ++row) {
        QModelIndex index = originalModel->index(row, 0);
        QString data = originalModel->data(index).toString();
        if (data.contains(idFilter, Qt::CaseInsensitive)) { // Case-insensitive filter
            QList<QStandardItem *> items;
            for (int col = 0; col < numCols; ++col) {
                items.append(new QStandardItem(originalModel->data(originalModel->index(row, col)).toString()));
            }
            filteredModel->appendRow(items);
        }
    }

    // Set the filtered model to the table view
    ui->tab_m->setModel(filteredModel);
}

void MainWindow::on_historique_clicked()
{
    if (historique.isEmpty()) {
        QMessageBox::information(nullptr, "Historique", "Aucune action enregistrée.", QMessageBox::Close);
        return;
    }

    QString fullHistory = "<b>Historique des actions:</b><br>";
    for (const QString &log : historique) {
        fullHistory += log + "<br><br>";
    }

    QMessageBox *msgBox = new QMessageBox;
    msgBox->setWindowTitle("Historique des Actions");
    msgBox->setText(fullHistory);
    msgBox->setStandardButtons(QMessageBox::Close);
    msgBox->setStyleSheet("QMessageBox { font-family: Arial, sans-serif; font-size: 12pt; }");

    msgBox->show();
    QTimer::singleShot(5000, msgBox, &QMessageBox::close);
}

void MainWindow::on_notif_clicked()
{
    checkQuantities();

}

void MainWindow::on_upload_image_2_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Sélectionnez une image", "", "Images (*.png *.jpg *.jpeg)");
    if (filePath.isEmpty()) {
        return; // Aucun fichier sélectionné
    }

    QString encodedImage = encodeImageToBase64(filePath);

    // Affecter l'image encodée à un objet pièce
    int id = ui->idBox_2->text().toInt();
    int reff = ui->reff->text().toInt();
    int quant = ui->quant->text().toInt();
    int prix = ui->prix->text().toInt();
    QString etat = ui->etat->currentText();

    piece p(id, reff, quant, prix, etat, encodedImage);
    if (p.ajouter()) {
        QMessageBox::information(this, "Succès", "Pièce ajoutée avec image.");
        ui->tableView_2->setModel(p.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la pièce.");
    }
}

void MainWindow::on_ajouterMatBox_2_clicked()
{
    int id = ui->idBox_2->text().toInt();
    int reff = ui->reff->text().toInt();
    int quant = ui->quant->text().toInt();
    int prix = ui->prix->text().toInt();
    QString etat = ui->etat->currentText();

    piece p(id, reff, quant, prix,etat,"");
    bool test = p.ajouter();

    if (test) {
        QMessageBox::information(this, "RDI Center", "Ajout effectué avec succès.");
        afficherStatistiques();
        showNotification("ajout de Livraison","");

        ui->tableView->setModel(p.afficher());
        // Clearing input fields
        ui->idBox_2->clear();
        ui->reff->clear();
        ui->quant->clear();
        ui->prix->clear();
    } else {
        QMessageBox::critical(this, "RDI Center", "Ajout non effectué.");
    }
}

void MainWindow::on_pushButton_11_clicked()
{
    if(ui->tableView_2->currentIndex().row()==-1)
        QMessageBox::information(nullptr, QObject::tr("QR code"),
                                 QObject::tr("Veuillez Choisir un CODE du Tableau.\n"
                                             "Click Ok to exit."), QMessageBox::Ok);
    else
    {

        int id=ui->tableView_2->model()->data(ui->tableView_2->model()->index(ui->tableView_2->currentIndex().row(),0)).toInt();
        const QrCode qr = QrCode::encodeText(std::to_string(id).c_str(), QrCode::Ecc::LOW);
        std::ofstream myfile;
        myfile.open ("qrcode.svg") ;
        myfile << qr.toSvgString(1);
        myfile.close();
        //QSvgRenderer svgRenderer(QString("qrcode.svg"));
        QPixmap pix( QSize(90, 90) );
        QPainter pixPainter( &pix );
        // svgRenderer.render( &pixPainter );
       // ui->QRCODE_3->setPixmap(pix);
    }
}

void MainWindow::on_ActualiserMatBox_clicked()
{
    ui->tableView_2->setModel(p.afficher());

}

void MainWindow::on_modifMatBox_clicked()
{
    int id = ui->idBox_4->text().toInt();
    int reff = ui->reffmodif->text().toInt();
    int quant = ui->quantmodif->text().toInt();
    int prix = ui->prixmodif->text().toInt();
    QString etat = ui->etat_m->currentText();

    piece m(id, reff, quant, prix,etat,"");
    bool check = m.modifier();

    if (check) {
        QMessageBox::information(this, "RDI Center", "Modification effectuée avec succès.");
        ui->tableView_2->setModel(m.afficher());
        showNotification("modifier de Livraison","success");

        // Clearing input fields
        ui->idBox_4->clear();
        ui->reffmodif->clear();
        ui->quantmodif->clear();
        ui->prixmodif->clear();
    } else {
        QMessageBox::critical(this, "RDI Center", "Modification échouée.");
    }
}

void MainWindow::on_pdf_2_clicked()
{
    // Ouvrir une boîte de dialogue pour choisir l'emplacement du fichier PDF
    QString fichierPDF = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "*.pdf");

    if (!fichierPDF.isEmpty()) {
        if (!fichierPDF.endsWith(".pdf", Qt::CaseInsensitive)) {
            fichierPDF += ".pdf";  // Ajouter l'extension si elle n'est pas fournie
        }

        // Générer le PDF à l'emplacement choisi
        p.genererPDF(fichierPDF);

        // Informer l'utilisateur que le PDF a été généré avec succès
        QMessageBox::information(this, "Succès", "Le PDF a été généré avec succès.");
    } else {
        // Si l'utilisateur annule l'opération
        QMessageBox::warning(this, "Annulé", "La génération du PDF a été annulée.");
    }
}

void MainWindow::on_trie_2_clicked()
{
    QString critere = ui->tri_2->currentText();

    if (critere != "id" && critere != "prix") {
        QMessageBox::warning(this, "Erreur", "Critère de tri invalide. Choisissez 'id' ou 'prix'.");
        return;
    }

    QSqlQueryModel *model = p.trier(critere);
    ui->tableView_2->setModel(model);
}

void MainWindow::on_recherche_2_clicked()
{
    QString critere = ui->rech_2->currentText();
    QString valeur = ui->valeur_r_2->text();


    QSqlQueryModel *model = p.rechercher(critere, valeur);
    ui->tableView_2->setModel(model);
}

void MainWindow::on_deleteBox_clicked()
{
    int id = ui->suppression_field->text().toInt();
    bool test = p.supprimer(id);

    if (test) {
        QMessageBox::information(nullptr, "RDI Center", "Suppression effectuée.");
        ui->tableView_2->setModel(p.afficher());
    } else {
        QMessageBox::critical(nullptr, "RDI Center", "Suppression non effectuée.");
    }
    ui->suppression_field->clear();
}

void MainWindow::on_afficher_image_clicked()
{
    int id = ui->id_image->text().toInt(); // Récupère l'ID depuis un champ d'entrée (par exemple, un QLineEdit)

    piece p;
    QString base64Data = p.getImageById(id); // Récupère l'image en Base64 depuis la base de données

    if (base64Data.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image trouvée pour cet ID.");
        return;
    }

    QByteArray imageData = QByteArray::fromBase64(base64Data.toUtf8()); // Décodage Base64
    QPixmap pixmap;
    if (!pixmap.loadFromData(imageData)) {
        QMessageBox::critical(this, "Erreur", "Échec du chargement de l'image.");
        return;
    }

    ui->imm->setPixmap(pixmap.scaled(ui->imm->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)); // Affiche l'image
}
QString MainWindow::encodeImageToBase64(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir l'image.");
        return "";
    }

    QByteArray imageData = file.readAll(); // stocker les datas en binaires

    file.close();
    return imageData.toBase64(); // Convertir en Base64
}

void MainWindow::reinitialiserChamps() {
    ui->lineEdit_2->clear();      // Réinitialise l'état de la commande
    ui->lineEdit_3->clear();      // Réinitialise le montant total
    ui->lineEdit_6->clear();      // Réinitialise l'ID de commande pour suppression/modification
    ui->dateEdit->clear();        // Réinitialise la date de commande
    ui->dateEdit_3->clear();      // Réinitialise la date de livraison
}

// Fonction pour ajouter une commande
void MainWindow::on_ajouterButton2_clicked() {
    QDate dateCommande = ui->dateEdit->date();
    QString etatCommande = ui->lineEdit_2->text();

    // Contrôle du montant total
    bool montantOk;
    int montantTotal = ui->lineEdit_3->text().toInt(&montantOk);
    QDate dateLivraison = ui->dateEdit_3->date();

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
        ui->tableView_3->setModel(commande.afficher()); // Actualiser l'affichage des commandes
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
        ui->tableView_3->setModel(commande.afficher()); // Actualiser l'affichage des commandes
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
    QDate dateLivraison = ui->dateEdit_3->date();

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
        ui->tableView_3->setModel(commande.afficher());
        ui->tableView_3->resizeColumnsToContents();
        reinitialiserChamps();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification des informations de la commande.");
    }
    updateModel();
}

void MainWindow::on_trierButton_clicked() {
    // Retrieve the selected criterion and sort order
    QString critere = ui->comboBoxTri_2->currentText();
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
        proxyModel->setSourceModel(ui->tableView_3->model());
        ui->tableView_3->setModel(proxyModel);
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
    ui->graphicsView_2->setScene(new QGraphicsScene());

    // Add the chart view to the scene
    ui->graphicsView_2->scene()->addWidget(chartView);

    // Adjust the view to fit the scene
    ui->graphicsView_2->fitInView(ui->graphicsView_2->scene()->sceneRect(), Qt::KeepAspectRatio);

    // Refresh the scene and display the chart
    ui->graphicsView_2->scene()->update();
    ui->graphicsView_2->show();
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
    ui->graphicsView_2->setScene(new QGraphicsScene());

    // Add the chart view to the scene
    ui->graphicsView_2->scene()->addWidget(chartView);

    // Use fitInView to make sure the chart fits within the view's scene, but keep the aspect ratio
    ui->graphicsView_2->fitInView(ui->graphicsView_2->scene()->sceneRect(), Qt::KeepAspectRatio);

    // Refresh the scene and display the chart
    ui->graphicsView_2->scene()->update();
    ui->graphicsView_2->show();
}
void MainWindow::on_loadHistoriqueButton_clicked()
{
    // Récupérer le QListView qui se trouve dans l'onglet "Historique"
    QListView *historiqueListView = ui->commande->findChild<QListView*>("historiqueListView");

    // Charger et afficher l'historique dans le QListView
    cmp.loadHistorique(historiqueListView);
}

void MainWindow::updateModel() {
    Commande commande; // Create an instance of Commande to fetch data
    QSqlQueryModel* model = commande.afficher(); // Fetch the model from the database
    proxyModel->setSourceModel(model); // Set the source model for the proxy
    ui->tableView_3->setModel(proxyModel); // Attach the proxy model to the table view
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
    ui->label_79->setText("État de la commande: " + state);

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
    query.prepare("UPDATE FLAMME SET flameStatus = :flameStatus");

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

    if (query.exec("SELECT flameStatus FROM FLAMME")) {
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


