/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_6;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_3;
    QWidget *chartWidget;
    QHBoxLayout *horizontalLayout_5;
    QWidget *chartWidgetError;
    QWidget *widgetWartoscSterowania;
    QWidget *widgetPID;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *checkBoxTrybStacjonarny;
    QLineEdit *lineEditStan;
    QComboBox *comboBoxRola;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditIP;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpinBox *spinBoxPort;
    QPushButton *buttonKonfSieciowa;
    QPushButton *testD;
    QGroupBox *groupBoxSignal;
    QGridLayout *gridLayout;
    QRadioButton *radioButtonUnit;
    QLabel *labelValue;
    QDoubleSpinBox *doubleSpinBoxValue;
    QRadioButton *radioButtonRect;
    QLabel *labelTime;
    QDoubleSpinBox *doubleSpinBoxTime;
    QRadioButton *radioButtonSinus;
    QLabel *labelAmplitude;
    QDoubleSpinBox *doubleSpinBoxSinusAmp;
    QGroupBox *groupBoxARX;
    QGridLayout *gridLayout_2;
    QLabel *labelA;
    QLineEdit *lineEditA;
    QLabel *labelB;
    QLineEdit *lineEditB;
    QLabel *labelK;
    QSpinBox *spinBoxK;
    QGroupBox *groupBoxPID;
    QGridLayout *gridLayout_3;
    QLabel *labelI;
    QDoubleSpinBox *doubleSpinBoxP;
    QLabel *labelD;
    QDoubleSpinBox *doubleSpinBoxD;
    QLabel *labelP;
    QDoubleSpinBox *doubleSpinBoxI;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_4;
    QSpinBox *spinBoxInterval;
    QLabel *labelInterval;
    QLabel *labelNoise;
    QCheckBox *checkBoxCalkaPodSuma;
    QDoubleSpinBox *doubleSpinBoxNoise;
    QGridLayout *gridLayout_5;
    QPushButton *pushButtonLoad;
    QPushButton *pushButtonSave;
    QPushButton *pushButtonARX;
    QPushButton *pushButtonReset;
    QPushButton *pushButtonStop;
    QPushButton *pushButtonStart;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(1900, 1000);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1900, 1000));
        MainWindow->setMaximumSize(QSize(1920, 1080));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        centralwidget->setMinimumSize(QSize(0, 0));
        centralwidget->setMaximumSize(QSize(16777215, 16777205));
        gridLayout_6 = new QGridLayout(centralwidget);
        gridLayout_6->setObjectName("gridLayout_6");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(-1, -1, 0, -1);
        chartWidget = new QWidget(centralwidget);
        chartWidget->setObjectName("chartWidget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(chartWidget->sizePolicy().hasHeightForWidth());
        chartWidget->setSizePolicy(sizePolicy1);
        chartWidget->setAutoFillBackground(false);
        chartWidget->setStyleSheet(QString::fromUtf8("border: 2px solid black;\n"
"border-radius: 5px;\n"
""));

        verticalLayout_3->addWidget(chartWidget);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(-1, 0, -1, -1);
        chartWidgetError = new QWidget(centralwidget);
        chartWidgetError->setObjectName("chartWidgetError");
        sizePolicy1.setHeightForWidth(chartWidgetError->sizePolicy().hasHeightForWidth());
        chartWidgetError->setSizePolicy(sizePolicy1);
        chartWidgetError->setAutoFillBackground(false);
        chartWidgetError->setStyleSheet(QString::fromUtf8("border: 2px solid black;\n"
"border-radius: 5px;\n"
""));

        horizontalLayout_5->addWidget(chartWidgetError);

        widgetWartoscSterowania = new QWidget(centralwidget);
        widgetWartoscSterowania->setObjectName("widgetWartoscSterowania");
        widgetWartoscSterowania->setAutoFillBackground(false);
        widgetWartoscSterowania->setStyleSheet(QString::fromUtf8("border: 2px solid black;\n"
"border-radius: 5px;\n"
"\n"
""));

        horizontalLayout_5->addWidget(widgetWartoscSterowania);

        widgetPID = new QWidget(centralwidget);
        widgetPID->setObjectName("widgetPID");
        widgetPID->setAutoFillBackground(false);
        widgetPID->setStyleSheet(QString::fromUtf8("border: 2px solid black;\n"
"border-radius: 5px;\n"
""));

        horizontalLayout_5->addWidget(widgetPID);


        verticalLayout_3->addLayout(horizontalLayout_5);


        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        verticalLayout->setContentsMargins(-1, -1, 0, -1);
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalLayout_7->setContentsMargins(-1, 10, -1, -1);
        checkBoxTrybStacjonarny = new QCheckBox(groupBox);
        checkBoxTrybStacjonarny->setObjectName("checkBoxTrybStacjonarny");
        checkBoxTrybStacjonarny->setEnabled(true);
        checkBoxTrybStacjonarny->setChecked(true);

        horizontalLayout_7->addWidget(checkBoxTrybStacjonarny);


        verticalLayout_2->addLayout(horizontalLayout_7);

        lineEditStan = new QLineEdit(groupBox);
        lineEditStan->setObjectName("lineEditStan");
        lineEditStan->setEnabled(false);
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lineEditStan->sizePolicy().hasHeightForWidth());
        lineEditStan->setSizePolicy(sizePolicy3);
        lineEditStan->setStyleSheet(QString::fromUtf8("color: Red;\n"
"background: white;"));

        verticalLayout_2->addWidget(lineEditStan);

        comboBoxRola = new QComboBox(groupBox);
        comboBoxRola->addItem(QString());
        comboBoxRola->addItem(QString());
        comboBoxRola->setObjectName("comboBoxRola");
        comboBoxRola->setEnabled(false);

        verticalLayout_2->addWidget(comboBoxRola);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        label = new QLabel(groupBox);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        lineEditIP = new QLineEdit(groupBox);
        lineEditIP->setObjectName("lineEditIP");
        lineEditIP->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(lineEditIP->sizePolicy().hasHeightForWidth());
        lineEditIP->setSizePolicy(sizePolicy4);

        horizontalLayout->addWidget(lineEditIP);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(-1, 0, -1, -1);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        horizontalLayout_2->addWidget(label_2);

        spinBoxPort = new QSpinBox(groupBox);
        spinBoxPort->setObjectName("spinBoxPort");
        spinBoxPort->setEnabled(false);

        horizontalLayout_2->addWidget(spinBoxPort);


        verticalLayout_2->addLayout(horizontalLayout_2);

        buttonKonfSieciowa = new QPushButton(groupBox);
        buttonKonfSieciowa->setObjectName("buttonKonfSieciowa");

        verticalLayout_2->addWidget(buttonKonfSieciowa);


        verticalLayout->addWidget(groupBox);

        testD = new QPushButton(centralwidget);
        testD->setObjectName("testD");

        verticalLayout->addWidget(testD);

        groupBoxSignal = new QGroupBox(centralwidget);
        groupBoxSignal->setObjectName("groupBoxSignal");
        sizePolicy2.setHeightForWidth(groupBoxSignal->sizePolicy().hasHeightForWidth());
        groupBoxSignal->setSizePolicy(sizePolicy2);
        gridLayout = new QGridLayout(groupBoxSignal);
        gridLayout->setObjectName("gridLayout");
        radioButtonUnit = new QRadioButton(groupBoxSignal);
        radioButtonUnit->setObjectName("radioButtonUnit");
        radioButtonUnit->setChecked(true);

        gridLayout->addWidget(radioButtonUnit, 0, 0, 1, 1);

        labelValue = new QLabel(groupBoxSignal);
        labelValue->setObjectName("labelValue");

        gridLayout->addWidget(labelValue, 0, 1, 1, 1);

        doubleSpinBoxValue = new QDoubleSpinBox(groupBoxSignal);
        doubleSpinBoxValue->setObjectName("doubleSpinBoxValue");
        doubleSpinBoxValue->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBoxValue, 0, 2, 1, 1);

        radioButtonRect = new QRadioButton(groupBoxSignal);
        radioButtonRect->setObjectName("radioButtonRect");

        gridLayout->addWidget(radioButtonRect, 1, 0, 1, 1);

        labelTime = new QLabel(groupBoxSignal);
        labelTime->setObjectName("labelTime");

        gridLayout->addWidget(labelTime, 1, 1, 1, 1);

        doubleSpinBoxTime = new QDoubleSpinBox(groupBoxSignal);
        doubleSpinBoxTime->setObjectName("doubleSpinBoxTime");
        doubleSpinBoxTime->setMinimum(0.250000000000000);
        doubleSpinBoxTime->setMaximum(99000.000000000000000);
        doubleSpinBoxTime->setSingleStep(0.100000000000000);
        doubleSpinBoxTime->setValue(10.000000000000000);

        gridLayout->addWidget(doubleSpinBoxTime, 1, 2, 1, 1);

        radioButtonSinus = new QRadioButton(groupBoxSignal);
        radioButtonSinus->setObjectName("radioButtonSinus");

        gridLayout->addWidget(radioButtonSinus, 2, 0, 1, 1);

        labelAmplitude = new QLabel(groupBoxSignal);
        labelAmplitude->setObjectName("labelAmplitude");

        gridLayout->addWidget(labelAmplitude, 2, 1, 1, 1);

        doubleSpinBoxSinusAmp = new QDoubleSpinBox(groupBoxSignal);
        doubleSpinBoxSinusAmp->setObjectName("doubleSpinBoxSinusAmp");
        doubleSpinBoxSinusAmp->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBoxSinusAmp, 2, 2, 1, 1);


        verticalLayout->addWidget(groupBoxSignal);

        groupBoxARX = new QGroupBox(centralwidget);
        groupBoxARX->setObjectName("groupBoxARX");
        sizePolicy2.setHeightForWidth(groupBoxARX->sizePolicy().hasHeightForWidth());
        groupBoxARX->setSizePolicy(sizePolicy2);
        gridLayout_2 = new QGridLayout(groupBoxARX);
        gridLayout_2->setObjectName("gridLayout_2");
        labelA = new QLabel(groupBoxARX);
        labelA->setObjectName("labelA");

        gridLayout_2->addWidget(labelA, 0, 0, 1, 1);

        lineEditA = new QLineEdit(groupBoxARX);
        lineEditA->setObjectName("lineEditA");
        lineEditA->setEnabled(false);
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(lineEditA->sizePolicy().hasHeightForWidth());
        lineEditA->setSizePolicy(sizePolicy5);

        gridLayout_2->addWidget(lineEditA, 0, 1, 1, 1);

        labelB = new QLabel(groupBoxARX);
        labelB->setObjectName("labelB");

        gridLayout_2->addWidget(labelB, 0, 2, 1, 1);

        lineEditB = new QLineEdit(groupBoxARX);
        lineEditB->setObjectName("lineEditB");
        lineEditB->setEnabled(false);
        sizePolicy5.setHeightForWidth(lineEditB->sizePolicy().hasHeightForWidth());
        lineEditB->setSizePolicy(sizePolicy5);

        gridLayout_2->addWidget(lineEditB, 0, 3, 1, 1);

        labelK = new QLabel(groupBoxARX);
        labelK->setObjectName("labelK");

        gridLayout_2->addWidget(labelK, 1, 0, 1, 2);

        spinBoxK = new QSpinBox(groupBoxARX);
        spinBoxK->setObjectName("spinBoxK");
        spinBoxK->setEnabled(false);
        sizePolicy5.setHeightForWidth(spinBoxK->sizePolicy().hasHeightForWidth());
        spinBoxK->setSizePolicy(sizePolicy5);
        spinBoxK->setMinimum(1);

        gridLayout_2->addWidget(spinBoxK, 1, 2, 1, 2);


        verticalLayout->addWidget(groupBoxARX);

        groupBoxPID = new QGroupBox(centralwidget);
        groupBoxPID->setObjectName("groupBoxPID");
        sizePolicy2.setHeightForWidth(groupBoxPID->sizePolicy().hasHeightForWidth());
        groupBoxPID->setSizePolicy(sizePolicy2);
        gridLayout_3 = new QGridLayout(groupBoxPID);
        gridLayout_3->setObjectName("gridLayout_3");
        labelI = new QLabel(groupBoxPID);
        labelI->setObjectName("labelI");

        gridLayout_3->addWidget(labelI, 0, 2, 1, 1);

        doubleSpinBoxP = new QDoubleSpinBox(groupBoxPID);
        doubleSpinBoxP->setObjectName("doubleSpinBoxP");
        doubleSpinBoxP->setSingleStep(0.100000000000000);

        gridLayout_3->addWidget(doubleSpinBoxP, 0, 1, 1, 1);

        labelD = new QLabel(groupBoxPID);
        labelD->setObjectName("labelD");

        gridLayout_3->addWidget(labelD, 0, 4, 1, 1);

        doubleSpinBoxD = new QDoubleSpinBox(groupBoxPID);
        doubleSpinBoxD->setObjectName("doubleSpinBoxD");
        doubleSpinBoxD->setSingleStep(0.100000000000000);

        gridLayout_3->addWidget(doubleSpinBoxD, 0, 5, 1, 1);

        labelP = new QLabel(groupBoxPID);
        labelP->setObjectName("labelP");

        gridLayout_3->addWidget(labelP, 0, 0, 1, 1);

        doubleSpinBoxI = new QDoubleSpinBox(groupBoxPID);
        doubleSpinBoxI->setObjectName("doubleSpinBoxI");
        doubleSpinBoxI->setSingleStep(0.100000000000000);

        gridLayout_3->addWidget(doubleSpinBoxI, 0, 3, 1, 1);


        verticalLayout->addWidget(groupBoxPID);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy2);
        gridLayout_7 = new QGridLayout(groupBox_2);
        gridLayout_7->setObjectName("gridLayout_7");
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName("gridLayout_4");
        spinBoxInterval = new QSpinBox(groupBox_2);
        spinBoxInterval->setObjectName("spinBoxInterval");
        spinBoxInterval->setMinimum(100);
        spinBoxInterval->setMaximum(2000);

        gridLayout_4->addWidget(spinBoxInterval, 0, 1, 1, 1);

        labelInterval = new QLabel(groupBox_2);
        labelInterval->setObjectName("labelInterval");

        gridLayout_4->addWidget(labelInterval, 0, 0, 1, 1);

        labelNoise = new QLabel(groupBox_2);
        labelNoise->setObjectName("labelNoise");

        gridLayout_4->addWidget(labelNoise, 0, 2, 1, 1);

        checkBoxCalkaPodSuma = new QCheckBox(groupBox_2);
        checkBoxCalkaPodSuma->setObjectName("checkBoxCalkaPodSuma");

        gridLayout_4->addWidget(checkBoxCalkaPodSuma, 0, 4, 1, 1);

        doubleSpinBoxNoise = new QDoubleSpinBox(groupBox_2);
        doubleSpinBoxNoise->setObjectName("doubleSpinBoxNoise");
        doubleSpinBoxNoise->setSingleStep(0.010000000000000);

        gridLayout_4->addWidget(doubleSpinBoxNoise, 0, 3, 1, 1);


        gridLayout_7->addLayout(gridLayout_4, 0, 0, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName("gridLayout_5");
        pushButtonLoad = new QPushButton(groupBox_2);
        pushButtonLoad->setObjectName("pushButtonLoad");

        gridLayout_5->addWidget(pushButtonLoad, 2, 1, 1, 1);

        pushButtonSave = new QPushButton(groupBox_2);
        pushButtonSave->setObjectName("pushButtonSave");

        gridLayout_5->addWidget(pushButtonSave, 2, 0, 1, 1);

        pushButtonARX = new QPushButton(groupBox_2);
        pushButtonARX->setObjectName("pushButtonARX");

        gridLayout_5->addWidget(pushButtonARX, 1, 0, 1, 1);

        pushButtonReset = new QPushButton(groupBox_2);
        pushButtonReset->setObjectName("pushButtonReset");

        gridLayout_5->addWidget(pushButtonReset, 1, 1, 1, 1);

        pushButtonStop = new QPushButton(groupBox_2);
        pushButtonStop->setObjectName("pushButtonStop");

        gridLayout_5->addWidget(pushButtonStop, 0, 1, 1, 1);

        pushButtonStart = new QPushButton(groupBox_2);
        pushButtonStart->setObjectName("pushButtonStart");

        gridLayout_5->addWidget(pushButtonStart, 0, 0, 1, 1);


        gridLayout_7->addLayout(gridLayout_5, 1, 0, 1, 1);


        verticalLayout->addWidget(groupBox_2);


        horizontalLayout_3->addLayout(verticalLayout);


        gridLayout_6->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1900, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Po\305\202\304\205czenie Sieciowe", nullptr));
        checkBoxTrybStacjonarny->setText(QCoreApplication::translate("MainWindow", "Tryb Stacjonarny", nullptr));
        lineEditStan->setText(QCoreApplication::translate("MainWindow", "Roz\305\202\304\205czono", nullptr));
        comboBoxRola->setItemText(0, QCoreApplication::translate("MainWindow", "Regulator", nullptr));
        comboBoxRola->setItemText(1, QCoreApplication::translate("MainWindow", "ModelARX", nullptr));

        label->setText(QCoreApplication::translate("MainWindow", "Adres IP:", nullptr));
        lineEditIP->setText(QCoreApplication::translate("MainWindow", "127.0.0.1", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Port:", nullptr));
        buttonKonfSieciowa->setText(QCoreApplication::translate("MainWindow", "Konfiguracja sieciowa", nullptr));
        testD->setText(QCoreApplication::translate("MainWindow", "Syr", nullptr));
        groupBoxSignal->setTitle(QCoreApplication::translate("MainWindow", "Sygna\305\202 wej\305\233ciowy", nullptr));
        radioButtonUnit->setText(QCoreApplication::translate("MainWindow", "jednostkowy", nullptr));
        labelValue->setText(QCoreApplication::translate("MainWindow", "warto\305\233\304\207:", nullptr));
        radioButtonRect->setText(QCoreApplication::translate("MainWindow", "prostok\304\205tny", nullptr));
        labelTime->setText(QCoreApplication::translate("MainWindow", "czas /okres [s]:", nullptr));
        radioButtonSinus->setText(QCoreApplication::translate("MainWindow", "sinusoidalny", nullptr));
        labelAmplitude->setText(QCoreApplication::translate("MainWindow", "amplituda:", nullptr));
        groupBoxARX->setTitle(QCoreApplication::translate("MainWindow", "Model ARX", nullptr));
        labelA->setText(QCoreApplication::translate("MainWindow", "a:", nullptr));
        lineEditA->setText(QString());
        labelB->setText(QCoreApplication::translate("MainWindow", "b:", nullptr));
        lineEditB->setText(QString());
        labelK->setText(QCoreApplication::translate("MainWindow", "op\303\263\305\272nienie:", nullptr));
        groupBoxPID->setTitle(QCoreApplication::translate("MainWindow", "Regulator PID", nullptr));
        labelI->setText(QCoreApplication::translate("MainWindow", "I:", nullptr));
        labelD->setText(QCoreApplication::translate("MainWindow", "D:", nullptr));
        labelP->setText(QCoreApplication::translate("MainWindow", "P:", nullptr));
        groupBox_2->setTitle(QString());
        labelInterval->setText(QCoreApplication::translate("MainWindow", "interwa\305\202 [ms]:", nullptr));
        labelNoise->setText(QCoreApplication::translate("MainWindow", "szum:", nullptr));
        checkBoxCalkaPodSuma->setText(QCoreApplication::translate("MainWindow", "CPS", nullptr));
        pushButtonLoad->setText(QCoreApplication::translate("MainWindow", "Odczytaj z pliku", nullptr));
        pushButtonSave->setText(QCoreApplication::translate("MainWindow", "Zapisz do pliku", nullptr));
        pushButtonARX->setText(QCoreApplication::translate("MainWindow", "ARX ustawienia", nullptr));
        pushButtonReset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        pushButtonStop->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        pushButtonStart->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
