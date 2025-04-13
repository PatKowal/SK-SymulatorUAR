#include "ModelARX.h"

#include <cstdlib>
#include <stdexcept>
#include <vector>

using namespace std;

ModelARX::ModelARX(const std::vector<double> &argA,
                   const std::vector<double> &argB,
                   int argK,
                   double argZ)
    : a(argA)
    , b(argB)
    , k(argK)
    , z(argZ)
    , last(0)
{
    if (k < 0) {
        throw std::invalid_argument("Opoznienie nie moze byc ujemne");
    }

    u.resize(k, 0.0);
    y.resize(k, 0.0);
}

ModelARX::~ModelARX() {}

void ModelARX::reset()
{
    u.clear();
    y.clear();
    last = 0;
}

double ModelARX::symuluj(double input)
{
    u.push_back(input);

    double output = 0.0;

    for (size_t i = 0; i < a.size(); ++i) {
        if (y.size() - 1 - i >= 0) {
            output -= a[i] * y[y.size() - 1 - i];
        }
    }

    for (size_t i = 0; i < b.size(); ++i) {
        if (u.size() - 1 - k >= 0) {
            double sign = rand() % 2 == 0 ? 1 : -1;
            double noise = sign * z * (float) rand() / RAND_MAX;
            output += b[i] * u[u.size() - 1 - k] + noise;
        }
    }

    //output += szum;
    y.push_back(output);

    last = output;
    return output;
}

double ModelARX::lastOutput() const
{
    return last;
}

void ModelARX::setParameters(const std::vector<double> &argA,
                             const std::vector<double> &argB,
                             int argK)
{
    a = argA;
    b = argB;
    k = argK;
}
