#include "dialog.h"
#include "perso.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->connexion, &QPushButton::clicked, this, &Dialog::on_loginButton_clicked);



}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::connecttoapp(){
    this->hide();
    accept();


}




void Dialog::on_loginButton_clicked()
{
    QString identifiant = ui->email->text();  // Get the email input from the user
    QString mdp = ui->password->text();  // Get the password input from the user
    // Instantiate an 'Employe' object to use the method for checking credentials
    perso emp;
    QRegularExpression regex("^\\d+$");  // ^\\d+$ signifie que toute la chaîne doit être composée de chiffres uniquement

    // Vérification des champs vides
    if (identifiant.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'identifiant est vide.");
        return;
    }
    if (mdp.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le mot de passe est vide.");
        return;
    }

    // Vérification que l'identifiant ne contient que des chiffres
    if (!regex.match(identifiant).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'identifiant ne doit contenir que des chiffres.");
        return;
    }

    // Vérification que le mot de passe ne contient que des chiffres
    if (!regex.match(mdp).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le mot de passe ne doit contenir que des chiffres.");
        return;
    }

    bool loginSuccess1 = emp.getemployeemailpassword(identifiant, mdp);

    if (loginSuccess1) {
        // If login is successful, display success message
        ui->label->setText("Login successful!");
        ui->label->setStyleSheet("color: green;");  // Set text color to green

        // Get the welcome message with the user's name
        QString welcomeMessage = getEmployeName();
        // Set the welcome message in label_2 after successful login
        if (!welcomeMessage.isEmpty()) {
            ui->label->setText(welcomeMessage);  // Display the welcome message
            ui->label->setStyleSheet("color: green;");  // Optionally, set green color
        } else {
            ui->label->setText("Failed to retrieve user name.");
            ui->label->setStyleSheet("color: red;");
        }
        // Simulate a delay while keeping the UI responsive
        for (int i = 0; i < 5000000; i++) {
            QCoreApplication::processEvents();  // Keeps the UI responsive
        }

        // After the delay, close the dialog
        accept();  // Close the dialog and return accepted status
    } else {
        // No matching record found, display an error message
        ui->label->setText("Incorrect username or password.");
        ui->label->setStyleSheet("color: red;"); // Set text color to red

        // Simulate delay while keeping the UI responsive
        for (int i = 0; i < 1000000; i++) {
            QCoreApplication::processEvents();  // Keeps the UI responsive
        }

        // After the delay, reset the label text
        ui->label->setText("");  // Clear the error message
    }
}


QString Dialog::getEmployeName()
{
    QString userName;
    QString login = ui->email->text();  // Get the email input from the user

    // Prepare the query to fetch the user's name based on the email (id_p)
    QSqlQuery query;
    query.prepare("SELECT poste FROM personnel WHERE id_p = :login");
    query.bindValue(":login", login);  // Bind the login value to the query

    // Execute the query and check if it succeeded
    if (query.exec() && query.next()) {  // Ensure the query is successful and move to the first result
        // If the query is successful and we have a result, retrieve the name from the result
        userName = query.value("nom").toString();
    } else {
        // If the query fails or there are no results, print the error to the debug log
        qDebug() << "Failed to retrieve user name:" << query.lastError().text();
        return "";  // Return empty if not found
    }

    // Return the message with the user's name
    QString welcomeMessage = "Bienvenue " + userName + " dans notre application!";
    return welcomeMessage;
}

int Dialog::getEmail() const {
    return ui->email->text().toInt();  // Return the email entered in the QLineEdit
}





