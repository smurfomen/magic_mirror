#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotter.h"
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Plotter::factory("credits.json")->plot(ui->customPlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

