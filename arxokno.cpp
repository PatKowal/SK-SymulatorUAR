#include "arxokno.h"
#include "ui_arxokno.h"

ARXokno::ARXokno(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ARXokno)
{
    ui->setupUi(this);
}

ARXokno::~ARXokno()
{
    delete ui;
}

QStringList ARXokno::getA() const
{
    return ui->lineEditA->text().split(";", Qt::SkipEmptyParts);
}

QStringList ARXokno::getB() const
{
    return ui->lineEditB->text().split(";", Qt::SkipEmptyParts);
}

int ARXokno::getOpóźnienie() const
{
    return ui->spinBoxK->value();
}

void ARXokno::on_buttonBox_accepted()
{
    accept();
}

void ARXokno::on_buttonBox_rejected()
{
    reject();
}
