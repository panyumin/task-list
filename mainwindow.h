
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "task_list.h"

class QListWidget;
class QStackedWidget;
class QPushButton;
class QListWidgetItem;
class QCalendarWidget;
class QCheckBox;



#include <QtGui/QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadFile();
    void saveasFile();
    void saveFile();
    void print();


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

};

#endif // MAINWINDOW_H
