#ifndef UNTITLED3_MODELARX_H
#define UNTITLED3_MODELARX_H

#include <vector>

using namespace std;

class ModelARX
{
public:
    ModelARX(const std::vector<double> &argA,
             const std::vector<double> &argB,
             int argK = 1,
             double argZ = 0);
    ~ModelARX();

    void reset();

    void setParameters(const std::vector<double> &argA, const std::vector<double> &argB, int argK);
    double symuluj(double input);

    double lastOutput() const;

    std::vector<double> getA() const { return a; }
    std::vector<double> getB() const { return b; }
    int getK() const { return k; }
    double getZ() const { return z; }

    void ustawA(std::vector<double> wartosc) { a = wartosc; }
    void ustawB(std::vector<double> wartosc) { b = wartosc; }
    void ustawK(int wartosc) { k = wartosc; }
    void ustawZ(double wartosc) { z = wartosc; }

private:
    std::vector<double> a;
    std::vector<double> b;
    int k;
    double z;

    std::vector<double> u;
    std::vector<double> y;
    double last;
};

#endif
