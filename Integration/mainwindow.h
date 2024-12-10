#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "perso.h"
#include <QMainWindow>
#include <QLineEdit>  // Include QLineEdit if not already included
#include <QList>
#include <QPair>
#include <QDateTime>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include "connexion.h"
#include "perso.h"
#include <QDialog>
#include <QModelIndex>
#include <QMainWindow>
#include "QMessageBox"
//#include "client.h"
#include "service.h"
#include "smtpclient.h"
#include <QTcpSocket>
#include <QSslSocket>

#include <QLCDNumber>
#include "piece.h"

#include <QSystemTrayIcon> // bibliotheque predéfinie et pour inserer icon
#include <QMenu>
#include <QAction>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include"commande.h"
#include "qserialport.h"
#include "qsortfilterproxymodel.h"
#include <QGraphicsView>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int email, QWidget *parent = nullptr);  // Explicit keyword added
    ~MainWindow();

    void populateEmployeeInfo();
    void privilage();


private:
    Ui::MainWindow *ui;
    Service service;
    int userEmail;
    QLineEdit *searchBar;  // Declare searchBar as a pointer
    perso pro;
    QList<QString> historique; ;
     QMessageBox *msgBox;


     piece p;
     QSystemTrayIcon *trayIcon;        // Déclaration de l'icône du tray
     QMenu *trayIconMenu;              // Déclaration du menu contextuel pour l'icône
     void setupTrayIcon();
     QString encodeImageToBase64(const QString &filePath); // Fonction pour encoder une image en Base64

    void on_addEmployeeButton_clicked();
    void on_supprimerEmploye_clicked();
    void on_modifyEmployeeButton_clicked();
    void on_btn_export_pdf_clicked();
void displayGenderStatistics();
void refreshGenderStatistics();
void displayHiringPeriodStatistics();
void refreshStatistics();
void on_idEmpChanged();
void onIdChanged();
void on_soumettreButton_clicked();
void on_acceptation_clicked();
void on_refuser_clicked();
void refreshTableView();
void filterServices();
//client
void loadClientDetailsForModification(int code_client);
//bool estAlphabetique(const QString& texte);
QStringList attachments;
void setupConnections();
//Smtp *smtpClient;
   //commande
    void on_ajouterButton2_clicked();
    void on_supprimerButton_clicked();
    void on_modifierButton_clicked();
    void reinitialiserChamps();
    void on_trierButton_clicked();
    void reapplySorting();
    void on_exportPdfButton_clicked();
    void displayOrdersPerMonthStatistics();
    void displayOrderStatusStatistics();
    void on_loadHistoriqueButton_clicked();
    void updateModel();
    void setupTimeline(const QStringList &labels, const QStringList &dates, int activeStep);
    void on_suiviButton_clicked();
    void setupArduino();
    void readFromArduino();
    int flameStatus;
    void updateFlammeTable(int flameStatus);
    int readFlammeStatusFromDB();
    void controlBuzzer(int flameStatus);
    void updateBuzzerStatus();
    void setupTimer();

    QSortFilterProxyModel *proxyModel; // Declare 'proxyModel' afterProxy model for filtering and sorting
       Commande cmp;
       QGraphicsScene *scene;
       QGraphicsScene *timelineScene;
       QSerialPort *arduino;


//commande





void onUpdateTypeCongeClicked();
public slots:
void on_searchButton_clicked();
    void on_comboBox_tri_changed(int index);
    void onSelectionChanged(const QItemSelection &selected);
    //client
private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifierClient_clicked();
    void on_affclient_clicked(const QModelIndex &index);
    void on_pushButton_refresh_clicked();
    void on_annulercl_clicked();
    void on_annulermodif_clicked();
    void on_fichiersButton_clicked();
    void sendSMS(const QString &toNumber, const QString &message);
    void on_envoyersms_clicked();
    void on_pushButton_recherche_clicked();
   // void trierTableView(QString columnName, QString ordre);
    void on_pdf_clicked();
    void appliquerTri(const QString &columnName, const QString &ordre);
    void on_envoyermailButton_clicked();
    void on_annuler_2_clicked();
    void on_statis_clicked();
    void on_effmail_clicked();
    void on_supprimerServiceButton_clicked();
    void on_modifierServiceButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_comboBoxTriChanged(int index);
    void on_pushButton_generatePDF_clicked();
    void afficherStatistiquesDansMessageBox();
    void on_pushButton_afficherStats_clicked();



    void on_ajouterServiceButton_clicked();
    void on_pushButton_2_clicked();

    //matiere

    void on_pb_ajouter_clicked();

    void on_pb_ok_clicked();

    void on_pb_supprimer_clicked();
    void on_tab_m_clicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void on_pushBstat_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();

    void on_pushBhm_clicked();

    void on_pb_ajouterr_clicked();

    void on_pb_modifierr_2_clicked();

    void on_pb_supprimerr_clicked();

    void on_pushButton_10_clicked();

    void on_radioButtonid_clicked();

    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);
void Stat();
void on_historique_clicked();

//piece
void checkQuantities();
void showNotification(const QString &title, const QString &message);

void afficherStatistiques() ;


void on_notif_clicked();
void on_upload_image_2_clicked();
void on_ajouterMatBox_2_clicked();
void on_pushButton_11_clicked();
void on_ActualiserMatBox_clicked();
void on_modifMatBox_clicked();
void on_pdf_2_clicked();
void on_trie_2_clicked();
void on_recherche_2_clicked();
void on_deleteBox_clicked();
void on_afficher_image_clicked();
};
#endif // MAINWINDOW_H
