#ifndef SYNC_WIDGET_H
#define SYNC_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QNetworkAccessManager>
#include "oauth_token.h"
#include "oauth_helper.h"
#include "simpleoauth_export.h"
#include "sync_server.h"
#include "tasklistcollection.h"
#include "error.h"
#include "service.h"

class sync_widget : public QWidget
{
    Q_OBJECT
public:
    explicit sync_widget(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~sync_widget();
    void sendTasks(QString tweet);
    void sendRequest();
    void getRequest();
    void getAccess();
    void listFiles();
    void xauth();
    void getRequestFiles();

    GTasks::Service *myGtasks;
    OAuth::Helper *m_oauthHelper;
    sync_server *myServer;


signals:
    void getSaveFile (QString &saveFilePath);
    void killServerConnection();
public slots:
    void googleClick();
    void dboxClick();
    void syncFiles();
    void sendFiles();
    void getFiles();

private slots:
    //void outputResponse(QByteArray response);
    void outputFileList();
    void outputResponseFile();
    void onTemporaryTokenReceived(OAuth::Token token);
    void onAccessTokenReceived(OAuth::Token token);
    void onAuthorizedRequestDone();
    void onRequestReady(QByteArray);

    void requestTokenReceived(OAuth::Token token);
    void accessTokenReceived(OAuth::Token token);

    void onTasklistsReceived(GTasks::TasklistCollection tasklists, GTasks::Error error);
private:
    QGridLayout *syncGrid;
    QNetworkReply *m_reply;
    void setupGTasks();
    void getAllTaskslists();

    QNetworkAccessManager* networkManager;
};

#endif // SYNC_WIDGET_H
