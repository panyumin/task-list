#ifndef SEARCH_DIAL_H
#define SEARCH_DIAL_H

#include <QWidget>
#include <QDialog>
#include <QtGui>

class QLabel;
class QLineEdit;


class search_dial : public QDialog
{
    Q_OBJECT
public:
    explicit search_dial(QWidget *parent = 0);
    QLineEdit *search_editor;
signals:
    void emitSearch(QString input);
public slots:
    void submitClicked();
protected:
    QLabel *search_label;

    QString search_text;
    QPushButton* submitButton;
    QPushButton* cancelButton;

};

#endif // SEARCH_DIAL_H
