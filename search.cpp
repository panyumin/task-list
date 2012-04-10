#include "search.h"
#include<QtGui>

search::search()
{
}

task_list* search::start_search(QString querry,task_list * data)
{
   task_list* result=new task_list;
    //throw error if there was no string passed.
    if(querry.isEmpty()||querry.isNull())
    {
        QMessageBox::warning(this,QString("Error Empty Search String"),
                             QString("Search received an Empty or NULL string, this is not valid for a search"));
        return result;
    }


    if(data->topLevelItemCount()==0)
    {
        QMessageBox::warning(this,QString("Error No Tasks to Search"),
                             QString("Search was passed a set of data that contained no tasks"));
        return result;
    }

    result->new_list(querry);
    for(int i=0;i<data->topLevelItemCount();i++)
    {
        for(int j=0; j<data->topLevelItem(i)->childCount();j++)
        {
            QTreeWidgetItem* test=data->topLevelItem(i)->child(j);
            if(test->text(0).contains(querry,Qt::CaseInsensitive)||
               test->text(1).contains(querry,Qt::CaseInsensitive)||
               test->text(2).contains(querry,Qt::CaseInsensitive))
            {
                result->addTopLevelItem(test);
            }
        }

    }

    return result;
}


