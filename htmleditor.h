#ifndef HTMLEDITOR_H
#define HTMLEDITOR_H

#include <QWidget>
#include <QtGui>


class QLabel;
class QPushButton;
class QTextEdit;
class QLineEdit;
class QDateTime;
class QMenu;
class QToolBar;
class QComboBox;
class QFontComboBox;

class htmleditor : public QWidget
{
    Q_OBJECT
public:
    explicit htmleditor(QWidget *parent = 0);
    QTextEdit *textEdit;

signals:
    
public slots:
    void bold();
    void italic();
    void underline();
    void strikethrough();
    void ali_left();
    void ali_center();
    void ali_right();
    void bullet();
    void number();
    void indent_left();
    void indent_right();
    void change_color();
    void change_size(const QString &str);
    void change_font(const QString &str);
    void format_changed(const QTextCharFormat &format);


private:
    void createActions();
    void createToolBars();
    void setup();



    QToolBar *editToolBar;
    QToolBar *styleToolBar;
    QToolBar *fontToolBar;

    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *underlineAct;
    QAction *strikethroughAct;
    QAction *leftAct;
    QAction *centerAct;
    QAction *rightAct;
    QAction *bulletAct;
    QAction *numberAct;
    QAction *indent_leftAct;
    QAction *indent_rightAct;
    QAction *colorAct;

    QComboBox *font_size_comb;
    QFontComboBox *font_comb;
    
};

#endif // HTMLEDITOR_H
