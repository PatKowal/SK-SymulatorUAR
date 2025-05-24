#include "mytcpserver.h"
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

void MyTCPServer::sendFramedToClients(quint8 type, const QByteArray& data)
{
    if (m_clients.isEmpty()) {
        qDebug() << "[SERVER] Brak połączonych klientów!";
        return;
    }

    QByteArray frame;
    QDataStream out(&frame, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << type << quint32(data.size());
    frame.append(data);

    for(QTcpSocket * client : m_clients){
        if(client->state() == QAbstractSocket::ConnectedState){
            client->write(frame);
            client->flush();
        }
    }
}

void MyTCPServer::requestModelARX()
{
    sendFramedToClients(1, QByteArray());
}

void MyTCPServer::requestCalc(double value)
{
    QByteArray payload;
    QDataStream out(&payload, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << value;

    sendFramedToClients(2, payload);
}

void MyTCPServer::slot_new_client()
{
    QTcpSocket* client = m_server.nextPendingConnection();
    if (!client || !client->isValid())
        return;

    m_clients.append(client);
    int idx = m_clients.indexOf(client);
    Q_UNUSED(idx);

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
    if (clientIndex == -1){
        return;
    }

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
        {
            return;
        }

        QByteArray data = buffer.mid(5, size);
        buffer.remove(0, 5 + size);

        QDataStream dataStream(&data, QIODevice::ReadOnly);
        dataStream.setVersion(QDataStream::Qt_6_0);
        if (type == 102) {
            double result;
            qint64 timeonsend;
            dataStream >> result >> timeonsend;
            emit resultReceived(result, timeonsend);
        }
    }
}

void MyTCPServer::slot_newMsg()
{
    // Optional handler if needed — can be connected to emit signals for external handling
}
