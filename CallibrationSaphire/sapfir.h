#ifndef SAPFIR_H
#define SAPFIR_H

#include <QObject>
#include <QDebug>
#include <ltr/include/ltr27api.h>

class Sapfir
{
    Q_OBJECT

    double mA = 0;
    TLTR27 *ltr = new TLTR27;
    DWORD data[16];     //Сырой массив
    double niceData[16];//Обработанный


public:
    Sapfir();
    ~Sapfir(){
        qDebug() << "Close  " << LTR27_Close(ltr);
    }
    qint8 init(){
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
                        ltr->FrequencyDivisor = 5; //Установка частоты опроса
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

    qreal getValue(qint8 num){
        // num - номер канала
        LTR27_ADCStart(ltr);
        LTR27_Recv(ltr, data, NULL, 16, 100);
        LTR27_ADCStop(ltr);
        DWORD size = 16;
        LTR27_ProcessData(ltr, data, niceData, &size, 1, 1);
        qDebug() << "value " << niceData[0] <<  niceData[1] << "mA";

        return niceData[num];
    }
};

#endif // SAPFIR_H
