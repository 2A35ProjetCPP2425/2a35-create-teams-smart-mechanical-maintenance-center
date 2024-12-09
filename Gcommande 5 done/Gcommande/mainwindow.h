#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    
private:
    void on_ajouterButton_clicked();
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


private:
    Ui::MainWindow *ui; // Declare 'ui' first
    QSortFilterProxyModel *proxyModel; // Declare 'proxyModel' afterProxy model for filtering and sorting
    Commande cmp;
    QGraphicsScene *scene;
    QGraphicsScene *timelineScene;
    QSerialPort *arduino;  // Pointer to the serial port object

};
#endif // MAINWINDOW_H
