#ifndef BARREL_PLOTTER_H
#define BARREL_PLOTTER_H

#include <qserializer.h>
#include <plotter.h>
#include <qcustomplot.h>

class BarrelSectionType : public QSerializer {
    Q_GADGET
    QS_SERIALIZER
public:
    QColor color(){
        return QColor(rgb.at(0), rgb.at(1), rgb.at(2));
    }

    QS_FIELD(QString, name);
    QS_COLLECTION(QVector, quint8, rgb);
};

class BarrelSection : public QSerializer {
    Q_GADGET
    QS_SERIALIZER
public:
    QS_FIELD(QString, name);
    QS_FIELD(double, value);
};


class Barrel : public QSerializer{
    Q_GADGET
    QS_SERIALIZER
public:
    QS_FIELD(QString, label);
    QS_FIELD(double, index);
    QS_COLLECTION_OBJECTS(QVector, BarrelSection, sections);
};

class BarrelPlotter : public QSerializer, public Plotter{
    Q_GADGET
    QS_SERIALIZER

public:
    BarrelPlotter(QString filepath)
    {
        QFile f(filepath);
        if(f.open(QIODevice::ReadOnly))
        {
            fromJson(f.readAll());
            f.close();
        }
    }

    virtual void plot(QCustomPlot * customPlot) override;

    QSharedPointer<QCPAxisTickerText> textTicker(){
        QSharedPointer<QCPAxisTickerText> ticker(new QCPAxisTickerText);
        for(auto  & barrel : barrels){
            ticker->addTick(barrel.index, barrel.label);
        }
        return ticker;
    }

    QS_COLLECTION_OBJECTS(QVector, Barrel, barrels);
    QS_COLLECTION_OBJECTS(QVector, BarrelSectionType, types);
};

#endif // BARREL_PLOTTER_H
