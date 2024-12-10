#ifndef IHMGRAPHIQUE_H
#define IHMGRAPHIQUE_H

#include <QtWidgets>
#include <QtCharts>

class IhmGraphique : public QMainWindow
{
    Q_OBJECT

private:
    QChartView *graphique; // un widget pour afficher un graphe
    QChart *graphe; // la représentation d'un graphe
    QLineSeries *courbe; // les données

public:
    IhmGraphique(QWidget *parent = 0);
    ~IhmGraphique();
};

#endif // IHMGRAPHIQUE_H
