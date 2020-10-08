#ifndef PLOTTER_H
#define PLOTTER_H
#include <qcustomplot.h>

///\class Plotter
///\brief Абстракция для формирования графиков
class Plotter {
public:
    virtual ~Plotter() = default;

    ///\brief Сформировать в переданном customPlot график на основе своих данных
    virtual void plot(QCustomPlot * customPlot) = 0;

    ///\brief Возвращает объект за абстракцией Plotter на основе параметра
    static Plotter * factory(QString param);
};

#endif // PLOTTER_H
