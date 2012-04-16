#include "task_list.h"
#include "task_dial.h"
#include "list_dial.h"
#include "html_delegate.h"
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
    HTMLDelegate* delegate = new HTMLDelegate();
    this->setItemDelegate(delegate);
    show_note=true;
    this->hideColumn(4);
}

task_list::~task_list()
{
    //delete table;
    //delete mod;
}

void task_list::new_list()
{
    list_dial t_dial;
    t_dial.setWindowTitle( tr("New") );

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
        list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled );
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
        QString task_note_html = t_dial.h_edit->textEdit->toHtml();
        QString task_note_plain = t_dial.h_edit->textEdit->toPlainText();
        qDebug() << "HTML: " << task_note_html;
        task_note_html.remove("<meta name=\"qrichtext\" content=\"1\" />");
        qDebug() << "HTML: " << task_note_html;
        qDebug() << "Plain: " << task_note_plain;
        QDate due_time = t_dial.due_t->date();

        addTask(task_name, task_note_html, due_time, task_note_plain);

    }
}

void task_list::addTask(QString name, QString note, QDate due, QString plain_note)
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
            //task_child->setData(2,Qt::UserRole,new QVariant(&due));
            task_child->setCheckState(3,Qt::Unchecked);
            task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled );//  | Qt::ItemIsEditable);
            task_child->setText(4, plain_note);
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

    if(currentItem()==NULL)
    {
        return;
    }

    int location = -1;
    for(int i=0; i<this->topLevelItemCount();i++)
    {
        if(this->currentItem() == this->topLevelItem(i))
        {
            location = i;
            break;
        }
    }

    if( location==-1 && this->currentItem()->type()==1  )
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
        t_dial.h_edit->textEdit->setHtml(note);
        t_dial.due_t->setDate(due);

        if(t_dial.exec())
        {
            QString task_name = t_dial.task_name->text();
            QString task_note_html = t_dial.h_edit->textEdit->toHtml();
            QDate due_time = t_dial.due_t->date();
            QString task_note_plain = t_dial.h_edit->textEdit->toPlainText();
            task_note_html.remove("<meta name=\"qrichtext\" content=\"1\" />");
            editTask(task_name, task_note_html, due_time, status, task_note_plain);
        }

    }

}

void task_list::editTask(QString name, QString note, QDate due, QString status, QString plain_note)
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
        this->currentItem()->setText(4, plain_note);

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
    if(this->currentItem()!=NULL)
    {
        this->currentItem()->~QTreeWidgetItem();
    }
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
            list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsDragEnabled);
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

                task_child->setText(4, task_ele.attribute("plain_note"));
                task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled );//| Qt::ItemIsEditable);
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
            task_node.setAttribute("plain_note", parent_loc->child(j)->text(4));
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
    list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    this->addTopLevelItem(list_name);

    QTreeWidgetItem* task_child;
    QDate currDate = QDate::currentDate();
    //add preset items
    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Bread");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);// | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Milk");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);// | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Apple");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);// | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Beer");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);// | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Orange juice");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);// | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Egg");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);// | Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Chips");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);// | Qt::ItemIsEditable);
    list_name->addChild(task_child);
}

void task_list::week_task()
{
    QTreeWidgetItem* list_name = new QTreeWidgetItem(this,0);
    list_name->setText(0,"week_task");
    list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    this->addTopLevelItem(list_name);

    QTreeWidgetItem* task_child;
    QDate currDate = QDate::currentDate();

    //add preset items
    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Homework");
    task_child->setText(1,"");   
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Wash car");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsDragEnabled);//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Fishing");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Laundry");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Clean room");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  | Qt::ItemIsDragEnabled);//| Qt::ItemIsEditable);
    list_name->addChild(task_child);
}

void task_list::reminder_task()
{
    QTreeWidgetItem* list_name = new QTreeWidgetItem(this,0);
    list_name->setText(0,"reminder");
    list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    this->addTopLevelItem(list_name);

    QTreeWidgetItem* task_child;
    QDate currDate = QDate::currentDate();

    //add preset items
    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Pay Electric Bill");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Mow the Lawn");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Call your Parents");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Wash the Car");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Change Air Filters");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);
}

void task_list::cleaning_task()
{
    QTreeWidgetItem* list_name = new QTreeWidgetItem(this,0);
    list_name->setText(0,"cleaning");
    list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    this->addTopLevelItem(list_name);

    QTreeWidgetItem* task_child;
    QDate currDate = QDate::currentDate();

    //add preset items
    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Sweep the Floor");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Mop the Bathroom");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Empty the Gutter");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Change Water Filter");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Burn Leaves");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);
}

void task_list::job_task()
{
    QTreeWidgetItem* list_name = new QTreeWidgetItem(this,0);
    list_name->setText(0,"job");
    list_name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    this->addTopLevelItem(list_name);

    QTreeWidgetItem* task_child;
    QDate currDate = QDate::currentDate();

    //add preset items
    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Debug Nuclear Plant Software");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Compose new Commercial");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Fishing");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Hire Lead");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);

    task_child = new QTreeWidgetItem(list_name,1);
    task_child->setText(0,"Fire Lead");
    task_child->setText(1,"");
    task_child->setText(2,currDate.toString("yyyy-MM-dd"));
    task_child->setCheckState(3,Qt::Unchecked);
    task_child->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled  );//| Qt::ItemIsEditable);
    list_name->addChild(task_child);
}

//the following part is the drag and drop implemention
bool task_list::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action)
{
    QList<QUrl> urlList;
    QTreeWidgetItem *task_item;

    urlList = data->urls(); // retrieve list of urls
    qDebug() << index;

    //because we just allow sigle selection
    //so we can use currentItem() to find the item be draged.
    // make new QTreeWidgetItem and set its text
    // if parent is null - add top level item (this parent)
    task_item = new QTreeWidgetItem();
    if (parent == NULL)
    {//int this case we drag to the empty area...


        if(currentItem()->type()==0)
        {//the dragged element is a whole list
            int loc = this->indexOfTopLevelItem(currentItem());
            if( (loc-index)==0 || (index-loc)==1 )
            {
                //do nothing
            }
            else if(loc > index)
            {
                task_item = this->takeTopLevelItem( loc );
                this->insertTopLevelItem(index,task_item);
                this->setCurrentItem(task_item);
            }
            else
            {
                task_item = this->takeTopLevelItem( loc );
                this->insertTopLevelItem(index-1,task_item);
                this->setCurrentItem(task_item);
            }


        }
        else
        {// the dragged element is a task

            QTreeWidgetItem * list_item = new QTreeWidgetItem(this,0);
            list_item->setText(0,"task list");
            task_item = currentItem()->parent()->takeChild( currentItem()->parent()->indexOfChild(currentItem()) );
            list_item->addChild(task_item);
            this->insertTopLevelItem(index,list_item);
            this->setCurrentItem(list_item);

        }

    }
    else
    {//we drop into the some top level item..
        //case1: drop in the same parent as it used to be
        //which means we are arranging the sequence of the elements in a list
        if(currentItem()->parent()==parent)
        {
            //when we remove the original child to index should change..
            int loc = currentItem()->parent()->indexOfChild(currentItem());
            if( (loc-index)==0 || (index-loc)==1 )
            {
                //do nothing
            }
            else if(loc > index)
            {
                task_item = currentItem()->parent()->takeChild( currentItem()->parent()->indexOfChild(currentItem()) );
                parent->insertChild(index,task_item);
            }
            else
            {

                task_item = currentItem()->parent()->takeChild( currentItem()->parent()->indexOfChild(currentItem()) );
                parent->insertChild(index-1,task_item);
            }

        }
        else
        {//drop into another list
            if(currentItem()->type()==1)
            {//if the dragged element is just a task
                task_item = currentItem()->parent()->takeChild( currentItem()->parent()->indexOfChild(currentItem()) );
                parent->insertChild(index,task_item);

            }
            else
            {// we have tp merge two list.
                int loc = this->indexOfTopLevelItem( currentItem() );
                QTreeWidgetItem * list_item = this->takeTopLevelItem(loc);

                int i = 0;
                while(list_item->childCount()>0)
                {
                    task_item = list_item->takeChild(0);
                    parent->insertChild(index + i,task_item);
                    i++;
                }

            }

        }

    }

    // set item text

    return true;

}

QStringList task_list::mimeTypes() const
{
    QStringList qstrList;
     // list of accepted mime types for drop
     qstrList.append("text/uri-list");
     return qstrList;
}

Qt::DropActions task_list::supportedDropActions () const
{
    return Qt::CopyAction | Qt::MoveAction;
}

void task_list::mouseMoveEvent(QMouseEvent *event)
{
    // if not left button - return
    if (!(event->buttons() & Qt::LeftButton)) return;

    // if no item selected, return (else it would crash)
    if (currentItem() == NULL) return;

    if(currentItem()->type()==0)
    {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        // construct list of QUrls
        // other widgets accept this mime type, we can drop to them
        QList<QUrl> list;
        list.append(QUrl(this->currentItem()->text(0)));
        list.append(QUrl(this->currentItem()->text(1)));
        list.append(QUrl(this->currentItem()->text(2)));
        list.append(QUrl(this->currentItem()->text(3)));
        list.append(QUrl(this->currentItem()->text(4)));

        // mime stuff
        mimeData->setUrls(list);
        drag->setMimeData(mimeData);

        // start drag
        drag->start(Qt::CopyAction | Qt::MoveAction);
    }

    if(currentItem()->type()==1)
    {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        // construct list of QUrls
        // other widgets accept this mime type, we can drop to them
        QList<QUrl> list;
        list.append(QUrl(this->currentItem()->text(0)));
        list.append(QUrl(this->currentItem()->text(1)));
        list.append(QUrl(this->currentItem()->text(2)));
        list.append(QUrl(this->currentItem()->text(3)));
        list.append(QUrl(this->currentItem()->text(4)));

        // mime stuff
        mimeData->setUrls(list);
        drag->setMimeData(mimeData);

        // start drag
        drag->start( Qt::CopyAction | Qt::MoveAction);

    }
}
