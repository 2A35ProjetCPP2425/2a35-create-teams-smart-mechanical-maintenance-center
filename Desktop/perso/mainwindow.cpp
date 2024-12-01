#include "mainwindow.h"
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

MainWindow::MainWindow(int email, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    userEmail(email)
{
    ui->setupUi(this);




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




void MainWindow::privilage() {
    // Créer une instance de la classe perso
    perso pro;
    // Obtenez le rôle de l'utilisateur en appelant la fonction
    QString  role = pro.getPosteById(userEmail);
    qDebug() << "Email passed to MainWindow:" << role;

        if (role == "personel"){
            ui->tabWidget_6->setCurrentIndex(0);
            ui->tabWidget_6->setTabEnabled(1, false);  // Index starts at 0, so 1 corresponds to tab 2
            ui->tabWidget_6->setTabEnabled(2, false);  // Disable tab 3
            ui->tabWidget_6->setTabEnabled(3, false);  // Disable tab 4
            ui->tabWidget_6->setTabEnabled(4, false);
            ui->tabWidget_6->setTabEnabled(5, false);// Disable tab 5
        }
        if (role == "client"){
            ui->tabWidget_6->setCurrentIndex(1);
            ui->tabWidget_6->setTabEnabled(0, false);  // Index starts at 0, so 1 corresponds to tab 2
            ui->tabWidget_6->setTabEnabled(2, false);  // Disable tab 3
            ui->tabWidget_6->setTabEnabled(3, false);  // Disable tab 4
            ui->tabWidget_6->setTabEnabled(4, false);
            ui->tabWidget_6->setTabEnabled(5, false);// Disable tab 5
        }
        if (role == "service"){
            ui->tabWidget_6->setCurrentIndex(2);
            ui->tabWidget_6->setTabEnabled(0, false);  // Index starts at 0, so 1 corresponds to tab 2
            ui->tabWidget_6->setTabEnabled(1, false);  // Disable tab 3
            ui->tabWidget_6->setTabEnabled(3, false);  // Disable tab 4
            ui->tabWidget_6->setTabEnabled(4, false);
            ui->tabWidget_6->setTabEnabled(5, false);// Disable tab 5
        }
        if (role == "piece"){
            ui->tabWidget_6->setCurrentIndex(3);
            ui->tabWidget_6->setTabEnabled(0, false);  // Index starts at 0, so 1 corresponds to tab 2
            ui->tabWidget_6->setTabEnabled(1, false);  // Disable tab 3
            ui->tabWidget_6->setTabEnabled(2, false);  // Disable tab 4
            ui->tabWidget_6->setTabEnabled(4, false);
            ui->tabWidget_6->setTabEnabled(5, false);// Disable tab 5
        }
        if (role == "commande"){
            ui->tabWidget_6->setCurrentIndex(4);
            ui->tabWidget_6->setTabEnabled(0, false);  // Index starts at 0, so 1 corresponds to tab 2
            ui->tabWidget_6->setTabEnabled(1, false);  // Disable tab 3
            ui->tabWidget_6->setTabEnabled(2, false);  // Disable tab 4
            ui->tabWidget_6->setTabEnabled(3, false);
            ui->tabWidget_6->setTabEnabled(5, false);// Disable tab 5
        }
        if (role == "equipement"){
            ui->tabWidget_6->setCurrentIndex(5);
            ui->tabWidget_6->setTabEnabled(0, false);  // Index starts at 0, so 1 corresponds to tab 2
            ui->tabWidget_6->setTabEnabled(1, false);  // Disable tab 3
            ui->tabWidget_6->setTabEnabled(2, false);  // Disable tab 4
            ui->tabWidget_6->setTabEnabled(3, false);
            ui->tabWidget_6->setTabEnabled(4, false);// Disable tab 5
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

