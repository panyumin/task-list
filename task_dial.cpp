#include "task_dial.h"
#include <QtGui>


task_dial::task_dial(QWidget *parent) :
    QDialog(parent)
{
    taskname_lab = new QLabel("task name");
    task_tag_lab = new QLabel("task tag");
    tasknote_lab = new QLabel("task note");
    due_time_lab = new QLabel("due time");

    task_name = new QLineEdit;
    task_tag = new QLineEdit;
    due_t = new QDateEdit;

    h_edit = new htmleditor;

    //setup layout
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(taskname_lab, 0, Qt::AlignCenter);
    main_layout->addWidget(task_name);
    main_layout->addWidget(task_tag_lab, 0, Qt::AlignCenter);
    main_layout->addWidget(task_tag);
    main_layout->addWidget(tasknote_lab, 0, Qt::AlignCenter);
    main_layout->addWidget(h_edit);
    main_layout->addWidget(due_time_lab, 0, Qt::AlignCenter);
    main_layout->addWidget(due_t);




    QHBoxLayout *button_layout = new QHBoxLayout;
    submitButton = new QPushButton("submit");
    cancelButton = new QPushButton("cancel");
    button_layout->addWidget(submitButton);
    button_layout->addWidget(cancelButton);

    main_layout->addLayout(button_layout);

    setLayout(main_layout);
    //connection
    connect(submitButton, SIGNAL(clicked()),
            this, SLOT(accept()));

    connect(cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));
}
