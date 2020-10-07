#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotset.h"
#include <QColor>

#define CONTRAST(plot, back, lines) \
    plot->setBackground(back); \
    plot->xAxis->setBasePen(QPen(lines)); \
    plot->xAxis->setTickPen(QPen(lines)); \
    plot->xAxis->setSubTickPen(QPen(lines)); \
    plot->xAxis->grid()->setVisible(false); \
    plot->xAxis->setTickLabelColor(lines); \
    plot->xAxis->setLabelColor(lines); \
    plot->yAxis->setBasePen(QPen(lines)); \
    plot->yAxis->setTickPen(QPen(lines)); \
    plot->yAxis->setSubTickPen(QPen(lines)); \
    plot->yAxis->grid()->setVisible(false); \
    plot->yAxis->setTickLabelColor(lines); \
    plot->yAxis->setLabelColor(lines);


struct Stack{
    QString label;
    double position;
    QMap<QString, double> set;

    Stack(double i, QString n){
        position = i;
        label = n;
    }

    void addValue(QString name, double value){
        set.insert(name, value);
    }
};


struct StackModel {
    StackModel(std::initializer_list<Stack> list) {
        for(auto it : list)
            stacks.insert(it.label, it);
    }

    StackModel(){

    }

    void addStack(Stack b){
        stacks.insert(b.label, b);
    }

    void addStack(QString label){
        double max = 1;
        for(auto s : stacks)
            if(s.position > max)
                max = s.position;

        addStack(Stack(max+1, label));
    }

    QSharedPointer<QCPAxisTickerText> textTicker(){
        QSharedPointer<QCPAxisTickerText> ticker(new QCPAxisTickerText);
        for(auto t : stacks){
            ticker->addTick(t.position, t.label);
        }
        return ticker;
    }

    Stack stackByName(QString label){
        return stacks.value(label);
    }

    QMap<QString, Stack> stacks;
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QCustomPlot * customPlot = ui->customPlot;
    CONTRAST(customPlot,Qt::black, Qt::white)

    Barrels gas(customPlot, "Газ", QColor(250,170,20));
    Barrels electro(customPlot, "Электричество", QColor(111,9,176));
    Barrels water(customPlot, "Вода", QColor(0,168,140));

    // stack bars on top of each other:
    water.bars->moveAbove(electro.bars);
    gas.bars->moveAbove(water.bars);

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;

    StackModel t;
    t.addStack("Январь");
    t.addStack("Февраль");
    t.addStack("Март");
    t.addStack("Апрель");
    t.addStack("Май");
    t.addStack("Июнь");
    t.addStack("Июль");
    t.addStack("Август");
    t.addStack("Сентябрь");
    t.addStack("Октябрь");
    t.addStack("Ноябрь");
    t.addStack("Декабрь");

    customPlot->xAxis->setTicker(t.textTicker());
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTicks(false);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->setRange(0, 8);

    // prepare y axis:
    customPlot->yAxis->setRange(0, 12.1);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel("Расходы\n(2020)");

    electro  << 0.31*5.5 << 0.32*5.5 << 0.20*5.5 << 0.31*5.8 << 0.05*5.2 << 0.20*4.2 << 0.24*6.2;
    water << 0.69*4.5 << 0.39*5.5 << 0.42*5.5 << 0.51*5.8 << 0.49*5.2 << 0.52*4.2 << 0.61*4.2;
    gas   << 0.29*5.5 << 0.22*5.5 << 0.13*5.5 << 0.11*5.8 << 0.09*5.2 << 0.05*4.2 << 0.04*6.2;

    // setup legend:
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    customPlot->legend->setBorderPen(Qt::NoPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

MainWindow::~MainWindow()
{
    delete ui;
}

