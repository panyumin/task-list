#ifndef TASK_DIAL_H
#define TASK_DIAL_H

#include <QDialog>
#include <QtGui>

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
    QTextEdit *task_note;
    QDateEdit *due_t;


signals:
    
public slots:

private:
    QLabel *taskname_lab;
    QLabel *tasknote_lab;
    QLabel *due_time_lab;
    QPushButton *submitButton;
    QPushButton *cancelButton;
    
};

#endif  //TASK_DIAL_H
