#include "sync_widget.h"
#include <QtKOAuth>
#include <QString>
#include <QtGui>
#include <QStringList>

QString apiReqUrl, apiAuthUrl, apiAccTokUrl;
QString apiConsKey, apiConsSecretKey;
QString saveFilePath;

QStringList syncFileList;

QString oauthKey, oauthSecretKey;
bool waitAccess;

void sync_widget::googleClick(){
    this->close();
}

void sync_widget::dboxClick(){
    apiReqUrl = "https://api.dropbox.com/1/oauth/request_token";
    apiAuthUrl = "https://www.dropbox.com/1/oauth/authorize?";
    apiAccTokUrl = "https://api.dropbox.com/1/oauth/access_token";

    apiConsKey = "jq85j637n7ph3eu";
    apiConsSecretKey = "j4sitl56kgcl7ov";
    oauthManager->serviceType = "dbox";

    waitAccess = false;
    this->getAccess();


    this->close();

//    QMessageBox *myMsgBox = new QMessageBox(this);

//    myMsgBox->exec();
//    QVBoxLayout *myMsgWin = new QVBoxLayout();
//    QPushButton *myMsgBut = new QPushButton("Authorized");
//    myMsgWin->addWidget(myMsgBut);
//    myMsgWin->activate();
//    QMainWindow *myMsgWindow = new QMainWindow(this, 0);
//    QWidget *tempWidget = new QWidget();
//    tempWidget->setLayout(myMsgWin);
//    myMsgWindow->setCentralWidget(tempWidget);
//    myMsgWindow->show();

    //connect(myMsgBut, SIGNAL(clicked()), this, SLOT(myAuthorizationReceived()));
}

void sync_widget::syncFiles() {
    this->sendRequest();
}

void sync_widget::sendFiles() {
    this->sendRequest();
}

void sync_widget::getFiles() {
    this->getRequest();
}

void sync_widget::getAccess()  {
    connect(oauthManager, SIGNAL(temporaryTokenReceived(QString,QString)),
            this, SLOT(onTemporaryTokenReceived(QString, QString)));

    connect(oauthManager, SIGNAL(authorizationReceived(QString,QString)),
            this, SLOT( onAuthorizationReceived(QString, QString)));

    connect(oauthManager, SIGNAL(accessTokenReceived(QString,QString)),
            this, SLOT(onAccessTokenReceived(QString,QString)));

    connect(oauthManager, SIGNAL(requestReady(QByteArray)),
            this, SLOT(onRequestReady(QByteArray)));

    oauthRequest->initRequest(KQOAuthRequest::TemporaryCredentials, QUrl(apiReqUrl));
    oauthRequest->setSignatureMethod(KQOAuthRequest::PLAINTEXT);
    oauthRequest->setConsumerKey(apiConsKey);
    oauthRequest->setConsumerSecretKey(apiConsSecretKey);

    oauthManager->setHandleUserAuthorization(true);

    oauthManager->executeRequest(oauthRequest);

}

void sync_widget::onTemporaryTokenReceived(QString token, QString tokenSecret)
{
    qDebug() << "Temporary token received: " << token << tokenSecret;

    apiAuthUrl += "&oauth_callback=" + oauthManager->serverCallbackUrl;
    //apiAuthUrl.append(QString::number(oauthManager->callbackServer->serverPort()));
    QUrl userAuthURL(apiAuthUrl);

    if( oauthManager->lastError() == KQOAuthManager::NoError) {
        qDebug() << "Asking for user's permission to access protected resources. Opening URL: " << userAuthURL;
        oauthManager->getUserAuthorization(userAuthURL);
    }

}

void sync_widget::onAuthorizationReceived(QString token, QString verifier) {
    qDebug() << "User authorization received: " << token << verifier;

    oauthManager->getUserAccessTokens(QUrl(apiAccTokUrl));
    if( oauthManager->lastError() != KQOAuthManager::NoError) {
        qDebug() << "Error in user authorization token retrieval";
    }
}

void sync_widget::myAuthorizationReceived() {
    qDebug() << "User authorization received";

    oauthManager->getUserAccessTokens(QUrl(apiAccTokUrl));
    if( oauthManager->lastError() != KQOAuthManager::NoError) {
        qDebug() << "Error in user authorization token retrieval";
    }
}

void sync_widget::onAccessTokenReceived(QString token, QString tokenSecret) {
    qDebug() << "Access token received: " << token << tokenSecret;

    oauthKey = token;
    oauthSecretKey = tokenSecret;

    qDebug() << "Access tokens now stored. You are ready to send files from user's account!";
    qDebug() << "oauth token: " << oauthKey;
    qDebug() << "oauth secret token: " << oauthSecretKey;

}

void sync_widget::onAuthorizedRequestDone() {
    qDebug() << "Request sent to Dropbox!";
}

void sync_widget::onRequestReady(QByteArray response) {
    qDebug() << "Response from the service: " << response;
}

//void sync_widget::xauth() {
//    connect(oauthManager, SIGNAL(accessTokenReceived(QString,QString)),
//            this, SLOT(onAccessTokenReceived(QString,QString)));

//    KQOAuthRequest_XAuth *oauthRequest = new KQOAuthRequest_XAuth(this);
//    oauthRequest->initRequest(KQOAuthRequest::AccessToken, QUrl(apiAccTokUrl));
//    oauthRequest->setConsumerKey(apiConsKey);
//    oauthRequest->setConsumerSecretKey(apiConsSecretKey);
//    oauthRequest->setXAuthLogin(/* Your username*/ /*,*/ /* Your password */);
//    oauthManager->executeRequest(oauthRequest);
//}

void sync_widget::outputResponse(QByteArray response){
    QFile file;
    file.open(stdout, QIODevice::WriteOnly);
    file.write(response);
    file.close();
    disconnect(oauthManager, SIGNAL(requestReady(QByteArray)),
            this, SLOT(outputResponse(QByteArray)));
}
void sync_widget::outputResponseFile(QByteArray response){
    QFile file(syncFileList.at(0).mid(1, syncFileList.at(0).count()));
    syncFileList.erase(syncFileList.begin());
    if(!file.open(QIODevice::WriteOnly))
        return;
    file.write(response);
    qDebug() << file.fileName();
    file.close();

    if(syncFileList.count() == 0){
        disconnect(oauthManager, SIGNAL(requestReady(QByteArray)),
                this, SLOT(outputResponseFile(QByteArray)));
    }
}

void sync_widget::outputFileList(QByteArray response){
    QString fileList(response);
    QStringList temp = fileList.split(",");
    for(int i = 0; i < temp.length(); i++){
        if(temp.at(i).contains("\"path\":") &&
                !temp.at(i).contains("\"path\": \"/\"")){
            qDebug() << temp.at(i);
            syncFileList << temp.at(i).mid(10,temp.at(i).length()-11);
            qDebug() << syncFileList.at(syncFileList.size()-1);
        }
    }
    disconnect(oauthManager, SIGNAL(requestReady(QByteArray)),
            this, SLOT(outputFileList(QByteArray)));
    getRequestFiles();
}

void sync_widget::sendRequest() {
    emit getSaveFile(saveFilePath);

    QFile listFile(saveFilePath);
    QFileInfo listInfo(listFile);
    //QString params = "file=" + listFile.fileName();
    QString crlf("\r\n");
    qsrand(QDateTime::currentDateTime().toTime_t());
    QString b=QVariant(qrand()).toString() + QVariant(qrand()).toString() + QVariant(qrand()).toString();
    QString boundaryStr = "---------------------------"+b;
    QString boundary = "--" + boundaryStr + crlf;

    if( oauthKey.isEmpty() ||
        oauthSecretKey.isEmpty()) {
        qDebug() << "No access tokens. Aborting.";
        return;
    }
    oauthRequest->setSignatureMethod(KQOAuthRequest::PLAINTEXT);
    oauthRequest->initRequest(KQOAuthRequest::AuthorizedRequest, QUrl("https://api-content.dropbox.com/1/files/sandbox/"));
    oauthRequest->setHttpMethod(KQOAuthRequest::POST);
    oauthRequest->setContentType("multipart/form-data; boundary=" + boundaryStr);
    oauthRequest->setConsumerKey(apiConsKey);
    oauthRequest->setConsumerSecretKey(apiConsSecretKey);
    oauthRequest->setToken(oauthKey);
    oauthRequest->setTokenSecret(oauthSecretKey);


    KQOAuthParameters params;

    listFile.open(QIODevice::ReadOnly);
    QByteArray fileData;

    listFile.objectName();
    fileData.append(boundary.toAscii());
    fileData.append(QString("Content-Disposition: form-data; name=\"file\"; filename=\"" + listInfo.fileName() +"\"" + crlf).toAscii());
    fileData.append(QString("Content-Type: text/plain" + crlf + crlf).toAscii());
    fileData.append(listFile.readAll());
    listFile.close();
    fileData.append(QString(crlf + "--" + boundaryStr + "--" + crlf).toAscii());

    params.insert("filename", listInfo.fileName());
    params.insert("file", listInfo.fileName());
    oauthRequest->setAdditionalParameters(params);
    outputResponse(oauthRequest->rawData());
    oauthRequest->setRawData(fileData);
    oauthManager->executeRequest(oauthRequest);

    connect(oauthManager, SIGNAL(requestReady(QByteArray)),
            this, SLOT(outputResponse(QByteArray)));
    connect(oauthManager, SIGNAL(authorizedRequestDone()),
            this, SLOT(onAuthorizedRequestDone()));
}

void sync_widget::getRequestFiles(){
    if( oauthKey.isEmpty() ||
        oauthSecretKey.isEmpty()) {
        qDebug() << "No access tokens. Aborting.";
        return;
    }
    connect(oauthManager, SIGNAL(requestReady(QByteArray)),
            this, SLOT(outputResponseFile(QByteArray)));

    int count = syncFileList.count();
    for(int i = 0; i < count; i++){
        QString fileUrl = "https://api-content.dropbox.com/1/files/sandbox/" + syncFileList.at(i);
        oauthRequest->initRequest(KQOAuthRequest::AuthorizedRequest, QUrl(fileUrl));
        oauthRequest->setHttpMethod(KQOAuthRequest::GET);
        oauthRequest->setConsumerKey(apiConsKey);
        oauthRequest->setConsumerSecretKey(apiConsSecretKey);
        oauthRequest->setToken(oauthKey);
        oauthRequest->setTokenSecret(oauthSecretKey);

        oauthManager->executeRequest(oauthRequest);
    }


}

void sync_widget::getRequest() {
    this->listFiles();
}

void sync_widget::listFiles() {
    if( oauthKey.isEmpty() ||
        oauthSecretKey.isEmpty()) {
        qDebug() << "No access tokens. Aborting.";
        return;
    }

    oauthRequest->initRequest(KQOAuthRequest::AuthorizedRequest, QUrl("https://api.dropbox.com/1/metadata/sandbox/"));
    oauthRequest->setConsumerKey(apiConsKey);
    oauthRequest->setConsumerSecretKey(apiConsSecretKey);
    oauthRequest->setToken(oauthKey);
    oauthRequest->setTokenSecret(oauthSecretKey);

    oauthManager->executeRequest(oauthRequest);

    connect(oauthManager, SIGNAL(requestReady(QByteArray)),
            this, SLOT(outputFileList(QByteArray)));
}

//int main(int argc, char *argv[])
//{
//    QCoreApplication app(argc, argv);
//    QCoreApplication::setOrganizationName("kQOAuth");
//    QCoreApplication::setApplicationName("TwitterCLI");

//    QStringList args = QCoreApplication::arguments();

//    TwitterCLI tAuth;
//    if(args.contains("-t")) {
//        if(args.last() != "-t") {
//            tAuth.sendTweet(args.last());
//        }
//     } else if( args.contains("-a")){
//        tAuth.getAccess();
//    } else if (args.contains("-x")) {
//        tAuth.xauth();
//    } else {
//        tAuth.showHelp();
//        return 0;
//    }

//    return app.exec();

//}


sync_widget::sync_widget(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent)
{
    syncGrid = new QGridLayout();
    QPushButton *syncOneBut = new QPushButton("Google Task");
    QPushButton *syncTwoBut = new QPushButton("DropBox");

    syncGrid->addWidget(syncOneBut, 0, 0);
    syncGrid->addWidget(syncTwoBut, 0, 1);

    this->setLayout(syncGrid);
    this->setWindowTitle("Select Service");
    this->setWindowFlags(f);

    oauthRequest = new KQOAuthRequest;
    oauthManager = new KQOAuthManager(this);
    oauthRequest->setEnableDebugOutput(true);

    connect(syncOneBut, SIGNAL(clicked()), this, SLOT(googleClick()));
    connect(syncTwoBut, SIGNAL(clicked()), this, SLOT(dboxClick()));
}

sync_widget::~sync_widget(){
    delete oauthRequest;
    delete oauthManager;
}
