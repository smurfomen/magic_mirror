#include "barrel_plotter.h"

void BarrelPlotter::plot(QCustomPlot * customPlot) {
    QColor back = Qt::black;
    QColor lines = Qt::white;
    customPlot->setBackground(back);

    customPlot->xAxis->setTicker(textTicker());
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTicks(false);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->setRange(0, 8);
    customPlot->xAxis->setBasePen(QPen(lines));
    customPlot->xAxis->setTickPen(QPen(lines));
    customPlot->xAxis->setSubTickPen(QPen(lines));
    customPlot->xAxis->grid()->setVisible(false);
    customPlot->xAxis->setTickLabelColor(lines);
    customPlot->xAxis->setLabelColor(lines);

    // prepare y axis:
    customPlot->yAxis->setRange(0, 12.1);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel("Расходы\n(2020)");
    customPlot->yAxis->setBasePen(QPen(lines));
    customPlot->yAxis->setTickPen(QPen(lines));
    customPlot->yAxis->setSubTickPen(QPen(lines));
    customPlot->yAxis->grid()->setVisible(false);
    customPlot->yAxis->setTickLabelColor(lines);
    customPlot->yAxis->setLabelColor(lines);

    QCPBars * bellowed = nullptr;
    for(auto & sectionType : types){
        QCPBars * area = new QCPBars(customPlot->xAxis,customPlot->yAxis);
        // gives more crisp, pixel aligned bar borders
        area->setAntialiased(false);
        area->setStackingGap(1);
        // задать имя и цвета
        area->setName(sectionType.name);                                 // имя
        area->setBrush(sectionType.color());                           // заливка
        area->setPen(QPen(sectionType.color().lighter(170)));       // края посветлее

        QVector<double> keys, values;
        for(auto & barrel : barrels)
        {
            for(auto & section : barrel.sections)
            {
                if(section.name == area->name())
                {
                    keys.append(barrel.index);
                    values.append(section.value);
                    break;
                }
            }
        }

        area->setData(keys,values);
        if(bellowed)
            area->moveBelow(bellowed);

        bellowed = area;
    }

    // setup legend:
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    customPlot->legend->setBorderPen(Qt::NoPen);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}
