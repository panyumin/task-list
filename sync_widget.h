#ifndef SYNC_WIDGET_H
#define SYNC_WIDGET_H

#include <QWidget>
#include <QGridLayout>

class KQOAuthManager;
class KQOAuthRequest;
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

signals:
    void getSaveFile (QString &saveFilePath);
public slots:
    void googleClick();
    void dboxClick();
    void syncFiles();
    void sendFiles();
    void getFiles();

private slots:
    void outputResponse(QByteArray response);
    void outputFileList(QByteArray response);
    void outputResponseFile(QByteArray response);
    void onTemporaryTokenReceived(QString temporaryToken, QString temporaryTokenSecret);
    void onAuthorizationReceived(QString token, QString verifier);
    void onAccessTokenReceived(QString token, QString tokenSecret);
    void onAuthorizedRequestDone();
    void onRequestReady(QByteArray);
    void myAuthorizationReceived();

private:
    KQOAuthManager *oauthManager;
    KQOAuthRequest *oauthRequest;
    QGridLayout *syncGrid;
};

#endif // SYNC_WIDGET_H
