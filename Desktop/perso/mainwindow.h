#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "perso.h"
#include <QMainWindow>

#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include "connexion.h"
#include "perso.h"
#include <QDialog>
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
    int userEmail;
    perso pro;
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




void onUpdateTypeCongeClicked();
public slots:
void on_searchButton_clicked();
    void on_comboBox_tri_changed(int index);
    void onSelectionChanged(const QItemSelection &selected);





};
#endif // MAINWINDOW_H
