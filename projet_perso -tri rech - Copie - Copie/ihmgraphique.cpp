#include "ihmgraphique.h"
#include <QSqlQuery>
#include <QValueAxis>
#include <QPainter>
#include <QDebug>

IhmGraphique::IhmGraphique(QWidget *parent)
    : QMainWindow(parent)
{
    QSqlQuery query("SELECT * FROM matieres ORDER BY QUANTITE_M");

    int index = 1;

    courbe = new QLineSeries();

    while (query.next()) {
        double QUANTITE_M = query.value("QUANTITE_M").toDouble();

        QString indexFromQuery = query.value("NATURE_M").toString();
        bool ok;
        qreal xValue = indexFromQuery.toDouble(&ok);

        if (ok) {
            courbe->append(xValue, QUANTITE_M);
        } else {
            qDebug() << "Failed to convert x-coordinate to double for index: " << index;
        }

        index++;
    }

    // Set point labels visible
    courbe->setPointsVisible(true);
    courbe->setPointLabelsVisible(true);
    courbe->setPointLabelsFormat("@yPoint");  // Show y-value as point label

    graphe = new QChart();
    graphe->addSeries(courbe);
    graphe->setTitle("Graph of Matieres");

    QValueAxis *axeX = new QValueAxis;
    axeX->setTickCount(index);
    axeX->setLabelFormat("%i");
    graphe->addAxis(axeX, Qt::AlignBottom);
    courbe->attachAxis(axeX);

    QValueAxis *axeY = new QValueAxis;
    axeY->setRange(0, 100);  // Adjust this range based on your data
    axeY->setLabelFormat("%.1f");
    axeY->setTitleText(QString::fromUtf8("QUANTITE_M"));
    graphe->addAxis(axeY, Qt::AlignLeft);

    courbe->attachAxis(axeY);

    graphique = new QChartView(graphe);
    graphique->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(graphique);

    resize(800, 600);
}

IhmGraphique::~IhmGraphique()
{
    // Release any dynamically allocated resources if needed
}
