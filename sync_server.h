#ifndef SYNC_SERVER_H
#define SYNC_SERVER_H

#include <QDialog>
class QTcpSocket;
class QNetworkSession;
class QTcpServer;
class QLabel;

class sync_server : public QDialog
{
    Q_OBJECT
public:
    sync_server(QWidget *parent = 0);
    ~sync_server();
    int myPort();
signals:
    
public slots:
    
private slots:
    void onConnect();
    void onReadyRead();

private:
    QLabel *statusLabel;
    QTcpServer *tcpServer;
    QNetworkSession *networkSession;
    QTcpSocket *socket;
};

#endif // SYNC_SERVER_H
