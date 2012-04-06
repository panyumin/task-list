#ifndef SYNC_WIDGET_H
#define SYNC_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QNetworkAccessManager>
#include "oauth_token.h"
#include "oauth_helper.h"
#include "simpleoauth_export.h"
#include "sync_server.h"
#include "error.h"
#include "service.h"
#include "jobs.h"
#include "tasklistcollection.h"
#include "taskcollection.h"
#include "tasklist.h"
#include <QMap>




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

    OAuth::Token googleAuth;
    OAuth::Token dboxAuth;

signals:
    void getSaveFile (QString &saveFilePath);
    void killServerConnection();
    void enableServiceButton (QString &serviceType);
    void openXml(QString &filename);
public slots:
    void googleClick();
    void dboxClick();
    void syncFiles();
    void sendFiles();
    void getFiles();
    void sendFilesGTask();
    void getFilesGTask();
    void getFilesGTaskWrite();
    void getFilesListFunc();

private slots:
    void outputFileList();
    void outputResponseFile();
    void onTemporaryTokenReceived(OAuth::Token token);
    void onAccessTokenReceived(OAuth::Token token);
    void onAuthorizedRequestDone();

    void requestTokenReceived(OAuth::Token token);
    void accessTokenReceived(OAuth::Token token);

    void getDboxAccess();

    void onTasklistsReceived(GTasks::TasklistCollection tasklists, GTasks::Error error);
    void onTasklistsSent(GTasks::Tasklist tasklistMeta, GTasks::Error error);
    void onTaskSent(GTasks::Task taskMeta, GTasks::Error error);
    void onClearSent(GTasks::Error error);


    void onListReturn(GTasks::Tasklist tasklist, GTasks::Error error);
    void onTaskListingReturn(GTasks::TaskCollection collection, GTasks::Error error);


private:
    QGridLayout *syncGrid;
    QNetworkReply *m_reply;
    bool syncCall;
    void setupGTasks();
    void getAllTaskslists();

    QString apiReqUrl, apiAuthUrl, apiAccTokUrl;
    QString apiConsKey, apiConsSecretKey;
    QString apiConsKeyGoogle, apiConsSecretKeyGoogle;
    QString apiKeyGoogle;
    QString saveFilePath;

    QStringList syncFileList;
    QMap<QString, QString> gtaskListMap;

    QString oauthKey, oauthSecretKey;
    QString oauthKeyGoogle, oauthSecretKeyGoogle;

    QNetworkAccessManager* networkManager;

    void listCreate();
    void listRemake();
};

#endif // SYNC_WIDGET_H
