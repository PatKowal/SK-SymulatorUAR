#ifndef PID_PID_H
#define PID_PID_H

class RegulatorPID
{
private:
    double wspolczynnikProporcjonalny;
    double wspolczynnikCalkujacy;
    double wspolczynnikRozniczkujacy;

    double bladPoprzedniejIteracji;
    double sumaBledowDoSkladnikaCalkujacego;
    double aktualnySygnalSterujacy;
    bool calkaPodSuma;

    double p = 0;
    double i = 0;
    double d = 0;

public:
    RegulatorPID(double wP, double wC = 0.0, double wR = 0.0);
    double obliczanieSygnalySterujacego(double wartoscZadana,
                                        double wartoscRzeczywistaProcesu,
                                        double roznicaCzasu);
    void zmianaNastawRegulator(double wP, double wC, double wR);
    void resetowaniePamieciRegulatora();
    double symuluj(double wartoscWe);
    void ustawCalkaPodSuma(bool nowySposob);

    void ustawP(double wartosc) { wspolczynnikProporcjonalny = wartosc; }
    void ustawI(double wartosc) { wspolczynnikCalkujacy = wartosc; }
    void ustawD(double wartosc) { wspolczynnikRozniczkujacy = wartosc; }

    double czlonP() const { return p; }
    double czlonI() const { return i; }
    double czlonD() const { return d; }

    double wartoscPID() const { return p + i + d; }
};

#endif
