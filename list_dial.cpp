#include "list_dial.h"
#include <QtGui>

list_dial::list_dial(QWidget *parent) :
    QDialog(parent)
{
    listname_lab = new QLabel("list name");
    submitButton = new QPushButton("submit");
    cancelButton = new QPushButton("cancel");

    list_name = new QLineEdit;

    //setup layout
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(listname_lab, 0, Qt::AlignCenter);
    main_layout->addWidget(list_name);

    QHBoxLayout *button_layout = new QHBoxLayout;
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
