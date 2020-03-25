#ifndef SAPFIR_H
#define SAPFIR_H

#include <QThread>
#include <QDebug>
#include <QVector>

#include <math.h>

#include <ltr/include/ltr27api.h>

class Sapfir: public QThread
{
    Q_OBJECT

    TLTR27 *ltr = new TLTR27;
    DWORD data[16*1000];     //Сырой массив
    double niceData[2];//Обработанный

    //coefficient Sapfir
    qreal A = 250; // 2kPa/20mA
    qreal B = -3000; // 12mA = 100Pa

    int N = 20; //Количество точек в массиве
    QVector <qreal> Pm;
    QVector <qreal> Pc;
    qreal Pm_a; //Среднее арифметическое
    qreal Pc_a;
    qreal Pm_s; //СКО
    qreal Pc_s;

public:
    Sapfir();
    ~Sapfir(){
        stopLTR();
    }
    qint8 init(){
        Pm.resize(N);
        Pc.resize(N);
        // Подключение к LTR27 по мануалу через внешнюю библиотеку
        if (LTR27_Init(ltr) == LTR_OK){
            int resOpen = LTR27_Open(ltr, SADDR_DEFAULT, SPORT_DEFAULT, "", CC_MODULE1);
            if (resOpen == LTR_WARNING_MODULE_IN_USE){
                qWarning() << "LTR_WARNING_MODULE_IN_USE";
                resOpen = LTR_OK;
            }
            if (LTR_OK == resOpen){
                if (LTR_OK == LTR27_GetConfig(ltr)){
                    if (LTR_OK == LTR27_GetDescription(ltr, LTR27_ALL_DESCRIPTION)){
                        int hz = 4;
                        int fd = 1000/hz - 1;
                        ltr->FrequencyDivisor = fd; //Установка частоты опроса
                        for (int i=0; i< LTR27_MEZZANINE_NUMBER; i++)
                        {
                            for(int j=0; j<4; j++)
                                ltr->Mezzanine[i].CalibrCoeff[j]= ltr->ModuleInfo.Mezzanine[i].Calibration[j];
                        }
                        if (LTR_OK == LTR27_SetConfig(ltr))

                            return 1;

                    }
                }
            }
        }
        return -1;
    }
    void setN(int n){
        N = n;
        Pm.resize(N);
        Pc.resize(N);
    }
    void startLTR(){
        qDebug() << "Start" << LTR27_ADCStart(ltr);
    }
    void stopLTR(){
        qDebug() << "Stop" << LTR27_ADCStop(ltr);
    }


    // Снятия показаний с сапфиров
    void getData(qreal &one, qreal &two){
        //LTR запускает АЦП, снимает разок показания и останавливает
        DWORD size = LTR27_Recv(ltr, data, NULL, 16*30, 5);
        qDebug() << size;
        if (size > 16*3) return;
        DWORD s = 2;
        LTR27_ProcessData(ltr, data, niceData, &s, 1, 1);

        one = A*niceData[0] + B;
        two = A*niceData[1] + B;

        //qDebug() << "value " << niceData[0] <<  niceData[1] << "mA";
    }

public: signals:
    void newPm(qreal);
    void newPc(qreal);

    void newPm_a(qreal);
    void newPc_a(qreal);

    void newPm_s(qreal);
    void newPc_s(qreal);

public slots:
    // Снимает показание с сапфиров, осредняет и вычисляет СКО
    void run(){
        //каждое срабатывание по кругу забивает массив данных
        static quint8 counter = 0;
        //Снятие давления
        if(counter > (N-1)) counter = 0;
        getData(Pm[counter], Pc[counter]);
        emit newPm(Pm[counter]);
        emit newPc(Pc[counter]);
        counter++;

        //Вычисление среднего
        Pm_a = 0;
        for(qreal pm: Pm)
            Pm_a += pm/N;
        emit newPm_a(Pm_a);
        Pc_a = 0;
        for(qreal pc: Pc)
            Pc_a += pc/N;
        emit newPc_a(Pc_a);

        //Вычисление СКО
        qreal sum = 0;
        Pm_s = 0;
        for(qreal pm: Pm)
            sum += pow(pm-Pm_a, 2);
        Pm_s = sqrt(sum/(N-1));
        emit newPm_s(Pm_s);

        sum = 0;
        for(qreal pc: Pc)
            sum += pow(pc-Pc_a, 2);
        Pc_s = sqrt(sum/(N-1));
        emit newPc_s(Pc_s);
    }
};

#endif // SAPFIR_H
