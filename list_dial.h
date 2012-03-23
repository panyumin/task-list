#ifndef LIST_DIAL_H
#define LIST_DIAL_H

#include <QDialog>

class QLabel;
class QPushButton;
class QLineEdit;

class list_dial : public QDialog
{
    Q_OBJECT
public:
    explicit list_dial(QWidget *parent = 0);
    QLineEdit *list_name;

signals:
    
public slots:

private:
    QLabel *listname_lab;
    QPushButton *submitButton;
    QPushButton *cancelButton;
    
};

#endif // LIST_DIAL_H
