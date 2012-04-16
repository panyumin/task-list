#include "search_dial.h"

search_dial::search_dial(QWidget *parent) :
    QDialog(parent)
{
    search_label=new QLabel("enter your search text");
    submitButton=new QPushButton("Search");
    cancelButton=new QPushButton("Cancel");
    search_editor=new QLineEdit;

    QVBoxLayout *main_layout=new QVBoxLayout();
    main_layout->addWidget(search_label,0,Qt::AlignCenter);
    main_layout->addWidget(search_editor);

    QHBoxLayout *buttons_layout=new QHBoxLayout();
    buttons_layout->addWidget(submitButton);
    buttons_layout->addWidget(cancelButton);

    main_layout->addLayout(buttons_layout);

    setLayout(main_layout);

    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void search_dial::submitClicked(){

    QString input = search_editor->text();
    qDebug() << "search input: " << input;

    emit emitSearch(input);
    this->accept();
}
