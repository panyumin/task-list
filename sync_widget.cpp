#include "sync_widget.h"
#include <QString>
#include <QtGui>
#include <QStringList>
#include <Parser>
#include <QUdpSocket>
#include <QtXml>
#include "oauth_helper.h"
#include "simpleoauth_export.h"
#include "service.h"
#include "jobs.h"
#include "tasklist.h"
#include "tasklistcollection.h"
#include "taskcollection.h"


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
    else{
        qDebug() << "Error in retrieving the request token";
        emit googleAuthResult(false);
    }
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
        emit googleAuthResult(true);

        getAllTaskslists();
    }
    else{
        qDebug() << "access failure";
        emit googleAuthResult(false);
    }
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
    syncCall = true;
    if(dboxAuth.tokenSecret() != ""){
        this->sendRequest();
        //this->getRequest();
    }

    if(googleAuth.tokenSecret() != ""){
        this->sendFilesGTask();
        //this->getFilesGTask();
    }
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

OAuth::Token dboxTempToken;
void sync_widget::onTemporaryTokenReceived(OAuth::Token token)
{
    qDebug() << "Temporary token received: ";
    if (token.type() == OAuth::Token::RequestToken){
        qDebug() << "Retrieved request token" << token.consSecret() << "& " << token.tokenSecret();
        m_oauthHelper->getUserAuthorization(token, QUrl(apiAuthUrl));

        dboxTempToken = token;

        QMessageBox *authorized = new QMessageBox(this);
        authorized->setWindowTitle("Authorization");
        authorized->setText("User authorized?");
        authorized->setButtonText(0, "Complete");
        authorized->show();
        connect(authorized, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(getDboxAccess()));
    }
    else{
        qDebug() << "Error in retrieving the request token";
        emit dboxAuthResult(false);
    }
}

void sync_widget::getDboxAccess(){
    m_oauthHelper->getAccessToken(dboxTempToken, QUrl(apiAccTokUrl));
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

    QString service = "dbox";
    emit enableServiceButton(service);
    emit dboxAuthResult(true);

    disconnect(m_oauthHelper, SIGNAL(requestTokenReceived(OAuth::Token)), this, SLOT(onTemporaryTokenReceived(OAuth::Token)));
    disconnect(m_oauthHelper, SIGNAL(accessTokenReceived(OAuth::Token)), this, SLOT(onAccessTokenReceived(OAuth::Token)));
}

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

void sync_widget::onAuthorizedRequestDone() {
    qDebug() << "File sent to drop box";
    emit dboxSendResult(true);
    if(syncCall)
        this->getRequest();
}

void sync_widget::getRequest() {
    this->listFiles();
}

void sync_widget::listFiles() {
    if( oauthKey.isEmpty() ||
        oauthSecretKey.isEmpty()) {
        qDebug() << "No access tokens. Aborting.";
        emit dboxRecvResult(false);
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
        emit dboxRecvResult(false);
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
        qDebug() << "no more files";
        emit dboxRecvResult(true);
        if(googleAuth.tokenSecret() == "")
            syncCall = false;
    }
}

void sync_widget::outputResponseFile(){
    QByteArray response = m_reply->readAll();
    qDebug() << "outputResponseFile - got response" << response;

    QFile file(syncFileList.at(0));
    syncFileList.erase(syncFileList.begin());
    if(!file.open(QIODevice::WriteOnly)){
        emit dboxRecvResult(false);
        disconnect(m_reply, SIGNAL(finished()), this, SLOT(outputResponseFile()));
        return;
    }
    file.write(response);
    qDebug() << file.fileName();
    file.close();

    disconnect(m_reply, SIGNAL(finished()), this, SLOT(outputResponseFile()));
    m_reply->deleteLater();
    qDebug() << "getting another file";
    getRequestFiles();
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

    QString service = "google";
    emit enableServiceButton(service);
}

void sync_widget::getAllTaskslists(){
    myGtasks->listTasklists().startAndCallback(this, SLOT(onTasklistsReceived(GTasks::TasklistCollection,GTasks::Error)));
}

QDomDocument gtaskDoc;
QDomElement gtaskRoot;
QDomElement gtaskListNode;
QMap<QString, QString>::iterator gtaskListMapIt;
void sync_widget::getFilesGTask(){
    gtaskListMapIt = gtaskListMap.begin();
    gtaskRoot = gtaskDoc.createElement("lists");
    gtaskDoc.appendChild(gtaskRoot);
    getFilesListFunc();
}

void sync_widget::getFilesListFunc(){
    if(gtaskListMapIt != gtaskListMap.end()){
        myGtasks->getTasklist(gtaskListMapIt.value()).startAndCallback(this, SLOT(onListReturn(GTasks::Tasklist,GTasks::Error)));
    }
    else
        getFilesGTaskWrite();
}

void sync_widget::onListReturn(GTasks::Tasklist tasklist, GTasks::Error error){
    if (error.code() != QNetworkReply::NoError) {
        // Do some error handling here
        qDebug() << "Error:" << error.code() << error.message();
        emit googleRecvResult(false);
        return;
    }
    qDebug() << "list returned";

    gtaskListNode = gtaskDoc.createElement("list");
    gtaskListNode.setAttribute("name", tasklist.title());
    if(gtaskListNode.attribute("name") != ""){
        gtaskRoot.appendChild(gtaskListNode);
        myGtasks->listTasks(tasklist.id()).startAndCallback(this, SLOT(onTaskListingReturn(GTasks::TaskCollection,GTasks::Error)));
    }
    else{
        gtaskListMapIt++;
        getFilesListFunc();
    }
}

void sync_widget::onTaskListingReturn(GTasks::TaskCollection collection, GTasks::Error error){
    if (error.code() != QNetworkReply::NoError) {
        // Do some error handling here
        qDebug() << "Error:" << error.code() << error.message();
        emit googleRecvResult(false);
        return;
    }
    qDebug() << "task collection returned";

    for(int j=0; j < collection.items().count(); j++)
    {
        QDomElement task_node = gtaskDoc.createElement("task");
        task_node.setAttribute("name", collection.items().at(j).title());
        task_node.setAttribute("note", collection.items().at(j).notes());
        task_node.setAttribute("plain_note", collection.items().at(j).notes());
        task_node.setAttribute("due", collection.items().at(j).due().toString("yyyy-MM-dd"));
        if(!collection.items().at(j).completed().toString().isEmpty())
        {
            task_node.setAttribute("status", "done");
        }
        else
        {
            task_node.setAttribute("status", "not done");
        }
        if(task_node.attribute("name") != "")
            gtaskListNode.appendChild(task_node);
    }

    gtaskListMapIt++;
    getFilesListFunc();
}

void sync_widget::getFilesGTaskWrite(){
    QString filename = "gtasks.xml";
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(this, tr("Cannot write to the file"), file.errorString());
        emit googleRecvResult(false);
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << gtaskDoc.toString();
        file.close();
    }

    gtaskDoc.clear();
    emit openXml(filename);

    emit googleRecvResult(true);
    if(syncCall == true)
        syncCall = false;
}



QDomElement tasks_ele;
QDomNodeList lists;
int curr, count;

void sync_widget::sendFilesGTask(){
    QMessageBox *syncWarning = new QMessageBox(this);
    syncWarning->setText("Google Tasks does not take HTML-formatted notes OR tags. If pulled back down, the notes will no longer have either (tags, colors, bold, italics, etc). Is this ok?");
    syncWarning->setIcon(QMessageBox::Warning);
    syncWarning->addButton(QMessageBox::Ok);
    syncWarning->addButton(QMessageBox::Cancel);

    switch(syncWarning->exec()){
    case QMessageBox::Ok:
        sendFilesGTaskOK();
        break;
    case QMessageBox::Cancel:
        this->hide();
        break;
    default:
        this->hide();
    }
}

void sync_widget::sendFilesGTaskOK(){
    emit getSaveFile(saveFilePath);
    QFile file(saveFilePath);
    QDomDocument doc;
    //getAllTaskslists();

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(this, tr("Cannot read the file"), file.errorString());
        emit googleSendResult(false);
        return;
    }
    else{
        if(!doc.setContent(&file)){
            QMessageBox::information(this, tr("Read XML Fail"), file.errorString());
            emit googleSendResult(false);
            return;
        }
        file.close();
    }
    //starting load domElement into my treeWidget
    int error_type = 0;
    QDomElement root = doc.firstChildElement();
    if(root.tagName()!="lists"){
        QMessageBox::information(this, tr("XML Format not correct"), file.errorString());
        emit googleSendResult(false);
        return;
    }
    lists = root.elementsByTagName("list");
    curr = 0;
    count = lists.count();
    listRemake();

    if(error_type==1)
    {//the XML file's format is wrong
        //this->clear();
        QMessageBox::information(this, tr("XML Format not correct"), file.errorString());
        emit googleSendResult(false);
        return;
    }
    else
    {
        //this->file_location = fileName;
    }
}

void sync_widget::listRemake(){
    if(curr != count){
        QDomNode tasks = lists.at(curr);
        if(tasks.isElement())
        {
            tasks_ele = tasks.toElement();
            //create the list
            if(gtaskListMap.find(tasks_ele.attribute("name","")) != gtaskListMap.end()){
                qDebug() << "found list " << tasks_ele.attribute("name","") << "online, updating..";
                myGtasks->deleteTasklist(gtaskListMap[tasks_ele.attribute("name","")]).startAndCallback(this, SLOT(onClearSent(GTasks::Error)));
                gtaskListMap.erase(gtaskListMap.find(tasks_ele.attribute("name","")));
            }
            else{
                qDebug() << "list not found, creating";
                GTasks::Tasklist *newList = new GTasks::Tasklist();
                newList->setTitle(tasks_ele.attribute("name",""));

                myGtasks->insertTasklist(*newList).startAndCallback(this, SLOT(onTasklistsSent(GTasks::Tasklist, GTasks::Error)));
            }
        }
    }
    qDebug() << "list addition complete";
    emit googleSendResult(true);
    if(syncCall == true)
        this->getFilesGTask();
}

void sync_widget::onClearSent(GTasks::Error error){
    if (error.code() != QNetworkReply::NoError) {
        // Do some error handling here
        qDebug() << "Error:" << error.code() << error.message();
        emit googleSendResult(false);
        return;
    }
    qDebug() << "clear complete";

    GTasks::Tasklist *newList = new GTasks::Tasklist();
    newList->setTitle(tasks_ele.attribute("name",""));

    myGtasks->insertTasklist(*newList).startAndCallback(this, SLOT(onTasklistsSent(GTasks::Tasklist, GTasks::Error)));
}

int currTask;
void sync_widget::onTasklistsSent(GTasks::Tasklist tasklistMeta, GTasks::Error error){
    if (error.code() != QNetworkReply::NoError) {
        // Do some error handling here
        qDebug() << "Error:" << error.code() << error.message();
        emit googleSendResult(false);
        return;
    }

    // Display the title and id of each tasklist
    qDebug() << "Tasklist: " << tasklistMeta.title() << "id: " << tasklistMeta.id();
    gtaskListMap[tasklistMeta.title()] = tasklistMeta.id();

    currTask = tasks_ele.childNodes().count() - 1;
    listCreate();
}


void sync_widget::listCreate(){
    //qDebug() << "did not find list " << tasks_ele.attribute("name","") << "online, creating..";

    if(currTask >= 0){
        QDomElement task_ele = tasks_ele.childNodes().at(currTask).toElement();
        GTasks::Task *newTask = new GTasks::Task();
        newTask->setTitle(task_ele.attribute("name"));
        newTask->setNotes(task_ele.attribute("plain_note"));
        QDate dueDate;
        dueDate = dueDate.fromString(task_ele.attribute("due"),"yyyy-MM-dd");
        qDebug() << "dueDateElt: " << task_ele.attribute("due") << "dueDate: " << dueDate.toString();
        QDateTime dueDateTime(dueDate);
        dueDateTime.setUtcOffset(-1*60*60*5);

        newTask->setDue(dueDateTime);
        if(task_ele.attribute("status") == "done"){
            newTask->setCompleted(dueDateTime);
        }
        myGtasks->insertTask(gtaskListMap[tasks_ele.attribute("name","")], *newTask).startAndCallback(this, SLOT(onTaskSent(GTasks::Task, GTasks::Error)));
    }
    else{
        listRemake();
        curr++;
    }
}



void sync_widget::onTaskSent(GTasks::Task taskMeta, GTasks::Error error){
    if (error.code() != QNetworkReply::NoError) {
        // Do some error handling here
        qDebug() << "Error:" << error.code() << error.message() << error.httpCode() << error.gtasksMessage();
        emit googleSendResult(false);
        return;
    }

    // Display the title and id of each tasklist
    qDebug() << "Task: " << taskMeta.title() << "id: " << taskMeta.id();
    currTask--;
    listCreate();
}

void sync_widget::onTasklistsReceived(GTasks::TasklistCollection tasklists, GTasks::Error error){
    if (error.code() != QNetworkReply::NoError) {
        // Do some error handling here
        qDebug() << "Error:" << error.code() << error.message();
        emit googleSendResult(false);
        return;
    }
    gtaskListMap.clear();
    // Display the title and id of each tasklist
    foreach (const GTasks::Tasklist& tasklist, tasklists.items()) {
        qDebug() << "Tasklist:" << tasklist.title() << "id:" << tasklist.id();
        gtaskListMap[tasklist.title()] = tasklist.id();
    }
}

sync_widget::sync_widget(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent)
{
    syncGrid = new QGridLayout();
    QPushButton *syncOneBut = new QPushButton("Google Task");
    QPushButton *syncTwoBut = new QPushButton("DropBox");
    syncCall = false;

    syncGrid->addWidget(syncOneBut, 0, 0);
    syncGrid->addWidget(syncTwoBut, 0, 1);

    this->setLayout(syncGrid);
    this->setWindowTitle("Select Service");
    this->setWindowFlags(f);

    networkManager = new QNetworkAccessManager(this);
    connect(syncOneBut, SIGNAL(clicked()), this, SLOT(googleClick()));
    connect(syncTwoBut, SIGNAL(clicked()), this, SLOT(dboxClick()));
}

sync_widget::~sync_widget(){
}
