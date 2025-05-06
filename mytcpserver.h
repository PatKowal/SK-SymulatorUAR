#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MyTCPServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTCPServer(QObject *parent = nullptr);
    bool startListening(int port);
    bool isListening() {return m_isListening;}
    void stopListening();
    int getNumClients();
    void sendMsg(QString msg, int numCli);
    void sendFramed(quint8 type, const QByteArray& data, int clientIndex);
    void requestModelARX(int clientIndex);
    void requestCalc(double value, int clientIndex);

signals:
    void newClientConnected(QString adr);
    void clientDisconnetced(int num);
    void newMsgFrom(QString, int num);

private slots:
    void slot_new_client();
    void slot_client_disconnetcted();
    void slot_newMsg();
    void slotReadyRead();

private:
    bool getClinetID();

    bool m_isListening = false;
    int m_port = 12345;
    QTcpServer m_server;
    QVector<QTcpSocket*> m_clients;
};

#endif // MYTCPSERVER_H
