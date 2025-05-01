#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "ModelARX.h"
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QVector>
#include <vector>

class Serializer{
public:
    static QByteArray serialize(const ModelARX& model){
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        std::vector<double> vecA = model.getA();
        std::vector<double> vecB = model.getB();
        QVector<double> qA(vecA.begin(), vecA.end());
        QVector<double> qB(vecB.begin(), vecB.end());

        out << qA << qB << model.getK() << model.getZ();
        return data;
    }

    static ModelARX deserialize(const QByteArray& data){
        QDataStream in(data);
        QVector<double> qA, qB;
        int k;
        double z;

        in >> qA >> qB >> k >> z;
        ModelARX model
            (
            std::vector<double>(qA.begin(),qA.end()),
            std::vector<double>(qB.begin(),qB.end()),
            k,
            z
            );
        return model;
    }
};

#endif // SERIALIZER_H
