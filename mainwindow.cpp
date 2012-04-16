#include "mainwindow.h"
#include "sync_widget.h"
#include "search_dial.h"
#include <QtGui>
#include <QWidget>
#include <QDebug>
#include <QtXml>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    syncTimer = new QTimer(this);
    my_task_list = new task_list;
    my_sync_widget = new sync_widget(this, Qt::Popup | Qt::Dialog);

    my_sync_widget->m_oauthHelper = new OAuth::Helper(this);
    initial();
    setWindowTitle("Task List");

    search_gen = new search;
    my_search_dial = new search_dial;

}

MainWindow::~MainWindow()
{
    delete my_task_list;
}

void MainWindow::googleSendStatus(bool result){
    if(result)
        statusBar()->showMessage(tr("Current file successfully sent"));
    else
        statusBar()->showMessage(tr("Error: current file not fully synced (Google)"));
}

void MainWindow::googleRecvStatus(bool result){
    if(result)
        statusBar()->showMessage(tr("Current file updated with Google Tasks"));
    else
        statusBar()->showMessage(tr("Error: current file not fully synced (Google)"));
}

void MainWindow::dboxSendStatus(bool result){
    if(result)
        statusBar()->showMessage(tr("Current file sent to Dropbox"));
    else
        statusBar()->showMessage(tr("Error: current file not fully synced (Dropbox)"));
}

void MainWindow::dboxRecvStatus(bool result){
    if(result)
        statusBar()->showMessage(tr("Dropbox files successfully pulled down"));
    else
        statusBar()->showMessage(tr("Error: current file not fully synced (Dropbox)"));
}

void MainWindow::googleAuthStatus(bool result){
    if(result)
        statusBar()->showMessage(tr("Google Authentication complete"));
    else
        statusBar()->showMessage(tr("Error: Tasks not authenticated, try again..."));
}

void MainWindow::dboxAuthStatus(bool result){
    if(result)
        statusBar()->showMessage(tr("Dropbox Authentication complete"));
    else
        statusBar()->showMessage(tr("Error: Dropbox not authenticated, try again..."));
}




void MainWindow::newServiceClick(){
    my_sync_widget->show();
}

void MainWindow::syncClick(){
    //if(syncTimer == NULL)
    if(!syncTimer->isActive()){
        syncTimer->start(30000);
        connect(syncTimer, SIGNAL(timeout()), this, SLOT(syncClick()));
        sync_service_off->setEnabled(true);
    }
    emit syncClickEmit();
}

void MainWindow::syncClickOff(){
    syncTimer->stop();
    disconnect(syncTimer, SIGNAL(timeout()), this, SLOT(syncClick()));
    sync_service_off->setDisabled(true);
}

void MainWindow::serviceAdded(QString &serviceType){
    sync_service->setEnabled(true);

    if(serviceType == "dbox"){
        send_service->setEnabled(true);
        get_service->setEnabled(true);
    }
    else if(serviceType == "google")
    {
        send_service_gtask->setEnabled(true);
        get_service_gtask->setEnabled(true);
    }
    else{
        qDebug() << "serviceAdded - error, service not recognized";
    }
}

void MainWindow::openXmlRecv(QString &filename){
    this->my_task_list->loadXml(filename);
}

void MainWindow::initial()
{
    my_sync_widget->setWindowModality(Qt::WindowModal);
    my_sync_widget->hide();

    connect(my_sync_widget, SIGNAL(getSaveFile(QString&)), this, SLOT(syncSaveFile(QString&)));
    connect(my_sync_widget, SIGNAL(enableServiceButton(QString&)), this, SLOT(serviceAdded(QString&)));
    connect(this, SIGNAL(syncClickEmit()), my_sync_widget, SLOT(syncFiles()));
    connect(my_sync_widget, SIGNAL(openXml(QString&)), this, SLOT(openXmlRecv(QString&)));

    connect(my_sync_widget, SIGNAL(dboxAuthResult(bool)), this, SLOT(dboxAuthStatus(bool)));
    connect(my_sync_widget, SIGNAL(dboxRecvResult(bool)), this, SLOT(dboxRecvStatus(bool)));
    connect(my_sync_widget, SIGNAL(dboxSendResult(bool)), this, SLOT(dboxSendStatus(bool)));

    connect(my_sync_widget, SIGNAL(googleAuthResult(bool)), this, SLOT(googleAuthStatus(bool)));
    connect(my_sync_widget, SIGNAL(googleRecvResult(bool)), this, SLOT(googleRecvStatus(bool)));
    connect(my_sync_widget, SIGNAL(googleSendResult(bool)), this, SLOT(googleSendStatus(bool)));


    listToolBar = new QToolBar("List");

    fileMenu = menuBar()->addMenu(tr("&File"));

    newList = new QAction( tr("&New lists..."), this );
    newList->setShortcuts(QKeySequence::New);
    fileMenu->addAction(newList);
    connect(newList, SIGNAL(triggered()),
            this->my_task_list,SLOT(new_list()));

    fileMenu->addSeparator();
    loadAction = new QAction(tr("&Open XML..."), this);
    loadAction->setShortcuts(QKeySequence::Open);
    fileMenu->addAction(loadAction);
    connect(loadAction, SIGNAL(triggered()),
            this, SLOT(loadFile()));

    saveAction = new QAction( tr("&Save..."), this );
    saveAction->setShortcuts(QKeySequence::Save);
    fileMenu->addAction(saveAction);
    connect(saveAction, SIGNAL(triggered()),
            this, SLOT(saveFile()));

    saveAsAction = new QAction( tr("&Save as..."), this );
    //saveAsAction->setShortcuts(QKeySequence::SaveAs);
    fileMenu->addAction(saveAsAction);
    connect(saveAsAction, SIGNAL(triggered()),
            this, SLOT(saveasFile()));
    fileMenu->addSeparator();


    printAction = new QAction(tr("&Print"),this);
    printAction->setShortcuts(QKeySequence::Print);
    fileMenu->addAction(printAction);
    connect(printAction, SIGNAL(triggered()),
            this,SLOT(print()));

    fileMenu->addSeparator();

    exitAction = new QAction(tr("&Exit"), this);
    fileMenu->addAction(exitAction);
    connect(exitAction, SIGNAL(triggered()),
            this, SLOT(close()));

    //list menu
    listMenu = menuBar()->addMenu(tr("&List"));
    addAct = new QAction(QIcon(":/style/all/images/new_task.png"),tr("&Add task"),this);
    addAct->setShortcut(tr("Ctrl+A"));
    listMenu->addAction(addAct);
    listToolBar->addAction(addAct);


    connect(addAct, SIGNAL(triggered()),
            this->my_task_list,SLOT(addTask()));

    editAct = new QAction(QIcon(":/style/all/images/edit.png"),tr("&Edit task"), this);
    editAct->setShortcut(tr("Ctrl+E"));
    listMenu->addAction(editAct);
    listToolBar->addAction(editAct);
    editAct->setCheckable(false);
//    connect(this->my_task_list, SIGNAL(itemSelectionChanged()),
//            editAct,SLOT(setEnabled(bool)));

    connect(editAct, SIGNAL(triggered()),
            this->my_task_list,SLOT(editTask()));

    delAct = new QAction(QIcon(":/style/all/images/delete.png"),tr("&Delete"),this);
    delAct->setShortcut(QKeySequence::Delete);
    listMenu->addAction(delAct);
    listToolBar->addAction(delAct);

    connect(delAct, SIGNAL(triggered()),
            this->my_task_list,SLOT(delTask()));


    listMenu->addSeparator();

    display_note = new QAction(tr("&Display/Hide Note"), this);
    listMenu->addAction(display_note);

    //template menu
    Template = menuBar()->addMenu(tr("&Template"));

    new_grocery = new QAction(tr("&Groceries"),this);
    Template->addAction(new_grocery);

    new_week_task = new QAction(tr("&Weekly Task"),this);
    Template->addAction(new_week_task);

    new_reminder_task = new QAction(tr("&Reminder List"), this);
    Template->addAction(new_reminder_task);

    new_cleaning_task = new QAction(tr("&Cleaning List"), this);
    Template->addAction(new_cleaning_task);

    new_job_task = new QAction(tr("&Job List"), this);
    Template->addAction(new_job_task);

    Sync = menuBar()->addMenu(tr("&Sync Menu"));

    new_service = new QAction(tr("&Add Service"), this);
    Sync->addAction(new_service);
    sync_service = new QAction(tr("&Sync Services (On)"), this);
    Sync->addAction(sync_service);
    sync_service_off = new QAction(tr("&Sync Services Off"), this);
    Sync->addAction(sync_service_off);
    send_service = new QAction(tr("&Send Current File"), this);
    Sync->addAction(send_service);
    get_service = new QAction(tr("&Get Dropbox Files"), this);
    Sync->addAction(get_service);
    send_service_gtask = new QAction(tr("&Send Current File (GTask)"), this);
    Sync->addAction(send_service_gtask);
    get_service_gtask = new QAction(tr("&Get GTask Files"), this);
    Sync->addAction(get_service_gtask);

    sync_service->setDisabled(true);
    send_service->setDisabled(true);
    sync_service_off->setDisabled(true);
    get_service->setDisabled(true);
    send_service_gtask->setDisabled(true);
    get_service_gtask->setDisabled(true);

    search_button = new QPushButton(tr("Search"));

    QWidget *main_widget = new QWidget;

    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *button_layout2 = new QHBoxLayout;


    this->my_task_list->setColumnCount(4);
    this->my_task_list->setDragEnabled(true);
    this->my_task_list->viewport()->setAcceptDrops(true);
    this->my_task_list->setDropIndicatorShown(true);
    this->my_task_list->setDragDropMode(QAbstractItemView::DragDrop);
    QStringList tmp_l;
    tmp_l << "Name" << "Note" << "Due Date" << "Status";
    this->my_task_list->setHeaderLabels(tmp_l );


    this->my_task_list->setEditTriggers(QAbstractItemView::DoubleClicked);
    this->my_task_list->setSelectionMode(QAbstractItemView::SingleSelection);
    this->my_task_list->setSelectionBehavior(QAbstractItemView::SelectRows);


    main_layout->addWidget(listToolBar);
    main_layout->addWidget( new QLabel(tr("Current Lists")),0,Qt::AlignCenter );
    main_layout->addWidget(this->my_task_list);


    button_layout2->addWidget(search_button);

    main_layout->addLayout(button_layout2);
    main_widget->setLayout(main_layout);
    main_widget->setMinimumSize(430,500);
    this->setCentralWidget(main_widget);

    //connection

    connect(this->my_task_list, SIGNAL(doubleClicked(QModelIndex)),
            this->my_task_list, SLOT(editTask()));

    connect(display_note, SIGNAL(triggered()),
            this->my_task_list, SLOT(show_hide_Note()));//the action one in the menu

    connect(new_grocery, SIGNAL(triggered()),
            this->my_task_list, SLOT(grocery()));

    connect(new_week_task, SIGNAL(triggered()),
            this->my_task_list, SLOT(week_task()));

    connect(new_reminder_task, SIGNAL(triggered()),
            this->my_task_list, SLOT(reminder_task()));

    connect(new_cleaning_task, SIGNAL(triggered()),
            this->my_task_list, SLOT(cleaning_task()));

    connect(new_job_task, SIGNAL(triggered()),
            this->my_task_list, SLOT(job_task()));

    connect(new_service, SIGNAL(triggered()),
            this, SLOT(newServiceClick()));

    connect(sync_service, SIGNAL(triggered()),
            this, SLOT(syncClick()));
    connect(sync_service_off, SIGNAL(triggered()),
            this, SLOT(syncClickOff()));
    connect(send_service, SIGNAL(triggered()),
            this->my_sync_widget, SLOT(sendFiles()));
    connect(get_service, SIGNAL(triggered()),
            this->my_sync_widget, SLOT(getFiles()));
    connect(send_service_gtask, SIGNAL(triggered()),
            this->my_sync_widget, SLOT(sendFilesGTask()));
    connect(get_service_gtask, SIGNAL(triggered()),
            this->my_sync_widget, SLOT(getFilesGTask()));

    connect(search_button, SIGNAL(clicked()), this, SLOT(search_start()));

}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        this->my_task_list->loadXml(fileName);
    }

}

void MainWindow::saveasFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
    {
        this->my_task_list->writeXml(fileName);
    }

}

void MainWindow::saveFile()
{
    if(this->my_task_list->file_location!="")
    {
        this->my_task_list->writeXml(this->my_task_list->file_location);
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this);
        if (!fileName.isEmpty())
        {
            this->my_task_list->writeXml(fileName);
        }
    }
}

void MainWindow::syncSaveFile(QString &syncPathName)
{
    if(this->my_task_list->file_location!="")
    {
        this->my_task_list->writeXml(this->my_task_list->file_location);
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this);
        if (!fileName.isEmpty())
        {
            this->my_task_list->writeXml(fileName);
        }
    }

    syncPathName = this->my_task_list->file_location;
}

void MainWindow::print()
{
         QPrinter *printer = new QPrinter;

         QPrintDialog *printDialog = new QPrintDialog(printer, this);

         if (printDialog->exec() == QDialog::Accepted)
         {
             QPainter temp_print(printer);
             QPixmap p_map = QPixmap::grabWidget(this);
             temp_print.drawPixmap(0, 0, p_map);
         }
}

void MainWindow::search_start(){
    connect(my_search_dial, SIGNAL(emitSearch(QString)), this, SLOT(searchRecv(QString)));
    if(my_search_dial->exec()){
        my_search_dial->search_editor->setText("");
    }
    disconnect(my_search_dial, SIGNAL(emitSearch(QString)), this, SLOT(searchRecv(QString)));
}

void MainWindow::searchRecv(QString searchText){
    task_list* my_result_list= this->search_gen->start_search(searchText, this->my_task_list);
    my_result_list->writeXml("search.xml");
    my_task_list->loadXml("search.xml");
}
