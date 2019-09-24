#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QAction>
#include <QQuickItem>
#include <QSystemTrayIcon>

class MyAction : public QAction
{
    Q_OBJECT
    Q_PROPERTY(QUrl icon READ icon WRITE setIcon NOTIFY iconChanged)

public:
    MyAction(QObject *parent = nullptr);
    ~MyAction();

    QUrl icon() const;

signals:
    void iconChanged();

public slots:
    void setIcon(const QUrl &arg);

private:
    QUrl m_icon;
};


class MySeparator : public QObject
{
public:
    MySeparator(QObject *parent = nullptr);
    ~MySeparator();
};

class SystemTray;
class MyMenu : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)

public:
    MyMenu(QQuickItem *parent = nullptr);
    ~MyMenu();

    int width() const;
    int height() const;
    void clear();

signals:
    void widthChanged();
    void heightChanged();

public slots:
    void setWidth(int arg);
    void setHeight(int arg);
    void addSeparator();
    void addAction(MyAction *action);
    void addMenu(MyMenu *menu);

public:
//protected:
    void componentComplete();

//private:
    friend class SystemTrayIcon;
    QMenu *m_menu;
};

class CustomTrayicon : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int x READ x CONSTANT)
    Q_PROPERTY(int y READ y CONSTANT)
    Q_PROPERTY(QUrl icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip NOTIFY toolTipChanged)
    Q_PROPERTY(MyMenu* menu READ menu WRITE setMenu NOTIFY menuChanged)

public:
    CustomTrayicon(QQuickItem *parent = nullptr);
    ~CustomTrayicon();

    int x() const;
    int y() const;
    QUrl icon() const;
    QString toolTip() const;
    MyMenu* menu() const;

signals:
    void trigger();
    void iconChanged();
    void toolTipChanged();
    void menuChanged();

public slots:
    void setIcon(const QUrl &arg);
    void setToolTip(const QString &arg);
    void setMenu(MyMenu *arg);
    void onVisibleChanged();
    void onActivated(QSystemTrayIcon::ActivationReason reason);
    void onExit();

private:
    QSystemTrayIcon *m_systemTray;
    MyMenu *m_menu;
    QString m_toolTip;
    QUrl m_icon;
};

#endif // SYSTEMTRAYICON_H
