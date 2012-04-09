#include "mainwindow.h"
#include "sync_widget.h"
#include <QtGui>
#include <QWidget>
#include <QDebug>
#include <QtXml>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    //syncTimer = new QTimer(this);
    my_task_list = new task_list;
//    my_sync_widget = new sync_widget(this, Qt::Popup | Qt::Dialog);

//    my_sync_widget->oauthRequest = new KQOAuthRequest(this);
//    my_sync_widget->oauthManager = new KQOAuthManager(this);

//    my_sync_widget->m_oauthHelper = new OAuth::Helper(this);
    initial();
    setWindowTitle("Task List");

}

MainWindow::~MainWindow()
{
    delete my_task_list;
}

/*void MainWindow::newServiceClick(){
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
}*/

void MainWindow::initial()
{
//    my_sync_widget->setWindowModality(Qt::WindowModal);
//    my_sync_widget->hide();
//    connect(my_sync_widget, SIGNAL(getSaveFile(QString&)), this, SLOT(syncSaveFile(QString&)));
//    connect(my_sync_widget, SIGNAL(enableServiceButton(QString&)), this, SLOT(serviceAdded(QString&)));
//    connect(this, SIGNAL(syncClickEmit()), my_sync_widget, SLOT(syncFiles()));
//    connect(my_sync_widget, SIGNAL(openXml(QString&)), this, SLOT(openXmlRecv(QString&)));

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

    OptMenu = menuBar()->addMenu(tr("&Options"));

    change_font = new QAction(tr("&Change Font"),this);
    OptMenu->addAction(change_font);

    display_note = new QAction(tr("&Display/Hide Note"), this);
    OptMenu->addAction(display_note);

    Template = menuBar()->addMenu(tr("&Template"));

    new_grocery = new QAction(tr("&Groceries"),this);
    Template->addAction(new_grocery);

    new_week_task = new QAction(tr("&Weekly Task"),this);
    Template->addAction(new_week_task);

//    Sync = menuBar()->addMenu(tr("&Sync Menu"));

//    new_service = new QAction(tr("&Add Service"), this);
//    Sync->addAction(new_service);
//    sync_service = new QAction(tr("&Sync Services (On)"), this);
//    Sync->addAction(sync_service);
//    sync_service_off = new QAction(tr("&Sync Services Off"), this);
//    Sync->addAction(sync_service_off);
//    send_service = new QAction(tr("&Send Current File"), this);
//    Sync->addAction(send_service);
//    get_service = new QAction(tr("&Get Dropbox Files"), this);
//    Sync->addAction(get_service);
//    send_service_gtask = new QAction(tr("&Send Current File (GTask)"), this);
//    Sync->addAction(send_service_gtask);
//    get_service_gtask = new QAction(tr("&Get GTask Files"), this);
//    Sync->addAction(get_service_gtask);

//    sync_service->setDisabled(true);
//    send_service->setDisabled(true);
//    sync_service_off->setDisabled(true);
//    get_service->setDisabled(true);
//    send_service_gtask->setDisabled(true);
//    get_service_gtask->setDisabled(true);

    addTask = new QPushButton( tr("Add Task") );
    delTask = new QPushButton( tr("Delete") );
    editTask = new QPushButton( tr("Edit Task") );
    pop_up = new QPushButton(tr("Pop Task Up"));
    move_down = new QPushButton(tr("Move Task Down"));

    QWidget *main_widget = new QWidget;

    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *button_layout1 = new QHBoxLayout;
    QHBoxLayout *button_layout2 = new QHBoxLayout;

    //here is the list name area
    //main_layout->addWidget( new QLabel(tr("Lists name")),0,Qt::AlignCenter );
    //main_layout->addWidget(my_task_list->lists_name);


    this->my_task_list->setColumnCount(4);
    QStringList tmp_l;
    tmp_l << "Name" << "Note" << "Due Date" << "Status";
    this->my_task_list->setHeaderLabels(tmp_l );

    this->my_task_list->setEditTriggers(QAbstractItemView::DoubleClicked);
    this->my_task_list->setSelectionMode(QAbstractItemView::SingleSelection);
    this->my_task_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    //here is the table content
    /*QStandardItemModel *tmp_mod = my_task_list->mod;
    tmp_mod->setColumnCount(4);
    tmp_mod->setHeaderData(0, Qt::Horizontal, tr("Name"));
    tmp_mod->setHeaderData(1,Qt::Horizontal, tr("Note"));
    tmp_mod->setHeaderData(2,Qt::Horizontal, tr("Due date"));
    tmp_mod->setHeaderData(3,Qt::Horizontal, tr("Status"));
    my_task_list->table->setModel(tmp_mod);
    my_task_list->table->setEditTriggers(QAbstractItemView::DoubleClicked);
    my_task_list->table->setSelectionMode(QAbstractItemView::SingleSelection);
    my_task_list->table->setSelectionBehavior(QAbstractItemView::SelectRows);*/

    /*QTreeWidgetItem* task_child = new QTreeWidgetItem(this->my_task_list,0);
    task_child->setText(0,"a");
    this->my_task_list->addTopLevelItem(task_child);
    QTreeWidgetItem* task_child1 = new QTreeWidgetItem(task_child,1);
    task_child1->setText(0,"1");
    QTreeWidgetItem* task_child2 = new QTreeWidgetItem(task_child,1);
    task_child2->setText(0,"2");
    QTreeWidgetItem* task_child3 = new QTreeWidgetItem(task_child,1);
    task_child3->setText(0,"3");
    task_child->addChild(task_child1);
        task_child->addChild(task_child2);
            task_child->addChild(task_child3);
            QTreeWidgetItem* task_child4 = new QTreeWidgetItem(task_child,1);
            task_child4->setText(0,"0");
            task_child->insertChild(-2,task_child4);
    */


    main_layout->addWidget( new QLabel(tr("List content")),0,Qt::AlignCenter );
    main_layout->addWidget(this->my_task_list);

    button_layout1->addWidget(addTask);
    button_layout1->addWidget(delTask);
    button_layout1->addWidget(editTask);

    button_layout2->addWidget(pop_up);
    button_layout2->addWidget(move_down);

    main_layout->addLayout(button_layout1);
    main_layout->addLayout(button_layout2);
    main_widget->setLayout(main_layout);
    main_widget->setMinimumSize(430,500);
    this->setCentralWidget(main_widget);

    //connection
    connect(addTask, SIGNAL(clicked()),
            this->my_task_list,SLOT(addTask()));

    connect(delTask, SIGNAL(clicked()),
            this->my_task_list,SLOT(delTask()));

    connect(editTask, SIGNAL(clicked()),
            this->my_task_list, SLOT(editTask()));

    connect(display_note, SIGNAL(triggered()),
            this->my_task_list, SLOT(show_hide_Note()));//the action one in the menu

    connect(change_font, SIGNAL(triggered()),
            this->my_task_list, SLOT(changeFont()));

    connect(pop_up, SIGNAL(clicked()),
            this->my_task_list, SLOT(pop_up()));

    connect(move_down, SIGNAL(clicked()),
            this->my_task_list, SLOT(move_down()));

    connect(new_grocery, SIGNAL(triggered()),
            this->my_task_list, SLOT(grocery()));

    connect(new_week_task, SIGNAL(triggered()),
            this->my_task_list, SLOT(week_task()));

//    connect(new_service, SIGNAL(triggered()),
//            this, SLOT(newServiceClick()));

//    connect(sync_service, SIGNAL(triggered()),
//            this, SLOT(syncClick()));
//    connect(sync_service_off, SIGNAL(triggered()),
//            this, SLOT(syncClickOff()));
//    connect(send_service, SIGNAL(triggered()),
//            this->my_sync_widget, SLOT(sendFiles()));
//    connect(get_service, SIGNAL(triggered()),
//            this->my_sync_widget, SLOT(getFiles()));
//    connect(send_service_gtask, SIGNAL(triggered()),
//            this->my_sync_widget, SLOT(sendFilesGTask()));
//    connect(get_service_gtask, SIGNAL(triggered()),
//            this->my_sync_widget, SLOT(getFilesGTask()));

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

//void MainWindow::syncSaveFile(QString &syncPathName)
//{
//    if(this->my_task_list->file_location!="")
//    {
//        this->my_task_list->writeXml(this->my_task_list->file_location);
//    }
//    else
//    {
//        QString fileName = QFileDialog::getSaveFileName(this);
//        if (!fileName.isEmpty())
//        {
//            this->my_task_list->writeXml(fileName);
//        }
//    }

//    syncPathName = this->my_task_list->file_location;
//}

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
