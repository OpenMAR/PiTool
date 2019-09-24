#include <QApplication>
#include <QMenu>
#include <QAction>
#include "CustomTrayicon.h"

MyAction::MyAction(QObject *parent)
    :   QAction(parent)
{
    setObjectName("MyAction");
}

MyAction::~MyAction()
{

}

QUrl MyAction::icon() const
{
    return m_icon;
}

void MyAction::setIcon(const QUrl &arg)
{
    if(m_icon != arg)
    {
        QString str = arg.toLocalFile();
        if(str == "")
            str = arg.toString();
        if( str.mid (0, 3) == "qrc")
            str = str.mid (3, str.count() - 3);
        QAction::setIcon(QIcon(str));
        m_icon = arg;
        emit iconChanged();
    }
}

MySeparator::MySeparator(QObject *parent)
    :   QObject(parent)
{
    setObjectName("MySeparator");
}

MySeparator::~MySeparator()
{

}

MyMenu::MyMenu(QQuickItem *parent)
    :   QQuickItem(parent)
{
    setObjectName("MyMenu");
    m_menu = new QMenu();
}

MyMenu::~MyMenu()
{

}

int MyMenu::width() const
{
    return m_menu->width();
}

int MyMenu::height() const
{
    return m_menu->height();
}

void MyMenu::clear()
{
    m_menu->clear();
}

void MyMenu::setWidth(int arg)
{
    if (m_menu->width() != arg)
    {
        m_menu->setFixedWidth(arg);
        emit widthChanged();
    }
}

void MyMenu::setHeight(int arg)
{
    if (m_menu->height() != arg)
    {
        m_menu->setFixedHeight(arg);
        emit heightChanged();
    }
}

void MyMenu::addAction(MyAction *action)
{
    m_menu->addAction(action);
}

void MyMenu::addSeparator()
{
    m_menu->addSeparator();
}

void MyMenu::addMenu(MyMenu *menu)
{
    m_menu->addMenu(menu->m_menu);
}

void MyMenu::componentComplete()
{
    QQuickItem::componentComplete();
    QObjectList list = children();
    for (auto it : list)
    {
        if (it->objectName() == "MyAction")
        {
            MyAction *action = qobject_cast<MyAction *>(it);
            m_menu->addAction(action);
        }
        else if (it->objectName() == "MySeparator")
        {
            m_menu->addSeparator();
        }
        else if (it->objectName() == "MyMenu")
        {
            MyMenu *menu = qobject_cast<MyMenu *>(it);
            m_menu->addMenu(menu->m_menu);
        }
    }
}

CustomTrayicon::CustomTrayicon(QQuickItem *parent)
    :   QQuickItem(parent)
{
    m_systemTray = new QSystemTrayIcon(this);
    connect(m_systemTray, &QSystemTrayIcon::activated, this, &CustomTrayicon::onActivated);
    connect(this, &CustomTrayicon::visibleChanged, this, &CustomTrayicon::onVisibleChanged);
    setVisible(false);
}

CustomTrayicon::~CustomTrayicon()
{

}

int CustomTrayicon::x() const
{
    return m_systemTray->geometry().x();
}

int CustomTrayicon::y() const
{
    return m_systemTray->geometry().y();
}

QUrl CustomTrayicon::icon() const
{
    return m_icon;
}

QString CustomTrayicon::toolTip() const
{
    return m_systemTray->toolTip();
}

MyMenu *CustomTrayicon::menu() const
{
    return m_menu;
}

void CustomTrayicon::setIcon(const QUrl &arg)
{
    if(m_icon != arg)
    {
        QString str = arg.toLocalFile();
        if(str == "") str = arg.toString();
        if( str.mid (0, 3) == "qrc")
            str = str.mid (3, str.count() - 3);
        m_systemTray->setIcon(QIcon(str));
        m_icon = arg;
        emit iconChanged();
    }
}

void CustomTrayicon::setToolTip(const QString &arg)
{
    if (m_toolTip != arg)
    {
        m_systemTray->setToolTip(arg);
        m_toolTip = arg;
        emit toolTipChanged();
    }
}

void CustomTrayicon::setMenu(MyMenu *arg)
{
    if (m_menu != arg)
    {
        m_menu = arg;
        m_systemTray->setContextMenu(m_menu->m_menu);
        m_systemTray->installEventFilter(this);
        emit menuChanged();
    }
}

void CustomTrayicon::onVisibleChanged()
{
    m_systemTray->setVisible(isVisible());
}

void CustomTrayicon::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::Trigger:
        emit trigger();

    default:
        break;
    }
}

void CustomTrayicon::onExit()
{
    m_systemTray->hide();
    QApplication::exit(0);
}
