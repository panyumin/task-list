#ifndef SEARCH_H
#define SEARCH_H
#include<QObject>
#include "task_list.h"

//use this class to search for tasks that contain
//the querry string, based to the fucntion start_search.

class search : public QWidget
{
    Q_OBJECT
public:
    search();
    task_list *start_search(QString, task_list*);

protected:


signals:
public slots:

};

#endif // SEARCH_H
