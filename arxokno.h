#ifndef ARXOKNO_H
#define ARXOKNO_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class ARXokno;
}

class ARXokno : public QDialog
{
    Q_OBJECT

public:
    explicit ARXokno(QWidget *parent = nullptr);
    ~ARXokno();

    QStringList getA() const;
    QStringList getB() const;
    int getOpóźnienie() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ARXokno *ui;
};

#endif // ARXOKNO_H
