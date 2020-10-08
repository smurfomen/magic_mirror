#ifndef PLOTTER_H
#define PLOTTER_H

#include <qcustomplot.h>
class Plotter {
public:
    virtual ~Plotter() = default;
    virtual void plot(QCustomPlot * customPlot) = 0;
    static Plotter * factory(QString param);
};

#endif // PLOTTER_H
