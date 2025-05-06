#include "mytcpserver.h"
#include "ModelARX.h"
#include "Serializer.h"
#include <QDataStream>
#include <QDebug>

MyTCPServer::MyTCPServer(QObject *parent)
    : QObject{parent}, m_server(this)
{
    connect(&m_server, &QTcpServer::newConnection, this, &MyTCPServer::slot_new_client);
}

bool MyTCPServer::startListening(int port)
{
    m_port = port;
    m_isListening = m_server.listen(QHostAddress::Any, port);
    if (!m_isListening)
        qDebug() << "[SERVER] Failed to listen:" << m_server.errorString();
    return m_isListening;
}

void MyTCPServer::stopListening()
{
    m_server.close();
    m_isListening = false;
}

int MyTCPServer::getNumClients()
{
    return m_clients.size();
}

void MyTCPServer::sendMsg(QString msg, int numCli)
{
    if (numCli >= 0 && numCli < m_clients.size()) {
        m_clients.at(numCli)->write(msg.toUtf8());
    }
}

void MyTCPServer::sendFramed(quint8 type, const QByteArray& data, int clientIndex)
{
    if (clientIndex < 0 || clientIndex >= m_clients.size())
        return;

    QTcpSocket* socket = m_clients[clientIndex];
    QByteArray frame;
    QDataStream out(&frame, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << type << quint32(data.size());
    frame.append(data);

    socket->write(frame);
    socket->flush();
}

void MyTCPServer::requestModelARX(int clientIndex)
{
    sendFramed(1, QByteArray(), clientIndex);
}

void MyTCPServer::requestCalc(double value, int clientIndex)
{
    QByteArray payload;
    QDataStream out(&payload, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << value;

    sendFramed(2, payload, clientIndex);
}

void MyTCPServer::slot_new_client()
{
    QTcpSocket* client = m_server.nextPendingConnection();
    if (!client || !client->isValid())
        return;

    m_clients.append(client);
    int idx = m_clients.indexOf(client);
    QString clientMsg = "Hello client " + QString::number(idx);
    client->write(clientMsg.toUtf8());

    connect(client, &QTcpSocket::readyRead, this, &MyTCPServer::slotReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &MyTCPServer::slot_client_disconnetcted);

    emit newClientConnected(client->peerAddress().toString());
}

void MyTCPServer::slot_client_disconnetcted()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    int index = m_clients.indexOf(client);
    if (index >= 0) {
        m_clients.removeAt(index);
        emit clientDisconnetced(index);
    }
}

void MyTCPServer::slotReadyRead()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    int clientIndex = m_clients.indexOf(clientSocket);
    if (clientIndex == -1)
        return;

    static QByteArray buffer;
    buffer.append(clientSocket->readAll());

    while (true) {
        if (buffer.size() < 5)
            return;

        QDataStream in(&buffer, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_6_0);
        quint8 type;
        quint32 size;
        in >> type >> size;

        if (buffer.size() < 5 + int(size))
            return;

        QByteArray payload = buffer.mid(5, size);
        buffer.remove(0, 5 + size);

        QDataStream payloadStream(&payload, QIODevice::ReadOnly);
        payloadStream.setVersion(QDataStream::Qt_6_0);

        if (type == 101) {
            ModelARX model = Serializer::deserialize(payload);
            qDebug() << "[SERVER] Received ModelARX A:" << model.getA()
                     << "B:" << model.getB()
                     << "k:" << model.getK()
                     << "z:" << model.getZ();
        } else if (type == 102) {
            double result;
            payloadStream >> result;
            qDebug() << "[SERVER] Received dane result:" << result;
        } else {
            qDebug() << "[SERVER] Unknown message type:" << type;
        }

        emit newMsgFrom(QString(payload), clientIndex);
    }
}

void MyTCPServer::slot_newMsg()
{
    // Optional handler if needed — can be connected to emit signals for external handling
}
