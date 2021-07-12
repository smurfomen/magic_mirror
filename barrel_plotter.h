#ifndef BARREL_PLOTTER_H
#define BARREL_PLOTTER_H

#include <qserializer.h>
#include <plotter.h>
#include <qcustomplot.h>

namespace  {
///\class DataType
///\brief тип данных для задания параметров отображения определенных даных
class DataType : public QSerializer {
    Q_GADGET
    QS_SERIALIZER
public:
    QColor color(){
        for(int i = rgb.size(); i < 3; i++)
            rgb.append(0);

        return QColor(rgb.at(0), rgb.at(1), rgb.at(2));
    }

    QS_FIELD(int, id);
    QS_FIELD(QString, name);
    QS_COLLECTION(QVector, quint8, rgb);
};

///\class Data
///\brief Модель данных конкретного типа, не привязанных к конкретному месту.
class Data : public QSerializer {
    Q_GADGET
    QS_SERIALIZER
    QS_FIELD(int, data_id);
    QS_FIELD(double, value);
};

///\class Segment
///\brief Модель для сегмента графика, содержит положение на графике, пометку и данные, которых может быть много
class Segment : public QSerializer{
    Q_GADGET
    QS_SERIALIZER
    QS_FIELD(QString, label);
    QS_FIELD(double, index);
    QS_COLLECTION_OBJECTS(QVector, Data, data);

public:
    QCPRange range(){
        QCPRange r;
        for(auto & d : data)
        {
            if(!r.contains(d.value))
               r.expand(d.value);
        }
        return r;
    }
};

}

///\class BarrelPlotter
///\brief Модель размеченных данных для составления гистограмм
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

        int i = 0;
        for(auto & segment : segments){
            i++;
            if(segment.index == 0)
                segment.index = i;
        }
    }

    ///\brief сформировать график на основе размеченных данных \a BarrelPlotter
    virtual void plot(QCustomPlot * customPlot) override;

    ///\brief Возвращает разметку имен колонн на оси, соответственно их индексам
    QSharedPointer<QCPAxisTickerText> textTicker(){
        QSharedPointer<QCPAxisTickerText> ticker(new QCPAxisTickerText);
        for(auto  & segment : segments){
            ticker->addTick(segment.index, segment.label);
        }
        return ticker;
    }

    QCPRange range() {
        QCPRange r;
        for(auto & s : segments)
            r.expand(s.range());

        return r;
    }

    QS_COLLECTION_OBJECTS(QVector, Segment, segments);
    QS_COLLECTION_OBJECTS(QVector, DataType, data_types);
};

#endif // BARREL_PLOTTER_H
