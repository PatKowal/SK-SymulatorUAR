/********************************************************************************
** Form generated from reading UI file 'arxokno.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARXOKNO_H
#define UI_ARXOKNO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_ARXokno
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditA;
    QLabel *label_2;
    QLineEdit *lineEditB;
    QLabel *label_3;
    QSpinBox *spinBoxK;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ARXokno)
    {
        if (ARXokno->objectName().isEmpty())
            ARXokno->setObjectName("ARXokno");
        ARXokno->resize(400, 300);
        gridLayout = new QGridLayout(ARXokno);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(ARXokno);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEditA = new QLineEdit(ARXokno);
        lineEditA->setObjectName("lineEditA");

        gridLayout->addWidget(lineEditA, 0, 1, 1, 1);

        label_2 = new QLabel(ARXokno);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEditB = new QLineEdit(ARXokno);
        lineEditB->setObjectName("lineEditB");

        gridLayout->addWidget(lineEditB, 1, 1, 1, 1);

        label_3 = new QLabel(ARXokno);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        spinBoxK = new QSpinBox(ARXokno);
        spinBoxK->setObjectName("spinBoxK");
        spinBoxK->setMinimum(1);

        gridLayout->addWidget(spinBoxK, 2, 1, 1, 1);

        buttonBox = new QDialogButtonBox(ARXokno);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel
                                      | QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 2);

        retranslateUi(ARXokno);
        QObject::connect(buttonBox,
                         &QDialogButtonBox::accepted,
                         ARXokno,
                         qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox,
                         &QDialogButtonBox::rejected,
                         ARXokno,
                         qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ARXokno);
    } // setupUi

    void retranslateUi(QDialog *ARXokno)
    {
        ARXokno->setWindowTitle(QCoreApplication::translate("ARXokno", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ARXokno", "a", nullptr));
        label_2->setText(QCoreApplication::translate("ARXokno", "b", nullptr));
        label_3->setText(QCoreApplication::translate("ARXokno", "opo\305\272nienie", nullptr));
    } // retranslateUi
};

namespace Ui {
class ARXokno : public Ui_ARXokno
{};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARXOKNO_H
