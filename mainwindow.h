#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QModelIndex>
#include <QMainWindow>
//#include "client.h"
#include "smtpclient.h"
#include <QTcpSocket>
#include <QSslSocket>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
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
    //void trierTableView(QString columnName, QString ordre);
    void on_pdf_clicked();
    void appliquerTri(const QString &columnName, const QString &ordre);
    void on_envoyermailButton_clicked();
    void on_annuler_2_clicked();
    void on_statis_clicked();
    void on_effmail_clicked();
private:
    Ui::MainWindow *ui;
    void loadClientDetailsForModification(int code_client);
    void trierTableView(QString columnName, QString ordre);
    //bool estAlphabetique(const QString& texte);
    QStringList attachments;
    void setupConnections();
    //Smtp *smtpClient;
};

#endif // MAINWINDOW_H
