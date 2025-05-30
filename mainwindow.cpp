#include "mainwindow.h"
#include <QChart>
#include <QChartView>
#include <QFileDialog>
#include <QGridLayout>
#include <QLineSeries>
#include <QMessageBox>
#include "arxokno.h"
#include "qdialogbuttonbox.h"
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

    ui->groupBoxARX->setVisible(false);

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

    // wykres error:
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

    // wykres PID:
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

    // wykres sterowanie:
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

MainWindow::~MainWindow(){ delete ui; }
void MainWindow::updateChart()
{
    double input = 0;

    time += newInterval / 1000.0;
    qDebug() << "TIME: " << time;

    if (ui->radioButtonUnit->isChecked()) {
            input = WARTOSC;
    }
    else if (ui->radioButtonRect->isChecked()) {
            unsigned long long okres = OKRES;
            bool yes = (unsigned long long) time % okres < 0.5 * okres;
            input = yes ? WARTOSC : 0;
    }
    else if (ui->radioButtonSinus->isChecked()) {
            double x = (double) (2 * M_PI * time) / OKRES;
            input = WARTOSC + AMPLITUDA * sin(x);
    }
    double output = 0.0;
    if(ui->checkBoxTrybStacjonarny->isChecked()){
        output = uar->symuluj(input);
    } else {
        if(m_server != nullptr) {
            if(ui->comboBoxRola->currentIndex() == 0 && m_server->getNumClients() > 0){
                double inputPid = input - OutputReceived;
                double pidOutput = pid->symuluj(inputPid);

                qint64 timeonsend = QDateTime::currentMSecsSinceEpoch();
                QByteArray msg;
                QDataStream out(&msg, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_6_0);
                out << input << pidOutput << timeonsend;
                m_server->sendFramedToClients(2,msg);

                output = OutputReceived;
            }
        }
        if(ui->comboBoxRola->currentIndex() == 1 && m_client->isConnected()){
            output = OutputReceived;
        }
    }

    auto rescaleYAxis = [](QChart *chart) {
        double minVal = std::numeric_limits<double>::max();
        double maxVal = std::numeric_limits<double>::lowest();

        for (auto series : chart->series()) {
            if (auto lineSeries = qobject_cast<QLineSeries*>(series)) {
                for (const QPointF &point : lineSeries->points()) {
                    minVal = std::min(minVal, point.y());
                    maxVal = std::max(maxVal, point.y());
                }
            }
        }

        if (minVal >= maxVal || (fabs(minVal) < 0.1 && fabs(maxVal) < 0.1)) {
            chart->axes(Qt::Vertical).first()->setRange(-0.1, 0.1);
        } else {
            chart->axes(Qt::Vertical).first()->setRange(
                minVal - 0.1 * fabs(minVal),
                maxVal + 0.1 * fabs(maxVal)
                );
        }
    };

     if(ui->checkBoxTrybStacjonarny->isChecked() || (ui->comboBoxRola->currentIndex() == 0 && m_server->getNumClients() > 0)){

        rescaleYAxis(chartPID);
        rescaleYAxis(chartSterowanie);
        rescaleYAxis(chartError);
        rescaleYAxis(chart);

        pSeries->append(time, pid->czlonP());
        iSeries->append(time, pid->czlonI());
        dSeries->append(time, pid->czlonD());

        sterowanieSeries->append(time, pid->wartoscPID());

        errSeries->append(time, fabs(input - output));
        inSeries->append(time, input);
        outSeries->append(time, output);
    }
    else {
        if(!ui->checkBoxTrybStacjonarny->isChecked() && m_client != nullptr && m_client->isConnected()){
            rescaleYAxis(chartSterowanie);
            rescaleYAxis(chart);

            //inSeries ->setVisible(false);

            sterowanieSeries->append(time, pidOutputReceived);
            outSeries->append(time, output);
            inSeries->append(time, inputReceived);
        }
    }

    if (time > withXAxis) {
        double t = time;
        chart->axes(Qt::Horizontal).first()->setRange(t - withXAxis, t);
        chartError->axes(Qt::Horizontal).first()->setRange(t - withXAxis, t);
        chartPID->axes(Qt::Horizontal).first()->setRange(t - withXAxis, t);
        chartSterowanie->axes(Qt::Horizontal).first()->setRange(t - withXAxis, t);

        double minTime = t - withXAxis;
        auto trimSeries = [minTime](QChart *chart) {
            for (QAbstractSeries *series : chart->series()) {
                if (QLineSeries *lineSeries = qobject_cast<QLineSeries *>(series)) {
                    QVector<QPointF> points = lineSeries->points();
                    while (!points.isEmpty() && points.first().x() < minTime) {
                        points.removeFirst();
                    }
                    lineSeries->replace(points);
                }
            }
        };
        trimSeries(chart);
        trimSeries(chartError);
        trimSeries(chartPID);
        trimSeries(chartSterowanie);
    }
}

void MainWindow::on_pushButtonStart_clicked()
{
    if(m_server != nullptr){
        if(ui->comboBoxRola->currentIndex() == 0 && m_server->getNumClients() > 0){ //Serwer
            QByteArray msg;
            QDataStream out(&msg, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);
            m_server->sendFramedToClients(1,msg);
        }
    }
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

void MainWindow::on_pushButtonReset_clicked() {
    if(m_server != nullptr){
        if(ui->comboBoxRola->currentIndex() == 0 && m_server->getNumClients() > 0){ //Serwer
            QByteArray msg;
            QDataStream out(&msg, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);
            m_server->sendFramedToClients(5,msg);
        }
    }
    resetDefaultValues();
    resetAllSettings();
}
void MainWindow::on_pushButtonStop_clicked() {
    if(m_server != nullptr){
        if(ui->comboBoxRola->currentIndex() == 0 && m_server->getNumClients() > 0){ //Serwer
            QByteArray msg;
            QDataStream out(&msg, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);
            m_server->sendFramedToClients(4,msg);
        }
    }
    timer->stop();
}
void MainWindow::on_spinBoxK_valueChanged(int arg1) { Q_UNUSED(arg1); updateSettings(); }
void MainWindow::on_lineEditA_editingFinished() { updateSettings(); }
void MainWindow::on_lineEditB_editingFinished() { updateSettings(); }

void MainWindow::on_spinBoxInterval_editingFinished()
{
    timer->stop();

    newInterval = ui->spinBoxInterval->value();
    if(m_server != nullptr){
        if(ui->comboBoxRola->currentIndex() == 0 && m_server->getNumClients() > 0){ //Serwer
            QByteArray msg;
            QDataStream out(&msg, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);
            out << newInterval;
            m_server->sendFramedToClients(3,msg);
        }
    }

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
    }
}

void MainWindow::on_doubleSpinBoxP_editingFinished() { updateSettings(); }
void MainWindow::on_doubleSpinBoxI_editingFinished() { updateSettings(); }
void MainWindow::on_doubleSpinBoxD_editingFinished() { updateSettings(); }
void MainWindow::on_doubleSpinBoxNoise_editingFinished() { updateSettings(); }
void MainWindow::on_doubleSpinBoxValue_editingFinished() { WARTOSC = ui->doubleSpinBoxValue->value(); updateSettings(); }
void MainWindow::on_doubleSpinBoxTime_editingFinished(){ OKRES = ui->doubleSpinBoxTime->value(); updateSettings(); }
void MainWindow::on_doubleSpinBoxSinusAmp_editingFinished() { AMPLITUDA = ui->doubleSpinBoxSinusAmp->value(); updateSettings(); }
void MainWindow::on_checkBoxCalkaPodSuma_toggled(bool checked) { pid->ustawCalkaPodSuma(checked); }

void MainWindow::onResultReceived(double result, qint64 timeonsend) {
    qint64 currtime = QDateTime::currentMSecsSinceEpoch();
    qint64 ping = currtime - timeonsend;
    ui->labelPing->setText(QString("%1 ms").arg(ping));
    if(ping > newInterval){
        ui->labelPing->setStyleSheet("color: red;");
    } else {
        ui->labelPing->setStyleSheet("color: green;");
    }
    if(ping < newInterval){
        OutputReceived = result;
    }
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
    connect(m_server, &MyTCPServer::resultReceived, this, &MainWindow::onResultReceived);
}

bool MainWindow::validatePort(int port)
{
    if(port < 0 || 65535 < port)
    {
        ui->lineEditStan->setText("Niepoprawny numer portu!");
        ui->lineEditStan->setStyleSheet("color: red;");
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
    connect(m_client, &MyTCPClient::ModelARXRequest, this, &MainWindow::onModelARXRequest);
    connect(m_client, &MyTCPClient::SymulujRequest, this, &MainWindow::onSymulujRequest);

    connect(m_client, &MyTCPClient::StartSimOnClient, this, &MainWindow::onStartSimOnClient);
    connect(m_client, &MyTCPClient::StopSimOnClient, this, &MainWindow::onStopSimOnClient);
    connect(m_client, &MyTCPClient::ResetSimOnClient, this, &MainWindow::onResetSimOnClient);

    connect(m_client, &MyTCPClient::IntervalOnServerChanged, this, &MainWindow::onIntervalOnServerChanged);
}

bool MainWindow::validateConnectionData(QString adr, int port)
{
    QHostAddress ipAdr(adr);
    if(ipAdr.protocol() != QAbstractSocket::IPv4Protocol)
    {
        ui->lineEditStan->setText("Niepoprawny adres IPv4!");
        ui->lineEditStan->setStyleSheet("color: red;");
        return false;
    }
    if(port < 0 || 65535 < port)
    {
        ui->lineEditStan->setText("Nieprawidłowy numer portu!");
         ui->lineEditStan->setStyleSheet("color: red;");
        return false;
    }
    return true;
}

void MainWindow::slot_newClientConnected(QString adr)
{
    ui->lineEditStan->setText("Klient: " + adr);
     ui->lineEditStan->setStyleSheet("color: green;");
    updateCliNum();
}

void MainWindow::slot_clientDisconnected(int num)
{
    QString text = "Klient " + QString::number(num) + " się rozłączył!";
    ui->lineEditStan->setText(text);
    ui->lineEditStan->setStyleSheet("color: red;");
    qDebug() << "Klient " + QString::number(num) + " się rozłączył!";
    updateCliNum();
    if(m_server->getNumClients() == 0){
        ui->checkBoxTrybStacjonarny->setChecked(true);
        on_checkBoxTrybStacjonarny_stateChanged(1);
    }
}

void MainWindow::updateCliNum() { int numCli = m_server->getNumClients(); Q_UNUSED(numCli); }

void MainWindow::slot_connected(QString adr, int port)
{
    ui->lineEditStan->setText("Połączono z " + adr + ":" + QString::number(port));
    ui->lineEditStan->setStyleSheet("color: green;");
}

void MainWindow::slot_disconnected()
{
    ui->lineEditStan->setText("Rozłączono");
    ui->lineEditStan->setStyleSheet("color: red;");
    ui->checkBoxTrybStacjonarny->setChecked(true);
    activeAll();
}

void MainWindow::on_btnPolacz_clicked()
{
    if (ui->checkBoxTrybStacjonarny->isChecked() == false) {
        QString host = ui->lineEditIP->text();

        if (ui->comboBoxRola->currentIndex() == 0) {
            if(m_client != nullptr){
                m_client->disconnectFrom();
            }
            if(m_server != nullptr && m_server->isListening())
            {
                m_server->stopListening();
                ui->lineEditStan->setText("Zatrzymano serwer");
                ui->lineEditStan->setStyleSheet("color: red;");
            }
            else
            {
                int port = ui->spinBoxPort->text().toInt();
                if(!validatePort(port))
                    return;
                resetServer();
                if(!m_server->startListening(port)) {
                    ui->lineEditStan->setText("Error uruchamiania serwera!");
                    ui->lineEditStan->setStyleSheet("color: red;");
                }
                else
                {
                    ui->lineEditStan->setText("Serwer nasłuchuje na porcie " + QString::number(port));
                    ui->lineEditStan->setStyleSheet("color: green;");
                }
            }
        } else {
            if(m_server != nullptr && m_server->isListening())
            {
                m_server->stopListening();
                delete m_server;
            }
            QString host = ui->lineEditIP->text();
            int port = ui->spinBoxPort->text().toInt();
            if(!validateConnectionData(host, port))
                return;
            resetClient();
            ui->lineEditStan->setText("[CLIENT]: Nasłuchiwanie na " + host + ":" + QString::number(port));
            ui->lineEditStan->setStyleSheet("color: green;");
            m_client->connectTo(host,port);
        }
    }
}

void MainWindow::on_checkBoxTrybStacjonarny_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    if (ui->comboBoxRola->currentIndex() == 0) {
        if(m_server != nullptr && m_server->isListening())
        {
            if(m_server->getNumClients() > 0){
                QByteArray msg;
                m_server->sendFramedToClients(3,msg); // emit u klienta
            }
            m_server->stopListening();
            ui->lineEditStan->setText("Zerwanie połączenia zmieniam na pracę stacjonarną.");
            ui->lineEditStan->setStyleSheet("color: red;");
            activeAll();
        }
    } else{
        if(m_client != nullptr && !m_client->isConnected())
        {
        m_client->disconnectFrom();
        // ui->lineEditStan->setText("Serwer jest wyłączony");
        } else {
            ui->lineEditStan->setText("Serwer jest wyłączony");
        }
    }
}

void MainWindow::onModelARXRequest(){
    QByteArray serial = Serializer::serialize(*arx);

    m_client->sendFramed(101, serial);
    qDebug() << "[MainWindow] ModelARX wysłany do serwera.";
}

void MainWindow::onIntervalOnServerChanged(int value){
    ui->spinBoxInterval->setValue(value);
    on_spinBoxInterval_editingFinished();
}

void MainWindow::onSymulujRequest(double inputReceived, double value, qint64 timeonsend){
    double result = arx->symuluj(value);
    pidOutputReceived = value;
    OutputReceived = result;
    this->inputReceived = inputReceived;
    QByteArray response;
    QDataStream out(&response, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << result << timeonsend;
    m_client->sendFramed(102, response);
}

void MainWindow::onStartSimOnClient(){ on_pushButtonStart_clicked(); }
void MainWindow::onStopSimOnClient(){ on_pushButtonStop_clicked(); }
void MainWindow::onResetSimOnClient(){ on_pushButtonReset_clicked(); }

void MainWindow::on_buttonKonfSieciowa_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Konfiguracja połączenia sieciowego");

    QFormLayout form(&dialog);

    QLineEdit ipEdit, portEdit;
    QComboBox trybCombo;
    trybCombo.addItems({"Regulator", "Model ARX"});

    QString currentIP = ui->lineEditIP->text().isEmpty() ? "127.0.0.1" : ui->lineEditIP->text();
    int currentPort = ui->spinBoxPort->value();
    if (currentPort == 0) currentPort = 1234;

    ipEdit.setText(currentIP);
    portEdit.setText(QString::number(currentPort));
    trybCombo.setCurrentIndex(ui->comboBoxRola->currentIndex());

    QCheckBox stacjonarnyCheckbox("Tryb stacjonarny");
    stacjonarnyCheckbox.setChecked(ui->checkBoxTrybStacjonarny->isChecked());

    form.addRow(&stacjonarnyCheckbox);
    form.addRow("IP:", &ipEdit);
    form.addRow("Port:", &portEdit);
    form.addRow("Tryb:", &trybCombo);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addWidget(&buttons);

    auto updateWidgetsEnabledState = [&]() {
        bool enabled = !stacjonarnyCheckbox.isChecked();
        ipEdit.setEnabled(enabled);
        portEdit.setEnabled(enabled);
        trybCombo.setEnabled(enabled);
    };

    QObject::connect(&stacjonarnyCheckbox, &QCheckBox::toggled, updateWidgetsEnabledState);
    updateWidgetsEnabledState();

    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        bool isStacjonarny = stacjonarnyCheckbox.isChecked();
        ui->checkBoxTrybStacjonarny->setChecked(isStacjonarny);

        QString selectedMode = trybCombo.currentText();
        bool isRegulator = (selectedMode == "Regulator");

        ui->doubleSpinBoxP->setEnabled(isRegulator);
        ui->doubleSpinBoxI->setEnabled(isRegulator);
        ui->doubleSpinBoxD->setEnabled(isRegulator);
        ui->spinBoxInterval->setEnabled(isRegulator);
        ui->checkBoxCalkaPodSuma->setEnabled(isRegulator);
        ui->pushButtonStart->setEnabled(isRegulator);
        ui->pushButtonStop->setEnabled(isRegulator);
        ui->pushButtonReset->setEnabled(isRegulator);
        ui->pushButtonSave->setEnabled(isRegulator);
        ui->pushButtonLoad->setEnabled(isRegulator);
        ui->radioButtonRect->setEnabled(isRegulator);
        ui->radioButtonSinus->setEnabled(isRegulator);
        ui->radioButtonUnit->setEnabled(isRegulator);
        ui->doubleSpinBoxSinusAmp->setEnabled(isRegulator);
        ui->doubleSpinBoxTime->setEnabled(isRegulator);
        ui->doubleSpinBoxValue->setEnabled(isRegulator);

        ui->pushButtonARX->setEnabled(!isRegulator);
        ui->doubleSpinBoxNoise->setEnabled(!isRegulator);
        ui->chartWidgetError->setVisible(isRegulator);
        ui->widgetPID->setVisible(isRegulator);

        if (!isStacjonarny) {
            QString ip = ipEdit.text();
            QString port = portEdit.text();

            ui->lineEditIP->setText(ip);
            ui->spinBoxPort->setValue(port.toInt());
            ui->comboBoxRola->setCurrentIndex(trybCombo.currentIndex());

            on_btnPolacz_clicked();
        }
        else {
            activeAll();
        }
    }
}

void MainWindow::activeAll(){
    ui->doubleSpinBoxP->setEnabled(true);
    ui->doubleSpinBoxI->setEnabled(true);
    ui->doubleSpinBoxD->setEnabled(true);
    ui->spinBoxInterval->setEnabled(true);
    ui->doubleSpinBoxNoise->setEnabled(true);
    ui->checkBoxCalkaPodSuma->setEnabled(true);
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonStop->setEnabled(true);
    ui->pushButtonReset->setEnabled(true);
    ui->pushButtonSave->setEnabled(true);
    ui->pushButtonLoad->setEnabled(true);
    ui->radioButtonRect->setEnabled(true);
    ui->radioButtonSinus->setEnabled(true);
    ui->radioButtonUnit->setEnabled(true);
    ui->doubleSpinBoxSinusAmp->setEnabled(true);
    ui->doubleSpinBoxTime->setEnabled(true);
    ui->doubleSpinBoxValue->setEnabled(true);
    ui->pushButtonARX->setEnabled(true);
    ui->chartWidgetError->setVisible(true);
    ui->widgetPID->setVisible(true);
}
