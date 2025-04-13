#ifndef PROSTYUAR_H
#define PROSTYUAR_H

#include "ModelARX.h"
#include "RegulatorPID.h"

class ProstyUAR
{
public:
    ProstyUAR(ModelARX &modelARX, RegulatorPID &regulatorPID);

    double symuluj(double input);

private:
    ModelARX &arx;
    RegulatorPID &pid;
};

#endif // PROSTYUAR_H
