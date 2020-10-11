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

    customPlot->yAxis->setPadding(5);
    customPlot->yAxis->setLabel("Расходы\n(2020)");
    customPlot->yAxis->setBasePen(QPen(lines));
    customPlot->yAxis->setTickPen(QPen(lines));
    customPlot->yAxis->setSubTickPen(QPen(lines));
    customPlot->yAxis->grid()->setVisible(false);
    customPlot->yAxis->setTickLabelColor(lines);
    customPlot->yAxis->setLabelColor(lines);

    // предыдущий тип данных
    QCPBars * prevSectionType = nullptr;
    // создание зон для всех доступных размеченных данных
    for(auto & sectionType : data_types){
        QCPBars * currentSectionType = new QCPBars(customPlot->xAxis,customPlot->yAxis);
        currentSectionType->setAntialiased(false);
        currentSectionType->setStackingGap(1);
        currentSectionType->setName(sectionType.name);                                 // имя
        currentSectionType->setBrush(sectionType.color());                           // заливка
        currentSectionType->setPen(QPen(sectionType.color().lighter(170)));       // края посветлее

        // индексы и соответствующие им значения у текущего типа данных
        QVector<double> columns, values;
        // в каждой колонке ищем совпадающую секцию и задаем ей значение
        for(auto & segment : segments)
        {
            for(auto & data : segment.data)
            {
                if(data.data_id == sectionType.id)
                {
                    columns.append(segment.index);
                    values.append(data.value);
                    break;
                }
            }
        }

        // задаем данные для текущего типа
        currentSectionType->setData(columns,values);

        // перемещаем над предыдущим типом в списке
        if(prevSectionType)
            currentSectionType->moveBelow(prevSectionType);
        prevSectionType = currentSectionType;
    }

    customPlot->yAxis->setRange(range());

    // задаем легенду
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    customPlot->legend->setBorderPen(Qt::NoPen);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}
