#include "sync_widget.h"
#include <QString>
#include <QtGui>
#include <QStringList>
#include <Parser>
#include <QUdpSocket>
#include "oauth_helper.h"
#include "simpleoauth_export.h"
#include "service.h"
#include "tasklist.h"
#include "tasklistcollection.h"
#include <QMutexLocker>
#include <QMutex>
#include <QWaitCondition>

QMutex mutex;
QWaitCondition fileReqOut;

QString apiReqUrl, apiAuthUrl, apiAccTokUrl;
QString apiConsKey, apiConsSecretKey;
QString apiConsKeyGoogle, apiConsSecretKeyGoogle;
QString apiKeyGoogle;
QString saveFilePath;

QStringList syncFileList;

QString oauthKey, oauthSecretKey;
QString oauthKeyGoogle, oauthSecretKeyGoogle;

OAuth::Token googleAuth;
OAuth::Token dboxAuth;
bool waitAccess;

void sync_widget::googleClick(){

    apiReqUrl = "https://www.google.com/accounts/OAuthGetRequestToken";
    apiAuthUrl = "https://www.google.com/accounts/OAuthAuthorizeToken";
    apiAccTokUrl = "https://www.google.com/accounts/OAuthGetAccessToken";

    apiConsKeyGoogle = "545441427334.apps.googleusercontent.com";
    apiConsSecretKeyGoogle = "hLrL0maDoBGkvIsnSYQLx2Hm";
    apiKeyGoogle = "AIzaSyCyETZ3-DhV09n7X10CoKiDkXzlMukADA8";


    connect(m_oauthHelper, SIGNAL(requestTokenReceived(OAuth::Token)), this, SLOT(requestTokenReceived(OAuth::Token)));
    connect(m_oauthHelper, SIGNAL(accessTokenReceived(OAuth::Token)), this, SLOT(accessTokenReceived(OAuth::Token)));

    OAuth::Token tempToken;
    tempToken.setConsumerKey("anonymous");
    tempToken.setConsumerSecret("anonymous");
    tempToken.setCallbackUrl(QUrl("oob"));
    tempToken.setService("google");

    m_oauthHelper->getRequestToken(tempToken, QUrl(apiReqUrl + "?scope=https://www.googleapis.com/auth/tasks"));

    this->close();
}

void sync_widget::requestTokenReceived(OAuth::Token token){
    if (token.type() == OAuth::Token::RequestToken){
        qDebug() << "Retrieved request token";
        m_oauthHelper->getUserAuthorization(token, QUrl(apiAuthUrl));

        bool ok;
        QString text = QInputDialog::getText(this, tr("Enter Verifier Code"), tr("Verifier:"), QLineEdit::Normal, "", &ok);
        if(ok){
            token.setVerifier(text);
            m_oauthHelper->getAccessToken(token, QUrl(apiAccTokUrl));
        }
    }
    else
        qDebug() << "Error in retrieving the request token";
}

void sync_widget::accessTokenReceived(OAuth::Token token){
    if (token.type() == OAuth::Token::AccessToken){
        qDebug() << "access success";
        oauthKeyGoogle = token.tokenString();
        oauthSecretKeyGoogle = token.tokenSecret();

        qDebug() << "Access tokens now stored. You are ready to send task requests from user's account!";
        qDebug() << "oauth token: " << oauthKeyGoogle;
        qDebug() << "oauth secret token: " << oauthSecretKeyGoogle;


        setupGTasks();
        getAllTaskslists();
    }
    else
        qDebug() << "access failure";
    disconnect(m_oauthHelper, SIGNAL(requestTokenReceived(OAuth::Token)), this, SLOT(requestTokenReceived(OAuth::Token)));
    disconnect(m_oauthHelper, SIGNAL(accessTokenReceived(OAuth::Token)), this, SLOT(accessTokenReceived(OAuth::Token)));
}

void sync_widget::dboxClick(){
    apiReqUrl = "https://api.dropbox.com/1/oauth/request_token";
    apiAuthUrl = "https://www.dropbox.com/1/oauth/authorize?";
    apiAccTokUrl = "https://api.dropbox.com/1/oauth/access_token";

    apiConsKey = "jq85j637n7ph3eu";
    apiConsSecretKey = "j4sitl56kgcl7ov";

    waitAccess = false;
    this->getAccess();
    this->close();
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
    connect(m_oauthHelper, SIGNAL(requestTokenReceived(OAuth::Token)), this, SLOT(onTemporaryTokenReceived(OAuth::Token)));
    connect(m_oauthHelper, SIGNAL(accessTokenReceived(OAuth::Token)), this, SLOT(onAccessTokenReceived(OAuth::Token)));

    OAuth::Token tempToken;

    tempToken.setConsumerKey(apiConsKey);
    tempToken.setConsumerSecret(apiConsSecretKey);
    tempToken.setService("dbox");

    m_oauthHelper->getRequestToken(tempToken, QUrl(apiReqUrl));
}

void sync_widget::onTemporaryTokenReceived(OAuth::Token token)
{
    qDebug() << "Temporary token received: ";
    if (token.type() == OAuth::Token::RequestToken){
        qDebug() << "Retrieved request token" << token.consSecret() << "& " << token.tokenSecret();
        m_oauthHelper->getUserAuthorization(token, QUrl(apiAuthUrl));

        bool ok;
        QString text = QInputDialog::getText(this, tr("Enter when verified"), tr("Answer:"), QLineEdit::Normal, "", &ok);
        if(ok){
            //token.setVerifier(text);
            m_oauthHelper->getAccessToken(token, QUrl(apiAccTokUrl));
        }

    }
    else
        qDebug() << "Error in retrieving the request token";
}

void sync_widget::onAccessTokenReceived(OAuth::Token token) {
    qDebug() << "Access token received: ";

    oauthKey = token.tokenString();
    oauthSecretKey = token.tokenSecret();

    qDebug() << "Access tokens now stored. You are ready to send files from user's account!";
    qDebug() << "oauth token: " << oauthKey;
    qDebug() << "oauth secret token: " << oauthSecretKey;

    dboxAuth.setType(OAuth::Token::AccessToken);
    dboxAuth.setConsumerKey(apiConsKey);
    dboxAuth.setConsumerSecret(apiConsSecretKey);
    dboxAuth.setTokenString(oauthKey);
    dboxAuth.setTokenSecret(oauthSecretKey);
    dboxAuth.setService("dbox");

    disconnect(m_oauthHelper, SIGNAL(requestTokenReceived(OAuth::Token)), this, SLOT(onTemporaryTokenReceived(OAuth::Token)));
    disconnect(m_oauthHelper, SIGNAL(accessTokenReceived(OAuth::Token)), this, SLOT(onAccessTokenReceived(OAuth::Token)));
}

void sync_widget::onAuthorizedRequestDone() {
    qDebug() << "Request sent to Dropbox!";
}

void sync_widget::onRequestReady(QByteArray response) {
    qDebug() << "Response from the service: " << response;
}

//void sync_widget::outputResponse(QByteArray response){
//    QFile file;
//    file.open(stdout, QIODevice::WriteOnly);
//    file.write(response);
//    file.close();
//    disconnect(oauthManager, SIGNAL(requestReady(QByteArray)),
//            this, SLOT(outputResponse(QByteArray)));
//}


void sync_widget::sendRequest() {
    emit getSaveFile(saveFilePath);

    QFile listFile(saveFilePath);
    QFileInfo listInfo(listFile);

    QUrl m_url("https://api-content.dropbox.com/1/files_put/sandbox/"+listInfo.fileName()+"?param=val");
    OAuth::Token::HttpMethod method = OAuth::Token::HttpPut;
    QByteArray authHeader = dboxAuth.signRequest(m_url, OAuth::Token::HttpHeader, method);

    QNetworkRequest request;
    request.setUrl(m_url);
    request.setRawHeader("Authorization", authHeader);
    listFile.open(QIODevice::ReadOnly);
    QByteArray fileData;
    fileData.append(listFile.readAll());

    m_reply = networkManager->put(request, fileData);

    connect(m_reply, SIGNAL(finished()), this, SLOT(onAuthorizedRequestDone()));
    m_reply->ignoreSslErrors();

    qDebug() << "Requesting... " << request.url() << "with data " << fileData << "auth:" << request.rawHeader("Authorization");
}

void sync_widget::outputResponseFile(){
    QByteArray response = m_reply->readAll();
    qDebug() << "outputResponseFile - got response" << response;

    QFile file(syncFileList.at(0));
    syncFileList.erase(syncFileList.begin());
    if(!file.open(QIODevice::WriteOnly))
        return;
    file.write(response);
    qDebug() << file.fileName();
    file.close();

    m_reply->deleteLater();
    qDebug() << "getting another file";
    getRequestFiles();
}

void sync_widget::getRequestFiles(){
    if(syncFileList.size() != 0){
        QUrl m_url("https://api-content.dropbox.com/1/files/sandbox/" + syncFileList.at(0));
        OAuth::Token::HttpMethod method = OAuth::Token::HttpGet;
        QByteArray authHeader = dboxAuth.signRequest(m_url, OAuth::Token::HttpHeader, method);

        QNetworkRequest request;
        request.setUrl(m_url);
        request.setRawHeader("Authorization", authHeader);

        m_reply = networkManager->get(request);
        m_reply->ignoreSslErrors();

        qDebug() << "Requesting... " << request.url() << "with no data auth:" << request.rawHeader("Authorization");

        connect(m_reply, SIGNAL(finished()), this, SLOT(outputResponseFile()));
    }
    else{
        disconnect(m_reply, SIGNAL(finished()), this, SLOT(outputResponseFile()));
        qDebug() << "no more files";
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
    QUrl m_url("https://api.dropbox.com/1/metadata/sandbox/");
    OAuth::Token::HttpMethod method = OAuth::Token::HttpGet;
    QByteArray authHeader = dboxAuth.signRequest(m_url, OAuth::Token::HttpHeader, method);

    QNetworkRequest request;
    request.setUrl(m_url);
    request.setRawHeader("Authorization", authHeader);

    m_reply = networkManager->get(request);

    connect(m_reply, SIGNAL(finished()), this, SLOT(outputFileList()));
    m_reply->ignoreSslErrors();

    qDebug() << "Requesting... " << request.url() << "with no data auth:" << request.rawHeader("Authorization");
}

void sync_widget::outputFileList(){
    QByteArray response = m_reply->readAll();
    qDebug() << "outputFileList - got response" << response;
    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse(response, &ok).toMap();
    if(!ok){
        qDebug() << "error in parsing json for listing";
    }

    foreach (QVariant file, result["contents"].toList()){
        QVariantMap fileMeta = file.toMap();
        QString temp = fileMeta["path"].toString();
        qDebug() << "file_path" << temp;
        temp = temp.mid(1, temp.length());
        syncFileList << temp;
        qDebug() << syncFileList.at(syncFileList.size()-1);
    }
    disconnect(m_reply, SIGNAL(finished()), this, SLOT(outputFileList()));

    m_reply->deleteLater();
    getRequestFiles();
}


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

    myServer = new sync_server(this);
    networkManager = new QNetworkAccessManager(this);
    connect(syncOneBut, SIGNAL(clicked()), this, SLOT(googleClick()));
    connect(syncTwoBut, SIGNAL(clicked()), this, SLOT(dboxClick()));
}

sync_widget::~sync_widget(){
}

void sync_widget::setupGTasks(){
    myGtasks = new GTasks::Service(new QNetworkAccessManager(this), this);
    myGtasks->setApiKey(apiKeyGoogle);

    googleAuth.setType(OAuth::Token::AccessToken);
    googleAuth.setConsumerKey("anonymous");
    googleAuth.setConsumerSecret("anonymous");
    googleAuth.setTokenString(oauthKeyGoogle);
    googleAuth.setTokenSecret(oauthSecretKeyGoogle);
    googleAuth.setService("google");
    myGtasks->setToken(googleAuth);
}

void sync_widget::getAllTaskslists(){
    myGtasks->listTasklists().startAndCallback(this, SLOT(onTasklistsReceived(GTasks::TasklistCollection,GTasks::Error)));
}

void sync_widget::onTasklistsReceived(GTasks::TasklistCollection tasklists, GTasks::Error error){
    if (error.code() != QNetworkReply::NoError) {
        // Do some error handling here
        qDebug() << "Error:" << error.code() << error.message();
        return;
    }

    // Display the title and id of each tasklist
    foreach (const GTasks::Tasklist& tasklist, tasklists.items()) {
        qDebug() << "Tasklist:" << tasklist.title() << "id:" << tasklist.id();
    }
}


