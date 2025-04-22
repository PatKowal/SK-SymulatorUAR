#include "mytcpserver.h"

MyTCPServer::MyTCPServer(QObject *parent)
    : QObject{parent},
      m_server(this)
{
    connect(&m_server,SIGNAL(newConnection()),this,SLOT(slot_new_client()));
}

bool MyTCPServer::startListening(int port)
{
    m_port = port;
    m_isListening = m_server.listen(QHostAddress::Any, port);
    return m_isListening;
}

void MyTCPServer::stopListening()
{
    m_server.close();
    m_isListening = false;
}

int MyTCPServer::getNumClients()
{
    return m_clients.length();
}

void MyTCPServer::sendMsg(QString msg, int numCli)
{
    if(numCli < m_clients.length())
        m_clients.at(numCli)->write(msg.toUtf8());
}

void MyTCPServer::slot_new_client()
{
    QTcpSocket *client = m_server.nextPendingConnection();
    m_clients.push_back(client);
    auto adr = client->peerAddress();
    QString msgBack = "Hello client " + QString::number( m_clients.indexOf(client));
    client->write(msgBack.toUtf8());

    connect(client,SIGNAL(disconnected()),this,SLOT(slot_client_disconnetcted()));
    connect(client,SIGNAL(readyRead()),this,SLOT(slot_newMsg()));

    emit newClientConnected(adr.toString());
}

void MyTCPServer::slot_client_disconnetcted()
{
    int idx = getClinetID();
    m_clients.removeAt(idx);
    emit clientDisconnetced(idx);
}

void MyTCPServer::slot_newMsg()
{
    int idx = getClinetID();
    QString msg = m_clients.at(idx)->readAll();
    emit newMsgFrom(msg, idx);
}

bool MyTCPServer::getClinetID()
{
    QTcpSocket *client = static_cast<QTcpSocket*> (QObject::sender());
    return m_clients.indexOf(client);
}


