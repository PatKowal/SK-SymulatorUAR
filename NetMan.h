#ifndef NETMAN_H
#define NETMAN_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

class NetMan : public QObject {
    Q_OBJECT

public:
    explicit NetMan(QObject *parent = nullptr)
        : QObject(parent), server(nullptr), socket(nullptr) {}

    ~NetMan() {
        if (server) {
            server->close();
            delete server;
        }
        if (socket) {
            socket->close();
            delete socket;
        }
    }

    // Metoda do uruchomienia połączenia
    void startConnection(const QString& ip, quint16 port, const QString& mode) {
        if (mode == "ModelARX" || mode == "Regulator") {
            // Sprawdzamy, czy serwer już działa
            if (isServerRunning(ip, port)) {
                qDebug() << "Serwer już działa na tym porcie.";
            } else {
                // Uruchamiamy serwer
                if (mode == "ModelARX") {
                    startServer(port);  // ModelARX działa jako serwer
                } else if (mode == "Regulator") {
                    startServer(port);  // Regulator działa jako serwer
                }
            }

            // Jeśli to nie jest serwer, uruchamiamy klienta
            if (mode == "ModelARX") {
                startClient(ip, port);  // Regulator działa jako klient
            } else if (mode == "Regulator") {
                startClient(ip, port);  // ModelARX działa jako klient
            }
        } else {
            qDebug() << "Nieznany tryb: " << mode;
        }
    }

    void startServer(quint16 port) {
        server = new QTcpServer(this);
        connect(server, &QTcpServer::newConnection, this, &NetMan::onNewConnection);

        if (server->listen(QHostAddress::Any, port)) {
            qDebug() << "Serwer nasłuchuje na porcie: " << port;
        } else {
            qDebug() << "Błąd uruchamiania serwera!";
        }
    }

    void startClient(const QString& ip, quint16 port) {
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::connected, this, &NetMan::onConnected);
        connect(socket, &QTcpSocket::readyRead, this, &NetMan::onReadyRead);

        socket->connectToHost(ip, port);
        qDebug() << "Łączenie z serwerem na IP: " << ip << " i porcie: " << port;
    }

    // Sprawdzanie, czy serwer działa
    bool isServerRunning(const QString& ip, quint16 port) {
        QTcpSocket testSocket;
        testSocket.connectToHost(ip, port);
        bool isRunning = testSocket.waitForConnected(100);  // Czekamy max 100 ms
        testSocket.close();
        return isRunning;
    }

    // Pobranie socketu
    QTcpSocket* getSocket() const {
        return socket;
    }

private slots:
    // Obsługuje połączenie przychodzące od klienta
    void onNewConnection() {
        socket = server->nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, &NetMan::onReadyRead);
        qDebug() << "Połączono z klientem!";
    }

    // Połączenie z serwerem jako klient
    void onConnected() {
        qDebug() << "Połączono z serwerem!";
    }

    // Odbiór danych
    void onReadyRead() {
        QByteArray data = socket->readAll();
        qDebug() << "Odebrano dane: " << data;
    }

private:
    QTcpServer* server;
    QTcpSocket* socket;
};

#endif // NETMAN_H
