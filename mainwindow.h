
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "task_list.h"

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
private slots:
    void loadFile();
    void saveasFile();
    void saveFile();    
    void print();

    void syncSaveFile(QString &syncPathName);
    void newServiceClick();
    void syncClick();
    void syncClickOff();

signals:
    void syncClickEmit();

private:
    void initial();
    QPushButton *addTask;
    QPushButton *delTask;
    QPushButton *editTask;
    QPushButton *pop_up;
    QPushButton *move_down;
    QStringList *lists_name;
    QVector<task_list*> lists;
    task_list *my_task_list;

    QMenu *fileMenu;
    QAction *loadAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *printAction;
    QAction *exitAction;
    QAction *newList;

    QMenu *OptMenu;
    QAction *change_font;
    QAction *display_note;

    QMenu *Template;
    QAction *new_grocery;
    QAction *new_week_task;

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
};

#endif // MAINWINDOW_H
