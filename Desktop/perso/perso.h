#ifndef PERSO_H
#define PERSO_H
#include <QString>
#include <QComboBox>
#include <QFileDialog>
#include "connexion.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QRandomGenerator>
#include <QCalendarWidget>
#include <QDate>
#include <QBrush>
#include <QColor>

class perso
{




private:
    int id_p;
    QString nom,prenom,email,poste,mdp,sexe;
    float salaire;


public:
    QDate date_n;
    perso();
    perso(int, QString, QString , QString , QString , QString , QString , float , QDate );
    bool ajout();
    int generateIdentifiant();
    QString generateNumericPassword();
    bool idExists(QString id);
    bool supprimer(QString id_p);
    QSqlQueryModel *afficher();
    bool modifier(QString nom, QString prenom, QString sexe, QString email, float salaire, QString poste, QDate date_n, QString id);
    QSqlQueryModel* tri(int index);


    int getid_p(){return id_p;}
    float getsalaire(){return salaire;}
    QString getnom (){return nom;};
    QString getprenom(){return prenom;};
    QString getemail(){return email;};
    QString getposte(){return poste;};
    QString getmdp(){return mdp;};
    QString getsexe(){return sexe;};





    bool getemployeemailpassword(const QString &id_p, const QString &mdp);
    QString getUserRole(const QString &id_p);
    void setidpersonnel(int id){id_p=id;}
    void setsalaire(int sal){salaire=sal;}
    void setnom(QString n){nom=n;}
    void setprenom(QString pre){prenom=pre;}
    void setemail(QString mail ){email=mail;}
    void setposte( QString post){poste=post;}
    void setsexe(QString sex){sexe=sex;}
    void setmdp(QString pass){mdp=pass;}
    QSqlQueryModel* rechercher(QString id);
    QSqlQueryModel* tri(Qt::SortOrder order, int columnIndex);


    bool exporterPDF(int id_p, const QString &filePath);
    bool getEmployeeDetailsById(const QString &id_emp, QString &nom, QString &prenom, QString &email, QString &poste);

    int generateId();

      QString generateLeaveDecision();
      bool ajouterconge(int id_p, const QString &nom, const QString &prenom,
                               const QString &email, const QString &raison, const QString &poste,
                               const QDate &date_d, const QDate &date_f);
        QSqlQueryModel * afficherconge();
        bool updateCongeStatus(const QString &id_conge, const QString &etat);
        bool hasOngoingLeave(int id_emp);
        void updateTypeConge();
        QString getPosteById(int id_p);





};

#endif // PERSO_H
