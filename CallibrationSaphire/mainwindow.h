#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCharts>

#include <sapfir.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void appendPm(qreal pm){
        for (QPointF &p: Pm){
            p.rx()--;
        }
        Pm << QPointF(60, pm);
        if(Pm.size()>80) Pm.remove(0,20);
        series->replace(Pm);
    }

private:
    Ui::MainWindow *ui;

    Sapfir *sapfir = new Sapfir();
    QTimer *timer = new QTimer(this); //для снятия кучи показаний в течении какого-то времени
    const quint8 N = 25; //количество снятий показаний
    const quint8 T = 250; //ms время между снятиями показаний

    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries(chart);
    QVector <QPointF> Pm;
};
#endif // MAINWINDOW_H
