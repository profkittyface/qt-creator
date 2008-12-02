/***************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
**
** Non-Open Source Usage
**
** Licensees may use this file in accordance with the Qt Beta Version
** License Agreement, Agreement version 2.2 provided with the Software or,
** alternatively, in accordance with the terms contained in a written
** agreement between you and Nokia.
**
** GNU General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the packaging
** of this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
**
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt GPL Exception
** version 1.2, included in the file GPL_EXCEPTION.txt in this package.
**
***************************************************************************/

#include "baseview.h"

#include <extensionsystem/ExtensionSystemInterfaces>

using namespace Core;

/*!
    \class BaseView
    \mainclass
    \ingroup qwb
    \inheaderfile baseview.h
    \brief A base implementation of IView.

    The BaseView class can be used directly for most IView implementations.
    It has setter functions for the views properties, and a convenience constructor
    for the most important ones.

    The ownership of the widget is given to the BaseView, so when the BaseView is destroyed it
    deletes its widget.

    A typical use case is to do the following in the init method of a plugin:
    \code
    bool MyPlugin::init(QString *error_message)
    {
        [...]
        addObject(new Core::BaseView("myplugin.myview",
            new MyWidget,
            QList<int>() << myContextId,
            Qt::LeftDockWidgetArea,
            this));
        [...]
    }
    \endcode
*/

/*!
    \fn BaseView::BaseView()

    Creates a View with empty view name, no widget, empty context, NoDockWidgetArea,
    no menu group and empty default shortcut. You should use the setter functions
    to give the view a meaning.
*/
BaseView::BaseView(QObject *parent)
        : IView(parent),
    m_viewName(""),
    m_widget(0),
    m_context(QList<int>()),
    m_defaultPosition(IView::First)
{
}

/*!
    \fn BaseView::BaseView(const char *name, QWidget *widget, const QList<int> &context, Qt::DockWidgetArea position, QObject *parent)

    Creates a view with the given properties.

    \a name
    \a widget
    \a context
    \a position
    \a parent
*/

BaseView::BaseView(const char *name, QWidget *widget, const QList<int> &context, IView::ViewPosition position, QObject *parent)
    : IView(parent),
    m_viewName(name),
    m_widget(widget),
    m_context(context),
    m_defaultPosition(position)
{
}

/*!
    \fn BaseView::~BaseView()
*/
BaseView::~BaseView()
{
    delete m_widget;
}

/*!
    \fn const QList<int> &BaseView::context() const
*/
QList<int> BaseView::context() const
{
    return m_context;
}

/*!
    \fn QWidget *BaseView::widget()
*/
QWidget *BaseView::widget()
{
    return m_widget;
}

/*!
    \fn const char *BaseView::uniqueViewName() const
*/
const char *BaseView::uniqueViewName() const
{
    return m_viewName;
}


/*!
    \fn IView::ViewPosition BaseView::defaultPosition() const
*/
IView::ViewPosition BaseView::defaultPosition() const
{
    return m_defaultPosition;
}

/*!
    \fn void BaseView::setUniqueViewName(const char *name)

    \a name
*/
void BaseView::setUniqueViewName(const char *name)
{
    m_viewName = name;
}

/*!
    \fn QWidget *BaseView::setWidget(QWidget *widget)

    The BaseView takes the ownership of the \a widget. The previously
    set widget (if existent) is not deleted but returned, and responsibility
    for deleting it moves to the caller of this method.
*/
QWidget *BaseView::setWidget(QWidget *widget)
{
    QWidget *oldWidget = m_widget;
    m_widget = widget;
    return oldWidget;
}

/*!
    \fn void BaseView::setContext(const QList<int> &context)

    \a context
*/
void BaseView::setContext(const QList<int> &context)
{
    m_context = context;
}

/*!
    \fn void BaseView::setDefaultPosition(IView::ViewPosition position)

    \a position
*/
void BaseView::setDefaultPosition(IView::ViewPosition position)
{
    m_defaultPosition = position;
}

