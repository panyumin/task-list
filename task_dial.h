#ifndef TASK_DIAL_H
#define TASK_DIAL_H

#include <QDialog>
#include <QtGui>
#include "htmleditor.h"

class QLabel;
class QPushButton;
class QTextEdit;
class QLineEdit;
class QDateTime;



class task_dial : public QDialog
{
    Q_OBJECT
public:
    explicit task_dial(QWidget *parent = 0);
    QLineEdit *task_name;
    QLineEdit *task_tag;
    QDateEdit *due_t;
    htmleditor* h_edit;


signals:
    
public slots:

private:
    QLabel *taskname_lab;
    QLabel *task_tag_lab;
    QLabel *tasknote_lab;
    QLabel *due_time_lab;
    QPushButton *submitButton;
    QPushButton *cancelButton;
    
};

#endif  //TASK_DIAL_H
