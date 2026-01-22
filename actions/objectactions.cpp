#include "objectactions.h"

#include <QPainter>
#include <QSvgRenderer>

QAction* ObjectActions::createObjectAction(const QString &text, ObjectType type, const QIcon &icon) {
    QAction *action = new QAction(text, this);
    action->setData(QVariant::fromValue(type));
    action->setIcon(icon);
    connect(action, &QAction::triggered, this, &ObjectActions::onObjectTriggered);

    return action;
}

QIcon ObjectActions::renderSvgIcon(const QString &path, QSize size) {
    QSvgRenderer renderer(path);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::LosslessImageRendering);
    renderer.render(&painter);

    return QIcon(pixmap);
}

ObjectActions::ObjectActions(QWidget *parentWindow, QObject *parent)
    : QObject{parent}, m_parentWindow(parentWindow)
{

    startObjectAction = createObjectAction(tr("&Start block"), ObjectType::Start, renderSvgIcon(":/icons/light/start.svg", QSize(32, 32)));
    stopObjectAction = createObjectAction(tr("&Stop block"), ObjectType::Stop, renderSvgIcon(":/icons/light/stop.svg", QSize(32, 32)));
    eventObjectAction = createObjectAction(tr("&Event block"), ObjectType::Event, renderSvgIcon(":/icons/light/event.svg", QSize(32, 32)));

    statementObjectAction = createObjectAction(tr("&Statement block"), ObjectType::Statement, renderSvgIcon(":/icons/light/statement.svg", QSize(32, 32)));
    conditionalObjectAction = createObjectAction(tr("&Conditional block"), ObjectType::Conditional, renderSvgIcon(":/icons/light/conditional.svg", QSize(32, 32)));
    interactionObjectAction = createObjectAction(tr("&Interaction block"), ObjectType::Interaction, renderSvgIcon(":/icons/light/interaction.svg", QSize(32, 32)));
}

void ObjectActions::populateMenu(QMenu *menu) {
    menu->addAction(startObjectAction);
}

QToolBar* ObjectActions::createToolBar(QWidget *parent) {
    QToolBar *toolbar = new QToolBar(tr("Object Toolbar"), parent);
    toolbar->setObjectName("objecttoolbar");
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
    emit objectRequested(type);
}
