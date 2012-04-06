/*
 *  GTasks - A C++/Qt API client for Google Tasks
 *
 *  Copyright (C) 2011 Gregory Schlomoff <gregory.schlomoff@gmail.com>
 *                     http://gregschlom.com
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include "task.h"
#include "task_p.h"

#include <QString>

namespace GTasks {

Task::Task()
	: d(new TaskPrivate)
{
}

/*!
  Convenience constructor to create a Task and initialize it with serialized data
*/
Task::Task(QVariantMap data)
	: d(new TaskPrivate)
{
	deserialize(data);
}

Task::~Task()
{
}

Task::Task(const Task& other)
	: d(other.d)
{
}

Task& Task::operator=(const Task& other)
{
	if (this != &other) {
		d = other.d;
	}
	return *this;
}

TaskPrivate::TaskPrivate()
	: QSharedData(),
	  kind("tasks#task"),
	  id(""),
	  etag(""),
	  title(""),
	  updated(QDateTime()),
	  selfLink(QUrl()),
	  parent(""),
	  position(""),
	  notes(""),
	  status(Task::NeedsAction),
	  due(QDateTime()),
	  completed(QDateTime()),
	  deleted(false),
	  hidden(false)
{
}

TaskPrivate::TaskPrivate(const TaskPrivate& other)
	: QSharedData(other),
	  kind(other.kind),
	  id(other.id),
	  etag(other.etag),
	  title(other.title),
	  updated(other.updated),
	  selfLink(other.selfLink),
	  parent(other.parent),
	  position(other.position),
	  notes(other.notes),
	  status(other.status),
	  due(other.due),
	  completed(other.completed),
	  deleted(other.deleted),
	  hidden(other.hidden)
{
}

QVariantMap Task::serialize() const
{
	QVariantMap result;
	result.insert("kind", d->kind);
	result.insert("id", d->id);
	result.insert("etag", d->etag);
	result.insert("title", d->title);
	result.insert("updated", d->updated);
	result.insert("selfLink", d->selfLink);
	result.insert("parent", d->parent);
	result.insert("position", d->position);
	result.insert("notes", d->notes);
	result.insert("status", d->statusAsString());
	result.insert("due", d->due);
	result.insert("completed", d->completed);
	result.insert("deleted", d->deleted);
	result.insert("hidden", d->hidden);
	return result;
}

void Task::deserialize(QVariantMap data)
{
	if (data.value("kind") != "tasks#task") {
		return;
	}

	d->id        = data.value("id").value<QString>();
	d->etag      = data.value("etag").value<QString>();
	d->title     = data.value("title").value<QString>();
	d->updated   = data.value("updated").value<QDateTime>();
	d->selfLink  = data.value("selfLink").value<QUrl>();
	d->parent    = data.value("parent").value<QString>();
	d->position  = data.value("position").value<QString>();
	d->notes     = data.value("notes").value<QString>();
	d->statusFromString(data.value("status").value<QString>());
	d->due       = data.value("due").value<QDateTime>();
	d->completed = data.value("completed").value<QDateTime>();
	d->deleted   = data.value("deleted").value<bool>();
	d->hidden    = data.value("hidden").value<bool>();
}

/*!
  Sets the status according to a string
  Used for deserializing a task
  Case-sensitive, defaults to NeedsAction if unknown string
*/
void TaskPrivate::statusFromString(const QString& s)
{
	if (s == "needsAction") { status = Task::NeedsAction; return; }
	if (s == "completed")   { status = Task::Completed; return; }

	qWarning(QString("TaskPrivate::statusFromString: unknown status: " + s).toLatin1());
	status = Task::NeedsAction;
}

/*!
  Returns a string according to the status
  Used for serializing a task
*/
QString TaskPrivate::statusAsString() const
{
	switch (status) {
		case Task::NeedsAction: return "needsAction";
		case Task::Completed:   return "completed";
		default: qWarning("TaskPrivate::statusAsString missing status string"); return "needsAction";
	}
}

QUrl Task::generateSelfLink(const QString& tasklistId, const QString& taskId)
{
	QUrl url("https://www.googleapis.com/tasks/v1/lists/" + tasklistId + "/tasks");
	if (!taskId.isEmpty()) {
		url.setPath(url.path() + "/" + taskId);
	}
	return url;
}

// Getters
QString      Task::kind()      const { return d->kind; }
QString      Task::id()        const { return d->id; }
QString      Task::etag()      const { return d->etag; }
QString      Task::title()     const { return d->title; }
QDateTime    Task::updated()   const { return d->updated; }
QUrl         Task::selfLink()  const { return d->selfLink; }
QString      Task::parent()    const { return d->parent; }
QString      Task::position()  const { return d->position; }
QString      Task::notes()     const { return d->notes; }
Task::Status Task::status()    const { return d->status; }
QDateTime    Task::due()       const { return d->due; }
QDateTime    Task::completed() const { return d->completed; }
bool         Task::deleted()   const { return d->deleted; }
bool         Task::hidden()    const { return d->hidden; }

// Setters
void Task::setTitle(const QString& title) { d->title = title; }
void Task::setNotes(const QString& notes) { d->notes = notes; }
void Task::setStatus(Status status)       { d->status = status; }
void Task::setDue(const QDateTime& due)   { d->due = due; }
void Task::setCompleted(const QDateTime& done) { d->completed = done; }

// Special setters
// setId is used only to assign a temporary id, when creating a new task
void Task::setId(const QString& id)
{
	Q_ASSERT(d->id.isEmpty());
	d->id = id;
}

} // namespace GTasks
