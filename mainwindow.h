
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "task_list.h"
#include "search.h"
#include "search_dial.h"

class QListWidget;
class QStackedWidget;
class QPushButton;
class QListWidgetItem;
class QCalendarWidget;
class QCheckBox;
class sync_widget;
class QTimer;


#include <QtGui/QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void serviceAdded(QString &serviceType);
    void openXmlRecv(QString &filename);
    void dboxRecvStatus(bool result);
    void dboxSendStatus(bool result);
    void googleRecvStatus(bool result);
    void googleSendStatus(bool result);

    void googleAuthStatus(bool result);
    void dboxAuthStatus(bool result);

    void searchRecv(QString searchText);
private slots:
    void loadFile();
    void saveasFile();
    void saveFile();    
    void print();

    void syncSaveFile(QString &syncPathName);
    void newServiceClick();
    void syncClick();
    void syncClickOff();
    void search_start();

signals:
    void syncClickEmit();

private:
    void initial();
    task_list *my_task_list;


    QMenu *fileMenu;
    QAction *loadAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *printAction;
    QAction *exitAction;
    QAction *newList;

    QMenu *listMenu;
    QAction *addAct;
    QAction *delAct;
    QAction *editAct;
    QAction *display_note;
    QAction *search_for;

    QToolBar *listToolBar;

    QMenu *Template;
    QAction *new_grocery;
    QAction *new_week_task;
    QAction *new_reminder_task;
    QAction *new_cleaning_task;
    QAction *new_job_task;

    QMenu *Sync;
    QAction *new_service;
    QAction *sync_service;
    QAction *sync_service_off;
    QAction *send_service;
    QAction *get_service;
    QAction *send_service_gtask;
    QAction *get_service_gtask;

    sync_widget *my_sync_widget;
    QTimer *syncTimer;

    QPushButton *search_button;
    search *search_gen;
    search_dial *my_search_dial;
};

#endif // MAINWINDOW_H
