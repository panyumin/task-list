#include "task_list.h"
#include "task_dial.h"
#include "list_dial.h"
#include <QtXml>
#include <QtGui>
#include <QtCore>
#include <QDebug>


//test..
task_list::task_list(QWidget *parent) :
    QTreeWidget(parent)
{
    //table = new QTableView;
    //lists_name = new QListWidget;
    //mod = new QStandardItemModel;
    //show_note = true;
    file_location = "";
}

task_list::~task_list()
{
    //delete table;
    //delete mod;
}

void task_list::new_list()
{
    list_dial t_dial;
    t_dial.setWindowTitle( tr("Add new Task") );

    if(t_dial.exec())
    {
        QString list_name = t_dial.list_name->text();
        new_list(list_name);
    }

}

void task_list::new_list(QString name)
{
    //do we have to put some constrain over the input?
    if(name.isEmpty())
    {
        QMessageBox::warning(this,tr("Information not complete"),"List name can not be empty!");
    }
    else
    {
        QTreeWidgetItem* list_name = new QTreeWidgetItem(this,0);
        list_name->setText(0,name);
        list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        this->addTopLevelItem(list_name);
    }
}



void task_list::addTask()
{
    task_dial t_dial;
    t_dial.due_t->setDate(QDate::currentDate().addDays(1));
    t_dial.setWindowTitle( tr("Add new Task") );

    if(t_dial.exec())
    {
        QString task_name = t_dial.task_name->text();
        QString task_note = t_dial.task_note->toPlainText();
        QDate due_time = t_dial.due_t->date();

        addTask(task_name, task_note, due_time);

    }
}

void task_list::addTask(QString name, QString note, QDate due)
{
    //do we have to put some constrain over the input?
    if(name.isEmpty())
    {
        QMessageBox::warning(this,tr("Information not complete"),"Name can not be empty!");
    }
    else
    {
        int location = -1;
        for(int i=0; i<this->topLevelItemCount();i++)
        {
            if(this->currentItem() == this->topLevelItem(i))
            {
                location = i;
                break;
            }
        }

        if(!this->selectedItems().empty() && location!=-1)
        {//we can add the child to this node
            QTreeWidgetItem* task_child = new QTreeWidgetItem(this->topLevelItem(location),1);
            task_child->setText(0,name);
            task_child->setText(1,note);
            task_child->setText(2,due.toString("yyyy-MM-dd"));
            task_child->setCheckState(3,Qt::Unchecked);
            task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
            this->topLevelItem(location)->addChild(task_child);

        }
        else
        {
            QMessageBox::warning(this,tr("Add task Fail"),"Please select a list to add task!");
        }

    }
}

void task_list::pop_up()
{
    if(!this->selectedItems().empty() && this->currentItem()->type()==1)
    {//this is a child node and we can pop it
        int loc = -1;
        QTreeWidgetItem* parent_loc = this->currentItem()->parent();
        for(int i=0; i<parent_loc->childCount(); i++)
        {
            if(this->currentItem()==parent_loc->child(i))
            {
                loc = i;
            }
        }
        if(loc>0)
        {//we can pop it up
            QTreeWidgetItem* child_node = parent_loc->takeChild(loc);
            parent_loc->insertChild(loc-1,child_node);
            this->setCurrentItem(child_node);
            //this->setItemWidget();
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Move task Fail"),"Please select a task to move!");
    }
}


void task_list::move_down()
{
    if(!this->selectedItems().empty() && this->currentItem()->type()==1)
    {//this is a child node and we can pop it
        int loc = -1;
        QTreeWidgetItem* parent_loc = this->currentItem()->parent();
        for(int i=0; i<parent_loc->childCount(); i++)
        {
            if(this->currentItem()==parent_loc->child(i))
            {
                loc = i;
            }
        }
        if(loc< parent_loc->childCount()-1)
        {//we can pop it up
            QTreeWidgetItem* child_node = parent_loc->takeChild(loc);
            parent_loc->insertChild(loc+1,child_node);
            this->setCurrentItem(child_node);
            //this->setItemWidget();
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Move task Fail"),"Please select a task to move!");
    }
}

void task_list::editTask()
{
    //QItemSelectionModel *select_model = table->selectionModel();
    //QModelIndexList idx = select_model->selectedRows();
    QModelIndex i;
    QString name;
    QString note;
    QDate due;
    QString status;

    int location = -1;
    for(int i=0; i<this->topLevelItemCount();i++)
    {
        if(this->currentItem() == this->topLevelItem(i))
        {
            location = i;
            break;
        }
    }

    if( location==-1  )
    {//we get a child node and we need to update it.
       name =  this->currentItem()->text(0);
       note =  this->currentItem()->text(1);
       QString tmp_str = this->currentItem()->text(2);
       due = QDate::fromString(tmp_str, "yyyy-MM-dd");

    }


    if(!name.isEmpty())
    {
        task_dial t_dial;
        t_dial.setWindowTitle( tr("Edit Task") );

        t_dial.task_name->setText(name);
        t_dial.task_note->setText(note);
        t_dial.due_t->setDate(due);

        if(t_dial.exec())
        {
            QString task_name = t_dial.task_name->text();
            QString task_note = t_dial.task_note->toPlainText();
            QDate due_time = t_dial.due_t->date();

            editTask(task_name, task_note, due_time, status);
        }

    }

}

void task_list::editTask(QString name, QString note, QDate due, QString status)
{
    if(name.isEmpty())
    {
        QMessageBox::warning(this,tr("Information not complete"),"Name can not be empty!");
    }
    else
    {

        int loc = -1;
        QTreeWidgetItem* parent_loc = this->currentItem()->parent();
        for(int i=0; i<parent_loc->childCount(); i++)
        {
            if(this->currentItem()==parent_loc->child(i))
            {
                loc = i;
            }
        }
        //qDebug() << loc;

        this->currentItem()->setText(0,name);
        this->currentItem()->setText(1,note);
        this->currentItem()->setText(2,due.toString("yyyy-MM-dd"));
        if(this->currentItem()->checkState(3)==Qt::Unchecked)
        {
            this->currentItem()->setCheckState(3,Qt::Unchecked);
        }
        else
        {
            this->currentItem()->setCheckState(3,Qt::Checked);
        }


        /*QTreeWidgetItem* task_child = new QTreeWidgetItem( parent_loc,1);
        task_child->setText(0,name);
        task_child->setText(1,note);
        task_child->setText(2,due.toString("yyyy-MM-dd"));
        task_child->setCheckState(3,Qt::Unchecked);
        task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
        parent_loc->insertChild(loc, task_child);*/

    }

}

void task_list::show_hide_Note()
{
    if(show_note)
    {//not working

        this->hideColumn(1);
        show_note = false;
    }
    else
    {
        this->showColumn(1);
        show_note = true;
    }

}

void task_list::changeFont()
{
    bool change;
    QFont ori_font = this->font();
    QFont font = QFontDialog::getFont(&change, ori_font, this);
    if(change)
    {
        this->setFont(font);
    }

}


void task_list::delTask()
{
    this->currentItem()->~QTreeWidgetItem();
    //this->removeItemWidget(this->currentItem(),0);
}


void task_list::loadXml(QString fileName)
{
    QFile file(fileName);
    QDomDocument doc;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, tr("Can read the file"), file.errorString());
        return;
    }
    else
    {
        if(!doc.setContent(&file))
        {
            QMessageBox::information(this, tr("Read XML Fail"), file.errorString());
            return;
        }
        file.close();

    }
    //starting load domElement into my treeWidget
    int error_type = 0;
    QDomElement root = doc.firstChildElement();
    if(root.tagName()!="lists")
    {
        QMessageBox::information(this, tr("XML Format not correct"), file.errorString());
        return;
    }
    QDomNodeList lists = root.elementsByTagName("list");

    this->clear();//first clean all the current stuff

    for(int i=0; i<lists.count(); i++)
    {
        QDomNode tasks = lists.at(i);
        if(tasks.isElement())
        {
            QDomElement tasks_ele = tasks.toElement();

            //create the list
            QTreeWidgetItem* list_name = new QTreeWidgetItem(this,0);
            list_name->setText(0, tasks_ele.attribute("name","") );
            list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            this->addTopLevelItem(list_name);

            if(tasks_ele.attribute("name","")=="")
            {//list name is empty
                error_type =1;
            }

            for(int j=0; j<tasks_ele.childNodes().count(); j++)
            {
                QDomElement task_ele = tasks_ele.childNodes().at(j).toElement();

                QTreeWidgetItem* task_child = new QTreeWidgetItem(list_name,1);
                task_child->setText(0,task_ele.attribute("name"));
                task_child->setText(1,task_ele.attribute("note"));
                task_child->setText(2,task_ele.attribute("due"));
                if(task_ele.attribute("status")=="done")
                {
                    task_child->setCheckState(3,Qt::Checked);
                }
                else if(task_ele.attribute("status")=="not done")
                {
                     task_child->setCheckState(3,Qt::Unchecked);
                }
                else
                {
                    error_type = 1;
                }

                if(task_ele.attribute("name")=="")
                {//task name is empty
                    error_type = 1;
                }

                task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
                list_name->addChild(task_child);

            }
        }

    }

    if(error_type==1)
    {//the XML file's format is wrong
        this->clear();
        QMessageBox::information(this, tr("XML Format not correct"), file.errorString());
        return;
    }
    else
    {
        this->file_location = fileName;
    }
}


void task_list::writeXml(QString fileName)
{
    QFile file(fileName);
    QDomDocument doc;
    QDomElement root = doc.createElement("lists");
    doc.appendChild(root);

    for(int i=0; i<this->topLevelItemCount(); i++)
    {
        QDomElement list_node = doc.createElement("list");
        list_node.setAttribute("name", this->topLevelItem(i)->text(0));
        root.appendChild(list_node);

        QTreeWidgetItem* parent_loc = this->topLevelItem(i);
        for(int j=0; j<parent_loc->childCount(); j++)
        {
            QDomElement task_node = doc.createElement("task");
            task_node.setAttribute("name", parent_loc->child(j)->text(0));
            task_node.setAttribute("note", parent_loc->child(j)->text(1));
            task_node.setAttribute("due", parent_loc->child(j)->text(2));
            if( parent_loc->child(j)->checkState(3)==Qt::Checked)
            {
                task_node.setAttribute("status", "done");
            }
            else
            {
                task_node.setAttribute("status", "not done");
            }
            list_node.appendChild(task_node);
        }
    }

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(this, tr("Can write to the file"), file.errorString());
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
    this->file_location = fileName;

}

void task_list::grocery()
{
    QTreeWidgetItem* list_name = new QTreeWidgetItem(this,0);
    list_name->setText(0,"grocery");
    list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    this->addTopLevelItem(list_name);

    QTreeWidgetItem* task_child;
    QDate currDate = QDate::currentDate();
    //add preset items
    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Bread");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Milk");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Apple");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Beer");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Orange juice");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Egg");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Chips");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);
}

void task_list::week_task()
{
    QTreeWidgetItem* list_name = new QTreeWidgetItem(this,0);
    list_name->setText(0,"week_task");
    list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    this->addTopLevelItem(list_name);

    QTreeWidgetItem* task_child;
    QDate currDate = QDate::currentDate();

    //add preset items
    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Homework");
    task_child->setText(1,"");   
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Wash car");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Fishing");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Laundry");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Clean room");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsEditable);
    list_name->addChild(task_child);
}
