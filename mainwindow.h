#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include "ModelARX.h"
#include "ProstyUAR.h"
#include "RegulatorPID.h"
#include "mytcpserver.h"
#include "mytcpclient.h"

class NetMan;
class QChartView;
class QChart;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateChart();

    void on_pushButtonStart_clicked();
    void on_pushButtonReset_clicked();
    void on_pushButtonStop_clicked();
    void on_spinBoxK_valueChanged(int arg1);
    void on_lineEditA_editingFinished();
    void on_lineEditB_editingFinished();
    void on_spinBoxInterval_editingFinished();
    void on_pushButtonLoad_clicked();
    void on_pushButtonSave_clicked();
    void on_pushButtonARX_clicked();
    void on_doubleSpinBoxP_editingFinished();
    void on_doubleSpinBoxI_editingFinished();
    void on_doubleSpinBoxD_editingFinished();
    void on_doubleSpinBoxValue_editingFinished();
    void on_doubleSpinBoxNoise_editingFinished();
    void on_doubleSpinBoxTime_editingFinished();
    void on_doubleSpinBoxSinusAmp_editingFinished();
    void on_checkBoxCalkaPodSuma_toggled(bool checked);

    void on_btnPolacz_clicked();

    void slot_newClientConnected(QString adr);
    void slot_clientDisconnected(int num);
    void slot_connected(QString adr, int port);
    void slot_disconnected();
    //void slot_newMsgFrom(QString msg, int num);
    //void slot_messageRecived(QString msg);

    //void on_testD_clicked();

    void on_checkBoxTrybStacjonarny_stateChanged(int arg1);

    void on_buttonKonfSieciowa_clicked();

    void onModelARXRequest();
    void onSymulujRequest(double inputReceived, double value, qint64 timeonsend);
    void onResultReceived(double result, qint64 timeonsend);
    void onStartSimOnClient();
    void onIntervalOnServerChanged(int value);
    void onStopSimOnClient();
    void onResetSimOnClient();
    void activeAll();

private:
    void resetDefaultValues();
    void resetChart();
    void resetAllSettings();
    void updateSettings();
    void resetServer();
    bool validatePort(int port);
    bool validateConnectionData(QString adr, int port);
    void resetClient();
    void updateCliNum();

    Ui::MainWindow *ui = nullptr;
    QTimer *timer = nullptr;
    QChartView *chartView = nullptr;
    QChart *chart = nullptr;
    QChartView *chartViewError = nullptr;
    QChart *chartError = nullptr;
    QChartView *chartPIDView = nullptr;
    QChart *chartPID = nullptr;
    QChartView *chartSterowanieView = nullptr;
    QChart *chartSterowanie = nullptr;

    ModelARX *arx = nullptr;
    RegulatorPID *pid = nullptr;
    ProstyUAR *uar = nullptr;

    std::vector<double> sygWe;
    std::vector<double> faktSygWy;

    QLineSeries *outSeries = nullptr;
    QLineSeries *inSeries = nullptr;
    QLineSeries *errSeries = nullptr;
    QLineSeries *pSeries = nullptr;
    QLineSeries *iSeries = nullptr;
    QLineSeries *dSeries = nullptr;

    QLineSeries *sterowanieSeries = nullptr;

    double time = 0, WARTOSC = 2.0, OKRES = 10.0, AMPLITUDA = 1.0;
    int newInterval = 800;


    // Siec:
    MyTCPServer *m_server = nullptr;
    MyTCPClient *m_client = nullptr;
    bool trybStacjonarny = true;

    double pidOutputReceived = 0.0;
    double OutputReceived = 0.0;
    double inputReceived = 0.0;
};

#endif // MAINWINDOW_H
