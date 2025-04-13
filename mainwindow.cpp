#include "mainwindow.h"
#include <QChart>
#include <QChartView>
#include <QFileDialog>
#include <QGridLayout>
#include <QLineSeries>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include <vector>
#include <QDebug>

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
    ,netMan(new NetMan(this))

{
    ui->setupUi(this);
    ui->spinBoxPort->setRange(0,65535);
    ui->spinBoxPort->setValue(1234);

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

    connect(ui->cbxTrybSieciowy, &QCheckBox::stateChanged,this, &MainWindow::on_cbxTrybSieciowy_stateChanged);
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

void MainWindow::on_btnPolacz_clicked() {
    // Pobieramy dane z UI
    QString ip = ui->lineEditIP->text();
    quint16 port = static_cast<quint16>(ui->spinBoxPort->value());
    QString mode = ui->comboBoxRola->currentText();

    // Zmienna do przechowywania trybu, który jest obecnie serwerem
    static QString currentServerMode = "";

    // Sprawdzamy, czy wybrany tryb to ModelARX lub Regulator
    if (mode == "ModelARX" || mode == "Regulator") {
        // Jeśli jeszcze nie uruchomiono serwera
        if (currentServerMode.isEmpty()) {
            // Jeśli nie ma serwera, uruchamiamy serwer na wybranym trybie
            currentServerMode = mode;  // Ten tryb uruchamia serwer

            // Uruchamiamy serwer na podstawie wybranego trybu
            if (mode == "ModelARX") {
                netMan->startServer(port);  // Uruchamiamy serwer dla ModelARX
                ui->lineEditStan->setText("Uruchomiono serwer jako ModelARX");
                ui->lineEditStan->setStyleSheet("QLineEdit{ color: blue; background: white; }");
                trybModelARX();
            } else if (mode == "Regulator") {
                netMan->startServer(port);  // Uruchamiamy serwer dla Regulatora
                ui->lineEditStan->setText("Uruchomiono serwer jako Regulator");
                ui->lineEditStan->setStyleSheet("QLineEdit{ color: blue; background: white; }");
                trybRegulator();
            }
        } else {
            // Jeśli serwer już działa, łączymy się jako klient
            if (currentServerMode == "ModelARX" && mode == "Regulator") {
                // Jeśli ModelARX jest serwerem, uruchamiamy klienta dla Regulatora
                netMan->startClient(ip, port);  // Regulator działa jako klient
                ui->lineEditStan->setText("Połączono z serwerem ModelARX jako Regulator");
                ui->lineEditStan->setStyleSheet("QLineEdit{ color: green; background: white; }");
                trybRegulator();
            } else if (currentServerMode == "Regulator" && mode == "ModelARX") {
                // Jeśli Regulator jest serwerem, uruchamiamy klienta dla ModelARX
                netMan->startClient(ip, port);  // ModelARX działa jako klient
                ui->lineEditStan->setText("Połączono z serwerem Regulator jako ModelARX");
                ui->lineEditStan->setStyleSheet("QLineEdit{ color: green; background: white; }");
                trybModelARX();
            }
        }
    } else {
        // W przypadku nieznanego trybu
        ui->lineEditStan->setText("Nieznany tryb połączenia");
        ui->lineEditStan->setStyleSheet("QLineEdit{ color: black; background: white; }");
    }
}

void MainWindow::trybModelARX(){
    ui->groupBoxPID->setDisabled(true);
    ui->groupBoxSignal->setDisabled(true);
    ui->pushButtonStart->setDisabled(true);
    ui->pushButtonSave->setDisabled(true);
    ui->pushButtonLoad->setDisabled(true);
    ui->pushButtonStop->setDisabled(true);
    ui->pushButtonReset->setDisabled(true);
    ui->spinBoxInterval->setDisabled(true);
    ui->checkBoxCalkaPodSuma->setDisabled(true);
    ui->pushButtonARX->setEnabled(true);
    ui->doubleSpinBoxNoise->setEnabled(true);
}
void MainWindow::trybRegulator(){
    ui->groupBoxPID->setEnabled(true);
    ui->groupBoxSignal->setEnabled(true);
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonSave->setEnabled(true);
    ui->pushButtonLoad->setEnabled(true);
    ui->pushButtonStop->setEnabled(true);
    ui->pushButtonReset->setEnabled(true);
    ui->spinBoxInterval->setEnabled(true);
    ui->checkBoxCalkaPodSuma->setEnabled(true);
    ui->pushButtonARX->setDisabled(true);
    ui->doubleSpinBoxNoise->setDisabled(true);
}



void MainWindow::on_cbxTrybSieciowy_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        ui->groupBoxPID->setDisabled(true);
        ui->groupBoxSignal->setDisabled(true);
        ui->pushButtonStart->setDisabled(true);
        ui->pushButtonSave->setDisabled(true);
        ui->pushButtonLoad->setDisabled(true);
        ui->pushButtonStop->setDisabled(true);
        ui->pushButtonReset->setDisabled(true);
        ui->spinBoxInterval->setDisabled(true);
        ui->checkBoxCalkaPodSuma->setDisabled(true);
        ui->pushButtonARX->setDisabled(true);
        ui->doubleSpinBoxNoise->setDisabled(true);

        ui->btnPolacz->setEnabled(true);
    }else{
        ui->groupBoxPID->setEnabled(true);
        ui->groupBoxSignal->setEnabled(true);
        ui->pushButtonStart->setEnabled(true);
        ui->pushButtonSave->setEnabled(true);
        ui->pushButtonLoad->setEnabled(true);
        ui->pushButtonStop->setEnabled(true);
        ui->pushButtonReset->setEnabled(true);
        ui->spinBoxInterval->setEnabled(true);
        ui->checkBoxCalkaPodSuma->setEnabled(true);
        ui->pushButtonARX->setEnabled(true);
        ui->doubleSpinBoxNoise->setEnabled(true);

        ui->btnPolacz->setDisabled(true);
    }
}

