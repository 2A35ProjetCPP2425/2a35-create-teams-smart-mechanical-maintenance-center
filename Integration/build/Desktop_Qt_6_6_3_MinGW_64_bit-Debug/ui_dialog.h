/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QPushButton *connexion;
    QLabel *label;
    QLineEdit *email;
    QLineEdit *password;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(1003, 463);
        Dialog->setStyleSheet(QString::fromUtf8("/* Style g\303\251n\303\251ral pour la fen\303\252tre */\n"
"QWidget {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1e1e2f, stop:1 #2e2e3f);\n"
"    color: #ffffff;\n"
"    font-family: 'Segoe UI', Arial, sans-serif;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Style des champs de texte (identifiant, password) */\n"
"QLineEdit {\n"
"    background-color: #2e2e3f;\n"
"    color: #ffffff;\n"
"    border: 2px solid #3a3f55;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"    transition: all 0.3s ease-in-out;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #7289da; /* Bordure color\303\251e lorsqu'on s\303\251lectionne le champ */\n"
"    background-color: #40455a; /* Changement de couleur du fond */\n"
"}\n"
"\n"
"/* Style des boutons */\n"
"QPushButton {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #4e8cff, stop:1 #375e97);\n"
"    color: #ffffff;\n"
"    border: 2px solid #3a3f55;\n"
"    border-radius:"
                        " 8px;\n"
"    padding: 10px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.25);\n"
"    transition: all 0.3s ease-in-out;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #6aa6ff, stop:1 #4e8cff);\n"
"    transform: scale(1.05); /* Effet zoom */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #375e97, stop:1 #2d4d7a);\n"
"    transform: scale(0.95); /* Effet d'enfoncement */\n"
"}\n"
"\n"
"/* Style des labels (titre et slogan) */\n"
"QLabel#title {\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: #00d4ff;\n"
"    text-align: center;\n"
"    text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5);\n"
"}\n"
"\n"
"QLabel#subtitle {\n"
"    font-size: 16px;\n"
"    color: #b0b3c5;\n"
"    text-align: center;\n"
"    margin-top: 5px;\n"
"    text-shadow: 1px 1px 3px rgba(0, 0, 0, 0.3);\n"
"}\n"
"\n"
"/* Style pour les boutons "
                        "\"connexion\" et \"connecter client\" */\n"
"QPushButton#connectButton {\n"
"    background: #3a8dff;\n"
"    color: #ffffff;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    padding: 10px 20px;\n"
"    border-radius: 8px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#clientButton {\n"
"    background: #ff8d3a;\n"
"    color: #ffffff;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    padding: 10px 20px;\n"
"    border-radius: 8px;\n"
"    border: none;\n"
"}\n"
"\n"
"/* Animation des boutons */\n"
"QPushButton:hover#connectButton,\n"
"QPushButton:hover#clientButton {\n"
"    filter: brightness(1.2);\n"
"    transform: scale(1.03);\n"
"}\n"
"\n"
"QPushButton:pressed#connectButton,\n"
"QPushButton:pressed#clientButton {\n"
"    transform: scale(0.97);\n"
"    filter: brightness(0.9);\n"
"}\n"
"\n"
"/* Ajout d'ombre autour des widgets */\n"
"QLineEdit, QPushButton {\n"
"    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.2);\n"
"}\n"
"\n"
"/* Transitions pour un effet fluide */\n"
"QWidget * {\n"
" "
                        "   transition: all 0.3s ease;\n"
"}\n"
""));
        connexion = new QPushButton(Dialog);
        connexion->setObjectName("connexion");
        connexion->setGeometry(QRect(580, 320, 261, 61));
        label = new QLabel(Dialog);
        label->setObjectName("label");
        label->setGeometry(QRect(580, 230, 271, 41));
        email = new QLineEdit(Dialog);
        email->setObjectName("email");
        email->setGeometry(QRect(590, 80, 251, 51));
        password = new QLineEdit(Dialog);
        password->setObjectName("password");
        password->setGeometry(QRect(590, 160, 251, 51));
        password->setEchoMode(QLineEdit::Password);
        label_3 = new QLabel(Dialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(120, 240, 321, 31));
        label_3->setStyleSheet(QString::fromUtf8("font: 700 11pt \"Segoe UI\";"));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(90, 90, 421, 151));
        label_4->setStyleSheet(QString::fromUtf8("font: 700 11pt \"Segoe UI\";"));
        label_4->setTextFormat(Qt::MarkdownText);
        label_4->setPixmap(QPixmap(QString::fromUtf8("images/autogenius-logo.png")));
        label_4->setScaledContents(true);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        connexion->setText(QCoreApplication::translate("Dialog", "connexion", nullptr));
        label->setText(QString());
        email->setPlaceholderText(QCoreApplication::translate("Dialog", "identifiant", nullptr));
        password->setPlaceholderText(QCoreApplication::translate("Dialog", "password", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "\"Entretien facile conduite tranquille\"", nullptr));
        label_4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
