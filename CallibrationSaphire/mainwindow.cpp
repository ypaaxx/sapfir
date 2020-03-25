#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Настройка крейтов LTR
    if (!sapfir->init())
        qDebug() << "Ошибка запуска LTR";

    //Настройка таймера
    timer->setInterval(T);
    connect( timer, SIGNAL(timeout()), sapfir, SLOT(run()) );
    connect( sapfir, SIGNAL(newPm(qreal)), ui->Pmlcd, SLOT(display(qreal)));
    connect( sapfir, SIGNAL(newPm_a(qreal)), ui->PmAlcd, SLOT(display(qreal)));
    connect( sapfir, SIGNAL(newPm_s(qreal)), ui->PmSlcd, SLOT(display(qreal)));
    Pm.reserve(100);
    chart->addSeries(series);

    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 60);
    chart->axisY()->setRange(-100, 100);
    QChartView *view = new QChartView(chart);
    view->setMinimumSize(800, 600);

    connect( sapfir, SIGNAL(newPm(qreal)), this, SLOT(appendPm(qreal)));
    connect( sapfir, SIGNAL(newPm(qreal)), view, SLOT(replace()));
}

MainWindow::~MainWindow()
{
    delete chart;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    sapfir->startLTR();
    timer->start();
}
