#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkSession>
#include <QMessageBox>
#include "sync_server.h"

sync_server::sync_server(QWidget *parent) :
    QDialog(parent)
{
    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen()){
        QMessageBox::critical(this, tr("Local Server"),
                                      tr("Unable to start the server: %1.")
                                      .arg(tcpServer->errorString()));
                close();
                return;
    }
    QString ipAddress;
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    qDebug() << tcpServer->serverAddress() << tcpServer->serverPort();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(onConnect()));

}

void sync_server::onConnect(){
    socket = tcpServer->nextPendingConnection();
    //connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::UniqueConnection);
}

void sync_server::onReadyRead(){
    QByteArray data = socket->readAll();
    qDebug() << "data in" << data;

    QByteArray content;
    QByteArray replyOut;
    QByteArray replySock;
    content.append("<HTML></HTML>");

    replyOut.append("HTTP/1.0 200 OK \r\n");
    replyOut.append("Content-Type: text/html; charset=\"utf-8\"\r\n");
    replyOut.append(QString("Content-Length: %1\r\n").arg(content.size()));
    replyOut.append("\r\n");
    replyOut.append(content);

    QDataStream out(&replySock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    out << replyOut;
    out.device()->seek(0);
    out << (quint16)(replySock.size() - sizeof(quint16));


    qDebug() << "replyOut " << replyOut;
    socket->write(replySock);
    socket->disconnectFromHost();
    //tcpServer->
}

int sync_server::myPort(){
    qDebug() << QString(tcpServer->serverPort());
    return tcpServer->serverPort();
}

sync_server::~sync_server(){
    tcpServer->close();
    socket->close();
}
