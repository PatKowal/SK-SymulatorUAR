#include "mytcpclient.h"

MyTCPClient::MyTCPClient(QObject *parent)
    : QObject{parent}, m_socket(this)
{
    connect(&m_socket,SIGNAL(connected()),this,SLOT(slot_connected()));
    connect(&m_socket,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(slot_readyRead()));
}

void MyTCPClient::connectTo(QString address, int port)
{
    m_ipAddress = address;
    m_port = port;
    m_socket.connectToHost(m_ipAddress,port);
}

void MyTCPClient::disconnectFrom()
{
    m_socket.close();
}

void MyTCPClient::sendMsg(QString msg)
{
    QByteArray data = msg.toUtf8();
    m_socket.write(data);
}

void MyTCPClient::slot_connected()
{
    emit connected(m_ipAddress,m_port);
}

void MyTCPClient::slot_readyRead()
{
    auto message = m_socket.readAll();
    emit messageRecived(message);
}
