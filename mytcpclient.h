#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class MyTCPClient : public QObject
{
    Q_OBJECT
public:
    explicit MyTCPClient(QObject *parent = nullptr);
    void connectTo(QString address, int port);
    void disconnectFrom();
    bool isConnected() {return m_socket->isOpen();}
    // void sendMsg(QString msg);

    void sendFramed(quint8 type, const QByteArray& data);
    QTcpSocket* getSocket() { return m_socket; } // isValid()?
signals:
    void connected(QString adr, int port);
    void disconnected();
    void ModelARXRequest();
    void StartSimOnClient();
    void IntervalOnServerChanged(int value);
    void SymulujRequest(double value, qint64 timeonsend);

private slots:
    void slot_connected();
    void slotReadyRead();

private:
    QTcpSocket* m_socket;
    QString m_ipAddress = "127.0.0.1";
    int m_port = 12345;
    QByteArray m_buffer;
};

#endif // MYTCPCLIENT_H
