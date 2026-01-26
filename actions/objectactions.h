#ifndef OBJECTACTIONS_H
#define OBJECTACTIONS_H

#include <QToolBar>
#include <QObject>
#include <QAction>
#include <QMenu>

#include "../objects/objecttypes.h"

class ObjectActions : public QObject
{
    Q_OBJECT
public:
    using ObjectType = ObjectTypes::BlockType;

    explicit ObjectActions(QWidget *parentWindow, QObject *parent = nullptr);

    QAction* startObjectAction;
    QAction* stopObjectAction;
    QAction* eventObjectAction;

    QAction* statementObjectAction;
    QAction* conditionalObjectAction;
    QAction* interactionObjectAction;

    void populateMenu(QMenu *menu);
    QToolBar* createToolBar(QWidget *parent);

signals:
    void objectRequested(ObjectType type);

private slots:
    void onObjectTriggered();

private:
    QIcon renderSvgIcon(const QString &path, QSize size);
    QAction *createObjectAction(const QString &text, ObjectType type, const QIcon &icon);
    QWidget *m_parentWindow;
};

#endif // OBJECTACTIONS_H
