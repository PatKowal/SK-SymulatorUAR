#include "mainwindow.h"
#include <QChart>
#include <QChartView>
#include <QFileDialog>
#include <QGridLayout>
#include <QLineSeries>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include <vector>
#include <Serializer.h>
#include <QFormLayout>

// skok jednostkowy param: wartość
// sygnal prostokatny param: wartość, czas
// sygnal sinusoidalny param: wartość, czas, amplituda

double withXAxis = 30;

double vChartMinRangeDefault = -1;
double vChartMinRange = vChartMinRangeDefault;
double vChartMaxRangeDefault = 4;
double vChartMaxRange = vChartMaxRangeDefault;

double vChartErrorMinRangeDefault = -1;
double vChartErrorMinRange = vChartErrorMinRangeDefault;
double vChartErrorMaxRangeDefault = 4;
double vChartErrorMaxRange = vChartErrorMaxRangeDefault;

double vChartPIDMinRangeDefault = -1;
double vChartPIDMinRange = vChartPIDMinRangeDefault;
double vChartPIDMaxRangeDefault = 4;
double vChartPIDMaxRange = vChartPIDMaxRangeDefault;

double vChartSterowanieMinRangeDefault = -1;
double vChartSterowanieMinRange = vChartSterowanieMinRangeDefault;
double vChartSterowanieMaxRangeDefault = 4;
double vChartSterowanieMaxRange = vChartSterowanieMaxRangeDefault;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->spinBoxPort->setRange(0,65535);

    chart = new QChart;
    outSeries = new QLineSeries();
    outSeries->setName("Wartość regulowana");
    inSeries = new QLineSeries();
    inSeries->setName("Wartość zadana");

    chart->addSeries(inSeries);
    chart->addSeries(outSeries);
    chart->createDefaultAxes();

    chart->axes(Qt::Vertical).first()->setRange(vChartMinRange, vChartMaxRange);
    chart->axes(Qt::Vertical).first()->setTitleText("Wartosc");
    chart->axes(Qt::Horizontal).first()->setRange(0, withXAxis);
    chart->axes(Qt::Horizontal).first()->setTitleText("Czas");

    chartView = new QChartView(chart, ui->chartWidget);
    chartView->setRenderHint(QPainter::Antialiasing);
    auto layout = new QVBoxLayout(ui->chartWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);

    // wykres error

    chartError = new QChart;
    errSeries = new QLineSeries();
    errSeries->setName("Uchyb");

    chartError->addSeries(errSeries);
    chartError->createDefaultAxes();

    chartError->axes(Qt::Vertical).first()->setRange(vChartErrorMinRange, vChartErrorMaxRange);
    chartError->axes(Qt::Vertical).first()->setTitleText("Wartosc");
    chartError->axes(Qt::Horizontal).first()->setRange(0, withXAxis);
    chartError->axes(Qt::Horizontal).first()->setTitleText("Czas");

    chartViewError = new QChartView(chartError);
    chartViewError->setRenderHint(QPainter::Antialiasing);
    auto layoutError = new QVBoxLayout(ui->chartWidgetError);
    layoutError->setContentsMargins(0, 0, 0, 0);
    layoutError->addWidget(chartViewError);


    // wykres PID

    chartPID = new QChart;
    pSeries = new QLineSeries();
    pSeries->setName("P");
    iSeries = new QLineSeries();
    iSeries->setName("I");
    dSeries = new QLineSeries();
    dSeries->setName("D");

    chartPID->addSeries(pSeries);
    chartPID->addSeries(iSeries);
    chartPID->addSeries(dSeries);
    chartPID->createDefaultAxes();

    chartPID->axes(Qt::Vertical).first()->setRange(vChartPIDMinRange, vChartPIDMaxRange);
    chartPID->axes(Qt::Vertical).first()->setTitleText("Wartosc");
    chartPID->axes(Qt::Horizontal).first()->setRange(0, withXAxis);
    chartPID->axes(Qt::Horizontal).first()->setTitleText("Czas");

    chartPIDView = new QChartView(chartPID);
    chartPIDView->setRenderHint(QPainter::Antialiasing);
    auto layoutPID = new QVBoxLayout(ui->widgetPID);
    layoutPID->setContentsMargins(0, 0, 0, 0);
    layoutPID->addWidget(chartPIDView);


    // wykres sterowanie

    chartSterowanie = new QChart;
    sterowanieSeries = new QLineSeries();
    sterowanieSeries->setName("Sterowanie");

    chartSterowanie->addSeries(sterowanieSeries);
    chartSterowanie->createDefaultAxes();

    chartSterowanie->axes(Qt::Vertical)
        .first()
        ->setRange(vChartSterowanieMinRange, vChartSterowanieMaxRange);
    chartSterowanie->axes(Qt::Vertical).first()->setTitleText("Wartosc");
    chartSterowanie->axes(Qt::Horizontal).first()->setRange(0, withXAxis);
    chartSterowanie->axes(Qt::Horizontal).first()->setTitleText("Czas");

    chartSterowanieView = new QChartView(chartSterowanie);
    chartSterowanieView->setRenderHint(QPainter::Antialiasing);
    auto layoutSterowanie = new QVBoxLayout(ui->widgetWartoscSterowania);
    layoutSterowanie->setContentsMargins(0, 0, 0, 0);
    layoutSterowanie->addWidget(chartSterowanieView);


    resetDefaultValues();
    resetAllSettings();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateChart);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateChart()
{
    //qDebug() << "UDPATE";

    double input = 0;

    time += newInterval / 1000.0;
    qDebug() << "TIME: " << time;

    if (ui->radioButtonUnit->isChecked()) {
        //qDebug() << "Unit";
        // if(numerProbki != 0)
        {
            input = WARTOSC;
        }
    } else if (ui->radioButtonRect->isChecked()) {
        qDebug() << "RECT SIGNAL";

        //if(/*numerProbki != 0 && */time < ui->doubleSpinBoxTime->value() )
        {
            unsigned long long okres = OKRES;
            bool yes = (unsigned long long) time % okres < 0.5 * okres;
            input = yes ? WARTOSC : 0;
        }
    } else if (ui->radioButtonSinus->isChecked()) {
        qDebug() << "Sinus";

        //if(n != 0)
        {
            double x = (double) (2 * M_PI * time) / OKRES;
            input = WARTOSC + AMPLITUDA * sin(x);
        }
    }

    double output = uar->symuluj(input);

    // skalowanie w pionie wykres 1
    if (output > vChartMaxRange - 0.1) {
        vChartMaxRange = output + 0.1;
        chart->axes(Qt::Vertical).first()->setRange(vChartMinRange, vChartMaxRange);
    }

    if (output < vChartMinRange + 0.1) {
        vChartMinRange = output - 0.1;
        chart->axes(Qt::Vertical).first()->setRange(vChartMinRange, vChartMaxRange);
    }

    if (input > vChartMaxRange - 0.1) {
        vChartMaxRange = input + 0.1;
        chart->axes(Qt::Vertical).first()->setRange(vChartMinRange, vChartMaxRange);
    }

    if (input < vChartMinRange + 0.1) {
        vChartMinRange = input - 0.1;
        chart->axes(Qt::Vertical).first()->setRange(vChartMinRange, vChartMaxRange);
    }

    // skalowanie wykres error

    if (fabs(input - output) > vChartErrorMaxRange - 0.1) {
        vChartErrorMaxRange = fabs(input - output) + 0.1;
        chartError->axes(Qt::Vertical).first()->setRange(vChartErrorMinRange, vChartErrorMaxRange);
    }

    if (fabs(input - output) < vChartErrorMinRange + 0.1) {
        vChartErrorMinRange = fabs(input - output) - 0.1;
        chartError->axes(Qt::Vertical).first()->setRange(vChartErrorMinRange, vChartErrorMaxRange);
    }

    // skalowanie w pionie wykres PID

    if (pid->czlonP() > vChartPIDMaxRange - 0.1) {
        vChartPIDMaxRange = pid->czlonP() + 0.1;
        chartPID->axes(Qt::Vertical).first()->setRange(vChartPIDMinRange, vChartPIDMaxRange);
    }

    if (pid->czlonP() < vChartPIDMinRange + 0.1) {
        vChartPIDMinRange = pid->czlonP() - 0.1;
        chartPID->axes(Qt::Vertical).first()->setRange(vChartPIDMinRange, vChartPIDMaxRange);
    }

    if (pid->czlonI() > vChartPIDMaxRange - 0.1) {
        vChartPIDMaxRange = pid->czlonI() + 0.1;
        chartPID->axes(Qt::Vertical).first()->setRange(vChartPIDMinRange, vChartPIDMaxRange);
    }

    if (pid->czlonI() < vChartPIDMinRange + 0.1) {
        vChartPIDMinRange = pid->czlonI() - 0.1;
        chartPID->axes(Qt::Vertical).first()->setRange(vChartPIDMinRange, vChartPIDMaxRange);
    }

    if (pid->czlonD() > vChartPIDMaxRange - 0.1) {
        vChartPIDMaxRange = pid->czlonD() + 0.1;
        chartPID->axes(Qt::Vertical).first()->setRange(vChartPIDMinRange, vChartPIDMaxRange);
    }

    if (pid->czlonD() < vChartPIDMinRange + 0.1) {
        vChartPIDMinRange = pid->czlonD() - 0.1;
        chartPID->axes(Qt::Vertical).first()->setRange(vChartPIDMinRange, vChartPIDMaxRange);
    }

    // skalowanie wykres sterowanie

    if (pid->wartoscPID() > vChartSterowanieMaxRange - 0.1) {
        vChartSterowanieMaxRange = pid->wartoscPID() + 0.1;
        chartSterowanie->axes(Qt::Vertical)
            .first()
            ->setRange(vChartSterowanieMinRange, vChartSterowanieMaxRange);
    }

    if (pid->wartoscPID() < vChartSterowanieMinRange + 0.1) {
        vChartSterowanieMinRange = pid->wartoscPID() - 0.1;
        chartSterowanie->axes(Qt::Vertical)
            .first()
            ->setRange(vChartSterowanieMinRange, vChartSterowanieMaxRange);
    }

    pSeries->append(time, pid->czlonP());
    iSeries->append(time, pid->czlonI());
    dSeries->append(time, pid->czlonD());

    sterowanieSeries->append(time, pid->wartoscPID());

    errSeries->append(time, fabs(input - output));
    inSeries->append(time, input);
    outSeries->append(time, output);
    // numerProbki++;

    if (time > withXAxis) {
        //double t = (n * ui->spinBoxInterval->value()) / 1000;
        double t = time;
        chart->axes(Qt::Horizontal).first()->setRange(t - withXAxis, t);
        chartError->axes(Qt::Horizontal).first()->setRange(t - withXAxis, t);
        chartPID->axes(Qt::Horizontal).first()->setRange(t - withXAxis, t);
        chartSterowanie->axes(Qt::Horizontal).first()->setRange(t - withXAxis, t);
    }
}

void MainWindow::on_pushButtonStart_clicked()
{
    sygWe.push_back(1);

    timer->setInterval(newInterval);

    timer->start();
}

void MainWindow::resetDefaultValues()
{
    ui->lineEditA->setText("-0.4");
    ui->lineEditB->setText("0.6");
    ui->spinBoxK->setValue(1);
    ui->doubleSpinBoxP->setValue(0.5);
    ui->doubleSpinBoxI->setValue(5.0);
    ui->doubleSpinBoxD->setValue(0.2);
    ui->spinBoxInterval->setValue(800);
    ui->doubleSpinBoxNoise->setValue(0);
    ui->radioButtonUnit->setChecked(true);
    ui->doubleSpinBoxValue->setValue(2);
    ui->doubleSpinBoxTime->setValue(10);
    ui->doubleSpinBoxSinusAmp->setValue(1);
    time = 0;
    WARTOSC = 2.0;
    OKRES = 10.0;
    AMPLITUDA = 1.0;
    newInterval = 800;
}

void MainWindow::resetChart()
{
    sygWe.clear();

    inSeries->clear();
    outSeries->clear();
    errSeries->clear();

    pSeries->clear();
    iSeries->clear();
    dSeries->clear();

    sterowanieSeries->clear();

    // numerProbki = 0;
    time = 0;

    vChartMinRange = vChartMinRangeDefault;
    vChartMaxRange = vChartMaxRangeDefault;
    vChartErrorMinRange = vChartErrorMinRangeDefault;
    vChartErrorMaxRange = vChartErrorMaxRangeDefault;
    vChartPIDMinRange = vChartPIDMinRangeDefault;
    vChartPIDMaxRange = vChartPIDMaxRangeDefault;
    vChartSterowanieMinRange = vChartSterowanieMinRangeDefault;
    vChartSterowanieMaxRange = vChartSterowanieMaxRangeDefault;

    chart->axes(Qt::Vertical).first()->setRange(vChartMinRangeDefault, vChartMaxRangeDefault);
    chartError->axes(Qt::Vertical)
        .first()
        ->setRange(vChartErrorMinRangeDefault, vChartErrorMaxRangeDefault);
    chartPID->axes(Qt::Vertical)
        .first()
        ->setRange(vChartPIDMinRangeDefault, vChartPIDMaxRangeDefault);
    chartSterowanie->axes(Qt::Vertical)
        .first()
        ->setRange(vChartSterowanieMinRangeDefault, vChartSterowanieMaxRangeDefault);

    chart->axes(Qt::Horizontal).first()->setRange(0, withXAxis);
    chartError->axes(Qt::Horizontal).first()->setRange(0, withXAxis);
    chartPID->axes(Qt::Horizontal).first()->setRange(0, withXAxis);
    chartSterowanie->axes(Qt::Horizontal).first()->setRange(0, withXAxis);
}

void MainWindow::resetAllSettings()
{
    delete arx;
    delete pid;
    delete uar;

    resetChart();

    QStringList aStr = ui->lineEditA->text().split(";");
    std::vector<double> a;
    for (const QString &str : aStr) {
        bool ok = false;
        double value = str.toDouble(&ok);
        if (ok)
            a.push_back(value);
    }

    qDebug() << "ARX a: " << a;

    QStringList bStr = ui->lineEditB->text().split(";");
    std::vector<double> b;
    for (const QString &str : bStr) {
        bool ok = false;
        double value = str.toDouble(&ok);
        if (ok)
            b.push_back(value);
    }

    qDebug() << "ARX b: " << b;

    int k = ui->spinBoxK->value();

    qDebug() << "ARX k: " << k;

    double z = ui->doubleSpinBoxNoise->value();
    qDebug() << "ARX z: " << z;

    double p = ui->doubleSpinBoxP->value();
    double i = ui->doubleSpinBoxI->value();
    double d = ui->doubleSpinBoxD->value();
    qDebug() << "PID: (" << p << ", " << i << ", " << d << ")";

    arx = new ModelARX(a, b, k, z);
    pid = new RegulatorPID(p, i, d);
    uar = new ProstyUAR(*arx, *pid);
}

void MainWindow::updateSettings()
{
    if (pid) {
        qDebug() << "Update PID Settings";
        pid->ustawP(ui->doubleSpinBoxP->value());
        pid->ustawI(ui->doubleSpinBoxI->value());
        pid->ustawD(ui->doubleSpinBoxD->value());
    }

    if (arx) {
        QStringList aStr = ui->lineEditA->text().split(";");
        std::vector<double> a;
        for (const QString &str : aStr) {
            bool ok = false;
            double value = str.toDouble(&ok);
            if (ok)
                a.push_back(value);
        }

        qDebug() << "ARX a: " << a;

        QStringList bStr = ui->lineEditB->text().split(";");
        std::vector<double> b;
        for (const QString &str : bStr) {
            bool ok = false;
            double value = str.toDouble(&ok);
            if (ok)
                b.push_back(value);
        }

        arx->ustawA(a);
        arx->ustawB(b);
        arx->ustawK(ui->spinBoxK->value());
        arx->ustawZ(ui->doubleSpinBoxNoise->value());
    }
}

void MainWindow::on_pushButtonReset_clicked()
{
    resetDefaultValues();
    resetAllSettings();
}

void MainWindow::on_pushButtonStop_clicked() { timer->stop(); }

void MainWindow::on_spinBoxK_valueChanged(int arg1)
{
    qDebug() << __FUNCTION__;
    updateSettings();
}

void MainWindow::on_lineEditA_editingFinished()
{
    qDebug() << __FUNCTION__;
    updateSettings();
}

void MainWindow::on_lineEditB_editingFinished()
{
    qDebug() << __FUNCTION__;
    updateSettings();
}

void MainWindow::on_spinBoxInterval_editingFinished()
{
    timer->stop();

    newInterval = ui->spinBoxInterval->value();

    // Zapewnienie, że interwał nie będzie mniejszy niż 500 ms
    // if (newInterval < 500) newInterval = 500;

    ui->spinBoxInterval->setValue(newInterval); // Aktualizacja wartości w interfejsie
    timer->setInterval(newInterval);
    timer->start();
    qDebug() << "Nowy interwał (ms): " << newInterval;
    updateSettings();
}

void MainWindow::on_pushButtonLoad_clicked()
{
    QString fileName
        = QFileDialog::getOpenFileName(this,
                                       tr("Wczytaj konfiguracje"),
                                       "",
                                       tr("Pliki binarne (*.bin);;Wszystkie pliki (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Blad"), tr("Nie mozna otworzyc pliku."));
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);

    double p, i, d, noise;
    int interval;
    in >> p >> i >> d >> interval >> noise;

    int signalType;
    double signalValue = 0.0, signalTime = 0.0, signalAmp = 0.0;
    in >> signalType >> signalValue >> signalTime >> signalAmp;

    QStringList aList, bList;
    int k;
    in >> aList >> bList >> k;

    ui->doubleSpinBoxP->setValue(p);
    ui->doubleSpinBoxI->setValue(i);
    ui->doubleSpinBoxD->setValue(d);
    ui->spinBoxInterval->setValue(interval);
    ui->doubleSpinBoxNoise->setValue(noise);

    if (signalType == 0) {
        ui->radioButtonUnit->setChecked(true);
        ui->doubleSpinBoxValue->setValue(signalValue);
    } else if (signalType == 1) {
        ui->radioButtonRect->setChecked(true);
        ui->doubleSpinBoxTime->setValue(signalTime);
    } else if (signalType == 2) {
        ui->radioButtonSinus->setChecked(true);
        ui->doubleSpinBoxSinusAmp->setValue(signalAmp);
    }

    ui->lineEditA->setText(aList.join(";"));
    ui->lineEditB->setText(bList.join(";"));
    ui->spinBoxK->setValue(k);

    file.close();
    QMessageBox::information(this, tr("Sukces"), tr("Konfiguracja zostala wczytana."));
}
void MainWindow::on_pushButtonSave_clicked()
{
    QString fileName
        = QFileDialog::getSaveFileName(this,
                                       tr("Zapisz konfiguracje"),
                                       "",
                                       tr("Pliki binarne (*.bin);;Wszystkie pliki (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Blad"), tr("Nie mozna zapisać pliku."));
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);

    double p = ui->doubleSpinBoxP->value();
    double i = ui->doubleSpinBoxI->value();
    double d = ui->doubleSpinBoxD->value();
    int interval = newInterval;
    double noise = ui->doubleSpinBoxNoise->value();

    int signalType = 0;
    double signalValue = 0.0, signalTime = 0.0, signalAmp = 0.0;

    if (ui->radioButtonUnit->isChecked()) {
        signalType = 0;
        signalValue = WARTOSC;
    } else if (ui->radioButtonRect->isChecked()) {
        signalType = 1;
        signalTime = OKRES;
    } else if (ui->radioButtonSinus->isChecked()) {
        signalType = 2;
        signalAmp = AMPLITUDA;
    }

    QStringList aList = ui->lineEditA->text().split(";");
    QStringList bList = ui->lineEditB->text().split(";");
    int k = ui->spinBoxK->value();

    out << p << i << d << interval << noise;
    out << signalType << signalValue << signalTime << signalAmp;
    out << aList << bList << k;

    file.close();
    QMessageBox::information(this, tr("Sukces"), tr("Konfiguracja zostala zapisana."));
}

void MainWindow::on_pushButtonARX_clicked()
{
    ARXokno oknoARX(this);
    if (oknoARX.exec() == QDialog::Accepted) {
        QStringList aList = oknoARX.getA();
        QStringList bList = oknoARX.getB();
        int opóźnienie = oknoARX.getOpóźnienie();

        ui->lineEditA->setText(aList.join(";"));
        ui->lineEditB->setText(bList.join(";"));
        ui->spinBoxK->setValue(opóźnienie);

        // 🚀 Aktualizacja modelu ARX
        std::vector<double> a, b;
        for (const QString &str : aList) {
            bool ok;
            double value = str.toDouble(&ok);
            if (ok)
                a.push_back(value);
        }

        for (const QString &str : bList) {
            bool ok;
            double value = str.toDouble(&ok);
            if (ok)
                b.push_back(value);
        }

        arx->ustawA(a);
        arx->ustawB(b);
        arx->ustawK(ui->spinBoxK->value());
        arx->ustawZ(ui->doubleSpinBoxNoise->value());

        //double noise = ui->doubleSpinBoxNoise->value();
    }
}

void MainWindow::on_doubleSpinBoxP_editingFinished()
{
    qDebug() << __FUNCTION__;
    updateSettings();
}

void MainWindow::on_doubleSpinBoxI_editingFinished()
{
    qDebug() << __FUNCTION__;
    updateSettings();
}

void MainWindow::on_doubleSpinBoxD_editingFinished()
{
    qDebug() << __FUNCTION__;
    updateSettings();
}

void MainWindow::on_doubleSpinBoxNoise_editingFinished()
{
    qDebug() << __FUNCTION__;
    updateSettings();
}



void MainWindow::on_doubleSpinBoxValue_editingFinished()
{
    qDebug() << __FUNCTION__;
    WARTOSC = ui->doubleSpinBoxValue->value();
    updateSettings();
}

void MainWindow::on_doubleSpinBoxTime_editingFinished()
{
    qDebug() << __FUNCTION__;
    OKRES = ui->doubleSpinBoxTime->value();
    updateSettings();
}


void MainWindow::on_doubleSpinBoxSinusAmp_editingFinished()
{
    qDebug() << __FUNCTION__;
    AMPLITUDA = ui->doubleSpinBoxSinusAmp->value();
    updateSettings();
}

void MainWindow::on_checkBoxCalkaPodSuma_toggled(bool checked)
{
    pid->ustawCalkaPodSuma(checked);
}

void MainWindow::resetServer()
{
    if(m_server != nullptr)
    {
        m_server->stopListening();
        delete m_server;
    }
    m_server = new MyTCPServer(this);
    connect(m_server,SIGNAL(newClientConnected(QString)),this,SLOT(slot_newClientConnected(QString)));
    connect(m_server,SIGNAL(clientDisconnetced(int)),this,SLOT(slot_clientDisconnected(int)));
    //connect(m_server,SIGNAL(newMsgFrom(QString,int)),this,SLOT(slot_newMsgFrom(QString,int)));
}

bool MainWindow::validatePort(int port)
{
    if(port < 0 || 65535 < port)
    {
        ui->lineEditStan->setText("Invalid TCP port number!");
        return false;
    }
    return true;
}

void MainWindow::resetClient()
{
    if(m_client != nullptr)
    {
        m_client->disconnectFrom();
        delete m_client;
    }
    m_client = new MyTCPClient(this);
    connect(m_client,SIGNAL(connected(QString,int)),this,SLOT(slot_connected(QString,int)));
    connect(m_client,SIGNAL(disconnected()),this,SLOT(slot_disconnected()));
    //connect(m_client,SIGNAL(messageRecived(QString)),this,SLOT(slot_messageRecived(QString)));
}

bool MainWindow::validateConnectionData(QString adr, int port)
{
    QHostAddress ipAdr(adr);
    if(ipAdr.protocol() != QAbstractSocket::IPv4Protocol)
    {
        ui->lineEditStan->setText("Invalid IPv4 Address!");
        return false;
    }
    if(port < 0 || 65535 < port)
    {
        ui->lineEditStan->setText("Invalid TCP port number!");
        return false;
    }
    return true;
}

void MainWindow::slot_newClientConnected(QString adr)
{
    ui->lineEditStan->setText("New client from: " + adr);
    updateCliNum();
}

void MainWindow::slot_clientDisconnected(int num)
{
    ui->lineEditStan->setText("Client " + QString::number(num) + " disconnected!");
    qDebug() << "Client " + QString::number(num) + " disconnected!";
    updateCliNum();
}

void MainWindow::updateCliNum()
{
    int numCli = m_server->getNumClients();
}

void MainWindow::slot_connected(QString adr, int port)
{
    ui->lineEditStan->setText("Connected to " + adr + " " + QString::number(port));
}

void MainWindow::slot_disconnected()
{
    ui->lineEditStan->setText("Disconnected");
}

void MainWindow::on_btnPolacz_clicked()
{
    if (ui->checkBoxTrybStacjonarny->isChecked() == false) {
        QString host = ui->lineEditIP->text();

        if (ui->comboBoxRola->currentIndex() == 0) {
            // Regulator jako serwer
            if(m_server != nullptr && m_server->isListening())
            {
                m_server->stopListening();
                ui->lineEditStan->setText("Start serwera");
            }
            else
            {
                int port = ui->spinBoxPort->text().toInt();
                if(!validatePort(port))
                    return;
                resetServer();
                if(!m_server->startListening(port))
                    ui->lineEditStan->setText("Error starting server!");
                else
                {
                    //ui->lineEditStan->setText("Stop serwera");
                    ui->lineEditStan->setText("Serwer nasłuchuje na porcie " + QString::number(port));
                }
            }
        } else {
            // ModelARX jako klient
            QString host = ui->lineEditIP->text();
            int port = ui->spinBoxPort->text().toInt();
            if(!validateConnectionData(host, port))
                return;
            resetClient();
            m_client->connectTo(host,port);
            ui->lineEditStan->setText("ModelARX: łączenie z " + host + ":" + QString::number(port));
        }
    }
}






void MainWindow::on_testD_clicked()
{
    QByteArray serial = Serializer::serialize(*arx);
    qDebug() << "Dane serial: " << serial;

    ModelARX deserial = Serializer::deserialize(serial);
    qDebug() << "Model: ";
    qDebug() << "A: " << deserial.getA();
    qDebug() << "B: " << deserial.getB();
    qDebug() << "k: " << deserial.getK();
    qDebug() << "z: " << deserial.getZ();
}

void MainWindow::on_checkBoxTrybStacjonarny_stateChanged(int arg1)
{
    if (ui->comboBoxRola->currentIndex() == 0) {
        if(m_server != nullptr && m_server->isListening())
        {
            if(m_server->getNumClients() > 0){
                //m_client->disconnectFrom(); // Trzeba dodać żeby to robił klienta po dostaniu informacji od serwera po sieci (Msg)
            }
            m_server->stopListening();
            ui->lineEditStan->setText("Praca stacjonarna");
        }
    } else{
        m_client->disconnectFrom();
        ui->lineEditStan->setText("Serwer jest wyłączony");
    }
}


void MainWindow::on_buttonKonfSieciowa_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Konfiguracja połączenia sieciowego");

    QFormLayout form(&dialog);

    QLineEdit ipEdit, portEdit;
    QComboBox trybCombo;
    trybCombo.addItems({"Regulator", "Model ARX"});

    // Domyślne wartości, jeśli brak danych w GUI
    QString currentIP = ui->lineEditIP->text().isEmpty() ? "127.0.0.1" : ui->lineEditIP->text();
    int currentPort = ui->spinBoxPort->value();
    if (currentPort == 0) currentPort = 1234; // lub ustaw np. 12345 jako wartość roboczą

    // Wypełnienie pól
    ipEdit.setText(currentIP);
    portEdit.setText(QString::number(currentPort));
    trybCombo.setCurrentIndex(ui->comboBoxRola->currentIndex());

    form.addRow("IP:", &ipEdit);
    form.addRow("Port:", &portEdit);
    form.addRow("Tryb:", &trybCombo);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addWidget(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString ip = ipEdit.text();
        QString port = portEdit.text();
        //QString tryb = trybCombo.currentText();

        // Przepisanie danych z dialogu do GUI
        ui->lineEditIP->setText(ip);
        ui->spinBoxPort->setValue(port.toInt());
        ui->comboBoxRola->setCurrentIndex(trybCombo.currentIndex());

        // Wywołanie funkcji połączenia
        on_btnPolacz_clicked();
    }
}


