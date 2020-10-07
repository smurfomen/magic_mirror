#ifndef PLOTSET_H
#define PLOTSET_H

#include <QString>
#include <QMap>
#include <QColor>
#include <qcustomplot.h>

class Barrels{
public:
    Barrels(QCustomPlot * plot, QString name, QColor color) {
        bars = new QCPBars(plot->xAxis,plot->yAxis);
        // gives more crisp, pixel aligned bar borders
        bars->setAntialiased(false);
        bars->setStackingGap(1);
        // задать имя и цвета
        bars->setName(name);                                 // имя
        bars->setBrush(color);                           // заливка
        bars->setPen(QPen(color.lighter(170)));       // края посветлее
    }

    void addValue(double && value){
        bars->addData(++count, value);
    }

    inline Barrels &operator<<(double && t)
    { addValue(std::forward<double>(t)); return *this; }

    int count{0};
    QCPBars * bars{nullptr};
};
































#endif // PLOTSET_H
