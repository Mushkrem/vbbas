#include "objectactions.h"

#include <QMetaEnum>

QAction* ObjectActions::createObjectAction(const QString &text, ObjectType type) {
    QAction *action = new QAction(text, this);
    action->setData(QVariant::fromValue(type));
    action->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DialogError));
    connect(action, &QAction::triggered, this, &ObjectActions::onObjectTriggered);

    return action;
}

ObjectActions::ObjectActions(QWidget *parentWindow, QObject *parent)
    : QObject{parent}, m_parentWindow(parentWindow)
{

    startObjectAction = createObjectAction(tr("&Start block"), ObjectType::Start);
    stopObjectAction = createObjectAction(tr("&Stop block"), ObjectType::Stop);
    eventObjectAction = createObjectAction(tr("&Event block"), ObjectType::Event);

    statementObjectAction = createObjectAction(tr("&Statement block"), ObjectType::Statement);
    conditionalObjectAction = createObjectAction(tr("&Conditional block"), ObjectType::Conditional);
    interactionObjectAction = createObjectAction(tr("&Interaction block"), ObjectType::Interaction);
}

void ObjectActions::populateMenu(QMenu *menu) {
    menu->addAction(startObjectAction);
}

QToolBar* ObjectActions::createToolBar(QWidget *parent) {
    QToolBar *toolbar = new QToolBar(tr("View Toolbar"), parent);
    toolbar->setObjectName("viewtoolbar");
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setFloatable(false);
    toolbar->setMovable(true);

    toolbar->addAction(startObjectAction);
    toolbar->addAction(stopObjectAction);
    toolbar->addAction(eventObjectAction);

    toolbar->addAction(statementObjectAction);
    toolbar->addAction(conditionalObjectAction);
    toolbar->addAction(interactionObjectAction);

    return toolbar;
}

void ObjectActions::onObjectTriggered() {
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action) return;

    ObjectType type = action->data().value<ObjectType>();
    qDebug() << "Object selected:" << QMetaEnum::fromType<ObjectType>().valueToKey(static_cast<int>(type));
    emit objectRequested(type);
}
