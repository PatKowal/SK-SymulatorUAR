#include "mytcpclient.h"
#include <QDataStream>
#include <QDebug>

MyTCPClient::MyTCPClient(QObject *parent)
    : QObject{parent}
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &MyTCPClient::slot_connected);
    connect(m_socket, &QTcpSocket::disconnected, this, &MyTCPClient::disconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &MyTCPClient::slotReadyRead);

    connect(m_socket, &QTcpSocket::stateChanged, this, [](QAbstractSocket::SocketState state) {
        qDebug() << "[CLIENT] State changed:" << state;
    });

    connect(m_socket, &QTcpSocket::errorOccurred, this, [](QAbstractSocket::SocketError socketError) {
        qDebug() << "[CLIENT] Error occurred:" << socketError;
    });
}

void MyTCPClient::connectTo(QString address, int port)
{
    m_ipAddress = address;
    m_port = port;
    m_socket->connectToHost(m_ipAddress, m_port);

    if (!m_socket->waitForConnected(3000)) {
        qDebug() << "[CLIENT] Connection failed:" << m_socket->errorString();
    }
}

void MyTCPClient::disconnectFrom()
{
    m_socket->close();
}

void MyTCPClient::sendMsg(QString msg)
{
    m_socket->write(msg.toUtf8());
}

void MyTCPClient::sendFramed(quint8 type, const QByteArray& data)
{
    if (!m_socket->isValid() || m_socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "[CLIENT] Cannot send. Invalid or disconnected socket.";
        return;
    }

    QByteArray frame;
    QDataStream out(&frame, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << type << quint32(data.size());
    frame.append(data);

    m_socket->write(frame);
    m_socket->flush();
}

void MyTCPClient::slot_connected()
{
    emit connected(m_ipAddress, m_port);
}

void MyTCPClient::slotReadyRead()
{
    m_buffer.append(m_socket->readAll());

    while (true) {
        if (m_buffer.size() < 5)
            return;

        QDataStream in(&m_buffer, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_6_0);
        quint8 type;
        quint32 size;
        in >> type >> size;

        if (m_buffer.size() < 5 + int(size))
            return;

        QByteArray payload = m_buffer.mid(5, size);
        m_buffer.remove(0, 5 + size);

        QDataStream payloadStream(&payload, QIODevice::ReadOnly);
        payloadStream.setVersion(QDataStream::Qt_6_0);

        if (type == 1) {
            emit requestSerial();
        } else if (type == 2) {
            double value;
            payloadStream >> value;
            emit requestDane(value);
        } else {
            qDebug() << "[CLIENT] Unknown message type:" << type;
        }
    }
}
