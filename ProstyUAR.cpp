#include "ProstyUAR.h"

ProstyUAR::ProstyUAR(ModelARX &modelARX, RegulatorPID &regulatorPID)
    : arx(modelARX)
    , pid(regulatorPID)
{}

double ProstyUAR::symuluj(double input)
{
    double inputPid = input - arx.lastOutput();
    double pidOutput = pid.symuluj(inputPid);
    double arxOutput = arx.symuluj(pidOutput);

    return arxOutput;
}
