/*
void uruchamianiePetliSprzezeniaZwrotnego() {
    double wspolczynnikA, wspolczynnikB;
    int opoznienieD;
    double setpoint;
    cout << "Podaj setpointa (double): ";
    cin >> setpoint;


    cout << "Podaj wspolczynnik A: ";
    cin >> wspolczynnikA;

    cout << "Podaj wspolczynnik B: ";
    cin >> wspolczynnikB;

    cout << "Podaj opoznienie D: ";
    cin >> opoznienieD;

    vector<double> wspA = { wspolczynnikA };
    vector<double> wspB = { wspolczynnikB };


    ModelARX model(wspA, wspB, opoznienieD);

    double wspolczynnikProporcjonalny;
    double wspolczynnikCalkujacy;
    double wspolczynnikRozniczkujacy;

    cout << "\nTeraz tworzymy objekt PID\n";
    cout << "Podaj wpolczynnik proporcjonalny: ";
    cin >> wspolczynnikProporcjonalny;
    cout << "Podaj wspolczynnik calkujacy: ";
    cin >> wspolczynnikCalkujacy;
    cout << " Podaj wspolczynnik rozniczkujacy:";
    cin >> wspolczynnikRozniczkujacy;

    RegulatorPID regulator(wspolczynnikProporcjonalny, wspolczynnikCalkujacy, wspolczynnikRozniczkujacy);


    double aktualnaWartosc = 0.0;
    double interwalCzasowy = 0.1;
    int maksymalnyCzas = 100;
    int liczbaIteracji = static_cast<int>(maksymalnyCzas / interwalCzasowy);

    for (int i = 0; i < liczbaIteracji; i++) {
        double aktualnyCzas = i * interwalCzasowy;
        double blad = setpoint - aktualnaWartosc;
        double sygnalSterujacy = regulator.obliczanieSygnalySterujacego(blad, aktualnaWartosc, interwalCzasowy);
        aktualnaWartosc = model.symuluj(sygnalSterujacy);

        cout << "\nCzas: " << aktualnyCzas << " s, " << endl;
        cout << "Blad: " << blad << endl;
        cout << "Sygnal sterujacy: " << sygnalSterujacy << endl;
        cout << "aktuana wartosc: " << aktualnaWartosc << endl;

        if (fabs(blad) < 0.01) {
            cout << "Blad jest wystarczajaco maly. Koniec symulacji." << endl;
            cout << "Symulacja zatrzymana po " << aktualnyCzas << " sekundach." << " Ostateczny blad: " << blad << endl;

            break;
        }
    }*/
